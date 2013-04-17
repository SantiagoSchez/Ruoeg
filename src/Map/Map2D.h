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
	Map2D(unsigned int height, unsigned int width);
	~Map2D();

	Tile& at(unsigned int row, unsigned int column);
	Tile& at(unsigned int index);
	unsigned int height() const;
	unsigned int width() const;
	void clear();

private:
	unsigned int height_, width_;
	std::vector<Tile> vec_;
};

#endif // RUOEG_MAP_MAP2D_H_