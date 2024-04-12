#ifndef __GRIDCELLL
#define __GRIDCELLL

#include <memory>
#include "Sprite.h"

class GridCell {
public:
	void Update();
	void SetTexture(std::shared_ptr<Sprite> cellTexture);
	bool taken{ false };
	int x{};
	int y{};

private:
	std::shared_ptr<Sprite> cellTexture{};
};


#endif