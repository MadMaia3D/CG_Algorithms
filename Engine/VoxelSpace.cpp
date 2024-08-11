#include "VoxelSpace.h"

Vec2 GetRotate90Clockwise(Vec2 vector) {
	return { vector.y, -vector.x };
}

void RenderVoxelMap(const MapData *pMapData, const Camera *pCam, Graphics & gfx) {
	const Surface &heightMap = *pMapData->pHeightMap;
	const Surface &colorMap = *pMapData->pColorMap;
	const Camera &cam = *pCam;
	const int wResolution = gfx.ScreenWidth;
	constexpr float scaleFactor = 500.0f;

	// calculate camera front and right vectors
	const Vec2 cameraFront = Vec2(cos(cam.angle), sin(cam.angle));
	const Vec2 cameraRight = GetRotate90Clockwise(cameraFront);

	// use cam position plus camera fron and right vectors to calculate view far left and far right
	const Vec2 lPoint = cam.pos + cameraFront * cam.zfar + cameraRight * cam.zfar;
	const Vec2 rPoint = cam.pos + cameraFront * cam.zfar - cameraRight * cam.zfar;

	const Vec2 deltaX = (rPoint - lPoint) / (float)wResolution;

	for (int i = 0; i < wResolution; i++) {
		Vec2 target = lPoint + deltaX * (float)i;
		Vec2 deltaY = (target - cam.pos) / cam.zfar;

		int maxHeight = Graphics::ScreenHeight - 1;
		for (int z = 1; z < cam.zfar; z++) {
			Vec2 currentMapPos = cam.pos + deltaY * (float)z;

			while (currentMapPos.x < 0.0f) { currentMapPos.x += heightMap.GetWidth(); }
			while (currentMapPos.x >= heightMap.GetWidth()) { currentMapPos.x -= heightMap.GetWidth(); }
			while (currentMapPos.y < 0.0f) { currentMapPos.y += heightMap.GetHeight(); }
			while (currentMapPos.y >= heightMap.GetHeight()) { currentMapPos.y -= heightMap.GetHeight(); }

			int rawHeight = heightMap.GetPixel((int)currentMapPos.x, (int)currentMapPos.y).GetR();
			int heightOnScreen = int((cam.height - rawHeight) * scaleFactor / z);

			if (heightOnScreen < 0) { heightOnScreen = 0; }
			if (heightOnScreen >= Graphics::ScreenHeight) { heightOnScreen = Graphics::ScreenHeight - 1; }

			if (heightOnScreen < maxHeight) {
				Color color = colorMap.GetPixel((int)currentMapPos.x, (int)currentMapPos.y);
				for (int h = heightOnScreen; h < maxHeight; h++) {
					gfx.PutPixel(i, h, color);
				}
				maxHeight = heightOnScreen;
			}
		}
	}
}
