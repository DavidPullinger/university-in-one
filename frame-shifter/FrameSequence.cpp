#include <cmath>
#include <iostream>
#include "FrameSequence.h"

PLLDAV013::FrameSequence::FrameSequence(void)
{
    frame_width = 0;
    frame_height = 0;
    start = end = coord(0, 0);
}

PLLDAV013::FrameSequence::FrameSequence(unsigned char **data, int w, int h, coord image_dims, std::vector<coord> t_params, std::vector<coord> p_params)
{
    // set attributes
    frame_width = w;
    frame_height = h;

    // if we have trajectory (-t), treat it as path(-p) with n = 2
    if (t_params.size() > 0 && p_params.size() == 0)
    {
        p_params.push_back(t_params[0]);
        p_params.push_back(t_params[1]);
    }

    // get frame coordinates along trajectory
    frame_coords = get_frame_coords(p_params);

    // loop through these coords and make a frame for each
    for (size_t i = 0; i < frame_coords.size(); i++)
    {
        image_sequence.push_back(make_frame(data, frame_coords[i], image_dims, w, h));
    }
}

PLLDAV013::FrameSequence::~FrameSequence()
{
    for (size_t i = 0; i < image_sequence.size(); i++)
    {
        for (size_t j = 0; j < frame_height; j++)
        {
            delete[] image_sequence[i][j];
        }
        delete[] image_sequence[i];
    }
}

std::vector<coord> PLLDAV013::FrameSequence::get_frame_coords(std::vector<coord> path)
{
    // final vector of coords to return
    std::vector<coord> frame_coords;

    for (size_t i = 0; i < path.size() - 1; i++)
    {
        // declare user input coordinates
        float x_start = path[i].first;
        float y_start = path[i].second;
        float x_end = path[i + 1].first;
        float y_end = path[i + 1].second;

        // vector to store coordinates along trajectory
        coord frame_coord;

        // calculate gradient
        float g = (y_end - y_start) / (x_end - x_start);
        // if gradient is < 45 degrees
        if (std::fabs(g) < 1.0)
        {
            // loop over x and find corresponding y on line
            float y = y_start;
            frame_coord = coord(x_start, y_start);
            frame_coords.push_back(frame_coord);
            // if x_start <= x_end, increment x
            if (x_start <= x_end)
            {
                for (int x = x_start + 1; x <= x_end; ++x)
                {
                    y += g;
                    frame_coord = coord(x, std::round(y));
                    frame_coords.push_back(frame_coord);
                }
            }
            // otherwise, decrement x
            else
            {
                for (int x = x_start - 1; x >= x_end; --x)
                {
                    y -= g;
                    frame_coord = coord(x, std::round(y));
                    frame_coords.push_back(frame_coord);
                }
            }
        }
        else
        {
            // loop over y and find corresponding x on line
            g = 1 / g;
            float x = x_start;
            frame_coord = coord(x_start, y_start);
            frame_coords.push_back(frame_coord);
            // if x_start <= x_end, increment x
            if (y_start <= y_end)
            {
                for (int y = y_start + 1; y <= y_end; ++y)
                {
                    x += g;
                    frame_coord = coord(std::round(x), y);
                    frame_coords.push_back(frame_coord);
                }
            }
            else
            {
                for (int y = y_start - 1; y >= y_end; --y)
                {
                    x -= g;
                    frame_coord = coord(std::round(x), y);
                    frame_coords.push_back(frame_coord);
                }
            }
        }
    }

    return frame_coords;
}

std::vector<unsigned char **> PLLDAV013::FrameSequence::get_image_sequence() { return image_sequence; }

unsigned char **PLLDAV013::FrameSequence::make_frame(unsigned char **data, coord frame_coord, coord image_dims, int w, int h)
{
    // create 2d dynamic array
    unsigned char **frame = new unsigned char *[h];
    for (size_t i = 0; i < h; i++)
    {
        frame[i] = new unsigned char[w];
    }

    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < w; j++)
        {
            if (((int)(i + frame_coord.second)) < 0 || ((int)(j + frame_coord.first)) < 0 || ((int)(i + frame_coord.second)) >= image_dims.second || ((int)(j + frame_coord.first)) >= image_dims.first)
            {
                frame[i][j] = 0;
            }
            // set pixel at (j,i) to original data at (frame_coord.x+j,frame_coord.y+i)
            else
            {
                frame[i][j] = data[(i + frame_coord.second)][(j + frame_coord.first)];
            }
        }
    }
    return frame;
}