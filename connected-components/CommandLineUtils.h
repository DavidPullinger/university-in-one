#include <vector>
#include <string>
typedef std::pair<int, int> pair;

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
        std::vector<std::string> b_params;

    public:
        CommandLineUtils(int argc, char const *argv[]);
        int get_t_params();
        pair get_s_params();
        std::string get_w_params();
        bool get_p_params();
        bool get_b_params();
    };
}
#endif