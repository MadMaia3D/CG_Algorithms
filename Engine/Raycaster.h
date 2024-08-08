#pragma once
#include "Ray.h"

class Raycaster {
public:
	Raycaster(const Map *pMap, Player *pPlayer)
		:
		pMap(pMap),
		pPlayer(pPlayer) {
		rays.reserve(nRays);
	}

	void CastAllRays(const Map *pMap) {
		rays.clear();
		float rayAngle = pPlayer->GetAngle() - FOV / 2;
		const float rayAngleStep = FOV / (nRays - 1);

		for (int i = 0; i < nRays; i++) {
			rays.emplace_back(pPlayer->GetPosition(), rayAngle);
			rayAngle += rayAngleStep;
			rays.back().Cast(pMap);
		}
	}

	void Draw(const Map *pMap, Graphics& gfx, Color c) {
		const Vec2 position = pPlayer->GetPosition();
		const float angle = pPlayer->GetAngle();
		const float rayAngle1 = angle - FOV / 2;
		const float rayAngle2 = angle + FOV / 2;
		const Vec2 direction1 = { cos(rayAngle1), sin(rayAngle1) };
		const Vec2 direction2 = { cos(rayAngle2), sin(rayAngle2) };
		const float distance = 100.0f;

		gfx.DrawLineClamped(position, position + direction1 * distance, c);
		gfx.DrawLineClamped(position, position + direction2 * distance, c);

		for (const Ray& r : rays) {
			r.Draw(pMap, gfx, c);
		}
	}
private:
	const Map *pMap;
	Player *pPlayer;
	const float FOV = MathUtilities::ToRadians(60.0f);
	std::vector<Ray> rays;
	const int nRays = 128;
};