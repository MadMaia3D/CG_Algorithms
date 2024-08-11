#include "VoxelSpace.h"

void RenderVoxelMap(const MapData *pMapData, const Camera *pCam, Graphics & gfx) {
	const Surface &heightMap = *pMapData->pHeightMap;
	const Surface &colorMap = *pMapData->pColorMap;
	const Camera &cam = *pCam;
	const int wResolution = gfx.ScreenWidth;
	constexpr float scaleFactor = 200.0f;

	const Vec2 lPoint = { cam.pos.x - cam.zfar, cam.pos.y - cam.zfar };
	const Vec2 rPoint = { cam.pos.x + cam.zfar, cam.pos.y - cam.zfar };

	const Vec2 deltaX = (rPoint - lPoint) / (float)wResolution;

	for (int i = 0; i < wResolution; i++) {
		Vec2 target = lPoint + deltaX * (float)i;
		Vec2 deltaY = (target - cam.pos) / cam.zfar;

		int maxHeight = Graphics::ScreenHeight - 1;
		for (int z = 1; z < cam.zfar; z++) {
			Vec2 currentMapPos = cam.pos + deltaY * (float)z;

			if (!IsBetween((int)currentMapPos.x, 0, (int)heightMap.GetWidth())) {
				continue;
			} else
				if (!IsBetween((int)currentMapPos.y, 0, (int)heightMap.GetHeight())) {
					continue;
				}

			int rawHeight = heightMap.GetPixel((int)currentMapPos.x, (int)currentMapPos.y).GetR();
			int heightOnScreen = int((scaleFactor - rawHeight) * scaleFactor / z);

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
