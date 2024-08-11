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
	constexpr float rotationSpeed = 1.2f;
	constexpr float liftSpeed = 120.0f;

	if (wnd.kbd.KeyIsPressed('W')) {
		cam.pos.x += cos(cam.angle) * moveSpeed * deltaTime;
		cam.pos.y += sin(cam.angle) * moveSpeed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		cam.pos.x -= cos(cam.angle) * moveSpeed * deltaTime;
		cam.pos.y -= sin(cam.angle) * moveSpeed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed('A')) {
		cam.angle -= rotationSpeed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		cam.angle += rotationSpeed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed(VK_CONTROL)) {
		cam.height -= liftSpeed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
		cam.height += liftSpeed * deltaTime;
	}
}

void Game::ComposeFrame() {
	RenderVoxelMap(&mapData, &cam, gfx);
}
