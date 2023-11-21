#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include "PGMimageProcessor.h"

typedef std::multiset<std::shared_ptr<PLLDAV013::ConnectedComponent>, PLLDAV013::ComponentCompare> compset;

/**
 * @brief Construct a new PGMImageProcessor object
 *
 */
PLLDAV013::PGMImageProcessor::PGMImageProcessor() {}

/**
 * @brief Construct a new PGMImageProcessor object and initialise 2D image array
 *
 * @param in_filename filename of PGM image we should read from
 */
PLLDAV013::PGMImageProcessor::PGMImageProcessor(std::string in_filename)
{
    // open file
    std::ifstream inf(in_filename);
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
        imageWidth = width, imageHeight = height;

        // get max grey value
        std::getline(inf, line);

        // read binary data
        image = new unsigned char *[height];
        for (size_t i = 0; i < height; i++)
        {
            image[i] = new unsigned char[width];
            inf.read((char *)(image[i]), width);
        }
        inf.close();
    }
    else
    {
        std::cout << "ERROR READING FILE" << std::endl;
    }
}

/**
 * @brief Destroy the PGMImageProcessor object
 *
 */
PLLDAV013::PGMImageProcessor::~PGMImageProcessor()
{
    // release image memory if it is managing any
    if (image)
    {
        for (int i = 0; i < imageHeight; i++)
        {
            delete[] image[i];
        }
        delete[] image;
    }
}

/**
 * @brief Construct a new PGMImageProcessor object, via copy constructor
 *
 * @param rhs
 */
PLLDAV013::PGMImageProcessor::PGMImageProcessor(const PGMImageProcessor &rhs) : image(nullptr), connectedComponents(rhs.connectedComponents), imageHeight(rhs.imageHeight), imageWidth(rhs.imageWidth)
{
    if (rhs.image) // if rhs.image is not a null pointer
    {
        // declare 2D array
        image = new unsigned char *[imageHeight];
        for (int i = 0; i < imageHeight; ++i)
        {
            image[i] = new unsigned char[imageWidth];
        }
        // copy 2D array
        for (int y = 0; y < imageHeight; y++)
        {
            for (int x = 0; x < imageWidth; x++)
            {
                image[y][x] = rhs.image[y][x];
            }
        }
    }
}

/**
 * @brief Construct a new PGMImageProcessor object, via move constructor
 *
 * @param rhs
 */
PLLDAV013::PGMImageProcessor::PGMImageProcessor(PGMImageProcessor &&rhs) : image(rhs.image), connectedComponents(std::move(rhs.connectedComponents)), imageHeight(rhs.imageHeight), imageWidth(rhs.imageWidth)
{
    rhs.image = nullptr;
}

/**
 * @brief Overload assignment operator (copy)
 *
 * @param rhs
 * @return PGMImageProcessor&
 */
PLLDAV013::PGMImageProcessor &PLLDAV013::PGMImageProcessor::operator=(const PLLDAV013::PGMImageProcessor &rhs)
{
    if (this != &rhs)
    {
        connectedComponents = rhs.connectedComponents;
        imageHeight = rhs.imageHeight;
        imageWidth = rhs.imageWidth;

        // release this image memory if it is managing any
        if (image)
        {
            for (int i = 0; i < imageHeight; i++)
            {
                delete[] image[i];
            }
            delete[] image;
            image = nullptr;
        }

        if (rhs.image) // if rhs.image is not a null pointer
        {
            // declare 2D array
            image = new unsigned char *[imageHeight];
            for (int i = 0; i < imageHeight; ++i)
            {
                image[i] = new unsigned char[imageWidth];
            }
            // copy 2D array
            for (int y = 0; y < imageHeight; y++)
            {
                for (int x = 0; x < imageWidth; x++)
                {
                    image[y][x] = rhs.image[y][x];
                }
            }
        }
    }
    return *this;
}

/**
 * @brief Overload assignment operator (move)
 *
 * @param rhs
 * @return PGMImageProcessor&
 */
PLLDAV013::PGMImageProcessor &PLLDAV013::PGMImageProcessor::operator=(PLLDAV013::PGMImageProcessor &&rhs)
{
    if (this != &rhs)
    {
        connectedComponents = std::move(rhs.connectedComponents);
        imageHeight = rhs.imageHeight;
        imageWidth = rhs.imageWidth;

        // release image memory if it is managing any
        if (image)
        {
            for (int i = 0; i < imageHeight; i++)
            {
                delete[] image[i];
            }
            delete[] image;
            image = nullptr;
        }

        if (rhs.image)
        {
            image = rhs.image;
            rhs.image = nullptr;
        }
    }
    return *this;
}

/**
 * @brief extract connected components from image
 *
 * @param threshold threshold for extracting components
 * @param minValidSize minimum allowed size in pixels of connected component
 * @return count of connected components after all are extracted
 */
int PLLDAV013::PGMImageProcessor::extractComponents(unsigned char threshold, int minValidSize)
{
    for (int y = 0; y < imageHeight; y++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
            if (image[y][x] >= threshold)
            {
                PLLDAV013::ConnectedComponent comp(connectedComponents.size());
                findConnectedPixels(threshold, coord{x, y}, comp);
                // if our newly created connected component has valid size
                if (comp.getPixelCount() >= minValidSize)
                {
                    std::unique_ptr<ConnectedComponent> comp_ptr = std::make_unique<ConnectedComponent>(comp);
                    connectedComponents.insert(std::move(comp_ptr));
                }
            }
        }
    }
    return connectedComponents.size();
}

/**
 * @brief Helper function of extractComponents, which uses a BFS to find connected
 *      pixels above the threshold
 *
 * @param threshold threshold for extracting components
 * @param c coordinate of starting pixel
 * @param comp component we are adding connected pixels to
 */
void PLLDAV013::PGMImageProcessor::findConnectedPixels(unsigned char threshold, coord c, PLLDAV013::ConnectedComponent &comp)
{
    // make queue and add beginning pixel
    std::queue<coord> coord_q;
    coord_q.push(c);
    image[c.y][c.x] = 0;
    // while our queue is non empty
    // add pixels to comp if they are valid
    while (!coord_q.empty())
    {

        // destructure pixel, and add it to comp
        int x = coord_q.front().x;
        int y = coord_q.front().y;
        comp.addPixel(x, y);
        coord_q.pop();
        // add neighbors on NSEW borders if they exceed threshold

        // north
        if (y - 1 >= 0 && image[y - 1][x] > threshold)
        {
            image[y - 1][x] = 0;
            coord_q.push(coord{x, y - 1});
        }
        // south
        if (y + 1 < imageHeight && image[y + 1][x] > threshold)
        {
            image[y + 1][x] = 0;
            coord_q.push(coord{x, y + 1});
        }
        // east
        if (x + 1 < imageWidth && image[y][x + 1] > threshold)
        {
            image[y][x + 1] = 0;
            coord_q.push(coord{x + 1, y});
        }
        // west
        if (x - 1 >= 0 && image[y][x - 1] > threshold)
        {
            image[y][x - 1] = 0;
            coord_q.push(coord{x - 1, y});
        }
    }
}

/**
 * @brief Filters container of connectedComponents, removing elements whose pixel counts
 *        are below minSize or above maxSize
 *
 * @param minSize min size of connectedComponent
 * @param maxSize max size of connectedComponent
 * @return number of connectedComponents remaining after filtration
 */
int PLLDAV013::PGMImageProcessor::filterComponentsBySize(int minSize, int maxSize)
{
    for (compset::const_iterator i = connectedComponents.begin(); i != connectedComponents.end();)
    {
        if ((**i).getPixelCount() < minSize || (**i).getPixelCount() > maxSize)
        {
            i = connectedComponents.erase(i);
        }
        else
        {
            ++i;
        }
    }
    return connectedComponents.size();
}

/**
 * @brief Write connectedComponents to output PGM file
 *
 * @param outFileName file name of output PGM
 * @return true if successful
 * @return false if unsuccessful
 */
bool PLLDAV013::PGMImageProcessor::writeComponents(const std::string &outFileName)
{
    // first set image to black canvas
    for (int i = 0; i < imageHeight; ++i)
    {
        for (int j = 0; j < imageWidth; ++j)
        {
            image[i][j] = 0;
        }
    }

    // then set connectedComponent pixels to white
    for (compset::const_iterator i = connectedComponents.begin(); i != connectedComponents.end(); ++i)
    {
        for (std::vector<coord>::iterator j = (**i).getPixels().begin(); j < (**i).getPixels().end(); ++j)
        {
            image[j->y][j->x] = 255;
        }
    }

    // open file
    std::ofstream outf(outFileName);

    // if we opened successfully
    if (outf)
    {
        // write metadata
        outf << "P5\n"
             << imageWidth << " " << imageHeight << "\n255"
             << std::endl;

        // write binary block
        for (int i = 0; i < imageHeight; i++)
        {
            outf.write((char *)image[i], imageWidth);
        }
        // close file
        outf.close();
    }
    else
    {
        return false;
    }
    return true;
}

/**
 * @brief Returns the number of connectedComponents
 *
 * @return number of components
 */
int PLLDAV013::PGMImageProcessor::getComponentCount() const
{
    return connectedComponents.size();
}

/**
 * @brief Get size in pixels of largest connected component
 *
 * @return int - number of pixels in componenent
 */
int PLLDAV013::PGMImageProcessor::getLargestSize() const
{
    return (**--connectedComponents.end()).getPixelCount();
}

/**
 * @brief Get size in pixels of smallest connected component
 *
 * @return int - number of pixels in component
 */
int PLLDAV013::PGMImageProcessor::getSmallestSize() const
{
    return (**connectedComponents.begin()).getPixelCount();
}

/**
 * @brief Print all connected components
 *
 */
void PLLDAV013::PGMImageProcessor::printComponents(bool includeBoundary) const
{
    for (compset::const_iterator i = connectedComponents.begin(); i != connectedComponents.end(); ++i)
    {
        std::cout << std::endl;
        printComponentData(**i, includeBoundary);
    }
}

/**
 * @brief Print single connected component
 *
 * @param theComponent componenent to print
 */
void PLLDAV013::PGMImageProcessor::printComponentData(const ConnectedComponent &theComponent, bool includeBoundary) const
{
    std::cout << "Component ID: " << theComponent.getId() << "\nComponent Size: " << theComponent.getPixelCount();
    if (includeBoundary)
    {
        std::cout << "\nBoundary Size: " << theComponent.getBoundaryCount();
    }
    std::cout << std::endl;
}

void PLLDAV013::PGMImageProcessor::calculateBoundaries()
{
    for (compset::const_iterator i = connectedComponents.begin(); i != connectedComponents.end(); ++i)
    {
        (**i).calculateBoundaryPixels(image, imageHeight, imageWidth);
    }
}

int PLLDAV013::PGMImageProcessor::getImageHeight() const
{
    return imageHeight;
}

int PLLDAV013::PGMImageProcessor::getImageWidth() const
{
    return imageWidth;
}

unsigned char **PLLDAV013::PGMImageProcessor::getImage() const
{
    return image;
}

compset &PLLDAV013::PGMImageProcessor::getComponents()
{
    return connectedComponents;
}