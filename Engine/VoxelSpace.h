#pragma once
#include "Surface.h"
#include "Vector2.h"

struct Camera {
	Vec2 pos;
	float zfar;
};

void TestRender(const Surface &heightMap, const Surface& colorMap, int wResolution, Graphics& gfx) {
	const Camera cam = { {512, 512}, 512 };
	const Vec2 lPoint = { cam.pos.x - cam.zfar, cam.pos.y - cam.zfar };
	const Vec2 rPoint = { cam.pos.x + cam.zfar, cam.pos.y - cam.zfar };

	const Vec2 deltaX = (rPoint - lPoint) / (float)wResolution;

	for (int i = 0; i < wResolution; i++) {
		Vec2 target = lPoint + deltaX * (float)i;
		Vec2 deltaY = (target - cam.pos) / cam.zfar;

		int maxHeight = Graphics::ScreenHeight - 1;
		for (int z = 1; z < cam.zfar; z++) {
			Vec2 currentPos = cam.pos + deltaY * (float)z;

			int currentHeight = 400 - heightMap.GetPixel((int)currentPos.x, (int)currentPos.y).GetR();

			if (currentHeight < 0) { currentHeight = 0; }
			if (currentHeight >= Graphics::ScreenHeight) { currentHeight = Graphics::ScreenHeight - 1; }

			if (currentHeight < maxHeight) {
				Color color = colorMap.GetPixel((int)currentPos.x, (int)currentPos.y);
				for (int h = currentHeight; h < maxHeight; h++) {
					gfx.PutPixel(i, h, color);
				}
				maxHeight = currentHeight;
			}
		}
	}
}