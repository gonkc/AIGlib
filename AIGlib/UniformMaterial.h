#ifndef __UNIFORMMATERIAL_H__
#define __UNIFORMMATERIAL_H__

#include "AIGlibMacros.h"
#include <glm/glm.hpp>

namespace AIGlib
{
	class UniformMaterial
	{
	public:
		UniformMaterial(int location, glm::vec3 color);
		~UniformMaterial();

		int getLocation();
		glm::vec3 getColor();

	private:
		int _location;
		glm::vec3 _color;
	};
}

#endif
