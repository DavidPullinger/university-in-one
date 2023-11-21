#include <iostream>
#include <string>
#include <cstring>
#include "CommandLineUtils.h"

PLLDAV013::CommandLineUtils::CommandLineUtils(int argc, char const *argv[])
{
    for (size_t i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-t") == 0)
        {
            t_params.push_back(argv[i + 1]);
            t_params.push_back(argv[i + 2]);
            t_params.push_back(argv[i + 3]);
            t_params.push_back(argv[i + 4]);
            i += 4;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            s_params.push_back(argv[i + 1]);
            s_params.push_back(argv[i + 2]);
            i += 2;
        }
        else if (strcmp(argv[i], "-w") == 0)
        {
            w_params.push_back(argv[i + 1]);
            w_params.push_back(argv[i + 2]);
            i += 2;
        }
        else if (strcmp(argv[i], "-p") == 0)
        {
            int n = std::stoi(argv[i + 1]);
            for (int j = 0; j < n * 2; j++)
            {
                p_params.push_back(argv[i + j + 2]);
            }
            i += 2 * n + 1;
        }
    }
}

std::vector<coord> PLLDAV013::CommandLineUtils::get_t_params()
{
    std::vector<coord> coords;
    // first check if we have t params
    if (t_params.size() == 0)
        return coords;
    // start coord
    coord start(std::stoi(t_params[0]), std::stoi(t_params[1]));
    // end coord
    coord end(std::stoi(t_params[2]), std::stoi(t_params[3]));
    // return vector of both coord
    coords.push_back(start);
    coords.push_back(end);
    return coords;
}

coord PLLDAV013::CommandLineUtils::get_s_params()
{
    // width
    int width = std::stoi(s_params[0]);
    // height
    int height = std::stoi(s_params[1]);
    // return vector of both coord
    coord dimensions(width, height);
    return dimensions;
}

std::vector<std::pair<std::string, std::string> > PLLDAV013::CommandLineUtils::get_w_params()
{
    std::vector<std::pair<std::string, std::string> > operations;
    std::string frameOperation;
    std::string frameName;
    for (size_t i = 0; i < w_params.size(); i += 2)
    {
        frameOperation = w_params[i];
        frameName = w_params[i + 1];
        operations.push_back(std::pair<std::string, std::string>(frameOperation, frameName));
    }
    return operations;
}

std::vector<coord> PLLDAV013::CommandLineUtils::get_p_params()
{
    std::vector<coord> coords;
    // start coord
    for (size_t i = 0; i < p_params.size(); i += 2)
    {
        coord point(std::stoi(p_params[i]), std::stoi(p_params[i + 1]));
        coords.push_back(point);
    }
    // return vector of n coords
    return coords;
}