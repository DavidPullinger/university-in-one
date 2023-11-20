import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Arrays;

/**
 * Facilitates input of raw data, mathematical functions involved in the
 * processing thereof and output of the processed data. Super class for
 * {@link SerialMedianFilter} and {@link ParallelMedianFilter}.
 */
public class MedianFilter {
    /**
     * Raw data read from text file
     */
    protected static Double[] data;
    /**
     * Data that has been processed by a median filter
     */
    protected static Double[] processedData;

    /**
     * Reads data from given file name
     * 
     * @param fileName name of file containing data
     */
    protected static void readData(String fileName) {
        try {
            // initialize scanner with file
            Scanner file = new Scanner(new File(fileName));
            // create array with size given in first line of file
            data = new Double[Integer.parseInt(file.nextLine())];
            processedData = new Double[data.length];
            // extract data from file
            while (file.hasNextLine()) {
                Scanner line = new Scanner(file.nextLine());
                data[Integer.parseInt(line.next())] = line.nextDouble();
            }
        } catch (FileNotFoundException e) {
            System.out.println("FILE NOT FOUND: " + e.getMessage());
            System.exit(1);
        }
    }

    /**
     * Simple math function to calculate the median of an array of numbers
     * 
     * @param numbers array of numbers
     * @return median of numbers
     */
    protected static double median(Double[] numbers) {
        // sort numbers in list
        Arrays.sort(numbers);
        int n = numbers.length;
        // return median; note numbers is odd length since filter width is
        // necessarily odd
        return numbers[((n + 1) / 2 - 1)];
    }

    /**
     * Outputs prefix border elements
     * 
     * @param border length of border
     */
    protected static void prefix(int border) {
        for (int i = 0; i < border; i++) {
            double d = Math.round(data[i] * 1e5) / 1e5;
            System.out.println(i + " " + String.format("%.5f", d));
        }
    }

    /**
     * Outputs suffix border elements
     * 
     * @param border length of border
     */
    protected static void suffix(int border) {
        for (int i = data.length - border; i < data.length; i++) {
            double d = Math.round(data[i] * 1e5) / 1e5;
            System.out.println(i + " " + String.format("%.5f", d));
        }
    }

    /**
     * Loops over {@link #data} from start index to end index, calculating the
     * median at each index
     * 
     * @param filterWidth width of median filter
     * @param start       start index
     * @param end         end index
     */
    protected static void medianFilter(int filterWidth, int start, int end) {
        for (int i = start; i <= end; i++) {
            // fill up temp array with raw data from index i to index i+filter width
            Double[] temp = new Double[filterWidth];
            for (int j = i; j < i + filterWidth; j++) {
                temp[j - i] = data[j];
            }
            // add median to processed data
            processedData[i] = median(temp);
        }
    }

    /**
     * Custom assert which verifies that the filter width is valid and that it is
     * odd
     * 
     * @param filterWidth width of median filter
     */
    protected static void myAssert(int filterWidth) {
        boolean expr = (filterWidth <= 21 && filterWidth >= 3 && filterWidth % 2 != 0);
        if (!expr) {
            System.out.println("Width must be odd and must be between 3 and 21 inclusive.");
            System.exit(1);
        }
    }
}
