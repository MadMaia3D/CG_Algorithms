#pragma once
#include "Graphics.h"
#include "Keyboard.h"
#include "Vector2.h"
#include "MathUtilities.h"

class Player {
public:
	Player(Vec2 position)
		:
		position(position) {
	}

	Vec2 GetPosition() const {
		return position;
	}

	float GetAngle() const {
		return angle;
	}

	void ProcessInput(Keyboard& kbd, float deltaTime) {
		float rotationDirection = 0.0f;
		float walkDirection = 0.0f;

		if (kbd.KeyIsPressed('A')) { rotationDirection -= 1; }
		if (kbd.KeyIsPressed('D')) { rotationDirection += 1; }

		angle += rotationDirection * rotationSpeed * deltaTime;
		direction = { cos(angle), sin(angle) };

		if (kbd.KeyIsPressed('W')) {
			walkDirection += 1;
		} else if (kbd.KeyIsPressed('S')) {
			walkDirection -= 1;
			rotationDirection = -rotationDirection;
		}

		position += direction * walkSpeed * walkDirection * deltaTime;
	}
	void Draw(Graphics& gfx, Color color) const {
		gfx.DrawCircle((Vei2)position, 5, color);
		gfx.DrawLineClamped(position, position + direction * 250, color);
	}
private:
	Vec2 position;
	Vec2 direction = { 1.0f, 0.0f };
	float angle = -3.14f / 2;
	static constexpr float rotationSpeed = 2.0f;
	static constexpr float walkSpeed = 100.0f;
};