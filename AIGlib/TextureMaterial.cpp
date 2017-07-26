#include "TextureMaterial.h"

namespace AIGlib {

	TextureMaterial::TextureMaterial(int binding, unsigned int textureId) {
    _binding = binding;
		_textureId = textureId;
	}
	TextureMaterial::~TextureMaterial() {

	}

	int TextureMaterial::getBinding() {
		return _binding;
	}
	unsigned int TextureMaterial::getTextureId() {
		return _textureId;
	}

}