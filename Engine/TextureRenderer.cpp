#include "TextureRenderer.h"

void TransformCamera(CameraTransform * pCam, Vec2 deltaPosition, float deltaZoom) {
	pCam->zoomLevel += deltaZoom * pCam->zoomLevel;
	pCam->zoomLevel = std::clamp(pCam->zoomLevel, 0.1f, 64.0f);
	pCam->cameraPos += deltaPosition / pCam->zoomLevel;
}

Vec2 TransformToCameraSpace(Vec2 point, const CameraTransform * ct) {
	constexpr Vec2 screenOffset(Graphics::ScreenWidth / 2.0f, Graphics::ScreenHeight / 2.0f);
	point -= screenOffset;
	point /= ct->zoomLevel;
	point += ct->cameraPos;
	return point;
}

void RenderTexture(CameraTransform &ct, const Surface & texture, Graphics & gfx, unsigned int options) {
	const unsigned int tWidth = texture.GetWidth();
	const unsigned int tHeight = texture.GetHeight();

	for (int sy = 0; sy < Graphics::ScreenHeight; sy++) {
		for (int sx = 0; sx < Graphics::ScreenWidth; sx++) {
			Vec2 result = { (float)sx, (float)sy };

			result = TransformToCameraSpace(result, &ct);
			result = result / 255.0f;		// normalized texture coordinates
			result += Vec2{ 0.5f, 0.5f };		// square center to center of screen

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
