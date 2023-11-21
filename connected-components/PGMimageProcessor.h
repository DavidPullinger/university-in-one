#include <string>
#include <set>
#include <memory>
#include "ConnectedComponent.h"

#ifndef PGM_PROCESSOR_H
#define PGM_PROCESSOR_H

namespace PLLDAV013
{
    struct ComponentCompare
    {
        bool operator()(const std::shared_ptr<ConnectedComponent> &lhs, const std::shared_ptr<ConnectedComponent> &rhs) const
        {
            return (lhs->getPixelCount() < rhs->getPixelCount());
        }
    };

    class PGMImageProcessor
    {
    private:
        typedef std::multiset<std::shared_ptr<ConnectedComponent>, ComponentCompare> compset;
        compset connectedComponents;
        unsigned char **image = nullptr;
        int imageHeight = 0;
        int imageWidth = 0;

    public:
        PGMImageProcessor();
        PGMImageProcessor(std::string in_filename);
        ~PGMImageProcessor();
        PGMImageProcessor(const PGMImageProcessor &rhs);
        PGMImageProcessor(PGMImageProcessor &&rhs);
        PGMImageProcessor &operator=(const PGMImageProcessor &rhs);
        PGMImageProcessor &operator=(PGMImageProcessor &&rhs);

        int extractComponents(unsigned char threshold, int minValidSize);
        void findConnectedPixels(unsigned char threshold, coord c, ConnectedComponent &comp);
        int filterComponentsBySize(int minSize, int maxSize);
        bool writeComponents(const std::string &outFileName);
        int getComponentCount(void) const;
        int getLargestSize(void) const;
        int getSmallestSize(void) const;
        void printComponentData(const ConnectedComponent &theComponent, bool includeBoundary) const;
        void printComponents(bool includeBoundary) const;
        void calculateBoundaries();
        int getImageHeight() const;
        int getImageWidth() const;
        unsigned char **getImage() const;
        compset &getComponents();
    };
}
#endif
