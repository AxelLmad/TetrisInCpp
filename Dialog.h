#ifndef __DIALOGG
#define __DIALOGG

#include <SDL.h>
#include <stdexcept>
#include <memory>
#include <string>
#include "Gameboard.h"
#include "Sprite.h"

class Dialog final {

public:
	Dialog();
	~Dialog();

private:
	Dialog(const Dialog& copy) = delete;
	void operator =(const Dialog& copy) = delete;
	Dialog(Dialog&& move) = delete;
	void operator =(Dialog&& move) = delete;

public:
	void Update();
	bool GameStatus();

private:
	void InitDialog();
	void DestroyDialog();
	void Render()const;
	void HandleEvents();

	SDL_Window* window{nullptr};
	std::shared_ptr<SDL_Renderer> renderer{};
	std::unique_ptr<Sprite> backgroundImage{};
	std::unique_ptr<Gameboard> gameboard{};

	bool isRunning{ true };
	const unsigned int WIDTH{ 800 };
	const unsigned int HEIGHT{ 600 };
	const char* title{ "Tetris" };
};
#endif
