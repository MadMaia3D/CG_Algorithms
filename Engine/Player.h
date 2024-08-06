#pragma once
#include "Graphics.h"
#include "Keyboard.h"
#include "Vector2.h"

class Player {
public:
	Player(Vec2 position)
	:
		position(position)
	{}
	void ProcessInput(Keyboard& kbd, float deltaTime) {
		direction = { cos(angle), sin(angle) };

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
	}
	void Draw(Graphics& gfx, Color color) const {
		gfx.DrawCircle((Vei2)position, 5, color);
		gfx.DrawLine(position, position + direction * 50, color);
	}
private:
	Vec2 position;
	Vec2 direction = { 1.0f, 0.0f };
	float angle = 0.0f;
	static constexpr float rotationSpeed = 2.0f;
	static constexpr float walkSpeed = 100.0f;
};