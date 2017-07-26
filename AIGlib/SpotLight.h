#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

#include "AIGlibMacros.h"
#include "AmbientIntensity.h"
#include "GroupNode.h"
#include "Enums.h"
#include <glm/glm.hpp>

namespace AIGlib 
{
	class AIGLIB_API SpotLight : public AmbientIntensity, public GroupNode
	{
	public:
		SpotLight(std::string name = std::string());
		SpotLight(glm::vec3 lightPosition, glm::vec3 coneDirection, float coneAngle, float constantAttenuation, float linearAttenuation,
			float quadraticAttenuation, glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name = std::string());
		SpotLight(glm::vec3 lightPosition, glm::vec3 coneDirection, float coneAngle, float constantAttenuation, float linearAttenuation,
			float quadraticAttenuation, glm::vec3 ambientIntensity, glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name = std::string());
		~SpotLight();

		glm::vec3 getLightPosition();
		glm::vec3 getConeDirection();
		float getConeAngle();
		float getConstantAttenuation();
		float getLinearAttenuation();
		float getQuadraticAttenuation();
		glm::vec3 getDiffuseIntensity();
		glm::vec3 getSpecularIntensity();

		void setLightPosition(glm::vec3 lightPosition);
		void setConeDirection(glm::vec3 coneDirection);
		void setConeAngle(float coneAngle);
		void setConstantAttenuation(float constantAttenuation);
		void setLinearAttenuation(float linearAttenuation);
		void setQuadraticAttenuation(float quadraticAttenuation);
		void setDiffuseIntensity(glm::vec3 diffuseIntensity);
		void setSpecularIntensity(glm::vec3 specularIntensity);

	private:
		glm::vec3 _lightPosition;
		glm::vec3 _coneDirection;
		float _coneAngle;

		float _constantAttenuation;
		float _linearAttenuation;
		float _quadraticAttenuation;

		glm::vec3 _diffuseIntensity;
		glm::vec3 _specularIntensity;
	};
}

#endif
