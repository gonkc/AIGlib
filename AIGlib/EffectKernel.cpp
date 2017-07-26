#include "EffectKernel.h"

namespace AIGlib
{
	EffectKernel::EffectKernel(const KernelType& kernelType, 
		const glm::mat3& kernelMatrix) :
		_kernelType(kernelType),
		_kernelMatrix(kernelMatrix)
	{
		Shader* shader = new Shader(nullptr);
		shader->addVertexShader("../shaders/shader.effectkernel.vert");
		shader->addFragmentShader("../shaders/shader.effectkernel.frag");
		shader->compileAndLinkShaders();
		setShader(shader);

		_uKernelType = glGetUniformLocation(getShader().getProgram(), "kernelType");
		_uKernelMatrix = glGetUniformLocation(getShader().getProgram(), "kernelMatrix");
	}

	EffectKernel::~EffectKernel()
	{
	
	}

	void EffectKernel::effectLoadUniforms()
	{
		if (_uKernelType != -1)
		{
			glUniform1i(_uKernelType, _kernelType);
		}
		if (_uKernelMatrix != -1)
		{
			glUniformMatrix3fv(_uKernelMatrix, 1, GL_FALSE, &(_kernelMatrix[0][0]));
		}
	}

	void EffectKernel::toggleKernelType()
	{
		switch (_kernelType)
		{
		case 0:
			_kernelType = 1;
			std::cout << "Sharpen 5 matrix enabled." << std::endl;
			break;
		case 1:
			_kernelType = 2;
			std::cout << "Sharpen 9 matrix enabled." << std::endl;
			break;
		case 2:
			_kernelType = 3;
			std::cout << "Blur 5 matrix enabled." << std::endl;
			break;
		case 3:
			_kernelType = 4;
			std::cout << "Edge enhance matrix enabled." << std::endl;
			break;
		case 4:
			_kernelType = 5;
			std::cout << "Edge detect matrix enabled." << std::endl;
			break;
		case 5:
			_kernelType = 6;
			std::cout << "Emboss matrix enabled." << std::endl;
			break;
		case 6:
			if (_kernelMatrix != glm::mat3(0.0f))
			{
				_kernelType = 0;
				std::cout << "Custom matrix enabled." << std::endl;
			}
			else
			{
				_kernelType = 1;
				std::cout << "Sharpen 5 matrix enabled." << std::endl;
			}
			break;
		}
	}

	void EffectKernel::setKernelType(const KernelType& kernelType,
		const glm::mat3& kernelMatrix)
	{
		_kernelType = kernelType;
		_kernelMatrix = kernelMatrix;
	}
}