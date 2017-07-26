#include "SpotLight.h"
#include "RenderEngine.h"
#include <glm/gtc/quaternion.hpp>

namespace AIGlib {
	SpotLight::SpotLight(std::string name) {
		setName(name);
		setNodeType(NodeType::SPOTLIGHT);
		setModel(glm::mat4(1.0f));
	}

	SpotLight::SpotLight(glm::vec3 lightPosition, glm::vec3 coneDirection, float coneAngle, 
		float constantAttenuation, float linearAttenuation,	float quadraticAttenuation, 
		glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name) :
		_lightPosition(lightPosition),
		_coneDirection(coneDirection),
		_coneAngle(coneAngle),
		_constantAttenuation(constantAttenuation),
		_linearAttenuation(linearAttenuation),
		_quadraticAttenuation(quadraticAttenuation),
		_diffuseIntensity(diffuseIntensity),
		_specularIntensity(specularIntensity)
	{

		setName(name);
		setNodeType(NodeType::SPOTLIGHT);
		setAmbientIntensity(glm::vec3(0.0f));

		setModel(glm::mat4(1.0f));
		_orientation = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	SpotLight::SpotLight(glm::vec3 lightPosition, glm::vec3 coneDirection, float coneAngle, 
		float constantAttenuation, float linearAttenuation,	float quadraticAttenuation, 
		glm::vec3 ambientIntensity, glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, 
		std::string name) :
		_lightPosition(lightPosition),
		_coneDirection(coneDirection),
		_coneAngle(coneAngle),
		_constantAttenuation(constantAttenuation),
		_linearAttenuation(linearAttenuation),
		_quadraticAttenuation(quadraticAttenuation),
		_diffuseIntensity(diffuseIntensity),
		_specularIntensity(specularIntensity)
	{

		setName(name);
		setNodeType(NodeType::SPOTLIGHT);
		setAmbientIntensity(ambientIntensity);

		setModel(glm::mat4(1.0f));
		_orientation = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	}
	SpotLight::~SpotLight()
	{

	}

	glm::vec3 SpotLight::getLightPosition()
	{
		return _lightPosition;
	}

	glm::vec3 SpotLight::getConeDirection() 
	{
		return _coneDirection;
	}

	float SpotLight::getConeAngle()
	{
		return _coneAngle;
	}

	float SpotLight::getConstantAttenuation() 
	{
		return _constantAttenuation;
	}

	float SpotLight::getLinearAttenuation() 
	{
		return _linearAttenuation;
	}

	float SpotLight::getQuadraticAttenuation() 
	{
		return _quadraticAttenuation;
	}

	/*glm::vec3 SpotLight::getAmbientIntensity() {
		return _ambientIntensity;
	}*/

	glm::vec3 SpotLight::getDiffuseIntensity() 
	{
		return _diffuseIntensity;
	}

	glm::vec3 SpotLight::getSpecularIntensity() 
	{
		return _specularIntensity;
	}

	void SpotLight::setLightPosition(glm::vec3 lightPosition) 
	{
		_lightPosition = lightPosition;
	}

	void SpotLight::setConeDirection(glm::vec3 coneDirection) 
	{
		_coneDirection = coneDirection;
	}

	void SpotLight::setConeAngle(float coneAngle)
	{
		_coneAngle = coneAngle;
	}
	void SpotLight::setConstantAttenuation(float constantAttenuation)
	{
		_constantAttenuation = constantAttenuation;
	}

	void SpotLight::setLinearAttenuation(float linearAttenuation) 
	{
		_linearAttenuation = linearAttenuation;
	}

	void SpotLight::setQuadraticAttenuation(float quadraticAttenuation) 
	{
		_quadraticAttenuation = quadraticAttenuation;
	}

	/*void SpotLight::setAmbientIntensity(glm::vec3 ambientIntensity) {
		_ambientIntensity = ambientIntensity;
	}*/

	void SpotLight::setDiffuseIntensity(glm::vec3 diffuseIntensity)
	{
		_diffuseIntensity = diffuseIntensity;
	}

	void SpotLight::setSpecularIntensity(glm::vec3 specularIntensity) 
	{
		_specularIntensity = specularIntensity;
	}
}