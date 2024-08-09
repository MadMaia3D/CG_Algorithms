#pragma once
#include "Graphics.h"

#define CLAMP_TO_BORDER_Y 0b0001
#define CLAMP_TO_BORDER_X 0b0010
#define CLAMP_TO_EDGE_X 0b0100
#define CLAMP_TO_EDGE_Y 0b1000

void RenderTexture(Vec2 cameraPos, float zoomLevel, const Surface &texture, Graphics& gfx, unsigned int options = 0) {
	const Vec2 screenOffset = { Graphics::ScreenWidth / 2.0f ,Graphics::ScreenHeight / 2.0f };
	zoomLevel = std::clamp(zoomLevel, 0.1f, 64.0f);
	const unsigned int tWidth = texture.GetWidth();
	const unsigned int tHeight = texture.GetHeight();

	for (int sy = 0; sy < Graphics::ScreenHeight; sy++) {
		for (int sx = 0; sx < Graphics::ScreenWidth; sx++) {
			Vec2 result = { (float)sx, (float)sy };
			result -= screenOffset;
			result /= zoomLevel;
			result += cameraPos;
			result += Vec2{ 128, 128 };
			result = result / 255.0f;

			if ((options & CLAMP_TO_BORDER_Y) && (result.y < 0.0f || result.y > 1.0f)) { continue; }

			if ((options & CLAMP_TO_BORDER_X) && (result.x < 0.0f || result.x > 1.0f)) { continue; }

			if ((options & CLAMP_TO_EDGE_X)) { result.x = std::clamp(result.x, 0.0f, 1.0f); }

			if ((options & CLAMP_TO_EDGE_Y)) { result.y = std::clamp(result.y, 0.0f, 1.0f); }

				float sampleX = fmod(result.x, 1.0f);
				float sampleY = fmod(result.y, 1.0f);

				if (sampleX < 0) { sampleX += 1.0f; };
				if (sampleY < 0) { sampleY += 1.0f; };
				Color sampledColor = texture.GetPixel(int(sampleX * tWidth), int(sampleY * tHeight));
				gfx.PutPixel(sx, sy, sampledColor);
		}
	}
}