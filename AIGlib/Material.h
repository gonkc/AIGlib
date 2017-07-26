#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "AIGlibMacros.h"
#include "Enums.h"
#include <string>

namespace AIGlib
{
	class AIGLIB_API Material
	{
	public:
		Material(int location, TextureMaterialType textureType);
		Material(int location, UniformMaterialType uniformType);
		Material(int location, unsigned int id, TextureMaterialType textureType);
		Material(int location, unsigned int id, UniformMaterialType uniformType);
		~Material();

		int getLocation();
		unsigned int getId();
		MaterialSourceType getSourceType();
		TextureMaterialType getTextureType();
		UniformMaterialType getUniformType();

		void setLocation(int materialLocation);
		void setId(unsigned int materialId);
		void setSourceType(MaterialSourceType sourceType);
		void setTextureType(TextureMaterialType textureType);
		void setUniformType(UniformMaterialType uniformType);

	private:
		int _location;
		unsigned int _id;
		MaterialSourceType _sourceType;
		TextureMaterialType _textureType;
		UniformMaterialType _uniformType;
	};
}
#endif
