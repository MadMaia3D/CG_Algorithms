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
	gfx(wnd),
	hMap(Surface::FromFile(L"Textures/D1.png")),
	cMap(Surface::FromFile(L"Textures/C1W.png")) {
	mapData.pHeightMap = &hMap;
	mapData.pColorMap = &cMap;
	cam.zfar = 1024.0f;
}

void Game::Go() {
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel() {
	const float deltaTime = timer.Mark();
	constexpr float moveSpeed = 150.0f;
	constexpr float rotationSpeed = 1.4f;
	constexpr float liftSpeed = 120.0f;
	constexpr float pitchSpeed = 500.0f;

	while (!wnd.mouse.IsEmpty()) {
		Mouse::Event e = wnd.mouse.Read();

		switch (e.GetType()) {
		case Mouse::Event::Type::LPress:
			mouseTracker.StartTracking(wnd.mouse.GetPosVec2());
			break;
		case Mouse::Event::Type::LRelease:
			mouseTracker.StopTracking();
			break;
		}
	}
	const Vec2 trackedMouseMove = mouseTracker.Track(wnd.mouse.GetPosVec2());
	cam.target_angle -= trackedMouseMove.x / gfx.ScreenWidth * cam.FOV;
	cam.target_pitch += trackedMouseMove.y;

	if (wnd.kbd.KeyIsPressed('W')) {
		CameraMoveForward(&cam, moveSpeed, deltaTime);
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		CameraMoveBackward(&cam, moveSpeed, deltaTime);
	}
	if (wnd.kbd.KeyIsPressed('A')) {
		CameraStrafeLeft(&cam, moveSpeed, deltaTime);
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		CameraStrafeRight(&cam, moveSpeed, deltaTime);
	}
	if (wnd.kbd.KeyIsPressed(VK_CONTROL)) {
		cam.target_height -= liftSpeed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
		cam.target_height += liftSpeed * deltaTime;
	}
	UpdateCamera(&cam, deltaTime);
	PreventUndergroundCamera(&cam, &mapData);
}

void Game::ComposeFrame() {
	for (int y = 0; y < Graphics::ScreenHeight; y++) {
		for (int x = 0; x < Graphics::ScreenWidth; x++) {
			gfx.PutPixel(x, y, {128,200,255});
		}
	}
	RenderVoxelMap(&mapData, &cam, gfx);
}
