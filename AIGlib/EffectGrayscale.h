#ifndef __EFFECTGRAYSCALE_H__
#define __EFFECTGRAYSCALE_H__

#include "AIGlibMacros.h"
#include "FramebufferObject.h"

namespace AIGlib
{
	class AIGLIB_API EffectGrayscale : public FramebufferObject
	{
	public:
		EffectGrayscale();
		~EffectGrayscale();
		void effectRender();
	};
}

#endif
