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
		Wall = '#',
		Lit = '.',
		Dummy = '?'
	};

	explicit GameObject(Type type);
	virtual ~GameObject();

	virtual Type type() const;

protected:
	Type type_;
};

#endif // RUOEG_GAMEOBJECTS_GAMEOBJECT_H_