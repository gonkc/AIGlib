#ifndef __TEXTUREMATERIAL_H__
#define __TEXTUREMATERIAL_H__

#include "AIGlibMacros.h"

namespace AIGlib
{
	class TextureMaterial
	{
	public:
		TextureMaterial(int binding, unsigned int textureId);
		~TextureMaterial();

		int getBinding();
		unsigned int getTextureId();

	private:
		int _binding;
		unsigned int _textureId;
	};
}

#endif