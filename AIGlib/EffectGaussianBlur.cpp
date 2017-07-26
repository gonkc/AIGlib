#include "EffectGaussianBlur.h"

namespace AIGlib
{

	EffectGaussianBlur::EffectGaussianBlur(const float& blurSize, const float& bloom) :
		_blurSize(blurSize),
		_bloom(bloom)
	{
		Shader* shader = new Shader(nullptr);
		shader->addVertexShader("../shaders/shader.effectgaussianblur.vert");
		shader->addFragmentShader("../shaders/shader.effectgaussianblur.frag");
		shader->compileAndLinkShaders();
		setShader(shader);

		_uBlurSize = glGetUniformLocation(getShader().getProgram(), "blurSize");
		_uBloom = glGetUniformLocation(getShader().getProgram(), "bloom");
	}

	EffectGaussianBlur::~EffectGaussianBlur()
	{

	}

	void EffectGaussianBlur::effectLoadUniforms()
	{
		if (_uBlurSize != -1)
		{
			glUniform1f(_uBlurSize, _blurSize);
		}
		if (_uBloom != -1)
		{
			glUniform1f(_uBloom, _bloom);
		}
	}

	void EffectGaussianBlur::setBlurSize(const float& blurSize)
	{
		_blurSize = blurSize;	
	}

	void EffectGaussianBlur::setBloom(const float& bloom)
	{
		_bloom = bloom;
	}

}