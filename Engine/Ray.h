#pragma once
#include <vector>
#include "Player.h"
#include "Map.h"

class Ray {
public:
	Ray(Vec2 position, float radians)
		:
		position(position),
		angle(radians) {
		angle = MathUtilities::WrapAngleRad(angle);
		direction = { cos(angle), sin(angle) };
		isFacingUp = angle > PI;
		isFacingDown = angle < PI;
		isFacingLeft = angle > PI/2 && angle < 3 * PI/2;
		isFacingRight = angle > 3 * PI / 2 || angle < PI / 2;
	}

	void Cast(const Map *pMap) {
		CheckHorizontalCollisions(pMap);
		CheckVerticalCollisions(pMap);
	}

	void CheckHorizontalCollisions(const Map *pMap) {
		const int cellHeight = pMap->GetCellHeight();

		Vec2 origin = position / (float)cellHeight; // grid space
		Vec2 hHit1;
		Vec2 hHit2;
		const float tanTheta = tan(angle);

		if (isFacingDown) {
			hHit1.y = ceil(origin.y);
			hHit2.y = hHit1.y + 1;
		} else if (isFacingUp) {
			hHit1.y = floor(origin.y);
			hHit2.y = hHit1.y - 1;
		} else {
			return;
		}

		hHit1.x = (hHit1.y - origin.y) / tanTheta + origin.x;
		hHit2.x = (hHit2.y - hHit1.y) / tanTheta + hHit1.x;
		const Vec2 hDelta = hHit2 - hHit1;

		Vec2 nextHit = hHit1;
		const int nRows = pMap->GetNRows();
		const int nColumns = pMap->GetNColumns();

		while (nextHit.y >= 0 && nextHit.y <= nRows && nextHit.x >= 0 && nextHit.x <= nColumns) {
			hits.push_back(nextHit);
			nextHit += hDelta;
		}
	}

	void CheckVerticalCollisions(const Map *pMap) {
		const int cellWidth = pMap->GetCellWidth();

		Vec2 origin = position / (float)cellWidth; // grid space
		Vec2 vHit1;
		Vec2 vHit2;
		const float tanTheta = tan(angle);

		if (isFacingRight) {
			vHit1.x = ceil(origin.x);
			vHit2.x = vHit1.x + 1;
		} else if (isFacingLeft) {
			vHit1.x = floor(origin.x);
			vHit2.x = vHit1.x - 1;
		}

		vHit1.y = (vHit1.x - origin.x) * tanTheta + origin.y;
		vHit2.y = (vHit2.x - vHit1.x) * tanTheta + vHit1.y;
		const Vec2 vDelta = vHit2 - vHit1;

		Vec2 nextHit = vHit1;
		const int nRows = pMap->GetNRows();
		const int nColumns = pMap->GetNColumns();

		while (nextHit.y >= 0 && nextHit.y <= nRows && nextHit.x >= 0 && nextHit.x <= nColumns) {
			hits.push_back(nextHit);
			nextHit += vDelta;
		}
	}

	void Draw(const Map *pMap, Graphics& gfx, Color c) const {
		float distance = 200.0f;
		gfx.DrawLineClamped(position, position + direction * distance, c);
		for (const Vec2& hit : hits) {
			Vec2 drawPos = { hit.x * pMap->GetCellHeight(), hit.y * pMap->GetCellHeight() };
			gfx.DrawCircle( Vei2(drawPos), 3, Colors::Magenta);
		}
	}

private:
	Vec2 position;
	float angle;
	Vec2 direction;
	static constexpr int maxHits = 10;
	std::vector<Vec2> hits;

	bool isFacingUp = angle > PI;
	bool isFacingDown = !isFacingUp;
	bool isFacingLeft = angle > PI && angle < 3 * PI / 2;
	bool isFacingRight = !isFacingLeft;
};




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
private:
	const Map *pMap;
	Player *pPlayer;
	const float FOV = MathUtilities::ToRadians(60.0f);
	std::vector<Ray> rays;
	const int nRays = 16;
};
