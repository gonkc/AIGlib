#include "EffectCrossStitching.h"

namespace AIGlib
{
	EffectCrossStitching::EffectCrossStitching(const unsigned int& invert,
		const float& size) :
		_invert(invert),
		_size(size)
	{
		Shader* shader = new Shader(nullptr);
		shader->addVertexShader("../shaders/shader.effectcrossstitching.vert");
		shader->addFragmentShader("../shaders/shader.effectcrossstitching.frag");
		shader->compileAndLinkShaders();
		setShader(shader);

		_uInvert = glGetUniformLocation(getShader().getProgram(), "invert");
		_uSize = glGetUniformLocation(getShader().getProgram(), "size");
	}

	EffectCrossStitching::~EffectCrossStitching()
	{

	}

	void EffectCrossStitching::effectLoadUniforms()
	{
		if (_uInvert != -1)
		{
			glUniform1i(_uInvert, _invert);
		}
		if (_uSize != -1)
		{
			glUniform1f(_uSize, _size);
		}
	}

	void EffectCrossStitching::setInvert(const unsigned int& invert)
	{
		_invert = invert;
	}

	void EffectCrossStitching::setSize(const float& size)
	{
		_size = size;
	}
}