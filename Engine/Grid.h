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

	void Draw(Graphics &gfx) const {
		for (int cy = 0; cy <= nRows; cy++) {
			const int posY = cy * cellsHeight;
			const Vei2 start = Vei2(0, posY);
			const Vei2 end = Vei2(totalWidth, posY);
			gfx.DrawLineClamped(Vec2(start + position), Vec2(end + position), Colors::White);
		}
		for (int cx = 0; cx <= nColumns; cx++) {
			int posX = cx * cellsWidth;
			const Vei2 start = Vei2(posX, 0);
			const Vei2 end = Vei2(posX, totalHeight);
			gfx.DrawLineClamped(Vec2(start + position), Vec2(end + position), Colors::White);
		}
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