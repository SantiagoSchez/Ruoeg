//
// ResourceManager class following a singleton pattern, as we only need
// one instance of the class.
// 

#ifndef RUOEG_RESOURCEMANAGER_H_
#define RUOEG_RESOURCEMANAGER_H_

#include <map>
#include <memory>

typedef const char* string_ptr;

class ResourceManager
{
public:
	static ResourceManager& getInstance();

	// Adder and getter for string resources
	void addString(string_ptr, string_ptr);
	string_ptr getString(string_ptr name);

private:
	ResourceManager();

	// Map data structure for game strings
	std::map<string_ptr, string_ptr> strings_;
	//////////////////////////////////////

	// Disallowing copy constructor and assignment operator
	ResourceManager(ResourceManager const&);
	ResourceManager& operator=(ResourceManager const&);
};

#endif // RUOEG_RESOURCEMANAGER_H_