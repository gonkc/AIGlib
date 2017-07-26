#ifndef __EFFECTDEPTH_H__
#define __EFFECTDEPTH_H__

#include "AIGlibMacros.h"
#include "FramebufferObject.h"

namespace AIGlib
{
	class AIGLIB_API EffectDepth : public FramebufferObject
	{
	public:
		EffectDepth(const float& depthFactor = 30.0f);
		~EffectDepth();
		void effectRender();
		void effectLoadUniforms();
		void setDepthFactor(const float& depthFactor);

	private:
		float _depthFactor;
		unsigned int _uDepthFactor;
	};
}

#endif
