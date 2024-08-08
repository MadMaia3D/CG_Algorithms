#pragma once
#include <optional>
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

		const Vec2 origin = Map::ToMapSpace(pMap, position);

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

		const Vec2 origin = Map::ToMapSpace(pMap, position);
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

		Vec2 origin = Map::ToMapSpace(pMap, position);
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
		if (hit.has_value()) {
			Vec2 drawPos = Map::ToScreenSpace(pMap, hit.value());
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
