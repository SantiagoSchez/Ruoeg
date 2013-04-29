#include "Tile.h"

Tile::Tile() : visited_(false) 
{
}

Tile::~Tile()
{
}

GameObjectPtr& Tile::top()
{
	return raw_.back();
}

GameObjectPtr& Tile::element(int layer)
{
	return raw_.at(layer);
}

void Tile::add(GameObjectPtr game_object)
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