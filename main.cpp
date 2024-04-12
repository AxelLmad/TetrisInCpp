#include <SDL.h>
#include <iostream>
#include <exception>

#include "Dialog.h"

int main(int argc, char* argv[]) {

	std::unique_ptr<Dialog> dialog;
	constexpr int FPS = 60;
	constexpr int FRAMEDELAY = 1000 / FPS;
	Uint32 framePrev{};
	int frameNow{};

	try {
		dialog.reset(new Dialog());
		while (dialog->GameStatus()) {
			framePrev = SDL_GetTicks();
			dialog->Update();
			frameNow = SDL_GetTicks()-framePrev;
			if (FRAMEDELAY > frameNow) {
				SDL_Delay(FRAMEDELAY-frameNow);
			}
		}	
	}

	catch (std::runtime_error e) {
		SDL_ShowSimpleMessageBox(0, "Error", e.what(), 0);
		
	}

	SDL_Quit();

	return 0;
}