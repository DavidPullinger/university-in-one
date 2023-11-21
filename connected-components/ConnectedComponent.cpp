#include "ConnectedComponent.h"

/**
 * @brief Construct a new ConnectedComponent object
 *
 */
PLLDAV013::ConnectedComponent::ConnectedComponent() {}

/**
 * @brief Destroy the ConnectedComponent object
 *
 */
PLLDAV013::ConnectedComponent::~ConnectedComponent() {}

/**
 * @brief Construct a new ConnectedComponent object, via copy constructor
 *
 * @param rhs
 */
PLLDAV013::ConnectedComponent::ConnectedComponent(const ConnectedComponent &rhs) : pixel_count(rhs.pixel_count), id(rhs.id), pixels(rhs.pixels) {}

/**
 * @brief Construct a new ConnectedComponent object, via move constructor
 *
 * @param rhs
 */
PLLDAV013::ConnectedComponent::ConnectedComponent(ConnectedComponent &&rhs) : pixel_count(rhs.pixel_count), id(rhs.id)
{
    rhs.id = -1;
    pixels = std::move(rhs.pixels);
}

/**
 * @brief Overload assignment operator (copy)
 *
 * @param rhs
 * @return ConnectedComponent&
 */
PLLDAV013::ConnectedComponent &PLLDAV013::ConnectedComponent::operator=(const PLLDAV013::ConnectedComponent &rhs)
{
    if (this != &rhs) // make sure that we are not performing a self-assignment
    {
        pixel_count = rhs.pixel_count;
        id = rhs.id;
        pixels = rhs.pixels;
    }
    return *this;
}

/**
 * @brief Overload assignment operator (move)
 *
 * @param rhs
 * @return ConnectedComponent&
 */
PLLDAV013::ConnectedComponent &PLLDAV013::ConnectedComponent::operator=(PLLDAV013::ConnectedComponent &&rhs)
{
    if (this != &rhs) // make sure that we are not performing a self-assignment
    {
        pixel_count = rhs.pixel_count;
        id = rhs.id;

        rhs.id = -1; // rhs should not have valid id
        pixels = std::move(rhs.pixels);
    }
    return *this;
}

/**
 * @brief Add pixel to this connected component
 *
 * @param x x coordinate of pixel
 * @param y y coordinate of pixel
 */
void PLLDAV013::ConnectedComponent::addPixel(int x, int y)
{
    pixels.push_back(coord({x, y}));
    pixel_count++;
}

/**
 * @brief Return number of pixels in this connected component
 *
 * @return int - count of pixels
 */
int PLLDAV013::ConnectedComponent::getPixelCount() const
{
    return pixel_count;
}

/**
 * @brief Return the pixel coordinates of this connected component
 *
 * @return std::vector<PLLDAV013::coord> - vector of pixel coordinates
 */
std::vector<PLLDAV013::coord> &PLLDAV013::ConnectedComponent::getPixels()
{
    return pixels;
}

/**
 * @brief Return the id of this connected component
 *
 * @return int - id of component
 */
int PLLDAV013::ConnectedComponent::getId() const
{
    return id;
}

// EVERYTHING BELOW IS FOR MASTERY WORK
int PLLDAV013::ConnectedComponent::getBoundaryCount() const
{
    return boundary_count;
}

bool PLLDAV013::ConnectedComponent::isComponentPixel(int x, int y)
{
    bool found = false;
    for (std::vector<coord>::iterator i = pixels.begin(); i != pixels.end() && !found; ++i)
    {
        if ((*i).x == x && (*i).y == y)
        {
            found = true;
        }
    }
    return found;
}

int PLLDAV013::ConnectedComponent::getBackgroundNeighbourPixels(coord c, unsigned char **image, int imageH, int imageW)
{
    int count = 0;
    int x = c.x;
    int y = c.y;

    // north
    if (y - 1 >= 0 && image[y - 1][x] == 0 && !isComponentPixel(x, y - 1))
    {
        count++;
    }
    // south
    else if (y + 1 < imageH && image[y + 1][x] == 0 && !isComponentPixel(x, y + 1))
    {
        count++;
    }
    // east
    else if (x + 1 < imageW && image[y][x + 1] == 0 && !isComponentPixel(x + 1, y))
    {
        count++;
    }
    // west
    else if (x - 1 >= 0 && image[y][x - 1] == 0 && !isComponentPixel(x - 1, y))
    {
        count++;
    }
    return count;
}

void PLLDAV013::ConnectedComponent::calculateBoundaryPixels(unsigned char **image, int imageH, int imageW)
{
    int boundary_c = std::count_if(pixels.begin(), pixels.end(), [image, imageH, imageW, this](coord pixel)
                                   { return getBackgroundNeighbourPixels(pixel, image, imageH, imageW) > 0; });
    boundary_count = boundary_c;
}