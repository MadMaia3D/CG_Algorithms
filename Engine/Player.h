#pragma once
#include "Graphics.h"
#include "Keyboard.h"
#include "Vector2.h"
#include "MathUtilities.h"

class Player {
public:
	Player(Vec2 position)
	:
		position(position)
	{}

	Vec2 GetPosition() const {
		return position;
	}

	float GetAngle() const {
		return angle;
	}

	void ProcessInput(Keyboard& kbd, float deltaTime) {
		if (kbd.KeyIsPressed('A')) {
			angle -= rotationSpeed * deltaTime;
		}
		if (kbd.KeyIsPressed('D')) {
			angle += rotationSpeed * deltaTime;
		}
		if (kbd.KeyIsPressed('W')) {
			position += direction * walkSpeed * deltaTime;
		}
		if (kbd.KeyIsPressed('S')) {
			position -= direction * walkSpeed * deltaTime;
		}
		direction = { cos(angle), sin(angle) };
	}
	void Draw(Graphics& gfx, Color color) const {
		gfx.DrawCircle((Vei2)position, 5, color);
		gfx.DrawLineClamped(position, position + direction * 50, color);
	}
private:
	Vec2 position;
	Vec2 direction = { 1.0f, 0.0f };
	float angle = -3.14f/2;
	static constexpr float rotationSpeed = 2.0f;
	static constexpr float walkSpeed = 100.0f;
};