#include "Grid.h"

#define STBI_IMAGE_IMPLEMENTAION
#include "stb_image.h"

#include <iostream>

Grid::Grid(std::string filename)
{
	/*int width, height, channelCount;
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channelCount, 3);

	data = new unsigned int[width * height];

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			data[x + y * width] = *((unsigned int*)(imageData + (x + y * width) * 4));
		}
	}

	stbi_image_free(imageData);*/
}

Grid::~Grid()
{
	//delete[] data;
}
