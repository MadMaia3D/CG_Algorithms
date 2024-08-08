#pragma once
#include <algorithm>
#include <unordered_map>
#include "Graphics.h"
#include "Surface.h"

// hashing for Color
namespace std {
	template <>
	struct hash<Color> {
		std::size_t operator()(const Color& color) const {
			return std::hash<int>()(color.GetR()) ^ std::hash<int>()(color.GetG()) ^ std::hash<int>()(color.GetB());
		}
	};
}

class Map {
private:
	enum class CellType {
		Empty,
		Wall,
		Unknown
	};
public:
	Map(Vei2 position, int nColumns, int nRows, int cellsWidth, int cellsHeight)
		:
		position(position),
		nColumns(nColumns),
		nRows(nRows),
		cellWidth(cellsWidth),
		cellHeight(cellsHeight),
		totalWidth(cellsWidth * nColumns),
		totalHeight(cellsHeight * nRows) {
	}

	Map(Vei2 position, int cellsWidth, int cellsHeight, const std::wstring & filePath)
		:
		position(position),
		cellWidth(cellsWidth),
		cellHeight(cellsHeight) {
		std::unordered_map<Color, CellType> selector;
		selector.insert({ Colors::White, CellType::Empty });
		selector.insert({ Colors::Black, CellType::Wall });

		Surface mapData = Surface::FromFile(filePath);
		nColumns = mapData.GetWidth();
		nRows = mapData.GetHeight();
		totalWidth = cellsWidth * nColumns;
		totalHeight = cellsHeight * nRows;

		data.reserve(nColumns * nRows);

		for (int y = 0; y < nRows; y++) {
			for (int x = 0; x < nColumns; x++) {
				Color c = mapData.GetPixel(x, y);
				c.SetA(0);
				c.SetX(0);
				if (selector.find(c) != selector.end()) {
					data.emplace_back(selector[c]);
				} else {
					data.emplace_back(CellType::Empty);
				}
			}
		}
	}

	void Draw(Graphics &gfx) {
		DrawAllCells(gfx);
		DrawLines(Colors::Black, gfx);
	}

	void PaintCell(Vei2 cell, Graphics &gfx, Color c) const {

		int left = std::clamp(cell.x * cellWidth + position.x, 0, (int)gfx.ScreenWidth);
		int top = std::clamp(cell.y * cellHeight + position.y, 0, (int)gfx.ScreenHeight);
		int right = std::clamp(left + cellWidth, 0, (int)gfx.ScreenWidth);
		int bottom = std::clamp(top + cellHeight, 0, (int)gfx.ScreenHeight);

		for (int cy = top; cy < bottom; cy++) {
			for (int cx = left; cx < right; cx++) {
				gfx.PutPixel(cx, cy, c);
			}
		}
	}

	/* ---------------- Convert From Screen Space to Grid Space and Vice-Versa ---------------- */
	static Vec2 ToMapSpace(const Map *pMap, Vec2 pos) {
		pos -= (Vec2)pMap->GetPosition();
		return { pos.x / pMap->GetCellHeight(), pos.y / pMap->GetCellWidth() };
	}

	static Vec2 ToScreenSpace(const Map *pMap, Vec2 pos) {
		return Vec2(pos.x * pMap->GetCellHeight(), pos.y * pMap->GetCellWidth()) + (Vec2)pMap->GetPosition();
	}

	/* ---------------- Questions ---------------- */
	bool HasWallAt(int x, int y) const {
		const int index = y * nColumns + x;
		return data[index] == CellType::Wall;
	}

	bool IsValidCell(Vec2 pos) {
		return (pos.x > 0 && pos.x < nColumns && pos.y > 0 && pos.y < nRows);
	}

	/* ---------------- Getters and Setters ----------------*/
	void SetPosition(Vei2 pos) { position = pos; }
	Vei2 GetPosition() const { return position; }
	int GetCellWidth() const { return cellWidth; }
	int GetCellHeight() const { return cellHeight; }
	int GetNRows() const { return nRows; }
	int GetNColumns() const { return nColumns; }

private:
	/* ---------------- Auxiliary Drawing functions ---------------- */
	void DrawLines(Color color, Graphics &gfx) const {
		for (int cy = 0; cy <= nRows; cy++) {
			const int posY = cy * cellHeight;
			const Vei2 start = Vei2(0, posY);
			const Vei2 end = Vei2(totalWidth, posY);
			gfx.DrawLineClamped(Vec2(start + position), Vec2(end + position), color);
		}
		for (int cx = 0; cx <= nColumns; cx++) {
			int posX = cx * cellWidth;
			const Vei2 start = Vei2(posX, 0);
			const Vei2 end = Vei2(posX, totalHeight);
			gfx.DrawLineClamped(Vec2(start + position), Vec2(end + position), color);
		}
	}

	void DrawSingleCell(Vei2 cell, Graphics &gfx) const {
		int cellIndex = cell.y * nColumns + cell.x;
		Color c;

		switch (data[cellIndex]) {
		case CellType::Empty:
			c = { 224,224,224 };
			break;
		case CellType::Wall:
			c = { 48,48,48 };
			break;
		default:
			c = Colors::Magenta;
			break;
		}

		PaintCell(cell, gfx, c);
	}

	void DrawAllCells(Graphics &gfx) const {
		for (int y = 0; y < nRows; y++) {
			for (int x = 0; x < nColumns; x++) {
				DrawSingleCell({ x, y }, gfx);
			}
		}
	}

private:
	Vei2 position = { 100, 100 };
	int nRows = 8;
	int nColumns = 15;
	int cellWidth = 30;
	int cellHeight = 30;
	int totalWidth = cellWidth * nColumns;
	int totalHeight = cellHeight * nRows;
	std::vector<CellType> data;
};