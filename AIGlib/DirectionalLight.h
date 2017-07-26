#ifndef __DIRECTIONALLIGHT_H__
#define __DIRECTIONALLIGHT_H__

#include "AIGlibMacros.h"
#include "AmbientIntensity.h"
#include "GroupNode.h"
#include "Enums.h"
#include <glm/glm.hpp>

namespace AIGlib
{
	class AIGLIB_API DirectionalLight : public AmbientIntensity, public GroupNode
	{
	public:

		DirectionalLight(std::string name = std::string());
		DirectionalLight(glm::vec3 lightDirection, glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name = std::string());
		DirectionalLight(glm::vec3 lightDirection, glm::vec3 ambientIntensity, glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name = std::string());
		~DirectionalLight();

		glm::vec3 getLightDirection();
		glm::vec3 getDiffuseIntensity();
		glm::vec3 getSpecularIntensity();

		void setLightDirection(glm::vec3 lightDirection);
		void setDiffuseIntensity(glm::vec3 diffuseIntensity);
		void setSpecularIntensity(glm::vec3 specularIntensity);

	private:
		glm::vec3 _lightDirection;
		glm::vec3 _diffuseIntensity;
		glm::vec3 _specularIntensity;
	};
}

#endif