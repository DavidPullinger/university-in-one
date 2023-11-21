#include <iostream>
#include <string>
#include <cstring>
#include "CommandLineUtils.h"

PLLDAV013::CommandLineUtils::CommandLineUtils(int argc, char const *argv[])
{
    for (size_t i = 0; i < argc; i++)
    {
        // threshold for component detection
        if (strcmp(argv[i], "-t") == 0)
        {
            t_params.push_back(argv[i + 1]);
            i += 1;
        }
        // min and max size for components
        else if (strcmp(argv[i], "-s") == 0)
        {
            s_params.push_back(argv[i + 1]);
            s_params.push_back(argv[i + 2]);
            i += 2;
        }
        // filename for writing out extracted components image
        else if (strcmp(argv[i], "-w") == 0)
        {
            w_params.push_back(argv[i + 1]);
            i += 1;
        }
        // should or shouldn't print
        else if (strcmp(argv[i], "-p") == 0)
        {
            p_params.push_back("PRINT");
        }
        // should or shouldn't calculate boundary pixels
        else if (strcmp(argv[i], "-b") == 0)
        {
            b_params.push_back("TRUE");
        }
    }
}

int PLLDAV013::CommandLineUtils::get_t_params()
{
    // first check if we have t params
    if (t_params.size() == 0)
        return 128;

    return std::stoi(t_params[0]);
}

pair PLLDAV013::CommandLineUtils::get_s_params()
{
    // check if we have s params
    if (s_params.size() == 0)
        return pair(3, INT32_MAX); // INT32_MAX indicates no max

    // min
    int min = std::stoi(s_params[0]);
    // max
    int max = std::stoi(s_params[1]);
    // return min max pair
    return pair(min, max);
}

std::string PLLDAV013::CommandLineUtils::get_w_params()
{
    if (w_params.size() == 0)
        return "outfile.pgm";
    return w_params[0];
}

bool PLLDAV013::CommandLineUtils::get_p_params()
{
    if (p_params.size() == 0)
        return false;
    return true;
}

bool PLLDAV013::CommandLineUtils::get_b_params()
{
    if (b_params.size() == 0)
        return false;
    return true;
}