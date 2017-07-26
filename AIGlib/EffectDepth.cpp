#include "EffectDepth.h"

namespace AIGlib
{
	EffectDepth::EffectDepth(const float& depthFactor) :
		_depthFactor(depthFactor)
	{
		Shader* shader = new Shader(nullptr);
		shader->addVertexShader("../shaders/shader.effectdepth.vert");
		shader->addFragmentShader("../shaders/shader.effectdepth.frag");
		shader->compileAndLinkShaders();
		setShader(shader);

		_uDepthFactor = glGetUniformLocation(getShader().getProgram(), "depthFactor");
	}

	EffectDepth::~EffectDepth()
	{

	}

	void EffectDepth::effectRender()
	{

	}

	void EffectDepth::effectLoadUniforms()
	{
		if (_uDepthFactor != -1)
		{
			glUniform1f(_uDepthFactor, _depthFactor);
		}
	}

	void EffectDepth::setDepthFactor(const float& depthFactor)
	{
		_depthFactor = depthFactor;
	}
}