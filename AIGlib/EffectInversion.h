#ifndef __EFFECTINVERSION_H__
#define __EFFECTINVERSION_H__

#include "AIGlibMacros.h"
#include "FramebufferObject.h"

namespace AIGlib
{
	class AIGLIB_API EffectInversion : public FramebufferObject
	{
	public:
		EffectInversion();
		~EffectInversion();
		void effectRender();
	};
}

#endif
