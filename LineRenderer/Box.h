#pragma once
#include "Utilities.h"
#include "float.h"

class Box
{
public:
	Box() = default;

private:
	
	float xMAX = -FLT_MAX;
	float xMIN = FLT_MAX;
	float yMAX = -FLT_MAX;
	float yMIN = FLT_MAX;
};

