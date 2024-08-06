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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
}

void Game::ComposeFrame()
{
	constexpr int rows = 8;
	constexpr int columns = 15;
	constexpr int cellWidth = 30;
	constexpr int cellHeight = 30;
	constexpr int totalWidth = cellWidth * columns;
	constexpr int totalHeight = cellHeight * rows;
	const Vec2 gridPos(wnd.mouse.GetPosX() - totalWidth/2, wnd.mouse.GetPosY() - totalHeight/2);

	for (int cy = 0; cy <= rows; cy++) {
		const int posY = cy * cellHeight;
		const Vec2 start = Vec2(0.0f, (float)posY);
		const Vec2 end = Vec2(totalWidth, (float)posY);
		gfx.DrawLineClamped(start + gridPos, end + gridPos, Colors::White);
	}
	for (int cx = 0; cx <= columns; cx++) {
		int posX = cx * cellWidth;
		const Vec2 start = Vec2((float)posX, 0.0f);
		const Vec2 end = Vec2((float)posX, totalHeight);
		gfx.DrawLineClamped(start + gridPos, end + gridPos, Colors::White);
	}
}
