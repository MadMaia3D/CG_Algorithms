#pragma once
#include "Raycaster.h"


#define RECTILINEAR_LENS 0b0000
#define FISH_EYE_LENS 0b0001


void RederRayscastAsFPS(const Raycaster *pCaster, Graphics& gfx, unsigned int renderOptions = RECTILINEAR_LENS) {
	const std::vector<Ray> *pRays = pCaster->GetRays();

	const int nRays = pRays->size();

	constexpr unsigned int screenHeight = Graphics::ScreenHeight;
	constexpr unsigned int screenWidth = Graphics::ScreenWidth;
	constexpr float wallHeight = 0.5f;
	const float FOV = pCaster->GetFOV();
	const float focalLength = (screenWidth / 2.0f) / tan(FOV / 2);

	const float columnWidth = screenWidth / (float)nRays;

	const float viewAngle = pCaster->GetAngle();

	for (int i = 0; i < nRays; i++) {
		const HitInfo *pHit = (*pRays)[i].GetHitInfo();

		float hitDistance = pHit->hitDistance;
		if (!(renderOptions & FISH_EYE_LENS)) {
			hitDistance *= cos(viewAngle - pHit->angleRad);
		}
		const float projWallHeight = (wallHeight / hitDistance) * focalLength;

		if (projWallHeight < screenHeight) {
			const float startY = screenHeight / 2 - projWallHeight / 2.0f;
			gfx.DrawRectangle((int)startY, i * (int)columnWidth, (int)columnWidth, (int)projWallHeight, pHit->color);
		} else {
			gfx.DrawRectangle(0, i * (int)columnWidth, (int)columnWidth, screenHeight, pHit->color);
		}
	}
}