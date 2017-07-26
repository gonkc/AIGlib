#include "Fog.h"

namespace AIGlib 
{
	Fog::Fog() :
		_fogType(FogType::None),
		_fogColor(glm::vec3(1.0f)),
		_fogStarts(20.0f),
		_fogEnds(80.0f),
		_fogDensity(0.05f)
	{

	}

	Fog::~Fog()
	{

	}

	void Fog::loadUniforms(Shader* shader)
	{
		std::array<int, N_FOG_UNIFORMS> uFogUniforms = shader->getFogUniforms();
		if (uFogUniforms[0] != -1)
		{
			glUniform1i(uFogUniforms[0], _fogType);
		}
		if (uFogUniforms[1] != -1)
		{
			glUniform3fv(uFogUniforms[1], 1, glm::value_ptr(_fogColor));
		}
		if (uFogUniforms[2] != -1)
		{
			glUniform1f(uFogUniforms[2], _fogDensity);
		}
		if (uFogUniforms[3] != -1)
		{
			glUniform1f(uFogUniforms[3], _fogStarts);
		}
		if (uFogUniforms[4] != -1)
		{
			glUniform1f(uFogUniforms[4], _fogEnds);
		}
	}

	void Fog::toggleFogType()
	{
		switch (_fogType)
		{
		case 0:
			_fogType = 1;
			std::cout << "Linear fog enabled." << std::endl;
			break;
		case 1:
			_fogType = 2;
			std::cout << "Exponential fog enabled." << std::endl;
			break;
		case 2:
			_fogType = 3;
			std::cout << "Exponential squared fog enabled." << std::endl;
			break;
		case 3:
			_fogType = 0;
			std::cout << "Fog disabled." << std::endl;
			break;
		}
	}
	void Fog::setFogType(const FogType& fogType)
	{
		_fogType = fogType;
	}
	
	void Fog::setNoneFog()
	{
		_fogType = FogType::None;
	}

	void Fog::setLinearFog(const float& fogStarts, const float& fogEnds)
	{
		_fogStarts = fogStarts;
		_fogEnds = fogEnds;
		_fogType = FogType::Linear;
	}

	void Fog::setExponentialFog(const float& fogDensity)
	{
		_fogDensity = fogDensity;
		_fogType = FogType::Exponential;
	}

	void Fog::setExponentialSquaredFog(const float& fogDensity)
	{
		_fogDensity = fogDensity;
		_fogType = FogType::ExponentialSquared;
	}

	void Fog::setFogDensity(const float& fogDensity)
	{
		_fogDensity = fogDensity;
	}

	void Fog::setFogColor(const glm::vec3& fogColor)
	{
		_fogColor = fogColor;
	}

	void Fog::setFogStarts(const float& fogStarts)
	{
		_fogStarts = fogStarts;
	}

	void Fog::setFogEnds(const float& fogEnds)
	{
		_fogEnds = fogEnds;
	}
}