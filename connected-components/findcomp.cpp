#include <iostream>
#include <string>
#include "CommandLineUtils.h"
#include "PGMimageProcessor.h"

int main(int argc, char const *argv[])
{
    // use command line utils to destructure params
    PLLDAV013::CommandLineUtils utils(argc, argv);

    // file name
    std::string in_filename(argv[argc - 1]);

    // min and max size of extracted components
    int min = utils.get_s_params().first;
    int max = utils.get_s_params().second;
    if (min <= 0 || max <= 0 || max < min)
    {
        std::cout << "Please enter valid min/max sizes for extracted components. \nNote that min must be <= max" << std::endl;
        return 1;
    }

    // threshold for component detection
    int threshold = utils.get_t_params();
    if (threshold < 0 || threshold > 255)
    {
        std::cout << "Please enter a valid threshold value. This value should be between 0 and 255." << std::endl;
        return 1;
    }

    // whether we should print component data
    bool should_print = utils.get_p_params();
    // with borders
    bool include_boundary = utils.get_b_params();

    // filename
    std::string out_filename = utils.get_w_params();

    // create PGM image processor
    PLLDAV013::PGMImageProcessor processor(in_filename);

    // extract connected components
    int count = processor.extractComponents(threshold, min);
    processor.filterComponentsBySize(min, max);
    // print components if requested
    if (should_print)
    {
        std::cout << "Total # of components: " << processor.getComponentCount() << std::endl;
        if (include_boundary)
            processor.calculateBoundaries();
        processor.printComponents(include_boundary);
    }

    // output to file
    processor.writeComponents(out_filename);

    return 0;
}