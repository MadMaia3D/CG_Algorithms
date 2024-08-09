/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd) {
}

void Game::Go() {
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel() {
}

void Game::ComposeFrame() {
	const Vec2 screenOffset = { Graphics::ScreenWidth / 2.0f ,Graphics::ScreenHeight / 2.0f };
	constexpr float cameraSpeed = 600.0f;
	constexpr float zoomSpeed = 5.0f;

	const float deltaTime = timer.Mark();

	while (!wnd.mouse.IsEmpty()) {
		Mouse::Event e = wnd.mouse.Read();

		switch (e.GetType()) {
		case Mouse::Event::Type::WheelUp:
			zoomLevel += zoomSpeed * zoomLevel * deltaTime;
			break;
		case Mouse::Event::Type::WheelDown:
			zoomLevel -= zoomSpeed * zoomLevel* deltaTime;
			break;
		default:
			break;
		}
	}

	Vec2 cameraInput = { 0,0 };
	if (wnd.kbd.KeyIsPressed('A')) {
		cameraInput.x -= 1.0f;
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		cameraInput.x += 1.0f;
	}
	if (wnd.kbd.KeyIsPressed('W')) {
		cameraInput.y -= 1.0f;
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		cameraInput.y += 1.0f;
	}
	zoomLevel = std::clamp(zoomLevel, 0.1f, 4.0f);
	cameraPos += cameraInput / zoomLevel * cameraSpeed * deltaTime;


	for (int sy = 0; sy < Graphics::ScreenHeight; sy++) {
		for (int sx = 0; sx < Graphics::ScreenWidth; sx++) {
			Vec2 result = { (float)sx, (float)sy };
			result -= screenOffset;
			result /= zoomLevel;
			result += cameraPos;
			result += Vec2{ 128, 128 };

			if (result.x > 0.0f && result.x < 255.0f && result.y > 0.0f && result.y < 255.0f) {
				result = result / 255.0f;
				float sampleX = fmod(result.x, 1.0f);
				float sampleY = fmod(result.y, 1.0f);
				if (sampleX < 0) { sampleX += 1.0f; };
				if (sampleY < 0) { sampleY += 1.0f; };
				Color sampledColor = texture.GetPixel(int(sampleX * tWidth), int(sampleY * tHeight));

				gfx.PutPixel(sx, sy, sampledColor);
			}
		}
	}
}
