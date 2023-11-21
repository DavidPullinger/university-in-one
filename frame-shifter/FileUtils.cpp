#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include "FileUtils.h"

unsigned char **PLLDAV013::read_file(std::string filename)
{
    // open file
    std::ifstream inf(filename);
    // if we opened successfully
    if (inf)
    {
        // get 'metadata' in beggining of file
        std::string line;

        // first get P1 - P5
        std::getline(inf, line);

        // then get comments
        while (std::getline(inf, line) && line[0] == '#')
        {
            continue;
        }

        // line now has dimensions, retrieve them
        std::stringstream ss(line);
        int width, height;
        ss >> width >> height;

        // get max grey value
        std::getline(inf, line);

        // read binary data
        int bytesAvailable = width * height;
        unsigned char **data = new unsigned char *[height];
        for (size_t i = 0; i < height; i++)
        {
            data[i] = new unsigned char[width];
            inf.read((char *)(data[i]), width);
        }
        inf.close();
        return data;
    }
    return nullptr;
}

void PLLDAV013::handle_operation(std::vector<unsigned char **> image_sequence, std::string op_name, std::string out_name, int w, int h)
{
    // ensure out_name is going to output/ directory
    out_name = "output/" + out_name;
    // check which operation we are dealing with, and deal with it appropriately
    char buffer[256];
    int image_seq_size = image_sequence.size();
    if (strcmp(op_name.c_str(), "none") == 0)
    {
        for (size_t i = 0; i < image_seq_size; i++)
        {
            sprintf(buffer, "%s%04d.pgm", out_name.c_str(), (int)i);
            PLLDAV013::write_file(buffer, image_sequence[i], w, h, false);
        }
    }
    else if (strcmp(op_name.c_str(), "invert") == 0)
    {
        for (size_t i = 0; i < image_seq_size; i++)
        {
            sprintf(buffer, "%s%04d.pgm", out_name.c_str(), (int)i);
            PLLDAV013::write_file(buffer, image_sequence[i], w, h, true);
        }
    }
    else if (strcmp(op_name.c_str(), "reverse") == 0)
    {
        for (size_t i = 0; i < image_seq_size; i++)
        {
            sprintf(buffer, "%s%04d.pgm", out_name.c_str(), (int)(image_seq_size - i - 1));
            PLLDAV013::write_file(buffer, image_sequence[i], w, h, false);
        }
    }
    else if (strcmp(op_name.c_str(), "revinvert") == 0)
    {
        for (size_t i = 0; i < image_seq_size; i++)
        {
            sprintf(buffer, "%s%04d.pgm", out_name.c_str(), (int)(image_seq_size - i - 1));
            PLLDAV013::write_file(buffer, image_sequence[i], w, h, true);
        }
    }
}

void PLLDAV013::write_file(std::string filename, unsigned char **data, int w, int h, bool shouldInvert)
{
    // open file
    std::ofstream outf(filename);
    // if we opened successfully
    if (outf)
    {
        // write metadata
        outf << "P5\n"
             << w << " " << h << "\n255"
             << std::endl;

        // write binary block
        // if we need to invert pixel value
        if (shouldInvert)
        {
            unsigned char **inv_data = invert(data, w, h);
            for (size_t i = 0; i < h; i++)
            {
                outf.write((char *)inv_data[i], w);
            }
            // free dynamic memory
            for (size_t i = 0; i < h; i++)
            {
                delete[] inv_data[i];
            }
            delete[] inv_data;
        }
        // otherwise
        else
        {
            for (size_t i = 0; i < h; i++)
            {
                outf.write((char *)data[i], w);
            }
        }
        // close file
        outf.close();
    }
}

unsigned char **PLLDAV013::invert(unsigned char **data, int w, int h)
{
    unsigned char **inv_data = new unsigned char *[h];
    for (size_t i = 0; i < h; i++)
    {
        inv_data[i] = new unsigned char[w];
        for (size_t j = 0; j < w; j++)
        {
            inv_data[i][j] = 255 - data[i][j];
        }
    }
    return inv_data;
}

coord PLLDAV013::get_input_dimensions(std::string filename)
{
    // open file
    std::ifstream inf(filename);
    // if we opened successfully
    if (inf)
    {
        // get 'metadata' in beggining of file
        std::string line;

        // first get P1 - P5
        std::getline(inf, line);

        // then get comments
        while (std::getline(inf, line) && line[0] == '#')
        {
            continue;
        }

        // line now has dimensions, retrieve them
        std::stringstream ss(line);
        int width, height;
        ss >> width >> height;
        return coord(width, height);
    }
    return coord(0, 0);
}
