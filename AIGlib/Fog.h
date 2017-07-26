#ifndef __FOG_H__
#define __FOG_H__

#include "AIGlibMacros.h"
#include "Shader.h"

namespace AIGlib
{
	enum FogType 
	{
		None = 0,
		Linear = 1,
		Exponential = 2,
		ExponentialSquared = 3
	};

	class AIGLIB_API Fog
	{
	public:
		Fog();
		~Fog();
		void loadUniforms(Shader* shader);
		void toggleFogType();
		void setFogColor(const glm::vec3& fogColor);
		void setFogType(const FogType& fogType);
		void setFogStarts(const float& fogStarts);
		void setFogEnds(const float& fogEnds);
		void setFogDensity(const float& fogDensity);
		void setNoneFog();
		void setLinearFog(const float& fogStarts, const float& fogEnds);
		void setExponentialFog(const float& fogDensity);
		void setExponentialSquaredFog(const float& fogDensity);

	private:
		unsigned int _fogType;
		glm::vec3 _fogColor;
		float _fogStarts;
		float _fogEnds;
		float _fogDensity;
	};
}

#endif