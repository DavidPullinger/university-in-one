/**
 * Controls File IO and uses a serial median filter to process data
 */
public class SerialMedianFilter extends MedianFilter {

    /**
     * Serially calls {@link #medianFilter} to process entire array of data
     * 
     * @param filterWidth width of median filter
     */
    private static void serialMedianFilter(int filterWidth) {
        medianFilter(filterWidth, 0, data.length - filterWidth);
    }

    /**
     * Reads in arguments from command line before it reads, process and outputs
     * data. Processing is done via a serial median filter
     * 
     * @param args Command line arguments
     */
    public static void main(String[] args) {
        // get args from command line
        String fileName = args[0];
        int filterWidth = Integer.parseInt(args[1]);
        int border = (int) Math.floor((filterWidth - 1) / 2.0);

        // assert filter width length and parity
        myAssert(filterWidth);

        // read and process data
        readData("data/" + fileName + ".txt");
        System.gc();
        serialMedianFilter(filterWidth);

        // output processed data
        System.out.println(processedData.length);
        prefix(border);
        for (int i = 0; i <= processedData.length - filterWidth; i++) {
            Double d = Math.round(processedData[i] * 1e5) / 1e5;
            System.out.println((i + border) + " " + String.format("%.5f", d));
        }
        suffix(border);

    }
}
