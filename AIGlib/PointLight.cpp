#include "PointLight.h"
#include "RenderEngine.h"

namespace AIGlib 
{
	PointLight::PointLight(std::string name) 
	{
		setName(name);
		setNodeType(NodeType::POINTLIGHT);
	}

	PointLight::PointLight(glm::vec3 lightPosition, float constantAttenuation, float linearAttenuation, float quadraticAttenuation,
		glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name) :
		_lightPosition(lightPosition),
		_constantAttenuation(constantAttenuation),
		_linearAttenuation(linearAttenuation),
		_quadraticAttenuation(quadraticAttenuation),
		_diffuseIntensity(diffuseIntensity),
		_specularIntensity(specularIntensity)
	{
		setName(name);
		setNodeType(NodeType::POINTLIGHT);
		setAmbientIntensity(glm::vec3(0.0f));
	}

	PointLight::PointLight(glm::vec3 lightPosition, float constantAttenuation, float linearAttenuation, float quadraticAttenuation,
		glm::vec3 ambientIntensity, glm::vec3 diffuseIntensity, glm::vec3 specularIntensity, std::string name) :
		_lightPosition(lightPosition),
		_constantAttenuation(constantAttenuation),
		_linearAttenuation(linearAttenuation),
		_quadraticAttenuation(quadraticAttenuation),
		_diffuseIntensity(diffuseIntensity),
		_specularIntensity(specularIntensity)
	{
		setName(name);
		setNodeType(NodeType::POINTLIGHT);
		setAmbientIntensity(ambientIntensity);
	}

	PointLight::~PointLight()
	{

	}

	glm::vec3 PointLight::getLightPosition() 
	{
		return _lightPosition;
	}

	float PointLight::getConstantAttenuation() 
	{
		return _constantAttenuation;
	}

	float PointLight::getLinearAttenuation() 
	{
		return _linearAttenuation;
	}

	float PointLight::getQuadraticAttenuation() 
	{
		return _quadraticAttenuation;
	}

	glm::vec3 PointLight::getDiffuseIntensity() 
	{
		return _diffuseIntensity;
	}

	glm::vec3 PointLight::getSpecularIntensity() 
	{
		return _specularIntensity;
	}

	void PointLight::setLightPosition(glm::vec3 lightPosition)
	{
		_lightPosition = lightPosition;
	}

	void PointLight::setConstantAttenuation(float constantAttenuation) 
	{
		_constantAttenuation = constantAttenuation;
	}

	void PointLight::setLinearAttenuation(float linearAttenuation)
	{
		_linearAttenuation = linearAttenuation;
	}

	void PointLight::setQuadraticAttenuation(float quadraticAttenuation) 
	{
		_quadraticAttenuation = quadraticAttenuation;
	}

	void PointLight::setDiffuseIntensity(glm::vec3 diffuseIntensity)
	{
		_diffuseIntensity = diffuseIntensity;
	}
	void PointLight::setSpecularIntensity(glm::vec3 specularIntensity)
	{
		_specularIntensity = specularIntensity;
	}
}