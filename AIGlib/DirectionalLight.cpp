#include "DirectionalLight.h"
#include "RenderEngine.h"

namespace AIGlib
{
	DirectionalLight::DirectionalLight(std::string name) 
	{
		setName(name);
		setNodeType(NodeType::DIRECTIONALLIGHT);
	}

	DirectionalLight::DirectionalLight(glm::vec3 lightDirection, glm::vec3 diffuseIntensity, 
		glm::vec3 specularIntensity, std::string name) :
		_lightDirection(lightDirection),
		_diffuseIntensity(diffuseIntensity),
		_specularIntensity(specularIntensity)
	{
		setName(name);
		setNodeType(NodeType::DIRECTIONALLIGHT);
		setAmbientIntensity(glm::vec3(0.0f));
	}

	DirectionalLight::DirectionalLight(glm::vec3 lightDirection, glm::vec3 ambientIntensity, 
		glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name) :
		_lightDirection(lightDirection),
		_diffuseIntensity(diffuseIntensity),
		_specularIntensity(specularIntensity)
	{
		setName(name);
		setNodeType(NodeType::DIRECTIONALLIGHT);
		setAmbientIntensity(ambientIntensity);
	}

	DirectionalLight::~DirectionalLight() 
	{

	}

	glm::vec3 DirectionalLight::getLightDirection() 
	{
		return _lightDirection;
	}

	glm::vec3 DirectionalLight::getDiffuseIntensity() 
	{
		return _diffuseIntensity;
	}

	glm::vec3 DirectionalLight::getSpecularIntensity() 
	{
		return _specularIntensity;
	}

	void DirectionalLight::setLightDirection(glm::vec3 lightDirection) 
	{
		_lightDirection = lightDirection;
	}

	void DirectionalLight::setDiffuseIntensity(glm::vec3 diffuseIntensity) 
	{
		_diffuseIntensity = diffuseIntensity;
	}

	void DirectionalLight::setSpecularIntensity(glm::vec3 specularIntensity) 
	{
		_specularIntensity = specularIntensity;
	}
}