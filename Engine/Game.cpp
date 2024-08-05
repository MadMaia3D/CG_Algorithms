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
	track.push_back({ 50.0f, 0.0f });
	track.push_back({ 250.0f, 0.0f });
	track.push_back({ 150.0f, 1.0f });
	track.push_back({ 250.0f, 0.0f });
	track.push_back({ 150.0f, -1.0f });
	track.push_back({ 50.0f, 0.0f });
	track.push_back({ 100.0f, 1.0f });
	track.push_back({ 200.0f, 0.0f });
	track.push_back({ 100.0f, -1.0f });
	track.push_back({ 100.0f, 0.0f });
	track.push_back({ 100.0f, 0.0f });

	trackLength = 0.0f;
	for (const auto &seg : track) {
		trackLength += seg.length;
	}
}

void Game::Go() {
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel() {
	const float deltaTime = frameTimer.Mark();

	// acceleration controls
	if (wnd.kbd.KeyIsPressed('W')) {
		carSpeed = Lerp(carSpeed, maxCarSpeed, deltaTime);
	} else {
		carSpeed = Lerp(carSpeed, 0, deltaTime / 2);
	}
	// steering controls
	constexpr float carSteeringSpeed = 1.25f;
	if (wnd.kbd.KeyIsPressed('A')) {
		accumCarCurvature -= carSteeringSpeed * deltaTime;
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		accumCarCurvature += carSteeringSpeed * deltaTime;
	}
	// forced braking
	if (abs(accumCarCurvature - accumTrackCurvature) > 0.7f) {
		carSpeed = Lerp(carSpeed, 0, deltaTime * 5.0f);
	}
	constexpr float trackLimits = 0.85f;
	if ((accumCarCurvature - accumTrackCurvature) < -trackLimits) {
		accumCarCurvature = accumTrackCurvature - trackLimits;
	} else if ((accumCarCurvature - accumTrackCurvature) > trackLimits) {
		accumCarCurvature = accumTrackCurvature + trackLimits;
	}
	distance += carSpeed * deltaTime;

	// find track point
	offset = 0.0f;
	trackSection = 0;

	while ((size_t)trackSection < track.size() && offset <= distance) {
		offset += track[trackSection].length;
		trackSection++;
	}
	// reset track lap
	if (distance >= trackLength) {
		distance -= trackLength;
	}
	
	// track curvature calculations
	targetCurvature = track[trackSection - 1].curvature;
	curvature = Lerp(curvature, targetCurvature, deltaTime);
	const float normalizedCarSpeed = carSpeed / maxCarSpeed;
	accumTrackCurvature += curvature * deltaTime * normalizedCarSpeed * 2.0f;
}

void Game::ComposeFrame() {
	constexpr int windowWidth = Graphics::ScreenWidth;
	constexpr int windowHeight = Graphics::ScreenHeight;
	for (int y = 0; y < windowHeight / 2; y++) {

		const float perspective = (float)y / (windowHeight / 2.0f);

		const float middlePoint = 0.5f + curvature * powf(1.0f - perspective, 5);
		const float roadWidth = 0.05f + 0.8f * perspective;
		const float curbsWidth = roadWidth * 0.15f;
		const float halfRoad = roadWidth / 2;

		const int leftCurb = int((middlePoint - halfRoad - curbsWidth) * windowWidth);
		const int leftRoad = int((middlePoint - halfRoad) * windowWidth);
		const int rightRoad = int((middlePoint + halfRoad) * windowWidth);
		const int rightCurb = int((middlePoint + halfRoad + curbsWidth) * windowWidth);

		const bool isDarkGrass = sin(4.0f * powf(1.5f - perspective, 5) + distance * 0.4) >= 0;
		const bool isDarkCurb = sin(25.0f * powf(1.5f - perspective, 5) + distance) >= 0;
		const Color grassColor = isDarkGrass ? Color(32, 128, 0) : Color(64, 196, 0);
		const Color curbsColor = isDarkCurb ? Color(200, 32, 32) : Color(225, 225, 225);
		const Color roadColor = Color(150, 150, 128);

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

		float carPosition = (accumCarCurvature - accumTrackCurvature) * 400.0f ;
		gfx.DrawRectCenter((int)carPosition + 450, 500, 100, 50, Colors::Black);
	}
}
