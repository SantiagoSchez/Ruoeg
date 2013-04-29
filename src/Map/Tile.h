//
// Wrapper for a std::vector of GameObjects.
// Example of a vector:
//  * Gold		4
//  * Item		3
//  * Chest		2
//  * Monster	1
//  * Lit/Wall  0
// 

#ifndef RUOEG_MAP_TILE_H_
#define RUOEG_MAP_TILE_H_

#include <vector>
#include <memory>

#include "../GameObjects/GameObject.h"

typedef std::shared_ptr<GameObject> GameObjectPtr;

class Tile
{
public:
	Tile();
	~Tile();

	GameObjectPtr& top();
	GameObjectPtr& element(int layer);
	void add(GameObjectPtr game_object);
	int elements() const;
	bool visited() const;
	void set_visited(bool visited);

private:
	std::vector<GameObjectPtr> raw_;
	bool visited_;
};

#endif  // RUOEG_MAP_TILE_H_