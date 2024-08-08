#pragma once
#include "Raycaster.h"

void RenderRaycasting(const Raycaster *pCaster, Graphics& gfx) {
	const std::vector<Ray> *pRays = pCaster->GetRays();

	const int nRays = pRays->size();

	constexpr unsigned int screenHeight = Graphics::ScreenHeight;
	constexpr unsigned int screenWidth = Graphics::ScreenWidth;

	const float columnWidth = screenWidth / (float)nRays;

	for (int i = 0; i < nRays; i++) {
		const HitInfo *pHit = (*pRays)[i].GetHitInfo();
		const float hitDistance = pHit->hitDistance;
		const float size = screenHeight / hitDistance;

		if (size < screenHeight) {
			const float startY = screenHeight / 2 - size / 2.0f;
			gfx.DrawRectangle((int)startY, i * (int)columnWidth, (int)columnWidth, (int)size, pHit->color);
		} else {
			gfx.DrawRectangle(0, i * (int)columnWidth, (int)columnWidth, screenHeight, pHit->color);
		}
	}
}