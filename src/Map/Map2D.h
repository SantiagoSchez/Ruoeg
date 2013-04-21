//
// Wrapper for 2D std::vector of Tiles.
// 

#ifndef RUOEG_MAP_MAP2D_H_
#define RUOEG_MAP_MAP2D_H_

#include <vector>

#include "Tile.h"

class Map2D
{
public:
	Map2D(int height, int width);
	~Map2D();

	Tile& at(int row, int column);
	Tile& at(int index);
	int height() const;
	int width() const;
	void clear();

private:
	int height_, width_;
	std::vector<Tile> vec_;
};

#endif // RUOEG_MAP_MAP2D_H_