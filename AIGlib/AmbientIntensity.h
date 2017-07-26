#ifndef __AMBIENTINTENSITY_H__
#define __AMBIENTINTENSITY_H__

#include "AIGlibMacros.h"
#include "Enums.h"
#include <glm/glm.hpp>
#include <string>

namespace AIGlib
{
	class AIGLIB_API AmbientIntensity 
	{
	public:

		AmbientIntensity(std::string name = std::string());
		AmbientIntensity(glm::vec3 ambientIntensity, std::string name = std::string());
		~AmbientIntensity();

		//Getters
		glm::vec3 getAmbientIntensity();

		//Setters
		void setAmbientIntensity(glm::vec3 ambientIntensity);

	private:
		glm::vec3 _ambientIntensity;
	};
}

#endif