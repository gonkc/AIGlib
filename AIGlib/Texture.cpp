#include "Texture.h"

namespace AIGlib 
{
	Texture::Texture(std::string filename)
	{
		_textureId = createTexture(filename);
	}

	Texture::~Texture()
	{
		freeResources();
	}

	unsigned int Texture::getTextureId() 
	{
		return _textureId;
	}

	void Texture::setTextureId(int textureId)
	{
		_textureId = textureId;
	}

	void Texture::freeResources() 
	{
		//Eliminamos el identificador de la textura
		glDeleteTextures(1, &_textureId);
		_textureId = invalidValue;
	}

	unsigned int Texture::createTexture(std::string filename)
	{
		unsigned char *map;
		unsigned int w, h;

		// Cargamos la textura
		map = loadTexture(filename.c_str(), w, h);
		if (!map) 
		{
			std::cerr << "Error cargando el fichero: "
				<< filename << std::endl;
			exit(-1);
		}

		//Creamos la textura, la activamos y la subimos a la tarjeta gráfica
		unsigned int texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);

		//Liberamos memoria de CPU
		delete[] map;

		//Creamos los mipmaps asociados a la textura
		glGenerateMipmap(GL_TEXTURE_2D);

		//Configuramos el modo de acceso
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		//Devolvemos el ID de la textura
		return texId;
	}

	unsigned char * Texture::loadTexture(const char* filename, unsigned int &w, unsigned int &h)
	{
		FreeImage_Initialise(TRUE);

		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
		if (format == FIF_UNKNOWN)
			format = FreeImage_GetFIFFromFilename(filename);
		if ((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format))
			return NULL;

		FIBITMAP* img = FreeImage_Load(format, filename);
		if (img == NULL)
			return NULL;

		FIBITMAP* tempImg = img;
		img = FreeImage_ConvertTo32Bits(img);
		FreeImage_Unload(tempImg);

		w = FreeImage_GetWidth(img);
		h = FreeImage_GetHeight(img);

		//BGRA a RGBA
		unsigned char * map = new unsigned char[4 * w*h];
		char *buff = (char*)FreeImage_GetBits(img);

		for (unsigned int j = 0; j < w*h; j++) {
			map[j * 4 + 0] = buff[j * 4 + 2];
			map[j * 4 + 1] = buff[j * 4 + 1];
			map[j * 4 + 2] = buff[j * 4 + 0];
			map[j * 4 + 3] = buff[j * 4 + 3];
		}

		FreeImage_Unload(img);
		FreeImage_DeInitialise();

		return map;
	}
}