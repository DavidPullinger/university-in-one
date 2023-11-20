import java.awt.Component;
import java.awt.Graphics;
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.awt.Font;
import javax.swing.JPanel;
import javax.swing.JLabel;

/**
 * This class contains the graphical interface for the falling words. It
 * continuously refreshes the score UI components and redraws the falling words
 */
public class WordPanel extends JPanel implements Runnable {
	/** words to be displayed on this {@link JPanel} */
	WordRecord[] words;
	/** words on screen at any time */
	int noWords;
	/** zone where words are eliminated */
	int maxY;

	/**
	 * Paints this JPanel component, specifically the falling words and their
	 * bounding rect
	 */
	public void paintComponent(Graphics g) {
		int width = getWidth();
		int height = getHeight();
		g.clearRect(0, 0, width, height);
		g.setColor(WordApp.red);
		g.fillRect(0, maxY - 10, width, height);

		g.setColor(WordApp.background);
		g.fillRect(0, 0, width, maxY - 10);

		g.setColor(WordApp.text);
		g.setFont(new Font("Helvetica", Font.PLAIN, 26));
		// draw the words
		for (int i = 0; i < noWords; i++) {
			// +20 is y-offset for skeleton so that you can see the words
			if (words[i].isActive())
				g.drawString(words[i].getWord(), words[i].getX(), words[i].getY() + 20);
		}

		// update text fields
		Component[] c = WordApp.txt.getComponents();
		((JLabel) (c[0])).setText("Caught: " + WordApp.score.getCaught() + "    ");
		((JLabel) (c[1])).setText("Missed: " + WordApp.score.getMissed() + "    ");
		((JLabel) (c[2])).setText("Score: " + WordApp.score.getScore() + "    ");
	}

	/**
	 * Parameterized constructor
	 * 
	 * @param words words that should fall
	 * @param maxY  zone where words are eliminated
	 */
	WordPanel(WordRecord[] words, int maxY) {
		this.words = words;
		noWords = words.length;
		this.maxY = maxY;
	}

	/**
	 * Actual execution when thread is started. Continuously repaints this
	 * {@link JPanel}
	 */
	@Override
	public void run() {
		// continue refreshing jpanel
		while (true) {
			repaint();
			// let developer know if there are any deadlocks
			if (detectDeadlock())
				System.out.println("Deadlock found!");

		}

	}

	/**
	 * Detects if the program has a deadlock
	 */
	private static boolean detectDeadlock() {
		ThreadMXBean threadBean = ManagementFactory.getThreadMXBean();
		long[] threadIds = threadBean.findDeadlockedThreads();
		boolean deadLock = threadIds != null && threadIds.length > 0;
		return deadLock;
	}

}
