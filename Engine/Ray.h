#pragma once
#include <vector>
#include <optional>
#include <limits>
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
		isFacingLeft = angle > PI / 2 && angle < 3 * PI / 2;
		isFacingRight = angle > 3 * PI / 2 || angle < PI / 2;
	}

	void Cast(const Map *pMap) {
		std::optional<Vec2> hCollision = CheckHorizontalCollisions(pMap);
		std::optional<Vec2> vCollision = CheckVerticalCollisions(pMap);

		const bool hHasValue = hCollision.has_value();
		const bool vHasValue = vCollision.has_value();

		if (!hHasValue && !vHasValue) { return; }

		const int cellWidth = pMap->GetCellWidth();
		const int cellHeight = pMap->GetCellHeight();

		const Vec2 origin = { position.x / (float)cellWidth , position.y / (float)cellWidth };

		float horizontalDistanceSqr = std::numeric_limits<float>::infinity();
		float verticalDistanceSqr = std::numeric_limits<float>::infinity();

		if (hHasValue) {
			horizontalDistanceSqr = Vec2(origin - hCollision.value()).LenSq();
		}
		if (vHasValue) {
			verticalDistanceSqr = Vec2(origin - vCollision.value()).LenSq();
		}

		if (horizontalDistanceSqr < verticalDistanceSqr) {
			hit = *hCollision;
		} else {
			hit = *vCollision;
		}
	}

	std::optional<Vec2> CheckHorizontalCollisions(const Map *pMap) {
		const int cellHeight = pMap->GetCellHeight();

		const Vec2 origin = position / (float)cellHeight; // grid space
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
			return {};
		}

		hHit1.x = (hHit1.y - origin.y) / tanTheta + origin.x;
		hHit2.x = (hHit2.y - hHit1.y) / tanTheta + hHit1.x;
		const Vec2 hDelta = hHit2 - hHit1;

		Vec2 nextHit = hHit1;
		const int nRows = pMap->GetNRows();
		const int nColumns = pMap->GetNColumns();

		while (nextHit.y > 0 && nextHit.y < nRows && nextHit.x > 0 && nextHit.x < nColumns) {
			const int targetCellY = int(floor(nextHit.y + hDelta.y / 2));
			if (pMap->HasWallAt((int)nextHit.x, targetCellY)) {
				return nextHit;
			}
			nextHit += hDelta;
		}
		return {};
	}

	std::optional<Vec2> CheckVerticalCollisions(const Map *pMap) {
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
		} else {
			return {};
		}

		vHit1.y = (vHit1.x - origin.x) * tanTheta + origin.y;
		vHit2.y = (vHit2.x - vHit1.x) * tanTheta + vHit1.y;
		const Vec2 vDelta = vHit2 - vHit1;

		Vec2 nextHit = vHit1;
		const int nRows = pMap->GetNRows();
		const int nColumns = pMap->GetNColumns();

		while (nextHit.y > 0 && nextHit.y < nRows && nextHit.x > 0 && nextHit.x < nColumns) {
			const int targetCellX = int(floor(nextHit.x + vDelta.x / 2));
			if (pMap->HasWallAt(targetCellX, (int)nextHit.y)) {
				return nextHit;
			}
			nextHit += vDelta;
		}
		return {};
	}

	void Draw(const Map *pMap, Graphics& gfx, Color c) const {
		//const float distance = 200.0f;
		//gfx.DrawLineClamped(position, position + direction * distance, c);
		if (hit.has_value()) {
			Vec2 drawPos = { hit.value().x * pMap->GetCellWidth(), hit.value().y * pMap->GetCellHeight() };
			gfx.DrawCircle(Vei2(drawPos), 3, Colors::Magenta);
		}
	}

private:
	Vec2 position;
	float angle;
	Vec2 direction;
	static constexpr int maxHits = 10;
	std::optional<Vec2> hit;

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
		const Vec2 position = pPlayer->GetPosition();
		const float angle = pPlayer->GetAngle();
		const float rayAngle1 = angle - FOV / 2;
		const float rayAngle2 = angle + FOV / 2;
		const Vec2 direction1 = {cos(rayAngle1), sin(rayAngle1)};
		const Vec2 direction2 = {cos(rayAngle2), sin(rayAngle2)};
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
