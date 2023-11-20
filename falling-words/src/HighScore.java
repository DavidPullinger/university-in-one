import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Locale;
import java.util.Scanner;

/**
 * This class simply holds this scores of the game and tells player their
 * placement over all time
 */
public class HighScore {
    public ArrayList<Double> scores;
    public int count = 0;

    /**
     * Default constructor which creates array list of score from file
     * 
     * @param fileName name of file to be read from
     */
    public HighScore(String fileName) {

        scores = new ArrayList<Double>();
        try {
            Scanner file = new Scanner(new File(fileName)).useLocale(Locale.US);
            while (file.hasNextLine()) {
                Scanner line = new Scanner(file.nextLine());
                scores.add(line.nextDouble());
            }
        } catch (FileNotFoundException e) {
            System.out.println("Could not find high scores file!");
            e.printStackTrace();
        }
    }

    /**
     * Adds the provided score to the array list of scores
     * 
     * @param score score to be added
     */
    public void addScore(Double score) {
        scores.add(score);
    }

    /**
     * Gets the placement of a score in the array
     * 
     * @param score the users score
     * @return the placement of the score in the array
     */
    public int getPlacing(Double score) {
        // sort scores
        Collections.sort(scores);
        // get placement
        return scores.size() - scores.indexOf(score);
    }

    /**
     * Saves scores to given fie name
     * 
     * @param fileName file name to save to
     */
    public void saveScores(String fileName) {
        FileWriter scoresFile;
        try {
            // attempt to open file for writing
            scoresFile = new FileWriter(fileName);
            // add all new scores
            for (Double d : scores) {
                scoresFile.write(d.toString() + "\n");
            }
            scoresFile.close();
        } catch (IOException e1) {
            System.out.println("Could not open file to write");
            e1.printStackTrace();
        }
    }
}
