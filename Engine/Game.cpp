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
#include "Grid.h"

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
	const float deltaTime = timer.Mark();

	playerDirection = { cos(playerAngle), sin(playerAngle) };

	constexpr float rotationSpeed = 2.0f;
	if (wnd.kbd.KeyIsPressed('A')) {
		playerAngle -= rotationSpeed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		playerAngle += rotationSpeed * deltaTime;
	}

	if (wnd.kbd.KeyIsPressed('W')) {
		playerPos += playerDirection * 100 * deltaTime;
	}
}

void Game::ComposeFrame() {
	Grid grid({ 100, 100 }, 14, 10, 40, 40);
	grid.DrawFilled(Colors::Black, { 150 ,150 ,150 }, gfx);

	gfx.DrawCircle((Vei2)playerPos, 5, Colors::Red);
	gfx.DrawLine(playerPos, playerPos + playerDirection * 100, Colors::Red);
}
