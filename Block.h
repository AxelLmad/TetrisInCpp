#ifndef __BLOCKK
#define __BLOCKK
#include <memory>
#include <chrono>
#include "Sprite.h"
#include "Status.h"


class Block {
protected:
	Block(std::shared_ptr<SDL_Renderer> renderer, int x, int y, const int borderX, const int borderY) :borderX{ borderX }, borderY{ borderY } {
		squareTexture.reset(new Sprite("block", "assets/square0.bmp", renderer, 8*3, 8*3, 0,0));
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				grid[i][j].SetTexture(squareTexture);
				grid[i][j].x = i * 8 * 3 + x;
				grid[i][j].y = j * 8 * 3 + y;
			}
		}
		previousTime = time.now();
	}
public:
	virtual void ChangeDirection() = 0;

	void Update(int speed){
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
			grid[i][j].Update();
			}
		}
		auto currentTime = time.now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count() >= speed) {
			previousTime = currentTime;
			if (!stop) {
				Fall();
			}
		}
		
	}

	void MoveLeft() {
		bool move = true;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (grid[i][j].taken == true && grid[i][j].x == 400 - (95 * 3 / 2)+8*3) {
					move = false;
					goto endOfLoop;
				}
			}
		}

	endOfLoop:
		if (move) {
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					grid[i][j].x -= 8 * 3;
				}
			}
		}
	}
	void MoveRight() {

		bool move = true;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (grid[i][j].taken == true && grid[i][j].x == 400 - (95 * 3 / 2) + 8*10 * 3) {
					move = false;
					goto endOfLoop;
				}
			}
		}

	endOfLoop:

		if (move) {
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					grid[i][j].x += 8 * 3;
				}
			}
		}
	}

	void Fall() {
		
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					grid[i][j].y+=8*3;
				}
		}
	}

	virtual ~Block() {

	}
protected:
	void ValidateRight() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (grid[i][j].x == borderX + 8*3*10) {   
					MoveLeft();
					return;
				}
			}
		}
	}

	void ValidateLeft() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (grid[i][j].x < borderX) {
					MoveRight();
					return;
				}
			}
		}
	}
public:
	bool ValidateDown() {
		for (int i = 0; i < 4; ++i) {
			if(grid[i][3].y==borderY+8*3*25){		
				stop = true;
				SDL_Delay(100);
				

				return true;
			}
		}
		return false;

	}

	GridCell* GetLastRow() {
		GridCell LastRow[4]{ grid[0][3], grid[1][3], grid[2][3], grid[3][3] };
		
		return LastRow;
	}

	GridCell *GetCells() {
		return *grid;
	}

	
protected:
	std::shared_ptr<Sprite> squareTexture{};
	std::unique_ptr<Status> statusAction{};
	GridCell grid[4][4]{};
	std::chrono::system_clock time{};
	std::chrono::time_point<std::chrono::system_clock> previousTime{};
	const int borderX{};
	const int borderY{};
	bool stop{ false };
};


class OBlock :public Block {
public:
	OBlock(std::shared_ptr<SDL_Renderer> renderer, int x, int y, const int borderX, const int borderY): Block(renderer, x, y, borderX, borderY) {
		
		statusAction.reset(new SingleStatus());

		grid[0][2].taken = true;
		grid[0][3].taken = true;
		grid[1][2].taken = true;
		grid[1][3].taken = true;
	}
	void ChangeDirection()override {
	}
};

class SBlock : public Block {
public:
	SBlock(std::shared_ptr<SDL_Renderer> renderer, int x, int y, const int borderX, const int borderY) : Block(renderer, x, y, borderX, borderY) {

		statusAction.reset(new DoubleStatus());

		grid[0][3].taken = true;
		grid[1][2].taken = true;
		grid[1][3].taken = true;
		grid[2][2].taken = true;
	}

	void ChangeDirection() override{
		statusAction->change();

		if (statusAction->GetStatus() == 0) {

			grid[0][1].taken = false;
			grid[0][2].taken = false;
			grid[1][2].taken = false;
			grid[1][3].taken = false;

			grid[0][3].taken = true;
			grid[1][2].taken = true;
			grid[1][3].taken = true;
			grid[2][2].taken = true;
			ValidateRight();
		}
		else {
			grid[0][3].taken = false;
			grid[1][2].taken = false;
			grid[1][3].taken = false;
			grid[2][2].taken = false;

			grid[0][1].taken = true;
			grid[0][2].taken = true;
			grid[1][2].taken = true;
			grid[1][3].taken = true;
		}
	}
};


class ZBlock : public Block {
public:
	ZBlock(std::shared_ptr<SDL_Renderer> renderer, int x, int y, const int borderX, const int borderY) : Block(renderer, x, y, borderX, borderY) {

		statusAction.reset(new DoubleStatus());

		grid[0][2].taken = true;
		grid[1][2].taken = true;
		grid[1][3].taken = true;
		grid[2][3].taken = true;
	}

	void ChangeDirection() override {
		statusAction->change();

		if (statusAction->GetStatus() == 0) {

			grid[0][2].taken = false;
			grid[0][3].taken = false;
			grid[1][1].taken = false;
			grid[1][2].taken = false;

			grid[0][2].taken = true;
			grid[1][2].taken = true;
			grid[1][3].taken = true;
			grid[2][3].taken = true;
			ValidateRight();
		}
		else {
			grid[0][2].taken = false;
			grid[1][2].taken = false;
			grid[1][3].taken = false;
			grid[2][3].taken = false;

			grid[0][2].taken = true;
			grid[0][3].taken = true;
			grid[1][1].taken = true;
			grid[1][2].taken = true;


			
		}
	}
};

class IBlock : public Block {
public:
	IBlock(std::shared_ptr<SDL_Renderer> renderer, int x, int y, const int borderX, const int borderY) : Block(renderer, x, y, borderX, borderY) {

		statusAction.reset(new DoubleStatus());

		grid[0][0].taken = true;
		grid[0][1].taken = true;
		grid[0][2].taken = true;
		grid[0][3].taken = true;


	}

	void ChangeDirection() override {

		statusAction->change();

		if (statusAction->GetStatus() == 0) {

			grid[0][3].taken = false;
			grid[1][3].taken = false;
			grid[2][3].taken = false;
			grid[3][3].taken = false;

			grid[0][0].taken = true;
			grid[0][1].taken = true;
			grid[0][2].taken = true;
			grid[0][3].taken = true;
		}
		else {
			grid[0][0].taken = false;
			grid[0][1].taken = false;
			grid[0][2].taken = false;
			grid[0][3].taken = false;

			grid[0][3].taken = true;
			grid[1][3].taken = true;
			grid[2][3].taken = true;
			grid[3][3].taken = true;
			ValidateRight();
			ValidateRight();
			ValidateRight();
		}
	}
	
};

class TBlock : public Block {
public:
	TBlock(std::shared_ptr<SDL_Renderer> renderer, int x, int y, const int borderX, const int borderY) : Block(renderer, x, y, borderX, borderY) {

		statusAction.reset(new QuadrupleStatus());

		grid[0][3].taken = true;
		grid[1][2].taken = true;
		grid[1][3].taken = true;
		grid[2][3].taken = true;

	}

	void ChangeDirection() override {
		statusAction->change();

		switch (statusAction->GetStatus()) {
			
			case 0: {
				grid[0][2].taken = false;
				grid[1][1].taken = false;
				grid[1][2].taken = false;
				grid[1][3].taken = false;

				grid[0][3].taken = true;
				grid[1][2].taken = true;
				grid[1][3].taken = true;
				grid[2][3].taken = true;
			
				break;
			}

			case 1: {
				grid[0][3].taken = false;
				grid[1][2].taken = false;
				grid[1][3].taken = false;
				grid[2][3].taken = false;

				grid[0][1].taken = true;
				grid[0][2].taken = true;
				grid[0][3].taken = true;
				grid[1][2].taken = true;
				break;
			}

			case 2: {
				grid[0][1].taken = false;
				grid[0][2].taken = false;
				grid[0][3].taken = false;
				grid[1][2].taken = false;

				grid[0][2].taken = true;
				grid[1][2].taken = true;
				grid[1][3].taken = true;
				grid[2][2].taken = true;
				ValidateRight();
				break;
			}
			case 3: {
				grid[0][2].taken = false;
				grid[1][2].taken = false;
				grid[1][3].taken = false;
				grid[2][2].taken = false;

				grid[0][2].taken = true;
				grid[1][1].taken = true;
				grid[1][2].taken = true;
				grid[1][3].taken = true;
				break;
			}

			default: {
				throw(std::runtime_error("Game control data corrupted"));
			}
		}
	}

};

class LBlock : public Block {
public:
	LBlock(std::shared_ptr<SDL_Renderer> renderer, int x, int y, const int borderX, const int borderY) : Block(renderer, x, y, borderX, borderY) {

		statusAction.reset(new QuadrupleStatus());

		grid[1][1].taken = true;
		grid[1][2].taken = true;
		grid[1][3].taken = true;
		grid[2][3].taken = true;


	}

	void ChangeDirection() override {
		statusAction->change();

		switch (statusAction->GetStatus()) {

			case 0: {
				grid[1][3].taken = false;
				grid[2][3].taken = false;
				grid[3][2].taken = false;
				grid[3][3].taken = false;

				grid[1][1].taken = true;
				grid[1][2].taken = true;
				grid[1][3].taken = true;
				grid[2][3].taken = true;
				break;
			}

			case 1: {
				grid[1][1].taken = false;
				grid[1][2].taken = false;
				grid[1][3].taken = false;
				grid[2][3].taken = false;

				grid[1][2].taken = true;
				grid[1][3].taken = true;
				grid[2][2].taken = true;
				grid[3][2].taken = true;
				ValidateRight();
				break;
			}

			case 2: {
				grid[1][2].taken = false;
				grid[1][3].taken = false;
				grid[2][2].taken = false;
				grid[3][2].taken = false;

				grid[1][1].taken = true;
				grid[2][1].taken = true;
				grid[2][2].taken = true;
				grid[2][3].taken = true;
				break;
			}
			case 3: {
				grid[1][1].taken = false;
				grid[2][1].taken = false;
				grid[2][2].taken = false;
				grid[2][3].taken = false;

				grid[1][3].taken = true;
				grid[2][3].taken = true;
				grid[3][2].taken = true;
				grid[3][3].taken = true;
				ValidateRight();
				break;
			}

			default: {
				throw(std::runtime_error("Game control data corrupted"));
			}



		}
	}

};


class JBlock : public Block {
public:
	JBlock(std::shared_ptr<SDL_Renderer> renderer, int x, int y, const int borderX, const int borderY) : Block(renderer, x, y, borderX, borderY) {

		statusAction.reset(new QuadrupleStatus());

		grid[2][1].taken = true;
		grid[2][2].taken = true;
		grid[2][3].taken = true;
		grid[1][3].taken = true;


	}

	void ChangeDirection() override {
		statusAction->change();

		switch (statusAction->GetStatus()) {

			case 0: {
				grid[1][2].taken = false;
				grid[2][2].taken = false;
				grid[3][3].taken = false;
				grid[3][2].taken = false;

				grid[2][1].taken = true;
				grid[2][2].taken = true;
				grid[2][3].taken = true;
				grid[1][3].taken = true;
				break;
			}

			case 1: {
				grid[2][1].taken = false;
				grid[2][2].taken = false;
				grid[2][3].taken = false;
				grid[1][3].taken = false;

				grid[1][3].taken = true;
				grid[1][2].taken = true;
				grid[2][3].taken = true;
				grid[3][3].taken = true;
				ValidateRight();
				break;
			}

			case 2: {
				grid[1][3].taken = false;
				grid[1][2].taken = false;
				grid[2][3].taken = false;
				grid[3][3].taken = false;

				grid[2][1].taken = true;
				grid[1][1].taken = true;
				grid[1][2].taken = true;
				grid[1][3].taken = true;
				break;
			}
			case 3: {
				grid[2][1].taken = false;
				grid[1][1].taken = false;
				grid[1][2].taken = false;
				grid[1][3].taken = false;

				grid[1][2].taken = true;
				grid[2][2].taken = true;
				grid[3][3].taken = true;
				grid[3][2].taken = true;
				ValidateRight();
				break;
			}

			default: {
				throw(std::runtime_error("Game control data corrupted"));
			}
		}
	}

};

#endif