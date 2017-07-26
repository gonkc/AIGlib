#include "EffectInversion.h"

namespace AIGlib
{

	EffectInversion::EffectInversion()
	{
		Shader* shader = new Shader(nullptr);
		shader->addVertexShader("../shaders/shader.effectinversion.vert");
		shader->addFragmentShader("../shaders/shader.effectinversion.frag");
		shader->compileAndLinkShaders();
		setShader(shader);
	}
	
	EffectInversion::~EffectInversion()
	{

	}

	void EffectInversion::effectRender()
	{

	}
}