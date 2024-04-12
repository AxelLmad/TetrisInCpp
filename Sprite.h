#ifndef __SPRITEE
#define __SPRITEE

#include <SDL_image.h>
#include <string>
#include <stdexcept>
#include <memory>
class Sprite final{

public:

	void Render()const;

	void Position(int x, int y);

	Sprite(std::string spriteName, std::string path, std::shared_ptr<SDL_Renderer> renderer, int width, int height, int x, int y);


	

	~Sprite();
private:

	void DestroySprite();

	SDL_Texture* texture{};
	std::shared_ptr<SDL_Renderer> renderer;
	std::string spriteName{};
	SDL_Rect rect{  };
};


#endif