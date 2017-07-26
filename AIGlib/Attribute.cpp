#include "Attribute.h"

namespace AIGlib {
	
	Attribute::Attribute(unsigned int location, std::string name,
		int size, GLenum type, GLboolean normalized, AttributeType attributeType) {
		_location = location;
		_name = name;
		_size = size;
		_type = type;
		_normalized = normalized;
		_attributeType = attributeType;
	}

	Attribute::~Attribute() {

	}

	unsigned int Attribute::getLocation() {
		return _location;
	}
	std::string Attribute::getName() {
		return _name;
	}
	int Attribute::getSize() {
		return _size;
	}
	GLenum Attribute::getType() {
		return _type;
	}
	GLboolean Attribute::getNormalized() {
		return _normalized;
	}
	AttributeType Attribute::getAttributeType() {
		return _attributeType;
	}
}