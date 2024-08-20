#pragma once
#include "Graphics.h"

struct Camera {
	Vec2 pos;
	float zfar = 512.0f;
	float height = 100.0f;
	float angle = -PI/2;
	float pitch = 250.0f;
};

struct MapData {
	const Surface *pHeightMap;
	const Surface *pColorMap;
};

#define EXCLUSIVE_LOWER 0b0001
#define EXCLUSIVE_UPPER 0b0010
#define INCLUSIVE_LOWER 0b0100
#define INCLUSIVE_UPPER 0b1000

template <typename T>
bool IsBetween(T value, T lowerLimit, T upperLimit, int options = EXCLUSIVE_LOWER | EXCLUSIVE_UPPER) {
	if (value < lowerLimit) { return false; }
	if (!(options & INCLUSIVE_LOWER) && value == lowerLimit) { return false; }
	if (value > upperLimit) { return false; }
	if (!(options & INCLUSIVE_UPPER) && value == upperLimit) { return false; }
	return true;
}

Vec2 GetRotated90Clockwise(Vec2 vector);
void RenderVoxelMap(const MapData *pMapData, const Camera *pCam, Graphics & gfx);