#pragma once
#include <string>

class Grid {
private:

	unsigned int* data;

public:
	Grid() = default;
	Grid(std::string filename);

	//int& At(int xCoord, int yCoord);
	//int& AtWrap(int xCoord, int yCoord);
	//int& AtClamp(int xCoord, int yCoord);

	~Grid();
	Grid(Grid& other) = delete;
	Grid& operator=(Grid& other) = delete;
};