#include "CompassItem.h"

CompassItem::CompassItem() : GameObject(GameObject::Type::CompassItem)
{
	walkable_ = false;
	color_ = GameObject::Color::Magenta_Black;
}

CompassItem::~CompassItem()
{
}