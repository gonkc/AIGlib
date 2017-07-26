#include "Material.h"

namespace AIGlib {

	Material::Material(int location, TextureMaterialType textureType) {
		_location = location;
		_sourceType = MaterialSourceType::TEXTURE;
		_textureType = textureType;

	}
	Material::Material(int location, unsigned int id, TextureMaterialType textureType) {
		_location = location;
		_id = id;
		_sourceType = MaterialSourceType::TEXTURE;
		_textureType = textureType;
	}
	Material::Material(int location, UniformMaterialType uniformType) {
		_location = location;
		_sourceType = MaterialSourceType::UNIFORM;
		_uniformType = uniformType;

	}
	Material::Material(int location, unsigned int id, UniformMaterialType uniformType) {
		_location = location;
		_id = id;
		_sourceType = MaterialSourceType::UNIFORM;
		_uniformType = uniformType;
	}
	Material::~Material() {

	}

	int Material::getLocation() {
		return _location;
	}
	unsigned int Material::getId() {
		return _id;
	}
	MaterialSourceType Material::getSourceType() {
		return _sourceType;
	}
	TextureMaterialType Material::getTextureType() {
		return _textureType;
	}
	UniformMaterialType Material::getUniformType() {
		return _uniformType;
	}

	void Material::setLocation(int location) {
		_location = location;
	}
	void Material::setId(unsigned int id) {
		_id = id;
	}
	void Material::setSourceType(MaterialSourceType sourceType) {
		_sourceType = sourceType;
	}
	void Material::setTextureType(TextureMaterialType textureType) {
		_textureType = textureType;
	}
	void Material::setUniformType(UniformMaterialType uniformType) {
		_uniformType = uniformType;
	}
}