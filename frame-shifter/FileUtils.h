typedef std::pair<int, int> coord;

#ifndef FILE_UTILS_H
#define FILE_UTILS_H
namespace PLLDAV013
{
    unsigned char **read_file(std::string filename);
    void handle_operation(std::vector<unsigned char **> image_sequence, std::string op_name, std::string out_name, int w, int h);
    void write_file(std::string filename, unsigned char **data, int w, int h, bool shouldInvert);
    unsigned char **invert(unsigned char **data, int w, int h);
    coord get_input_dimensions(std::string filename);
}
#endif