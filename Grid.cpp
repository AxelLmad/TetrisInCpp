#include "Grid.h"

bool Grid::IsRowFull(int index)const {

	for (int i = 0; i < 10; ++i) {
		if (!grid[index][i].taken) {
			return false;
		}
	}
	return true;
}

void Grid::Update() {
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 10; ++j) {
			grid[i][j].Update();
		}
	}

}

bool Grid::GetCellFromPosition(int x, int y) {
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (grid[i][j].x == x && grid[i][j].y == y) {
				
				return grid[i][j].taken;

			}

		}

	}

	return false;

}

Grid::Grid(std::shared_ptr<SDL_Renderer> renderer, int offSetX, int offSetY) {
	const int imgW = 8;
	const int imgH = 8;
	cellTexture.reset(new Sprite("gridCell", "assets/square1.bmp", renderer, imgW * 3, imgH * 3, 0, 0));

	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 10; ++j) {
			grid[i][j].SetTexture(cellTexture);
			grid[i][j].x = j * 8 * 3 + offSetX;
			grid[i][j].y = i * 8 * 3 + offSetY;
		}
	}
}

