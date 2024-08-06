#pragma once
#include "Graphics.h"

class Grid {
public:
	Grid(Vei2 position, int nColumns, int nRows, int cellsWidth, int cellsHeight)
		:
		position(position),
		nColumns(nColumns),
		nRows(nRows),
		cellsWidth(cellsWidth),
		cellsHeight(cellsHeight)
	{}

	void Draw(Color color, Graphics &gfx) const {
		for (int cy = 0; cy <= nRows; cy++) {
			const int posY = cy * cellsHeight;
			const Vei2 start = Vei2(0, posY);
			const Vei2 end = Vei2(totalWidth, posY);
			gfx.DrawLineClamped(Vec2(start + position), Vec2(end + position), color);
		}
		for (int cx = 0; cx <= nColumns; cx++) {
			int posX = cx * cellsWidth;
			const Vei2 start = Vei2(posX, 0);
			const Vei2 end = Vei2(posX, totalHeight);
			gfx.DrawLineClamped(Vec2(start + position), Vec2(end + position), color);
		}
	}

	void DrawFilled(Color line, Color fill, Graphics &gfx) const {
		int left = position.x;
		int top = position.y;
		int right = position.x + totalWidth;
		int bottom = position.y + totalHeight;

		left = std::clamp(left, 0, gfx.ScreenWidth);
		right = std::clamp(right, 0, gfx.ScreenWidth);
		top = std::clamp(top, 0, gfx.ScreenHeight);
		bottom = std::clamp(bottom, 0, gfx.ScreenHeight);

		for (int cy = top; cy < bottom; cy++) {
			for (int cx = left; cx < right; cx++) {
				gfx.PutPixel(cx, cy, fill);
			}
		}
		Draw(line, gfx);
	}

	void SetPosition(Vei2 pos) {
		position = pos;
	}

private:
	Vei2 position = { 100, 100 };
	int nRows = 8;
	int nColumns = 15;
	int cellsWidth = 30;
	int cellsHeight = 30;
	int totalWidth = cellsWidth * nColumns;
	int totalHeight = cellsHeight * nRows;
};