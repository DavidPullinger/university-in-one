#include <vector>

#ifndef CONN_COMP_H
#define CONN_COMP_H
namespace PLLDAV013
{
    struct coord
    {
        int x;
        int y;
    };

    class ConnectedComponent
    {
    private:
        int pixel_count = 0;
        int boundary_count = 0;
        int id = -1;
        std::vector<coord> pixels;

    public:
        ConnectedComponent();
        ConnectedComponent(int ident) : id(ident) {}
        ~ConnectedComponent();
        ConnectedComponent(const ConnectedComponent &rhs);
        ConnectedComponent(ConnectedComponent &&rhs);
        ConnectedComponent &operator=(const ConnectedComponent &rhs);
        ConnectedComponent &operator=(ConnectedComponent &&rhs);

        void addPixel(int x, int y);
        int getPixelCount() const;
        int getId() const;
        std::vector<coord> &getPixels();
        // for mastery work
        bool isComponentPixel(int x, int y);
        int getBoundaryCount() const;
        int getBackgroundNeighbourPixels(coord c, unsigned char **image, int imageH, int imageW);
        void calculateBoundaryPixels(unsigned char **image, int imageH, int imageW);
    };
}
#endif