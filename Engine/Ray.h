#pragma once
#include <vector>
#include "Player.h"

class Ray {
public:
	Ray(Vec2 position, float radians)
		:
		position(position),
		angle(radians) {
		const float tau = 2 * PI;
		while (angle > tau) {
			angle -= tau;
		}
		while (angle < 0) {
			angle += tau;
		}
		direction = { cos(angle), sin(angle) };
	}

	void Cast() {
		int cellSize = 30;

		Vec2 origin = position / (float)cellSize; // grid space
		Vec2 hit1;
		Vec2 hit2;

		if (angle < PI) {
			hit1.y = ceil(position.y / cellSize);
			hit2.y = hit1.y + 1;
		} else if (angle > PI) {
			hit1.y = floor(position.y / cellSize);
			hit2.y = hit1.y - 1;
		} else {
			return;
		}

		float tanTheta = tan(angle);
		hit1.x = (hit1.y - origin.y) / tanTheta + origin.x;
		hit2.x = (hit2.y - hit1.y) / tanTheta + hit1.x;
		const Vec2 delta = hit2 - hit1;

		hits.reserve(maxHits);
		hits.push_back(hit1);
		hits.push_back(hit2);

		for (size_t i = hits.size(); i < hits.capacity(); i++) {
			hits.emplace_back(hits.back() + delta);
		}

	}

	void Draw(Graphics& gfx, Color c) const {
		float distance = 100.0f;
		gfx.DrawLineClamped(position, position + direction * distance, c);
		for (const Vec2& hit : hits) {
			gfx.DrawCircle((Vei2)(hit * 30), 5, Colors::Magenta);
		}
	}

private:
	Vec2 position;
	float angle;
	Vec2 direction;
	static constexpr int maxHits = 10;
	std::vector<Vec2> hits;
};




class Raycaster {
public:
	Raycaster(Player *pPlayer)
		:
		pPlayer(pPlayer)
	{
		rays.reserve(nRays);
	}

	void CastAllRays() {
		rays.clear();
		float rayAngle = pPlayer->GetAngle() - FOV / 2;
		const float rayAngleStep = FOV / (nRays - 1);

		for (int i = 0; i < nRays; i++) {
			rays.emplace_back(pPlayer->GetPosition(), rayAngle);
			rayAngle += rayAngleStep;
			rays.back().Cast();
		}

	}

	void Draw(Graphics& gfx, Color c) {
		for (const Ray& r : rays) {
			r.Draw(gfx, c);
		}
	}
private:
	const float FOV = MathUtilities::ToRadians(60.0f);
	const int nRays = 8;
	Player *pPlayer;
	std::vector<Ray> rays;
};
