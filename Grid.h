#ifndef __GRIDD
#define __GRIDD

#include <memory>
#include "GridCell.h"
#include "Sprite.h"

class Grid {

public:
	Grid(std::shared_ptr<SDL_Renderer> renderer, int offSetX, int offSetY);
	void Update();

protected:
	bool GetCellFromPosition(int x, int y);
	bool IsRowFull(int index)const;
	GridCell grid[20][10]{};

private:
	std::shared_ptr<Sprite> cellTexture{};

};





#endif