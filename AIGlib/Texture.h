#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "AIGlibMacros.h"
#include <iostream>
#include <string>
#include <FreeImage/FreeImage.h>
#include <gl/glew.h>

namespace AIGlib 
{
	class AIGLIB_API Texture 
	{
	public:
		Texture(std::string filepath);
		~Texture();

		unsigned int getTextureId();
		void setTextureId(int textureId);

	private:
		unsigned int _textureId;

		void freeResources();
		unsigned int createTexture(std::string filename);
		unsigned char* loadTexture(const char* filename, unsigned int &w, unsigned int &h);
	};
}

#endif