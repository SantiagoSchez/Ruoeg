#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::addString(string_ptr name, string_ptr value)
{
	strings_[name] = value;
}

string_ptr ResourceManager::getString(string_ptr name)
{
	return strings_[name];
}