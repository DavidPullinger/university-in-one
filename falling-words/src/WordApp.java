import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Color;
import java.awt.BorderLayout;
import java.awt.Component;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Scanner;

/**
 * This class is the main controller of the game; it controls I/O from player as
 * well as initialization and termination of the game. This class also sets up
 * the main GUI.
 */
public class WordApp {
	// variables shared by entire game
	/** words on screen at any time */
	static int noWords;
	/** total words shown in entire game */
	static int totalWords;
	/** words that are falling down screen */
	static WordRecord[] words;
	/** must be volatile - lets threads know if game is playing */
	static volatile boolean gameIsPlaying = false;
	/** score of player */
	static Score score = new Score();
	/** use default dictionary, to read from file eventually */
	static WordDictionary dict = new WordDictionary();
	/** current word that player has entered in txtfield */
	static String currentWord = "";

	// variables for look and feel
	static int frameX = 1000;
	static int frameY = 600;
	static int yLimit = 480;
	static Color red = new Color(222, 10, 2);
	static Color background = Color.white;
	static Color text = new Color(24, 26, 27);
	static boolean darkMode = false;

	// variables for WordApp
	static WordManager[] managers;
	static HighScore highScore;
	static long startTime;
	static long pausedTime;
	static long wastedTime = 0;

	// jcomponents used in setupGUI and some other classes
	static JFrame frame;
	static JPanel g;
	static WordPanel w;
	static JPanel txt;
	static JPanel b;
	static JButton startB;

	/**
	 * Sets up the main frame of the GUI
	 * 
	 * @param frameX preferred width
	 * @param frameY preferred height
	 * @param yLimit zone where words are eliminated
	 */
	public static void setupGUI(int frameX, int frameY, int yLimit) {
		// Frame init and dimensions
		frame = new JFrame("WordGame");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(frameX, frameY);
		g = new JPanel();
		g.setLayout(new BoxLayout(g, BoxLayout.PAGE_AXIS));
		g.setSize(frameX, frameY);

		w = new WordPanel(words, yLimit);
		w.setSize(frameX, yLimit + 100);
		g.add(w);

		txt = new JPanel();
		txt.setLayout(new BoxLayout(txt, BoxLayout.LINE_AXIS));
		JLabel caught = new JLabel("Caught: " + score.getCaught() + "    ");
		JLabel missed = new JLabel("Missed:" + score.getMissed() + "    ");
		JLabel scr = new JLabel("Score:" + score.getScore() + "    ");
		txt.add(caught);
		txt.add(missed);
		txt.add(scr);

		final JTextField textEntry = new JTextField("", 20);
		textEntry.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				String text = textEntry.getText();
				setCurrentWord(text);
				textEntry.setText("");
				textEntry.requestFocus();
			}
		});
		textEntry.setEnabled(false);

		txt.add(textEntry);
		txt.setMaximumSize(txt.getPreferredSize());
		g.add(txt);

		b = new JPanel();
		b.setLayout(new BoxLayout(b, BoxLayout.LINE_AXIS));

		JButton endB = new JButton("End Game");
		// add the listener to the jbutton to handle the "pressed" event
		endB.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				endGame("Would you like to play again?");
				textEntry.requestFocus(); // return focus to the text entry field
			}
		});
		endB.setEnabled(false);

		JButton pauseB = new JButton("Pause");
		// add the listener to the jbutton to handle the "pressed" event
		pauseB.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// toggle resume/pause text
				if (gameIsPlaying) {
					pauseB.setText("Resume");
					// we must pause the timer
					pausedTime = System.currentTimeMillis();
				} else {
					pauseB.setText("Pause");
					// increment time that we have wasted while paused
					wastedTime += System.currentTimeMillis() - pausedTime;
				}

				// toggle the gameIsPlaying state for threads
				gameIsPlaying = !gameIsPlaying;

				// disable or enable the buttons and text fields
				textEntry.setEnabled(gameIsPlaying);
				endB.setEnabled(gameIsPlaying);
				startB.setEnabled(gameIsPlaying);

				textEntry.requestFocus(); // return focus to the text entry field
			}
		});
		pauseB.setEnabled(false);

		// made a class variable so it can be accessed above
		startB = new JButton("Start Game");
		// add the listener to the jbutton to handle the "pressed" event
		startB.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				startTime = System.currentTimeMillis();
				gameIsPlaying = true; // let threads know that game has started
				textEntry.setEnabled(true); // enable text field
				endB.setEnabled(true);
				pauseB.setEnabled(true);
				textEntry.requestFocus(); // return focus to the text entry field
			}
		});

		JButton quitB = new JButton("Quit");
		// add the listener to the jbutton to handle the "pressed" event
		quitB.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});
		quitB.setForeground(red);

		JButton toggleB = new JButton("Dark Mode");
		// add the listener to the jbutton to handle the "pressed" event
		toggleB.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				toggleAppearance();
				darkMode = !darkMode; // toggle dark mode boolean
				if (darkMode)
					toggleB.setText("Light Mode");
				else
					toggleB.setText("Dark Mode");
				textEntry.requestFocus(); // return focus to the text entry field
			}
		});

		// left align toggle
		b.add(toggleB);
		b.add(Box.createHorizontalGlue());
		// center first 3 buttons
		b.add(startB);
		b.add(endB);
		b.add(pauseB);
		// right align quit button
		b.add(Box.createHorizontalGlue());
		b.add(quitB);

		g.add(b);

		frame.setLocationRelativeTo(null); // Center window on screen.
		frame.add(g); // add contents to window
		frame.setContentPane(g);
		// frame.pack(); // don't do this - packs it into small space
		frame.setVisible(true);
	}

	/**
	 * Reads file into WordDictionary
	 * 
	 * @param filename name of file containing words
	 * @return array of Strings, which are the lines of the file
	 */
	public static String[] getDictFromFile(String filename) {
		String[] dictStr = null;
		try {
			Scanner dictReader = new Scanner(new FileInputStream(filename));
			int dictLength = dictReader.nextInt();
			// System.out.println("read '" + dictLength+"'");

			dictStr = new String[dictLength];
			for (int i = 0; i < dictLength; i++) {
				dictStr[i] = new String(dictReader.next());
				// System.out.println(i+ " read '" + dictStr[i]+"'"); //for checking
			}
			dictReader.close();
		} catch (IOException e) {
			System.err.println("Problem reading file " + filename + " default dictionary will be used");
		}
		return dictStr;
	}

	/**
	 * Ends current game, showing the player a message and allowing them to restart
	 * or quit
	 * 
	 * @param message message to be displayed to player
	 */
	public static void endGame(String message) {
		// halt words
		gameIsPlaying = false;

		// calculate elapsed time and wpm and save
		double elapse = System.currentTimeMillis() - startTime;
		double wpm = Math.round(((score.getCaught() * 1.0) / ((elapse - wastedTime) / 60000.0)) * 100.0) / 100.0;
		highScore.addScore(wpm);

		// display dialog
		int res = JOptionPane.showOptionDialog(frame, endGamePanel(message, wpm), "End of Game",
				JOptionPane.YES_NO_OPTION, JOptionPane.PLAIN_MESSAGE, null, new String[] { "No, quit", "Yes" }, null);

		// see what user chose
		if (res == 0) // if they dont want to play
		{
			highScore.saveScores("data/scores.txt");
			System.exit(0);
		} else if (res == JOptionPane.CLOSED_OPTION) { // if they click cancel
			if (score.getTotal() == totalWords) { // check if game is over
				highScore.saveScores("data/scores.txt");
				System.exit(0);
			}
			gameIsPlaying = true;
		} else { // if they do want to play
			reset();
		}
	}

	/**
	 * Panel which is displayed when game ends
	 * 
	 * @param message message to be displayed to player
	 * @return a JPanel with an icon and message
	 */
	public static JPanel endGamePanel(String message, double wpm) {
		// picture of game
		Icon icon = new ImageIcon("data/scrabble.jpg");
		JLabel iconLbl = new JLabel(icon);

		// text for user prompt
		JLabel lbl = new JLabel("<html><br/>" + message + "<br/><br/>Your typing speed was: " + wpm + "<br/>"
				+ "which is placed number " + (highScore.getPlacing(wpm)) + " overall</html>");
		JPanel txt = new JPanel();
		txt.add(Box.createHorizontalGlue());
		txt.add(lbl, BorderLayout.CENTER);
		txt.add(Box.createHorizontalGlue());

		// jpanel to center icon and text
		JPanel mainPanel = new JPanel(new BorderLayout());
		mainPanel.add(iconLbl, BorderLayout.NORTH);
		mainPanel.add(txt, BorderLayout.SOUTH);

		return mainPanel;
	}

	/**
	 * Calls the {@link WordManager#resetT()} method of {@link WordManager} for all
	 * threads, and resets score
	 */
	public static void reset() {
		// reset words of threads - dont want to end threads
		for (int i = 0; i < noWords; i++) {
			managers[i].resetT();
		}
		// reset score
		score.resetScore();
		// reset word
		currentWord = "";
		// restart timer
		startTime = System.currentTimeMillis();
		wastedTime = 0;
		// continue playing
		gameIsPlaying = true;
	}

	/**
	 * Gets the currentWord that has been entered by the user
	 * 
	 * @return current word entered by the user
	 */
	public static synchronized String getCurrentWord() {
		return currentWord;
	}

	/**
	 * Sets the currentWord that has been entered by the user
	 * 
	 * @param s string to set the current word equal to
	 */
	public static synchronized void setCurrentWord(String s) {
		currentWord = s;
	}

	/**
	 * Resets the currentWord that has been entered by the user
	 */
	public static synchronized void clearCurrentWord() {
		currentWord = "";
	}

	/**
	 * Toggles light and dark mode for this application, setting background and text
	 * on relevant comps
	 */
	public static void toggleAppearance() {
		// swap color of text and background
		Color temp = background;
		background = text;
		text = temp;

		// set background colors
		g.setBackground(background);
		txt.setBackground(background);
		b.setBackground(background);

		// set text colors
		for (Component c : txt.getComponents()) {
			if (!(c instanceof JTextField))
				c.setForeground(text);
		}
	}

	/**
	 * Main method of this class (and game). It reads in command line parameters and
	 * initializes the WordRecord array as well as the threads managing them
	 * 
	 * @param args command line parameters
	 */
	public static void main(String[] args) {
		// deal with command line arguments
		totalWords = Integer.parseInt(args[0]); // total words to fall
		noWords = Integer.parseInt(args[1]); // total words falling at any point
		assert (totalWords >= noWords); // this could be done more neatly
		String[] tmpDict = getDictFromFile(args[2]); // file of words

		// read high scores
		highScore = new HighScore("data/scores.txt");

		// set the class dictionary for the words.
		if (tmpDict != null)
			dict = new WordDictionary(tmpDict);
		WordRecord.dict = dict;

		// set shared variables
		words = new WordRecord[noWords]; // shared array of current words
		managers = new WordManager[noWords]; // shared array of threads managing words

		// setup UI for this app
		setupGUI(frameX, frameY, yLimit);

		// Start WordPanel thread - for redrawing animation
		Thread wordPanelT = new Thread(w);
		wordPanelT.start();

		// initialize shared array of current words
		int x_inc = (int) frameX / noWords;
		for (int i = 0; i < noWords; i++) {
			words[i] = new WordRecord(dict.getNewWord(), i * x_inc, yLimit);
		}

		// Start WordManager threads - to control movement and state of words
		for (int i = 0; i < noWords; i++) {
			managers[i] = new WordManager(words[i], totalWords, noWords);
			new Thread(managers[i]).start();
		}
	}
}