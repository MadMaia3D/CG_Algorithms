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
		for (const Ray& r : rays) {
			r.Draw(pMap, gfx, c);
		}
	}

	const std::vector<Ray> *GetRays() const {
		return &rays;
	}

private:
	const Map *pMap;
	Player *pPlayer;
	const float FOV = MathUtilities::ToRadians(60.0f);
	std::vector<Ray> rays;
	const int nRays = Graphics::ScreenWidth;
};