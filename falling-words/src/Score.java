/**
 * This class holds and modifies the score of the current game, including:
 * missedWords,caughtWords and gameScore.
 */
public class Score {
	/** words that player has missed */
	private int missedWords;
	/** words that player has caught (gotten correct) */
	private int caughtWords;
	/** score of player, determined by length of caught words */
	private int gameScore;

	/**
	 * default constructor
	 */
	Score() {
		missedWords = 0;
		caughtWords = 0;
		gameScore = 0;
	}

	/**
	 * Gets number of words that user has missed
	 * 
	 * @return integer which is number of missed words
	 */
	public synchronized int getMissed() {
		return missedWords;
	}

	/**
	 * Gets number of words that user has caught
	 * 
	 * @return integer which is number of caught words
	 */
	public synchronized int getCaught() {
		return caughtWords;
	}

	/**
	 * Gets total number of words that user has missed and caught
	 * 
	 * @return integer which is total number of words
	 */
	public synchronized int getTotal() {
		return (missedWords + caughtWords);
	}

	/**
	 * Gets users score
	 * 
	 * @return integer which is users score
	 */
	public synchronized int getScore() {
		return gameScore;
	}

	/**
	 * Increment number of missed words
	 */
	public synchronized void missedWord() {
		missedWords++;
	}

	/**
	 * Increment number of caught words and increase score according to caught word
	 * length
	 * 
	 * @param length length of word that user has caught
	 */
	public synchronized void caughtWord(int length) {
		caughtWords++;
		gameScore += length;
	}

	/**
	 * Resets user score
	 */
	public synchronized void resetScore() {
		caughtWords = 0;
		missedWords = 0;
		gameScore = 0;
	}
}
