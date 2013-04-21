//
// Virtual class for all entities in the game 
//

#ifndef RUOEG_GAMEOBJECTS_GAMEOBJECT_H_
#define RUOEG_GAMEOBJECTS_GAMEOBJECT_H_

class GameObject
{
public:
	enum class Type : char
	{
		// Terrains
		None = ' ',
		VerticalWall = '|',
		HorizontalWall = '-',
		Corridor = '#',
		Lit = '.',
		Door = '+',
		DownStairs = '>',

		// Player
		Player = '@',

		// Enemies
		SmallDragon = 'd',
		Dragon = 'D',
		SmallGoblin = 'g',
		Goblin = 'G',
		SmallSkeleton = 's',
		Skeleton = 'S',
		SmallTroll = 't',
		Troll = 'T',

		// Chests
		Chest = 'C',

		// Debug purposes
		Dummy = '?'
	};

	explicit GameObject(Type type);
	virtual ~GameObject();

	virtual Type type() const;
	virtual bool walkable() const;

protected:
	Type type_;
	bool walkable_;
};

#endif // RUOEG_GAMEOBJECTS_GAMEOBJECT_H_