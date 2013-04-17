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

#include "../GameObjects/GameObject.h"

class Tile
{
public:
	Tile();
	~Tile();

	GameObject& top();
	GameObject& layer(unsigned int layer);
	void add(GameObject &game_object);
	int layers() const;
	bool visited() const;
	void set_visited(bool visited);

private:
	std::vector<GameObject> raw_;
	bool visited_;
};

#endif  // RUOEG_MAP_TILE_H_