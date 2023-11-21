#define CATCH_CONFIG_MAIN // This tells catch to provide a main() function. Only do this in one cpp file
#include "catch.hpp"
#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"
using namespace PLLDAV013;

// model values for PGMimageProcessor initialized with chess.pgm
PGMImageProcessor p("chess.pgm");
unsigned char **MODEL_IMAGE = p.getImage();
const int MODEL_WIDTH = p.getImageWidth();
const int MODEL_HEIGHT = p.getImageHeight();

bool imagesEqual(unsigned char **firstImage, unsigned char **secondImage)
{
	bool equal = true;
	for (int y = 0; y < MODEL_HEIGHT && equal; ++y)
	{
		for (int x = 0; x < MODEL_WIDTH && equal; ++x)
		{
			if (firstImage[y][x] != secondImage[y][x])
			{
				std::cout << "Images not equal: " << (int)firstImage[y][x] << "(first) vs " << (int)secondImage[y][x] << "(second)" << std::endl;
				equal = false;
			}
		}
	}
	return equal;
}

bool pixelsEqual(std::vector<coord> firstPixels, std::vector<coord> secondPixels)
{
	bool equal = true;
	for (int i = 0; i < firstPixels.size() && equal; ++i)
	{
		if (firstPixels[i].x != secondPixels[i].x || firstPixels[i].y != secondPixels[i].y)
		{
			std::cout << "Pixels not equal" << std::endl;
			equal = false;
		}
	}
	return equal;
}

// Tests for the PGMImageProcessor Class
TEST_CASE("PGMimageProcessor Unit Tests")
{
	SECTION("Default Constructor")
	{
		PGMImageProcessor p;
		REQUIRE(p.getImageWidth() == 0);
		REQUIRE(p.getImageHeight() == 0);
		REQUIRE(p.getImage() == nullptr);
	}

	SECTION("Custom Constructor")
	{
		PGMImageProcessor p("chess.pgm");

		REQUIRE(p.getImageWidth() == MODEL_WIDTH);
		REQUIRE(p.getImageHeight() == MODEL_HEIGHT);
		REQUIRE(imagesEqual(p.getImage(), MODEL_IMAGE)); // ensure bits are equal
	}

	SECTION("Copy Constructor")
	{
		PGMImageProcessor p1("chess.pgm");
		PGMImageProcessor p2(p1); // Call the copy constructor

		REQUIRE(p1.getImageWidth() == p2.getImageWidth());
		REQUIRE(p1.getImageHeight() == p2.getImageHeight());

		REQUIRE(p1.getImage() != p2.getImage()); // Make sure that they are NOT pointing to the same bit of memory
		REQUIRE(*p1.getImage() != *p2.getImage());
		REQUIRE(imagesEqual(p1.getImage(), p2.getImage())); // But make sure they have the same value
	}
	SECTION("Move Constructor")
	{
		PGMImageProcessor p1("chess.pgm");
		unsigned char **imageAdr = p1.getImage(); // We are just getting the address of p1.image so that we can make sure that p2.image has the same address.

		PGMImageProcessor p2(std::move(p1));

		// Test for p1
		REQUIRE(p1.getImage() == nullptr); // We need to make sure that p1 is no longer managing any dynamic memory (since we moved it)

		// Test for p2
		REQUIRE(p2.getImageWidth() == MODEL_WIDTH);
		REQUIRE(p2.getImageHeight() == MODEL_HEIGHT);

		REQUIRE(p2.getImage() == imageAdr);				  // Here we make sure that p2 is managing the bit of memory that p1 used to manage
		REQUIRE(imagesEqual(p2.getImage(), MODEL_IMAGE)); // Here we make sure that the value of image has not changed
	}

	SECTION("Copy Assignment Operator")
	{
		PGMImageProcessor p1("chess.pgm");
		PGMImageProcessor p2;

		p2 = p1; // Call the copy assignment operator (PGMImageProcessor p2 = p1 will actually call the copy constructor, not the copy assignment operator)

		REQUIRE(p1.getImageWidth() == p2.getImageWidth());
		REQUIRE(p1.getImageHeight() == p2.getImageHeight());

		REQUIRE(p1.getImage() != p2.getImage()); // Make sure that they are NOT pointing to the same bit of memory
		REQUIRE(*p1.getImage() != *p2.getImage());
		REQUIRE(imagesEqual(p1.getImage(), p2.getImage())); // But make sure they have the same value

		// Check the self assignment case
		unsigned char **imageAdr = p1.getImage();

		p1 = p1;

		REQUIRE(p1.getImage() == imageAdr);				  // Make sure that the memory address of p1.image has not changed
		REQUIRE(imagesEqual(p1.getImage(), MODEL_IMAGE)); // and that it has the same value
	}

	SECTION("Move Assignment Operator")
	{
		PGMImageProcessor p1("chess.pgm");
		unsigned char **imageAdr = p1.getImage();

		PGMImageProcessor p2;

		p2 = std::move(p1); // Call the move assignment operator

		REQUIRE(p2.getImageWidth() == MODEL_WIDTH);
		REQUIRE(p2.getImageHeight() == MODEL_HEIGHT);

		REQUIRE(imageAdr == p2.getImage());				  // Here we make sure that p2 is managing the bit of memory that p1 used to manage
		REQUIRE(imagesEqual(p2.getImage(), MODEL_IMAGE)); // Here we make sure that the value of the image has not changed

		REQUIRE(p1.getImage() == nullptr); // Here we make sure that p1 is no longer managing any memory

		// Check the self assignment case
		p2 = std::move(p2);

		REQUIRE(p2.getImage() == imageAdr);				  // Make sure that the memory address of p1.image has not changed
		REQUIRE(imagesEqual(p2.getImage(), MODEL_IMAGE)); // and that it has the same value
	}

	SECTION("Exctracting Components")
	{
		{
			PGMImageProcessor p1("chess.pgm");
			int components = p1.extractComponents(1, 0);
			REQUIRE(components == 1); // one big connected component, containing all the pixels
		}
		{
			PGMImageProcessor p1("chess.pgm");
			int components = p1.extractComponents(180, 1000);
			REQUIRE(components == 6); // 6 big connected components, which are the 6 chess pieces
		}
	}

	SECTION("Filtering Components")
	{
		{
			PGMImageProcessor p1("chess.pgm");
			int components = p1.extractComponents(1, 0);
			components = p1.filterComponentsBySize(0, MODEL_WIDTH * MODEL_HEIGHT); // shouldnt change anything
			REQUIRE(components == 1);											   // one big connected component, containing all the pixels
		}

		PGMImageProcessor p1("chess.pgm");
		int components = p1.extractComponents(180, 1000);

		components = p1.filterComponentsBySize(5000, MODEL_WIDTH * MODEL_HEIGHT); // should filter out pawn, which has size < 5000
		REQUIRE(components == 5);												  // 6 pieces subtract 1 pawn

		components = p1.filterComponentsBySize(7000, MODEL_WIDTH * MODEL_HEIGHT); // should filter out pawn, rook and bishop which have sizes < 7000
		REQUIRE(components == 3);												  // 6 pieces subtract 3 pieces

		components = p1.filterComponentsBySize(8000, MODEL_WIDTH * MODEL_HEIGHT); // should filter out all pieces which have sizes < 7000
		REQUIRE(components == 1);												  // all pieces are filtered, except knight
	}

	SECTION("Writing Components")
	{
		PGMImageProcessor p1("chess.pgm");
		p1.extractComponents(180, 1000);
		REQUIRE(p1.writeComponents("out.pgm"));
	}

	SECTION("Accessor Methods")
	{
		PGMImageProcessor p1("chess.pgm");
		p1.extractComponents(180, 1000);

		// values retrieved from running findcomp.exe with above params
		REQUIRE(p1.getComponentCount() == 6);
		REQUIRE(p1.getLargestSize() == 8008);
		REQUIRE(p1.getSmallestSize() == 4334);
		REQUIRE(p1.getImageHeight() == MODEL_HEIGHT);
		REQUIRE(p1.getImageWidth() == MODEL_WIDTH);
	}
}

// Tests for the ConnectedComponent Class
TEST_CASE("ConnectedComponent Unit Tests")
{
	SECTION("Default Constructor")
	{
		ConnectedComponent c;
		REQUIRE(c.getId() == -1);
		REQUIRE(c.getPixelCount() == 0);
		REQUIRE(c.getPixels().size() == 0);
	}

	SECTION("Custom Constructor")
	{
		ConnectedComponent c(1);

		REQUIRE(c.getId() == 1);
		REQUIRE(c.getPixelCount() == 0);
		REQUIRE(c.getPixels().size() == 0);
	}

	SECTION("Copy Constructor")
	{
		ConnectedComponent p1(1);
		ConnectedComponent p2(p1); // Call the copy constructor

		REQUIRE(p1.getId() == p2.getId());
		REQUIRE(p1.getPixelCount() == p2.getPixelCount());

		REQUIRE(pixelsEqual(p1.getPixels(), p2.getPixels()));
	}
	SECTION("Move Constructor")
	{
		ConnectedComponent p1(1);
		std::vector<coord> pixels = p1.getPixels();
		ConnectedComponent p2(std::move(p1));

		// Test for p1
		REQUIRE(p1.getPixels().empty()); // We need to make sure that p1 is no longer managing any dynamic memory (since we moved it)

		// Test for p2
		REQUIRE(p2.getId() == 1);
		REQUIRE(p2.getPixelCount() == 0);
		REQUIRE(pixelsEqual(p2.getPixels(), pixels));
	}

	SECTION("Copy Assignment Operator")
	{
		ConnectedComponent p1(1);
		ConnectedComponent p2;

		p2 = p1; // Call the copy assignment operator

		REQUIRE(p1.getId() == p2.getId());
		REQUIRE(p1.getPixelCount() == p2.getPixelCount());

		REQUIRE(&p1.getPixels() != &p2.getPixels());		  // Make sure that they are NOT pointing to the same bit of memory
		REQUIRE(pixelsEqual(p1.getPixels(), p2.getPixels())); // But make sure they have the same value

		// Check the self assignment case
		std::vector<coord> *pAddr = &p1.getPixels();

		p1 = p1;

		REQUIRE(&p1.getPixels() == pAddr); // Make sure that the memory address of p1.image has not changed
	}

	SECTION("Move Assignment Operator")
	{
		ConnectedComponent p1(1);

		ConnectedComponent p2;
		p2 = std::move(p1); // Call the move assignment operator

		REQUIRE(p2.getId() == 1);
		REQUIRE(p2.getPixelCount() == 0);

		REQUIRE(p1.getId() == -1);

		// Check the self assignment case
		p2 = std::move(p2);
		REQUIRE(p2.getId() == 1);
		REQUIRE(p2.getPixelCount() == 0);
	}

	SECTION("Add Pixel")
	{
		ConnectedComponent p1(1);
		p1.addPixel(1, 1);
		bool correct = p1.getPixels()[0].x == 1 && p1.getPixels()[0].y == 1;
		REQUIRE(correct);
	}

	SECTION("Pixel Count")
	{
		ConnectedComponent p1(1);
		p1.addPixel(1, 1);
		REQUIRE(p1.getPixelCount() == 1);
		p1.addPixel(2, 2);
		REQUIRE(p1.getPixelCount() == 2);
		p1.addPixel(3, 3);
		REQUIRE(p1.getPixelCount() == 3);
	}

	SECTION("Accessor Methods")
	{
		ConnectedComponent p1(1);
		REQUIRE(p1.getId() == 1);
	}
}