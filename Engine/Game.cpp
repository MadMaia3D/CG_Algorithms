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
	const float deltaTime = frameTimer.Mark();
	const float speed = 25.0f;
	if (wnd.kbd.KeyIsPressed('W')) {
		distance += speed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		distance -= speed * deltaTime;
	}
}

void Game::ComposeFrame() {
	constexpr int windowWidth = Graphics::ScreenWidth;
	constexpr int windowHeight = Graphics::ScreenHeight;
	for (int y = 0; y < windowHeight / 2; y++) {

			const float perspective = (float)y / (windowHeight / 2.0f);

			const float middlePoint = 0.5f;
			const float roadWidth = 0.05f + perspective * 0.8f;
			const float curbsWidth = roadWidth * 0.1f;
			const float halfRoad = roadWidth / 2;

			const int leftCurb = int ((middlePoint - halfRoad - curbsWidth) * windowWidth);
			const int leftRoad = int ((middlePoint - halfRoad) * windowWidth);
			const int rightRoad = int ((middlePoint + halfRoad) * windowWidth);
			const int rightCurb = int ((middlePoint + halfRoad + curbsWidth) * windowWidth);

			const bool isDarkGrass = sin(5.0f * powf(1.5f - perspective, 5) + distance) >= 0;
			const bool isDarkCurb = sin(30.0f * powf(1.5f - perspective, 3) + distance * 3.5f) >= 0;
			const Color grassColor = isDarkGrass ? Color(32, 128, 0) : Color(64, 196, 0);
			const Color curbsColor = isDarkCurb ? Color(200, 32, 32) : Color(200, 200, 200);
			const Color roadColor = Color(112, 112, 112);

		for (int x = 0; x < windowWidth; x++) {
			const int nRow = y + windowHeight / 2;
			if (x < leftCurb || x > rightCurb) {
				gfx.PutPixel(x, nRow, grassColor);
			} else if (x < leftRoad || x > rightRoad) {
				gfx.PutPixel(x, nRow, curbsColor);
			} else {
				gfx.PutPixel(x, nRow, roadColor);
			}


		}
	}
}
