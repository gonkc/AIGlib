#include "EffectGrayscale.h"

namespace AIGlib
{
	EffectGrayscale::EffectGrayscale()
	{
		Shader* shader = new Shader(nullptr);
		shader->addVertexShader("../shaders/shader.effectgrayscale.vert");
		shader->addFragmentShader("../shaders/shader.effectgrayscale.frag");
		shader->compileAndLinkShaders();
		setShader(shader);
	}

	EffectGrayscale::~EffectGrayscale()
	{

	}

	void EffectGrayscale::effectRender()
	{

	}
}