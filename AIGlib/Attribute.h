#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

#include "AIGlibMacros.h"
#include "Enums.h"
#include <string>
#include "GL/glew.h"

namespace AIGlib
{
	class AIGLIB_API Attribute
	{
		public:
			Attribute(unsigned int location, std::string name, 
				int size, GLenum type, GLboolean normalized, AttributeType attributeType);
			~Attribute();

			unsigned int getLocation();
			std::string getName();
			int getSize();
			GLenum getType();
			GLboolean getNormalized();
			AttributeType getAttributeType();

		private:
			unsigned int _location;
			std::string _name;
			int _size;
			GLenum _type;
			GLboolean _normalized;
			AttributeType _attributeType;
	};
}

#endif