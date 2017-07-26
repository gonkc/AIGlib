#include "RenderEngine.h"
#include "LightObject.h"

namespace AIGlib 
{
	std::unique_ptr<RenderEngine> RenderEngine::_instance = nullptr;
	std::once_flag RenderEngine::_onceFlag;

	RenderEngine& RenderEngine::get()
	{
		std::call_once(RenderEngine::_onceFlag, []()
		{
			_instance.reset(new RenderEngine);
		});
		return *_instance;
	}

	Scene& RenderEngine::getScene() const 
	{
		return *_scene;
	}

	GeometryProperties& RenderEngine::getGeometryProperties(unsigned int index) const
	{
		return *_geometries[index];
	}

	MaterialProperties& RenderEngine::getMaterialProperties(unsigned int index) const
	{
		return *_materialProperties[index];
	}

	Shader& RenderEngine::getShader(MeshProperties* meshProperties) const
	{
		if (!_useCustomShaders) 
		{
			Shader* shader = nullptr;
 			for (auto s : _shaders) 
			{
				if (*s.first == *meshProperties)
				{
					shader = s.second;
					break;
				}
			}
			return *shader;
		}
		else 
		{
			return *_customShaders[meshProperties->getCustomShaderId()];
		}
	}

	Texture& RenderEngine::getTexture(unsigned int index) const
	{
		return *_textures[index];
	}

	void RenderEngine::setScene(Scene *scene)
	{
		_scene = scene;
	}

  unsigned int RenderEngine::addGeometry(GeometryProperties* geometryProperties)
	{
    unsigned int index = invalidValue;
    for (unsigned int i = 0; i < _geometries.size(); ++i)
    {
      if (*geometryProperties == *_geometries.at(i))
      {
        index = i;
        break;
      }
    }
    if (index != invalidValue)
    {
      //std::cout << "GeometryProperties already exists on index " << index << ", using it." << std::endl;
    }
    else
    {
      geometryProperties->uploadGeometry();
      _geometries.push_back(geometryProperties);
      index = static_cast<unsigned int>(_geometries.size() - 1);
      //std::cout << "GeometryProperties registered successfully on index " << index << "." << std::endl;
    }
    return index;
	}

	std::vector<MaterialProperties*> RenderEngine::getMaterialProperties() const
	{
		return _materialProperties;
	}
	unsigned int RenderEngine::addMaterialProperties(MaterialProperties* materialProperties)
	{
		_materialProperties.push_back(materialProperties);
		return (unsigned int)_materialProperties.size() - 1;
	}

	void RenderEngine::addShader(MeshProperties* meshProperties, Shader* shader) 
	{
		if (_useCustomShaders) 
		{
			std::cerr << "Error - You can't use addShader because You're using custom shaders." << std::endl;
			exit(-1);
		}
		if (!_shaders.insert(std::make_pair(meshProperties, shader)).second) 
		{
			std::cerr << "Error - Shader already exists!." << std::endl;
			exit(-1);
		}
	}
	int RenderEngine::addCustomShader(Shader* shader)
	{
		if (!_useCustomShaders) 
		{
			std::cerr << "Error - You can't use AddCustomShader because You're using generic shaders." << std::endl;
			exit(-1);
		}
		_customShaders.push_back(shader);
		return (unsigned int)_customShaders.size() - 1;
	}



	void RenderEngine::useCustomShaders() 
	{
		_useCustomShaders = true;
	}
	bool RenderEngine::getIfCustomShaders() 
	{
		return _useCustomShaders;
	}

	unsigned int RenderEngine::registerTexture(std::string texturePath)
	{
		_textures.push_back(new Texture(texturePath));
		std::cout << "Texture '" << texturePath.c_str() << "' registered with textureId '" << _textures[_textures.size() - 1]->getTextureId() << "'" << std::endl;
		return _textures[_textures.size() - 1]->getTextureId();
	}

	void RenderEngine::registerAttribute(unsigned int location, std::string name, int size, GLenum type, bool normalized, AttributeType attributeType)
	{
		Attribute* attribute = new Attribute(location, name, size, type, normalized, attributeType);

		if (!_attributes.insert(std::make_pair(name, attribute)).second)
		{
			// El elemento ya existe
			std::cerr << "Error - Attribute " << name.c_str() << " is already registered!" << std::endl;
			exit(-1);
		}

		std::cout << "Attribute '" << name.c_str() << "' was succesfully registered." <<std::endl;
	}

  GLenum RenderEngine::getOpenGLDataType(AttributeDataType type)
  {
    GLenum dataType = 0;
    switch (type)
    {
    case AttributeDataType::FLOAT:
      dataType = GL_FLOAT;
      break;
    case AttributeDataType::INT:
      dataType = GL_INT;
      break;
    case AttributeDataType::UNSIGNED_INT:
      dataType = GL_UNSIGNED_INT;
      break;
    case AttributeDataType::BYTE:
      dataType = GL_BYTE;
      break;
    case AttributeDataType::UNSIGNED_BYTE:
      dataType = GL_UNSIGNED_BYTE;
      break;
    case AttributeDataType::SHORT:
      dataType = GL_SHORT;
      break;
    case AttributeDataType::UNSIGNED_SHORT:
      dataType = GL_UNSIGNED_SHORT;
      break;
    case AttributeDataType::HALF_FLOAT:
      dataType = GL_HALF_FLOAT;
      break;
    case AttributeDataType::DOUBLE:
      dataType = GL_DOUBLE;
      break;
    case AttributeDataType::FIXED:
      dataType = GL_FIXED;
      break;
    case AttributeDataType::INT_2_10_10_10_REV:
      dataType = GL_INT_2_10_10_10_REV;
      break;
    case AttributeDataType::UNSIGNED_INT_2_10_10_10_REV:
      dataType = GL_UNSIGNED_INT_2_10_10_10_REV;
      break;
    case AttributeDataType::UNSIGNED_INT_10F_11F_11F_REV:
      dataType = GL_UNSIGNED_INT_10F_11F_11F_REV;
      break;
    }
    return dataType;
  }

  AttributeDataType RenderEngine::getOpenGLDataType(std::string type)
  {
    AttributeDataType dataType;
    if (type == "FLOAT")
    {
      dataType = AttributeDataType::FLOAT;
    }
    else if (type == "INT")
    {
      dataType = AttributeDataType::INT;
    }
    else if (type == "UNSIGNED_INT")
    {
      dataType = AttributeDataType::UNSIGNED_INT;
    }
    else if (type == "BYTE")
    {
      dataType = AttributeDataType::BYTE;
    }
    else if (type == "UNSIGNED_BYTE")
    {
      dataType = AttributeDataType::UNSIGNED_BYTE;
    }
    else if (type == "SHORT")
    {
      dataType = AttributeDataType::SHORT;
    }
    else if (type == "UNSIGNED_SHORT")
    {
      dataType = AttributeDataType::UNSIGNED_SHORT;
    }
    else if (type == "HALF_FLOAT")
    {
      dataType = AttributeDataType::HALF_FLOAT;
    }
    else if (type == "DOUBLE")
    {
      dataType = AttributeDataType::DOUBLE;
    }
    else if (type == "FIXED")
    {
      dataType = AttributeDataType::FIXED;
    }
    else if (type == "INT_2_10_10_10_REV")
    {
      dataType = AttributeDataType::INT_2_10_10_10_REV;
    }
    else if (type == "UNSIGNED_INT_2_10_10_10_REV")
    {
      dataType = AttributeDataType::UNSIGNED_INT_2_10_10_10_REV;
    }
    else if (type == "UNSIGNED_INT_10F_11F_11F_REV")
    {
      dataType = AttributeDataType::UNSIGNED_INT_10F_11F_11F_REV;
    }
    else
    {
      std::cerr << "Error reading field 'type' of XML attributes file!" << std::endl;
      exit(-1);
    }
    return dataType;
  }

	void RenderEngine::registerCustomAttribute(unsigned int location, std::string name, int size, AttributeDataType type, bool normalized, AttributeType attributeType)
	{
		if (!_useCustomShaders) 
		{
			std::cerr << "Error - You can't register a custom attribute while using generic shaders! Use RegisterGenericAttribute(location, size, type, normalized, textureMaterialType) instead." << std::endl;
			exit(-1);
		}
    GLenum dataType = getOpenGLDataType(type);
		registerAttribute(location, name, size, dataType, normalized, attributeType);
	}

	void RenderEngine::registerGenericAttribute(unsigned int location, int size, AttributeDataType type, bool normalized, AttributeType attributeType)
	{
		std::string name;
		if (_useCustomShaders) {
			std::cerr << "Error - You can't register a generic attribute while using custom shaders! Use RegisterCustomAttribute(location, name, size, type, normalized, textureMaterialType) instead." << std::endl;
			exit(-1);
		}

		switch (attributeType) {
		case AttributeType::POS:
			name = "inPos";
			break;
		case AttributeType::COLOR:
			name = "inColor";
			break;
		case AttributeType::NORMAL:
			name = "inNormal";
			break;
		case AttributeType::TEXCOORD:
			name = "inTexCoord";
			break;
		case AttributeType::TANGENT:
			name = "inTangent";
			break;
		case AttributeType::BINORMAL:
			name = "inBinormal";
			break;
		}
    GLenum dataType = getOpenGLDataType(type);
		registerAttribute(location, name, size, dataType, normalized, attributeType);
	}

	void RenderEngine::registerAttributesFromXML(std::string filename)
	{
		pugi::xml_document doc;

		if (!doc.load_file(filename.c_str())) {
			std::cerr << "Error reading XML file '" << filename.c_str() << "'!." << std::endl;
			exit(-1);
		}

		pugi::xml_node attr = doc.child("Attributes");

		for (pugi::xml_node_iterator it = attr.begin(); it != attr.end(); ++it)
		{
			//std::cout << "Attribute:";

			unsigned int location = -1;
			std::string name = "";
			int size = -1;
			AttributeDataType type;
			bool normalized = false;
			AttributeType attributeType;

			for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
			{
				name = "";

				//std::cout << " " << ait->name() << "=" << ait->value();
				std::string n(ait->name());
				std::string v(ait->value());

				if (n == "location")
				{
					location = std::stoi(v);
				}
				else if (n == "name") 
				{
					name = v;
				}
				else if (n == "size")
				{
					//size = std::stoi(v);
					if (v == "1")
					{
						size = 1;
					}
					else if (v == "2")
					{
						size = 2;
					}
					else if (v == "3") 
					{
						size = 3;
					}
					else if (v == "4") 
					{
						size = 4;
					}
					else if (v == "BGRA") 
					{
						size = GL_BGRA;
					}
					else
					{
						std::cerr << "Error reading field 'size' of XML attributes file!" << std::endl;
						exit(-1);
					}
				}
				else if (n == "type") 
				{
          type = getOpenGLDataType(v);
				}
				else if (n == "normalized") 
				{
					if (v == "false")
					{
						normalized = GL_FALSE;
					}
					else if (v == "true") 
          {
						normalized = GL_TRUE;
					}
					else 
					{
						exit(-1);
					}
				}
				else if (n == "attributeType")
				{
					if (v == "POS") 
					{
						attributeType = AttributeType::POS;
					}
					else if (v == "COLOR")
					{
						attributeType = AttributeType::COLOR;
					}
					else if (v == "NORMAL")
					{
						attributeType = AttributeType::NORMAL;
					}
					else if (v == "TEXCOORD") 
					{
						attributeType = AttributeType::TEXCOORD;
					}
					else if (v == "TANGENT") 
					{
						attributeType = AttributeType::TANGENT;
					}
					else if (v == "BINORMAL") 
					{
						attributeType = AttributeType::BINORMAL;
					}
					else
					{
						std::cerr << "Error reading field 'attributeType' of XML attributes file!" << std::endl;
						exit(-1);
					}
				}

			}
			if (name == "")
			{
				registerGenericAttribute(location, size, type, normalized, attributeType);
			}
			else 
			{
				registerCustomAttribute(location, name, size, type, normalized, attributeType);
			}
		}
	}

	std::map<std::string, Attribute*> RenderEngine::getAttributes() 
	{
		return _attributes;
	}

	void RenderEngine::registerTextureMaterial(unsigned int index, std::string materialName, TextureMaterialType textureMaterialType) 
	{
		
		if (!_materials.insert(std::make_pair(materialName, new Material(index, textureMaterialType))).second)
		{
			// El elemento ya existe
			std::cerr << "Error - Texture material '" << materialName << "' is already registered!" << std::endl;
			exit(-1);
		}
		std::cout << "Texture material '" << materialName.c_str() << "' was sucesfully registered." << std::endl;
	}

	void RenderEngine::registerCustomTextureMaterial(unsigned int index, std::string materialName, TextureMaterialType textureMaterialType)
	{
		if (!_useCustomShaders)
		{
			std::cerr << "Error - You can't register a custom texture material while using generic shaders! Use RegisterGenericTextureMaterial(index, textureMaterialType) instead." << std::endl;
			exit(-1);
		}
		registerTextureMaterial(index, materialName, textureMaterialType);
	}

	void RenderEngine::registerGenericTextureMaterial(unsigned int index, TextureMaterialType textureMaterialType) 
	{
		std::string name;
		if (_useCustomShaders) 
		{
			std::cerr << "Error - You can't register a generic texture material while using custom shaders! Use RegisterCustomTextureMaterial(index, materialName, uniformMaterialType) instead." << std::endl;
			exit(-1);
		}

		switch (textureMaterialType)
		{
		case TextureMaterialType::AMBIENT:
			name = "ambientTex";
			break;
		case TextureMaterialType::DIFFUSE:
			name = "diffuseTex";
			break;
		case TextureMaterialType::SPECULAR:
			name = "specularTex";
			break;
		case TextureMaterialType::EMISSIVE:
			name = "emissiveTex";
			break;
		case TextureMaterialType::NORMAL:
			name = "normalTex";
			break;
		case TextureMaterialType::LIGHTMAP:
			name = "lightmapTex";
			break;
		case TextureMaterialType::HEIGHT:
			name = "heightTex";
			break;
		case TextureMaterialType::SHININESS:
			name = "shininessTex";
			break;
		case TextureMaterialType::OPACITY:
			name = "opacityTex";
			break;
		/*case TextureMaterialType::REFLECTIVE:
			name = "reflectiveTex";
			break;*/
		case TextureMaterialType::DISPLACEMENT:
			name = "displacementTex";
			break;
		case TextureMaterialType::AUXILIAR:
			name = "auxiliarTex";
			break;
		}

		registerTextureMaterial(index, name, textureMaterialType);
	}

	void RenderEngine::registerUniformMaterial(unsigned int index, std::string materialName, UniformMaterialType uniformMaterialType) 
	{
		if (!_materials.insert(std::make_pair(materialName, new Material(index, uniformMaterialType))).second)
		{
			// El elemento ya existe
			std::cerr << "Error - Material '" << materialName << "' is already registered!" << std::endl;
			exit(-1);
		}
		std::cout << "Uniform material '" << materialName.c_str() << "' was sucesfully registered." << std::endl;
	}

	void RenderEngine::registerCustomUniformMaterial(unsigned int index, std::string materialName, UniformMaterialType uniformMaterialType) 
	{
		if (!_useCustomShaders) 
		{
			std::cerr << "Error - You can't register a custom uniform material while using generic shaders! Use RegisterGenericUniformMaterial(index, uniformMaterialType) instead." << std::endl;
			exit(-1);
		}
		registerUniformMaterial(index, materialName, uniformMaterialType);
	}

	void RenderEngine::registerGenericUniformMaterial(unsigned int index, UniformMaterialType uniformMaterialType)
	{
		std::string name;
		if (_useCustomShaders)
		{
			std::cerr << "Error - You can't register a generic uniform material while using custom shaders! Use RegisterCustomUniformMaterial(index, materialName, uniformMaterialType) instead." << std::endl;
			exit(-1);
		}

		switch (uniformMaterialType) {
		case UniformMaterialType::AMBIENT:
			name = "ambientK";
			break;
		case UniformMaterialType::DIFFUSE:
			name = "diffuseK";
			break;
		case UniformMaterialType::SPECULAR:
			name = "specularK";
			break;
		case UniformMaterialType::EMISSIVE:
			name = "emissiveK";
			break;
		case UniformMaterialType::OPACITY:
			name = "opacityK";
			break;
		/*case UniformMaterialType::REFLECTIVE:
			name = "reflectiveK";
			break;*/
		}

		registerUniformMaterial(index, name, uniformMaterialType);
	}

	void RenderEngine::registerMaterialsFromXML(std::string filename) 
	{
		pugi::xml_document doc;

		if (!doc.load_file(filename.c_str()))
		{
			std::cout << "Error reading XML file '" << filename.c_str() << "'!" << std::endl;
			exit(-1);
		}

		pugi::xml_node materials = doc.child("Materials");

		for (pugi::xml_node_iterator it = materials.begin(); it != materials.end(); ++it)
		{
			//std::cout << "Material:";

			unsigned int location;
			std::string name;
			MaterialSourceType sourceType;
			TextureMaterialType textureType;
			UniformMaterialType uniformType;

			for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
			{
				name = "";

				//std::cout << " " << ait->name() << "=" << ait->value();
				std::string n(ait->name());
				std::string v(ait->value());

				if ((n == "location") || (n == "binding"))
				{
					location = std::stoi(v);
				}
				else if (n == "name")
				{
					name = v;
				}
				else if (n == "source") 
				{
					if (v == "TEXTURE")
					{
						sourceType = MaterialSourceType::TEXTURE;
					}
					else if (v == "UNIFORM")
					{
						sourceType = MaterialSourceType::UNIFORM;
					}
					else if (v == "ATTRIBUTE")
					{
						sourceType = MaterialSourceType::ATTRIBUTE;
					}
				}
				else if (n == "type")
				{
					if (sourceType == MaterialSourceType::TEXTURE) 
					{
						if (v == "AMBIENT")
						{
							textureType = TextureMaterialType::AMBIENT;
						}
						else if (v == "DIFFUSE") 
						{
							textureType = TextureMaterialType::DIFFUSE;
						}
						else if (v == "SPECULAR") 
						{
							textureType = TextureMaterialType::SPECULAR;
						}
						else if (v == "EMISSIVE")
						{
							textureType = TextureMaterialType::EMISSIVE;
						}
						else if (v == "NORMAL")
						{
							textureType = TextureMaterialType::NORMAL;
						}
						else if (v == "LIGHTMAP") 
						{
							textureType = TextureMaterialType::LIGHTMAP;
						}
						else if (v == "HEIGHT") 
						{
							textureType = TextureMaterialType::HEIGHT;
						}
						else if (v == "SHININESS") 
						{
							textureType = TextureMaterialType::SHININESS;
						}
						else if (v == "OPACITY") 
						{
							textureType = TextureMaterialType::OPACITY;
						}
						/*else if (v == "REFLECTIVE")
						{
							textureType = TextureMaterialType::REFLECTIVE;
						}*/
						else if (v == "DISPLACEMENT") 
						{
							textureType = TextureMaterialType::DISPLACEMENT;
						}
						else if (v == "AUXILIAR")
						{
							textureType = TextureMaterialType::AUXILIAR;
						}
					}
					else if (sourceType == MaterialSourceType::UNIFORM)
					{
						if (v == "AMBIENT")
						{
							uniformType = UniformMaterialType::AMBIENT;
						}
						else if (v == "DIFFUSE")
						{
							uniformType = UniformMaterialType::DIFFUSE;
						}
						else if (v == "SPECULAR")
						{
							uniformType = UniformMaterialType::SPECULAR;
						}
						else if (v == "EMISSIVE") 
						{
							uniformType = UniformMaterialType::EMISSIVE;
						}
						else if (v == "OPACITY") 
						{
							uniformType = UniformMaterialType::OPACITY;
						}
						/*else if (v == "REFLECTIVE")
						{
							uniformType = UniformMaterialType::REFLECTIVE;
						}*/
					}
				}
			}

			if (sourceType == MaterialSourceType::TEXTURE)
			{
				if (name == "")
				{
					registerGenericTextureMaterial(location, textureType);
				}
				else
				{
					registerCustomTextureMaterial(location, name, textureType);
				}
			}
			else if (sourceType == MaterialSourceType::UNIFORM)
			{
				if (name == "")
				{
					registerGenericUniformMaterial(location, uniformType);
				}
				else
				{
					registerCustomUniformMaterial(location, name, uniformType);
				}
			}
		}
	}
	std::map<std::string, Material*> RenderEngine::getMaterials()
	{
		return _materials;
	}

	int RenderEngine::getTextureMaterialBinding(TextureMaterialType textureMaterialType) 
	{
		int binding = -1;
    for (const auto& iterator : _materials)
		{
			if (iterator.second->getTextureType() == textureMaterialType) 
			{
        binding = iterator.second->getLocation();
				break;
			}
		}
		return binding;
	}
	int RenderEngine::getUniformMaterialLocation(UniformMaterialType uniformMaterialType) 
	{
		int location = -1;
    for (const auto& iterator : _materials)
		{
			if (iterator.second->getUniformType() == uniformMaterialType) 
			{
				location = iterator.second->getLocation();
				break;
			}
		}
		return location;
	}

	void RenderEngine::freeResources()
	{
		//Eliminamos las geometrías
		for (auto geometryProperties : _geometries) 
		{
			delete geometryProperties;
		}

		//Eliminamos las propiedades de material
		for (auto materialProperty : _materialProperties)
		{
			delete materialProperty;
		}

		//Eliminamos los shaders
		for (auto shader : _shaders) 
		{
			delete shader.first;
			delete shader.second;
		}

		//Eliminamos los custom shaders
		for (auto customShader : _customShaders)
		{
			delete customShader;
		}

		//Eliminamos las texturas
		for (auto texture : _textures) 
		{
			delete texture;
		}

		//Eliminamos los materiales
		for (auto material : _materials)
		{
			//material.first es un string
			delete material.second;
		}

		//Eliminamos los atributos
		for (auto attribute : _attributes) 
		{
			//attribute.first es un string
			delete attribute.second;
		}

		//Eliminamos la escena
		delete _scene;
	}

	void RenderEngine::activeAnisotropicFilterIfSupported() 
	{
		GLfloat fLargest = 0.0f;
		if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
		{
			std::cout << "Anisotropic filter supported." << std::endl;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
		}
		else {
			std::cout << "Anisotropic filter NOT supported." << std::endl;
		}
	}

	void RenderEngine::initOpenGLsettings()
	{
		//glEnable(GL_DEPTH_TEST);  //Z Buffer --> Hay que activarlo explicitamente

		//Orientación de la cara front, configuración de la etapa de rasterizado y activación del culling
		glFrontFace(GL_CCW); //Cara front ordenada en sentido antihorario
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //FILL generar los fragmentos
		glEnable(GL_CULL_FACE); //Descartar triángulos que no miren a la cámara
		glEnable(GL_MULTISAMPLE);
	}

	glm::vec4 RenderEngine::getBackgroundColor()
	{
		return _scene->getBackgroundColor();
	}

	void RenderEngine::setBackgroundColor(glm::vec4 backgroundColor) 
	{
		_scene->setBackgroundColor(backgroundColor);
	}

	void RenderEngine::printOpenGLVersionInfo() 
	{
		const GLubyte *oglVendor = glGetString(GL_VENDOR);
		const GLubyte *oglRenderer = glGetString(GL_RENDERER);
		const GLubyte *oglVersion = glGetString(GL_VERSION);
		std::cout << "Vendor: " << oglVendor << std::endl;
		std::cout << "Renderer: " << oglRenderer << std::endl;
		std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;
	}

	void RenderEngine::printNumExtensions() 
	{
		GLint res;
		glGetIntegerv(GL_NUM_EXTENSIONS, &res);
		std::cout << "Number of extensions: " << res << std::endl;
	}

	void RenderEngine::printFBOLimits() 
	{
		int res;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &res);
		std::cout << "Max Color Attachments: " << res << std::endl;

		glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &res);
		std::cout << "Max Framebuffer Width: " << res << std::endl;

		glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &res);
		std::cout << "Max Framebuffer Height: " << res << std::endl;

		glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &res);
		std::cout << "Max Framebuffer Samples: " << res << std::endl;

		glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &res);
		std::cout << "Max Framebuffer Layers: " << res << std::endl;
	}

	void RenderEngine::printUBOLimits()
	{
		GLint res;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &res);
		std::cout << "Max uniform buffer bindings: " << res << std::endl;

		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &res);
		std::cout << "Max uniform block size: " << res << std::endl;

		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &res);
		std::cout << "Max vertex uniform blocks: " << res << std::endl;

		glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS, &res);
		std::cout << "Max tessellation control uniform blocks: " << res << std::endl;

		glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS, &res);
		std::cout << "Max tessellation evaluation uniform blocks: " << res << std::endl;

		glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &res);
		std::cout << "Max geometryProperties uniform blocks: " << res << std::endl;

		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &res);
		std::cout << "Max fragment uniform blocks: " << res << std::endl;

		glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &res);
		std::cout << "Max compute uniform blocks: " << res << std::endl;
	}	

	RenderEngine::RenderEngine() :
		_useCustomShaders(false)
	{

		//Inicialización de glew
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
			exit(-1);
		}
		
		//Info de OpenGL
		printOpenGLVersionInfo();

		//Número de extensiones
		printNumExtensions();

		//Límites del UBO en la GPU
		printUBOLimits();

		//Límites del FBO en la GPU
		printFBOLimits();

		//Inicializamos las mallas que pueden utilizar las luces
		//InitLightsMeshes();

		//Inicialización de una escena vacía. Es importante hacerlo
		//por si se intenta inicializar la cámara o las matrices de
		//proyección y vista sobre la escena apuntando a nullptr.
		_scene = new Scene();
	}

	std::map<MeshProperties*, Shader*> RenderEngine::getMeshProperties()
	{
		return _shaders;
	}

}