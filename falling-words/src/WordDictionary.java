/**
 * This class holds all the words that can be access by the {@link WordRecord}
 * class and facilitates random word generation
 */
public class WordDictionary {
	/** number of words in dictionary */
	int size;
	/** default dictionary */
	static String[] theDict = { "litchi", "banana", "apple", "mango", "pear", "orange", "strawberry", "cherry", "lemon",
			"apricot", "peach", "guava", "grape", "kiwi", "quince", "plum", "prune", "cranberry", "blueberry",
			"rhubarb", "fruit", "grapefruit", "kumquat", "tomato", "berry", "boysenberry", "loquat", "avocado" }; // default
																													// dictionary

	/**
	 * Parametrized constructor which copies dictionary parameter
	 * 
	 * @param tmp temporary array of string to be copied over
	 */
	WordDictionary(String[] tmp) {
		size = tmp.length;
		theDict = new String[size];
		for (int i = 0; i < size; i++) {
			theDict[i] = tmp[i];
		}

	}

	/**
	 * Default constructor
	 */
	WordDictionary() {
		size = theDict.length;

	}

	/**
	 * Returns a random word. This method is usually called by {@link WordRecord}
	 * running on {@link WordManager} threads
	 * 
	 * @return random word from dictionary
	 */
	public synchronized String getNewWord() {
		int wdPos = (int) (Math.random() * size);
		return theDict[wdPos];
	}

}
