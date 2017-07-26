#ifndef __EFFECTKERNEL_H__
#define __EFFECTKERNEL_H__

#include "AIGlibMacros.h"
#include "FramebufferObject.h"

namespace AIGlib
{
	enum KernelType
	{
		Custom = 0,
		Sharpen5 = 1,
		Sharpen9 = 2,
		Blur = 3,
		EdgeEnhance = 4,
		EdgeDetect = 5,
		Emboss = 6
	};

	class AIGLIB_API EffectKernel : public FramebufferObject
	{
	public:
		EffectKernel(const KernelType& kernelType, 
			const glm::mat3& kernelMatrix = glm::mat3(0.0f));
		~EffectKernel();
		void effectLoadUniforms();
		void toggleKernelType();
		void setKernelType(const KernelType& kernelType, 
			const glm::mat3& kernelMatrix = glm::mat3(0.0f));

	private:
		unsigned int _kernelType;
		glm::mat3 _kernelMatrix;
		unsigned int _uKernelType;
		unsigned int _uKernelMatrix;
	};
}

#endif