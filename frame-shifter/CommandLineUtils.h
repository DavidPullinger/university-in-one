#include <vector>
#include <string>
typedef std::pair<int, int> coord;

#ifndef CL_UTILS_H
#define CL_UTILS_H

namespace PLLDAV013
{
    class CommandLineUtils
    {
    private:
        std::vector<std::string> t_params;
        std::vector<std::string> s_params;
        std::vector<std::string> w_params;
        std::vector<std::string> p_params;

    public:
        CommandLineUtils(int argc, char const *argv[]);
        std::vector<coord> get_t_params();
        coord get_s_params();
        std::vector<std::pair<std::string, std::string> > get_w_params();
        std::vector<coord> get_p_params();
    };
}
#endif