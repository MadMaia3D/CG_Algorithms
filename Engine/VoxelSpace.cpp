#include "VoxelSpace.h"

void PreventUndergroundCamera(Camera * pCam, MapData * pMapData) {
	Vec2 currentMapPos = pCam->pos;
	const Surface &heightMap = *pMapData->pHeightMap;

	while (currentMapPos.x < 0.0f) { currentMapPos.x += heightMap.GetWidth(); }
	while (currentMapPos.x >= heightMap.GetWidth()) { currentMapPos.x -= heightMap.GetWidth(); }
	while (currentMapPos.y < 0.0f) { currentMapPos.y += heightMap.GetHeight(); }
	while (currentMapPos.y >= heightMap.GetHeight()) { currentMapPos.y -= heightMap.GetHeight(); }

	const int rawHeight = heightMap.GetPixel((int)currentMapPos.x, (int)currentMapPos.y).GetR();

	if (pCam->height < rawHeight) {
		pCam->target_height = (float)rawHeight;
		pCam->height = (float)rawHeight;
	}
}

Vec2 GetRotated90Clockwise(Vec2 vector) {
	return { vector.y, -vector.x };
}

Vec2 GetRotated90CounterClockwise(Vec2 vector) {
	return GetRotated90Clockwise(-vector);
}

void RenderVoxelMap(const MapData *pMapData, const Camera *pCam, Graphics & gfx, const Vec3 fogColor) {
	const Surface &heightMap = *pMapData->pHeightMap;
	const Surface &colorMap = *pMapData->pColorMap;
	const Camera &cam = *pCam;
	const int wResolution = gfx.ScreenWidth;
	constexpr float terrainHeightFactor = 400.0f;

	// calculate camera front and right vectors
	const Vec2 cameraFront = Vec2(cos(cam.angle), sin(cam.angle));
	const Vec2 cameraRight = GetRotated90Clockwise(cameraFront);

	// use cam position plus camera fron and right vectors to calculate view far left and far right
	const Vec2 lPoint = cam.pos + cameraFront * cam.zfar + cameraRight * cam.zfar;
	const Vec2 rPoint = cam.pos + cameraFront * cam.zfar - cameraRight * cam.zfar;

	const Vec2 deltaX = (rPoint - lPoint) / (float)wResolution;

	for (int i = 0; i < wResolution; i++) {
		const Vec2 target = lPoint + deltaX * (float)i; // represents the furthest point that the camera can see in the current pixel column
		const Vec2 deltaY = (target - cam.pos) / cam.zfar; // represents each step from the camera position to the target

		int maxHeight = Graphics::ScreenHeight - 1;
		for (int z = 1; z < cam.zfar; z++) {
			const Vec2 currentPosition = cam.pos + deltaY * (float)z;
			Vec2 currentMapPosWrapped = currentPosition;

			while (currentMapPosWrapped.x < 0.0f) { currentMapPosWrapped.x += heightMap.GetWidth(); }
			while (currentMapPosWrapped.x >= heightMap.GetWidth()) { currentMapPosWrapped.x -= heightMap.GetWidth(); }
			while (currentMapPosWrapped.y < 0.0f) { currentMapPosWrapped.y += heightMap.GetHeight(); }
			while (currentMapPosWrapped.y >= heightMap.GetHeight()) { currentMapPosWrapped.y -= heightMap.GetHeight(); }

			const int rawHeight = heightMap.GetPixel((int)currentMapPosWrapped.x, (int)currentMapPosWrapped.y).GetR();
			// this is the "magic" formula for the algorithm
			int heightOnScreen = int((cam.height - rawHeight) * terrainHeightFactor / z + cam.pitch);

			if (heightOnScreen < 0) { heightOnScreen = 0; }
			if (heightOnScreen >= Graphics::ScreenHeight) { heightOnScreen = Graphics::ScreenHeight - 1; }

			if (heightOnScreen < maxHeight) {
				// fog factor is calculate by dividing the current distance to camera by the current target distance
				const float fogIntensity = cam.pos.DistanceToSqr(currentPosition) / cam.pos.DistanceToSqr(target);

				const Vec3 terrainColor = Vec3(colorMap.GetPixel((int)currentMapPosWrapped.x, (int)currentMapPosWrapped.y));
				const Vec3 shaderColor = Interpolate(terrainColor, fogColor, fogIntensity);
				for (int h = heightOnScreen; h < maxHeight; h++) {
					gfx.PutPixel(i, h, Color(shaderColor));
				}
				maxHeight = heightOnScreen;
			}
		}
	}
}
