import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveAction;

/**
 * Controls File IO and uses a parallel median filter to process data
 */
public class ParallelMedianFilter extends MedianFilter {

    /**
     * {@link RecursiveAction} which calculates the median filter of an array once
     * it reaches the sequential cutoff.
     */
    private static class MedianFilterTask extends RecursiveAction {
        private static int SEQ_CUTOFF;
        private int start;
        private int end;
        private int filterWidth;

        /**
         * Creates a new {@link MedianFilterTask} with given start and end indices
         * 
         * @param s  array start index
         * @param e  array end index
         * @param fw width of median filter
         */
        public MedianFilterTask(int s, int e, int fw) {
            start = s;
            end = e;
            filterWidth = fw;
        }

        /**
         * The main computation performed by this task
         */
        @Override
        protected void compute() {
            if (end - start < SEQ_CUTOFF) {
                medianFilter(filterWidth, start, end);
            } else {
                MedianFilterTask left = new MedianFilterTask(start, (start + end) / 2, filterWidth);
                MedianFilterTask right = new MedianFilterTask((start + end) / 2, end, filterWidth);
                invokeAll(left, right);
            }
        }
    }

    private static final ForkJoinPool fjPool = new ForkJoinPool();

    /**
     * Invokes ForkJoinPool to perform MedianFilterTask
     * 
     * @param filterWidth width of median filter
     */
    private static void parallelMedianFilter(int filterWidth) {
        fjPool.invoke(new MedianFilterTask(0, data.length - filterWidth, filterWidth));
    }

    /**
     * Reads in arguments from command line before it reads, process and outputs
     * data. Processing is done via a parallel median filter
     * 
     * @param args Command line arguments
     */
    public static void main(String[] args) {
        // get args from command line
        String fileName = args[0];
        int filterWidth = Integer.parseInt(args[1]);
        MedianFilterTask.SEQ_CUTOFF = Integer.parseInt(args[2]);
        int border = (int) Math.floor((filterWidth - 1) / 2.0);

        // assert filter width length is correct
        myAssert(filterWidth);

        // read and process data
        readData("data/" + fileName + ".txt");
        System.gc();
        parallelMedianFilter(filterWidth);

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
