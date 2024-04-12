#include "Sprite.h"

void Sprite::Render()const {

	if (SDL_RenderCopy(renderer.get(), texture, 0, &rect) < 0) {
		std::string error("Failed to render " + spriteName + ": " + std::string(IMG_GetError()));
		throw(std::runtime_error(error));
	}
}

void Sprite::Position(int x, int y) {
	rect.x = x;
	rect.y = y;
}

Sprite::Sprite(std::string spriteName, std::string path, std::shared_ptr<SDL_Renderer> renderer, int width, int height, int x, int y) {

	this->renderer = renderer;
	this->spriteName = spriteName;
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	texture = SDL_CreateTextureFromSurface(renderer.get(), tempSurface);
	if (texture == nullptr) {
		DestroySprite();
		std::string error("Failed to create" + spriteName + "'s texture: " + std::string(IMG_GetError()));
		throw std::runtime_error(error);
	}
	SDL_FreeSurface(tempSurface);
	tempSurface = nullptr;

	rect.w = width;
	rect.h = height;
	rect.x = x;
	rect.y = y;
}

Sprite::~Sprite() {
	DestroySprite();
}

void Sprite::DestroySprite() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
	}
}