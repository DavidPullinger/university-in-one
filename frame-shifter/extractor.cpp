#include <iostream>
#include <string>
#include <vector>

#include "CommandLineUtils.h"
#include "FileUtils.h"
#include "FrameSequence.h"
typedef std::pair<int, int> coord;

int main(int argc, char const *argv[])
{
    // use command line utils to destructure params
    PLLDAV013::CommandLineUtils utils(argc, argv);

    // file name
    std::string file_name(argv[1]);

    // dimensions of frame
    int frame_width = utils.get_s_params().first;
    int frame_height = utils.get_s_params().second;
    if (frame_width <= 0 || frame_height <= 0)
    {
        std::cout << "Please enter valid frame dimensions" << std::endl;
        return 1;
    }
    // operations for PGM frames
    std::vector<std::pair<std::string, std::string> > operations = utils.get_w_params();

    // read in PGN file
    unsigned char **data = PLLDAV013::read_file(file_name);

    // create sequence of frames using width, height and start/end coordinates, and delete data
    PLLDAV013::FrameSequence frame_seq(data, frame_width, frame_height, PLLDAV013::get_input_dimensions(file_name), utils.get_t_params(), utils.get_p_params());
    for (int i = 0; i < frame_height; ++i)
    {
        delete[] data[i];
    }
    delete[] data;

    // get sequence of frames
    std::vector<unsigned char **> image_sequence = frame_seq.get_image_sequence();

    // loop through user's operations and output a sequence of image files for each
    for (size_t i = 0; i < operations.size(); i++)
    {
        // get operation info
        std::string op_name = operations[i].first;
        std::string out_name = operations[i].second;

        // write out data, operating it simultaneously
        PLLDAV013::handle_operation(image_sequence, op_name, out_name, frame_width, frame_height);
    }

    return 0;
}
