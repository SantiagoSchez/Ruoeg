#include "Tile.h"

Tile::Tile() : visited_(false) 
{
}

Tile::~Tile()
{
}

GameObject& Tile::top()
{
	return raw_.back();
}

GameObject& Tile::element(int layer)
{
	return raw_.at(layer);
}

void Tile::add(GameObject &game_object)
{
	raw_.push_back(game_object);
}

int Tile::elements() const
{
	return raw_.size();
}

bool Tile::visited() const
{
	return visited_;
}

void Tile::set_visited(bool visited)
{
	visited_ = visited;
}