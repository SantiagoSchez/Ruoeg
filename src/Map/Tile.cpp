#include "Tile.h"
#include "../GameObjects/Enemies/Enemy.h"

Tile::Tile() : visited_(false) 
{
}

Tile::~Tile()
{
}

GameObjectPtr Tile::top()
{
	if(raw_.back()->delete_object())
	{
		raw_.pop_back();
		raw_.back()->set_in_fov(true);
	}

	return raw_.back();
}

GameObjectPtr Tile::element(int layer)
{
	return raw_.at(layer);
}

void Tile::add(GameObjectPtr game_object)
{
	raw_.push_back(game_object);
}

void Tile::pop()
{
	raw_.pop_back();
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
