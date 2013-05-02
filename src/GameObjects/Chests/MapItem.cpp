#include "MapItem.h"

MapItem::MapItem() : GameObject(GameObject::Type::MapItem)
{
	walkable_ = false;
	color_ = GameObject::Color::Magenta_Black;
}

MapItem::~MapItem()
{
}