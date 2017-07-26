#include "UniformMaterial.h"

namespace AIGlib {

	UniformMaterial::UniformMaterial(int location, glm::vec3 color)
	{
		_location = location;
		_color = color;
	}

	UniformMaterial::~UniformMaterial() 
	{

	}

	int UniformMaterial::getLocation()
	{
		return _location;
	}

	glm::vec3 UniformMaterial::getColor() 
	{
		return _color;
	}

}