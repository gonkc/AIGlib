#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__

#include "AIGlibMacros.h"
#include "AmbientIntensity.h"
#include "GroupNode.h"
#include "Enums.h"
#include <glm/glm.hpp>

namespace AIGlib
{
	class AIGLIB_API PointLight : public AmbientIntensity, public GroupNode
	{
	public:
		PointLight(std::string name = std::string());
		PointLight(glm::vec3 lightPosition, float constantAttenuation, float linearAttenuation, float quadraticAttenuation,
			glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name = std::string());
		PointLight(glm::vec3 lightPosition, float constantAttenuation, float linearAttenuation, float quadraticAttenuation,
			glm::vec3 ambientIntensity, glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name = std::string());
		~PointLight();

		glm::vec3 getLightPosition();
		float getConstantAttenuation();
		float getLinearAttenuation();
		float getQuadraticAttenuation();
		glm::vec3 getDiffuseIntensity();
		glm::vec3 getSpecularIntensity();

		void setLightPosition(glm::vec3 lightPosition);
		void setConstantAttenuation(float constantAttenuation);
		void setLinearAttenuation(float linearAttenuation);
		void setQuadraticAttenuation(float quadraticAttenuation);
		void setDiffuseIntensity(glm::vec3 diffuseIntensity);
		void setSpecularIntensity(glm::vec3 specularIntensity);

	private:
		glm::vec3 _lightPosition;

		float _constantAttenuation;
		float _linearAttenuation;
		float _quadraticAttenuation;

		glm::vec3 _diffuseIntensity;
		glm::vec3 _specularIntensity;

	};
}
#endif