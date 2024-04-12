#include "Gameboard.h"
#include <thread>
#include<iostream>

constexpr float gridWidth = 400.f;
constexpr float gridHeight = 300.f;
constexpr float gridOffsetX = gridWidth - (95 * 3 / 2) + 8 * 3;
constexpr float gridOffsetY = gridHeight - (175 * 3 / 2) + 8 * 3;

Gameboard::Gameboard(std::shared_ptr<SDL_Renderer> renderer) :Grid(renderer, gridOffsetX, gridOffsetY) {
	this->renderer = renderer;
	std::string spriteName = "gameboard";
	std::string spritePath = "assets/gameboard.bmp";
	const int spriteWidth = imageWidth * 3;
	const int spriteHeight = imageHeight * 3;
	const int spriteXPosition = gridWidth - (imageWidth * 3 / 2);
	const int spriteYPosition = gridHeight - (imageHeight * 3 / 2);
	std::thread spriteInitializer([&]() {texture.reset(new Sprite(spriteName, spritePath, renderer, spriteWidth, spriteHeight, spriteXPosition, spriteYPosition)); });
	std::thread blockGenerator([&]() {	GenerateBlock(borderX, borderY); });
	spriteInitializer.join();
	blockGenerator.detach();
	randomizer.reset();
}

void Gameboard::Render() {
	texture->Render();
	if (sPressed) {
		constexpr int lowSpeedValue = 50;
		gamePiece->Update(lowSpeedValue);
	}
	else {
		constexpr int highSpeedValue = 600;
		gamePiece->Update(highSpeedValue);
	}
	
}

void Gameboard::Update() {
	std::thread checkMatchesThread([&]() {	CheckMatches(); });
	checkMatchesThread.join();
	std::thread validateRowsThread([&]() {	ValidateRows(); });
	validateRowsThread.detach();
	if (mPressed) {
		if (!mFixed) {
			gamePiece->ChangeDirection();
			mFixed = true;
		}
	}

	if (aPressed) {
		if (!aFixed) {
			gamePiece->MoveLeft();
			aFixed = true;
		}
	}

	if (dPressed) {
		if (!dFixed) {
			gamePiece->MoveRight();
			dFixed = true;
		}
	}
	Render();
	Grid::Update();
}

bool Gameboard::CheckBlockCollision(GridCell* fichaGrid) {

	for (int i = 0; i < 16; ++i) {
		if (fichaGrid[i].taken) {
			if (GetCellFromPosition(fichaGrid[i].x, fichaGrid[i].y)) {
				for (int j = 0; j < 16; ++j) {
					if (fichaGrid[j].y <= borderY+24*3 + 50) {
						gameEnded = true;
					}
				}
				return true;
			}
		}
}
	return false;
}

void Gameboard::CheckMatches() {
	GridCell* fichaGrid = gamePiece->GetCells();
	if (gamePiece->ValidateDown()||CheckBlockCollision(fichaGrid)) {
		int positionX[4]{};
		int positionY[4]{};
		int posXCounter{ 0 };
		int posYCounter{ 0 };
		for (int i = 0; i < 16; ++i) {
			if (fichaGrid[i].taken == true) {
				positionX[posXCounter++] = fichaGrid[i].x;
				positionY[posYCounter++] = fichaGrid[i].y - 8 * 3;
				if (posXCounter == 4) {
					break;
				}
			}

		}
		GenerateBlock(borderX, borderY);
		posXCounter = 0;
		posYCounter = 0;
		while (posXCounter < 4) {

			for (int i = 0; i < 20; ++i) {
				for (int j = 0; j < 10; ++j) {
					if (grid[i][j].x == positionX[posXCounter] && grid[i][j].y == positionY[posYCounter]) {
						grid[i][j].taken = true;
						++posXCounter;
						++posYCounter;
					}
				}
			}
		}

	}
}

void Gameboard::ValidateRows() {
	for (int i = 0; i < 20; ++i) {
		if (IsRowFull(i)) {
			for (int j = i; j > 0; --j) {
				for (int k = 0; k < 10; ++k) {
					grid[j][k].taken=grid[j - 1][k].taken;
				}
			}
			return;
		}
	}
}


void Gameboard::GenerateBlock(int x, int y) {
	switch (randomizer->SendNumber(0, 6)) {

	case 0: {
		gamePiece.reset(new OBlock(renderer, x, y, borderX, borderY));
		break;
	}
	case 1: {
		gamePiece.reset(new SBlock(renderer, x, y, borderX, borderY));
		break;
	}
	case 2: {
		gamePiece.reset(new ZBlock(renderer, x, y, borderX, borderY));
		break;
	}
	case 3: {
		gamePiece.reset(new TBlock(renderer, x, y, borderX, borderY));
		break;
	}
	case 4: {
		gamePiece.reset(new IBlock(renderer, x, y, borderX, borderY));
		break;
	}
	case 5: {
		gamePiece.reset(new LBlock(renderer, x, y, borderX, borderY));
		break;
	}
	case 6: {
		gamePiece.reset(new JBlock(renderer, x, y, borderX, borderY));
		break;
	}
	default: {
		throw std::runtime_error("Block data corrupted");
	}
	}
}