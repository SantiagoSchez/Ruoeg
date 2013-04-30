//
// Wrapper for a std::vector of GameObjects*.
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

	GameObjectPtr top();
	GameObjectPtr element(int index);
	void add(GameObjectPtr game_object);
	void pop();
	int elements() const;
	bool visited() const;
	void set_visited(bool visited);

private:
	std::vector<GameObjectPtr> raw_;
	bool visited_;
};

#endif  // RUOEG_MAP_TILE_H_