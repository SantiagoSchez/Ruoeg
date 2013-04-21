#include "Map2D.h"

Map2D::Map2D(int height, int width) : 
	vec_(height*width), height_(height), width_(width)
{
}

Map2D::~Map2D()
{
}

Tile& Map2D::at(int row, int column) 
{
	return vec_[row*width_+column];
}

Tile& Map2D::at(int index)
{
	return vec_[index];
}

int Map2D::height() const
{
	return height_;
}

int Map2D::width() const
{
	return width_;
}

void Map2D::clear()
{
	if(!vec_.empty())
	{
		vec_.clear();
		vec_.resize(height_*width_);
	}
}