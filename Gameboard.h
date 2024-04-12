#ifndef __GAMEBOARDD
#define __GAMEBOARDD
#include <SDL.h>
#include <exception>
#include <memory>
#include <random>
#include "Sprite.h"
#include "Grid.h"
#include "Block.h"

template <typename T> class RandomData {
public:
	RandomData() {
		std::random_device rd{};
	}

	T SendNumber(T min, T max) {
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(min, max);
		return distr(gen);
	}

private:
	std::random_device rd{};
	
};

class Gameboard final: public Grid {
public:
	Gameboard() = delete;
	Gameboard(std::shared_ptr<SDL_Renderer> renderer);
	
	void Update();
	void GenerateBlock(int x, int y);
	bool CheckBlockCollision(GridCell* fichaGrid);

private:
	void Render();
	void CheckMatches();
	void ValidateRows();

public:
	bool mPressed{ false };
	bool mFixed{ false };
	bool aPressed{ false };
	bool aFixed{ false };
	bool dPressed{ false };
	bool dFixed{ false };
	bool sPressed{ false };
	bool gameEnded{ false };

private:
	std::shared_ptr<SDL_Renderer> renderer;
	std::unique_ptr<Block> gamePiece{};
	std::unique_ptr<Sprite> texture;
	static constexpr int imageWidth{ 95 };
	static constexpr int imageHeight{ 175 };
	static constexpr int borderX{ 400 - (imageWidth * 3 / 2) + 8 * 3 };
	static constexpr int borderY{ 300 - (imageHeight * 3 / 2) - 32 * 3 };
	std::unique_ptr<RandomData<char>> randomizer;

};
#endif
