#include "AmbientIntensity.h"
#include "RenderEngine.h"

namespace AIGlib
{
	AmbientIntensity::AmbientIntensity(std::string name) 
	{
	}

	AmbientIntensity::AmbientIntensity(glm::vec3 ambientIntensity, std::string name) 
	{
		_ambientIntensity = ambientIntensity;
	}

	AmbientIntensity::~AmbientIntensity() 
	{

	}

	glm::vec3 AmbientIntensity::getAmbientIntensity()
	{
		return _ambientIntensity;
	}

	void AmbientIntensity::setAmbientIntensity(glm::vec3 ambientIntensity)
	{
		_ambientIntensity = ambientIntensity;
	}
}