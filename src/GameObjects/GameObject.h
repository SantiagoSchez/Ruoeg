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
		OpenedDoor = '/',
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

		// Chests/Items
		Chest = 'c',
		MapItem = 'M',
		CompassItem = 'C',

		// Debug purposes
		Dummy = '?'
	};

	enum class Color
	{
		None = 0,
		Red_Black = 1,		// Walls/Non-walkable
		Green_Black = 2,	// Floor/Walkable
		Yellow_Red = 3,		// Bosses
		Cyan_Black = 4,		// Doors/Stairs/Chests
		Yellow_Black = 5,	// Enemies
		White_Black = 6,	// Default/Human/Not in FOV
		White_Green = 7,	// Orc
		White_Yellow = 8,	// Elf
		White_Red = 9,		// Dwarf
		Black_Black = 10,	// None
		Magenta_Black = 11	// Experience
	};

	explicit GameObject(Type type);
	virtual ~GameObject();

	virtual Color color() const;
	virtual void set_color(GameObject::Color color);
	virtual Type type() const;
	virtual bool walkable() const;
	virtual bool delete_object() const;
	virtual void kill_object();
	virtual bool discovered() const;
	virtual void set_discovered(bool discovered);

protected:
	Color color_;
	Type type_;
	bool walkable_;
	bool discovered_;
	bool delete_object_;

	int time_to_flash_;
};

#endif // RUOEG_GAMEOBJECTS_GAMEOBJECT_H_