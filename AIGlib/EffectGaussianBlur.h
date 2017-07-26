#ifndef __EFFECTGAUSSIANBLUR_H__
#define __EFFECTGAUSSIANBLUR_H__

#include "AIGlibMacros.h"
#include "FramebufferObject.h"

namespace AIGlib
{
	class AIGLIB_API EffectGaussianBlur : public FramebufferObject
	{
	public:
		EffectGaussianBlur(const float& blurSize = 20.0f, const float& bloom = 0.5f);
		~EffectGaussianBlur();
		void effectLoadUniforms();
		void setBlurSize(const float& blurSize);
		void setBloom(const float& bloom);
	private:
		float _blurSize;
		float _bloom;
		unsigned int _uBlurSize;
		unsigned int _uBloom;
	};
}

#endif
