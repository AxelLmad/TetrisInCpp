#include "Dialog.h"
#include <thread>



void Dialog::Render() const {
	SDL_RenderClear(renderer.get());
	backgroundImage->Render();
	gameboard->Update();
	SDL_RenderPresent(renderer.get());
}


void Dialog::Update() {
	Render();
	HandleEvents();
	this->isRunning = !gameboard->gameEnded;
}

void Dialog::InitDialog() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::string error = "SDL could not be loaded" + std::string(SDL_GetError());
		DestroyDialog();
		throw std::runtime_error(error);
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::string error = "SDLWindow could not be created" + std::string(SDL_GetError());
		DestroyDialog();
		throw std::runtime_error(error);
	}

	renderer.reset(SDL_CreateRenderer(window, -1, 0), SDL_DestroyRenderer);

	if (renderer == nullptr) {
		std::string error("Failed to create renderer: " + std::string(SDL_GetError()));
		DestroyDialog();
		throw std::runtime_error(error);
	}

	if (SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 1) < 0) {
		DestroyDialog();
		throw std::runtime_error(SDL_GetError());
	}
}

void Dialog::HandleEvents() {

	SDL_Event event{};

	if (SDL_PollEvent(&event) < 0) {			
		DestroyDialog();
		throw std::runtime_error(SDL_GetError());
	}

	switch (event.type) {
	case SDL_QUIT: {
		isRunning = false;
		break;
	}

	case SDL_KEYDOWN:{
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: {
					isRunning = false;
					break; }

				case SDLK_m: {
					gameboard->mPressed = true;
	
					break;
				}

				case SDLK_a: {
					gameboard->aPressed = true;
					break;
				}
				case SDLK_d: {
					gameboard->dPressed = true;
					break;
				}

				case SDLK_s: {
					gameboard->sPressed = true;
					break;
				}
			}
		break;
		}

	case SDL_KEYUP: {
		switch (event.key.keysym.sym) {
			case SDLK_m: {
				gameboard->mPressed = false;
				gameboard->mFixed = false;

				break;
			}
			case SDLK_a: {
				gameboard->aPressed = false;
				gameboard->aFixed = false;

				break;
			}
			case SDLK_d: {
				gameboard->dPressed = false;
				gameboard->dFixed = false;

				break;
			}

			case SDLK_s: {
				gameboard->sPressed = false;

			}
		}
	break; 
	}

	default: {
		break;
	}
	}
}

void Dialog::DestroyDialog() {
	if (window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}

bool Dialog::GameStatus() {
	return isRunning;
}

Dialog::Dialog() {
	InitDialog();
	std::string backgroundName{ "background" };
	std::string backgroundPath{ "assets/background.bmp" };
	constexpr int backgroundPosition = 0;
	std::thread backgroundInitializer([&]() {	backgroundImage.reset(new Sprite(backgroundName, backgroundPath, renderer, WIDTH, HEIGHT, backgroundPosition, backgroundPosition)); });
	std::thread gameBoardInitializer([&]() {	gameboard.reset(new Gameboard(renderer)); });
	backgroundInitializer.join();
	gameBoardInitializer.join();
}

Dialog::~Dialog() {
	DestroyDialog();
}