/**
 * This runnable class handles a single word, managing its position, speed and
 * text. It sets the position at a fixed rate to ensure smooth animation and
 * correct speed
 */
public class WordManager implements Runnable {
    // fields
    /** actual {@link WordRecord} managed by this thread */
    private WordRecord word;
    /** total words shown in entire game */
    private int totalWords;
    /** words on screen at any time */
    private int noWords;
    /** sets the speed of the word */
    private int delay;

    /**
     * Parameterized constructor
     * 
     * @param w   actual {@link WordRecord} managed by this thread
     * @param tot total words shown in entire game
     * @param num words on screen at any time
     */
    public WordManager(WordRecord w, int tot, int num) {
        word = w;
        totalWords = tot;
        noWords = num;
        delay = w.getSpeed();
    }

    /**
     * Contains all logic that needs to happen on each tick (update). Controls the
     * movement of the string down the screen and re spawning of word
     */
    public void tick() {
        // if word has been dropped and can not be restarted
        if (!word.isActive())
            return;
        // if word is dropped
        else if (word.dropped()) {
            // increment missed and check if word can be reused
            WordApp.score.missedWord();
            if (WordApp.score.getTotal() + noWords - 1 < totalWords) // missed words+caught words+words on screen
            {
                word.resetWord();
                delay = word.getSpeed(); // change our local record of the speed which is set in the constructor
            } else {
                word.deactivate();
                // check if this is last word
                if (WordApp.score.getTotal() == totalWords)
                    WordApp.endGame("Well done! Your score was: " + WordApp.score.getScore()
                            + "<br/>Would you like to play again?");
            }
        }
        // check if text field matches this words text
        else if (word.matchWord(WordApp.getCurrentWord())) { // resets word if true
            // clear word that has been entered into the text field
            WordApp.clearCurrentWord();
            // increment score and check if word can be reused
            WordApp.score.caughtWord(word.getWord().length());
            if (WordApp.score.getTotal() + noWords - 1 < totalWords) // missed words+caught words+words on screen
            {
                delay = word.getSpeed(); // change our local record of the speed which is set in the constructor
            } else {
                word.deactivate();
                // check if this is last word
                if (WordApp.score.getTotal() == totalWords)
                    WordApp.endGame("Well done! Your score was: " + WordApp.score.getScore()
                            + "<br/>Would you like to play again?");
            }
        }
        // drop word
        else
            word.drop(1);
    }

    /**
     * Actual execution when thread is started. Continuously calls {@link #tick()}
     * at a fixed time (60 fps) if {@link WordApp#gameIsPlaying} is true
     * 
     * The code for the animation (with regards to delays, etc.) was implemented
     * using https://zetcode.com/javagames/animation/ as a guideline
     */
    @Override
    public void run() {
        long beforeTime, timeDiff, sleep;
        beforeTime = System.currentTimeMillis();

        while (true) {
            // while game is playing
            if (WordApp.gameIsPlaying) {
                // update word as required
                tick();

                // calculate time that has elapsed
                timeDiff = System.currentTimeMillis() - beforeTime;
                // calculate how long we should wait
                sleep = delay - timeDiff;
                if (sleep < 0) {
                    sleep = 2;
                }

                try {
                    Thread.sleep(sleep);
                } catch (InterruptedException e) {
                    System.out.println(String.format("Thread interrupted: %s", e.getMessage()));
                }
                beforeTime = System.currentTimeMillis();
            }
        }
    }

    /**
     * Resets the position and string of the word by calling
     * {@link WordRecord#resetWord()} and activates it
     */
    public void resetT() {
        word.resetWord();
        word.activate();
    }
}
