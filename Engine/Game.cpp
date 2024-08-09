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
	constexpr float zoomSpeed = 0.25f;

	const float deltaTime = timer.Mark();

	while (!wnd.mouse.IsEmpty()) {
		Mouse::Event e = wnd.mouse.Read();

		switch (e.GetType()) {
		case Mouse::Event::Type::WheelUp:
			ct.zoomLevel += zoomSpeed * ct.zoomLevel;
			break;
		case Mouse::Event::Type::WheelDown:
			ct.zoomLevel -= zoomSpeed * ct.zoomLevel;
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
	ct.zoomLevel = std::clamp(ct.zoomLevel, 0.1f, 64.0f);
	ct.cameraPos += cameraInput / ct.zoomLevel * cameraSpeed * deltaTime;

	RenderTexture(ct, texture, gfx, CLAMP_TO_BORDER_Y | CLAMP_TO_BORDER_X);
}
