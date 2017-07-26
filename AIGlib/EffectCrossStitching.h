#ifndef __EFFECTCROSSSTITCHING_H__
#define __EFFECTCROSSSTITCHING_H__

#include "AIGlibMacros.h"
#include "FramebufferObject.h"

namespace AIGlib
{
	class AIGLIB_API EffectCrossStitching : public FramebufferObject
	{
	public:
		EffectCrossStitching(const unsigned int& invert = 0,
			const float& size = 3.0f);
		~EffectCrossStitching();
		void effectLoadUniforms();
		void setInvert(const unsigned int& invert);
		void setSize(const float& size);

	private:
		unsigned int _invert;
		float _size;
		unsigned int _uInvert;
		unsigned int _uSize;
	};
}

#endif