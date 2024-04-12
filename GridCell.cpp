#include "GridCell.h"

void GridCell::Update(){
		cellTexture->Position(x, y);
		if (taken) {
			cellTexture->Render();
		}
}

void GridCell::SetTexture(std::shared_ptr<Sprite> cellTexture) {
	this->cellTexture = cellTexture;
}

