#include <utility>
#include <vector>
typedef std::pair<int, int> coord;

#ifndef FRAME_SEQ_H
#define FRAME_SEQ_H
namespace PLLDAV013
{
    class FrameSequence
    {
    private:
        int frame_width;
        int frame_height;
        coord start;
        coord end;
        std::vector<coord> frame_coords;
        std::vector<unsigned char **> image_sequence;

        std::vector<coord> get_frame_coords(std::vector<coord> path);
        unsigned char **make_frame(unsigned char **data, coord frame_coord, coord image_dims, int w, int h);

    public:
        FrameSequence(void);
        FrameSequence(unsigned char **data, int w, int h, coord image_dims, std::vector<coord> t_params, std::vector<coord> p_params);
        ~FrameSequence();
        std::vector<unsigned char **> get_image_sequence();
    };
}
#endif