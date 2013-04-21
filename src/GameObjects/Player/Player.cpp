#include "Player.h"

Player::Player() : GameObject(GameObject::Type::Player)
{
	walkable_ = true;
}

Player::~Player()
{
}