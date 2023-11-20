/**
 * This class holds and modifies the text, position and speed of a word.
 */
public class WordRecord {
	/** actual text in the falling words */
	private String text;
	/** x location of object */
	private int x;
	/** y location of object */
	private int y;
	/** elimination zone for words */
	private int maxY;
	/** whether this word has entered the elimination zone */
	private boolean dropped;
	/**
	 * whether this word is active. i.e. whether it has been dropped and can not be
	 * refreshed
	 */
	private boolean active;
	/**
	 * falling speed of this word which can vary between {@link #minWait} and
	 * {@link #maxWait}
	 */
	private int fallingSpeed;
	/** slowest falling speed this word can have */
	private static int maxWait = 20;
	/** fastest falling speed this word can have */
	private static int minWait = 5;
	/** dictionary of words that can be chosen for this word record */
	public static WordDictionary dict;

	/**
	 * Default constructor which creates random falling speed
	 */
	WordRecord() {
		text = "";
		x = 0;
		y = 0;
		maxY = 300;
		dropped = false;
		active = true;
		fallingSpeed = (int) (Math.random() * (maxWait - minWait) + minWait);
	}

	/**
	 * Parameterized constructor which also sets words text
	 * 
	 * @param text text of WordRecord
	 */
	WordRecord(String text) {
		this();
		this.text = text;
	}

	/**
	 * Parameterized constructor which also sets words text and x coords and
	 * elimination zone
	 * 
	 * @param text text of WordRecord
	 * @param x    x coordinate of WordRecord
	 * @param maxY elimination zone
	 */
	WordRecord(String text, int x, int maxY) {
		this(text);
		this.x = x;
		this.maxY = maxY;
	}

	/**
	 * Sets y coordinate of this word and checks if it is in elimination zone
	 * 
	 * @param y the y coordinate to set this word to
	 */
	public synchronized void setY(int y) {
		if (y > maxY) {
			y = maxY;
			dropped = true;
		}
		this.y = y;
	}

	/**
	 * Sets x coordinate of this word
	 * 
	 * @param x the x coordinate to set this word to
	 */
	public synchronized void setX(int x) {
		this.x = x;
	}

	/**
	 * Sets the actual text held by this word
	 * 
	 * @param text the text to set
	 */
	public synchronized void setWord(String text) {
		this.text = text;
	}

	/**
	 * Returns the actual text held by this word
	 * 
	 * @return a String equal to the actual text held by this word
	 */
	public synchronized String getWord() {
		return text;
	}

	/**
	 * Returns the x coordinate of this word
	 * 
	 * @return integer equal to this word's x coordinate
	 */
	public synchronized int getX() {
		return x;
	}

	/**
	 * Returns the y coordinate of this word
	 * 
	 * @return integer equal to this word's y coordinate
	 */
	public synchronized int getY() {
		return y;
	}

	/**
	 * Returns this word's speed
	 * 
	 * @return integer equal to this word's speed
	 */
	public synchronized int getSpeed() {
		return fallingSpeed;
	}

	/**
	 * Sets the x and y coordinates of this word
	 * 
	 * @param x x coordinate to set this word to
	 * @param y y coordinate to set this word to
	 */
	public synchronized void setPos(int x, int y) {
		setY(y);
		setX(x);
	}

	/**
	 * Resets this words position (y coordinate)
	 */
	public synchronized void resetPos() {
		setY(0);
	}

	/**
	 * Resets this word's position, gets a new random word and sets a random speed
	 */
	public synchronized void resetWord() {
		resetPos();
		text = dict.getNewWord();
		dropped = false;
		fallingSpeed = (int) (Math.random() * (maxWait - minWait) + minWait);
	}

	/**
	 * Check if the given text is equal to this words text. If it is, it resets the
	 * current word
	 * 
	 * @param typedText the word to compare this word's text against
	 * @return true if the word's match, false otherwise
	 */
	public synchronized boolean matchWord(String typedText) {
		if (typedText.equals(this.text)) {
			resetWord();
			return true;
		} else
			return false;
	}

	/**
	 * Drops (increments y coordinate) this word by the given increment
	 * 
	 * @param inc increment equal to how far this word should be dropped
	 */
	public synchronized void drop(int inc) {
		setY(y + inc);
	}

	/**
	 * Whether this word has reached the elimination zone
	 * 
	 * @return true if this word has dropped into elimination zone, false otherwise
	 */
	public synchronized boolean dropped() {
		return dropped;
	}

	/**
	 * Deactivates this word so it can not be reset
	 */
	public synchronized void deactivate() {
		active = false;
	}

	/**
	 * Activates this word so it can be reset; used if the game has restarted
	 */
	public synchronized void activate() {
		active = true;
	}

	/**
	 * Whether this word is active
	 * 
	 * @return true if this word is active, false otherwise
	 */
	public synchronized boolean isActive() {
		return active;
	}

}
