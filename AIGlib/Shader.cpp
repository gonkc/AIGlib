#include "Shader.h"
#include "RenderEngine.h"

namespace AIGlib 
{	
	Shader::Shader(MeshProperties* meshProperties) :
		_program(invalidValue),
		_shaderString(std::string())
	{
		_uMatrixUniforms = { -1, -1, -1, -1, -1, -1 };
		_uMaterialScalars = { -1, -1, -1, -1, -1 };

		if (meshProperties != nullptr)
		{
			if (!RenderEngine::get().getIfCustomShaders())
			{
				//Cabecera del shader genérico
				_shaderString += createShaderHeader();

				//Defines del shader genérico
				_shaderString += createShaderDefines(meshProperties);
			}
		}
	}

	Shader::~Shader()
	{
		freeResources();
	}

	void Shader::addVertexShader(const char *vshaderPath)
	{
		_vertexShaders.push_back(loadShader(vshaderPath, GL_VERTEX_SHADER));
	}

	void Shader::addTessellationControlShader(const char *tcshaderPath) 
	{
		_tessellationControlShaders.push_back(loadShader(tcshaderPath, GL_TESS_CONTROL_SHADER));
	}

	void Shader::addTessellationEvalShader(const char *teshaderPath) 
	{
		_tessellationEvaluationShaders.push_back(loadShader(teshaderPath, GL_TESS_EVALUATION_SHADER));
	}

	void Shader::addGeometryShader(const char *gshaderPath) 
	{
		_geometryShaders.push_back(loadShader(gshaderPath, GL_GEOMETRY_SHADER));
	}

	void Shader::addFragmentShader(const char *fshaderPath)
	{
		_fragmentShaders.push_back(loadShader(fshaderPath, GL_FRAGMENT_SHADER));
	}

	void Shader::addComputeShader(const char *cshaderPath) 
	{
		_computeShaders.push_back(loadShader(cshaderPath, GL_COMPUTE_SHADER));
	}

	void Shader::addGenericShaders() 
	{
		if (_vertexShaders.size() == 0)
		{
			_vertexShaders.push_back(loadShader("../shaders/shader.generic.vert", GL_VERTEX_SHADER));
		}
		if (_tessellationControlShaders.size() == 0) 
		{
			//_tessellationControlShaders.push_back(loadShader("../shaders/shader.generic.tesscont", GL_TESS_CONTROL_SHADER));
		}
		if (_tessellationEvaluationShaders.size() == 0) 
		{
			//_tessellationEvaluationShaders.push_back(loadShader("../shaders/shader.generic.tesseval", GL_TESS_EVALUATION_SHADER));
		}
		if (_geometryShaders.size() == 0) 
		{
			//_geometryShaders.push_back(loadShader("../shaders/shader.generic.geom", GL_GEOMETRY_SHADER));
		}
		if (_fragmentShaders.size() == 0) 
		{
			_fragmentShaders.push_back(loadShader("../shaders/shader.generic.frag", GL_FRAGMENT_SHADER));
		}
		if (_computeShaders.size() == 0) 
		{
			//_computeShaders.push_back(loadShader("../shaders/shader.generic.comp", GL_COMPUTE_SHADER));
		}
	}

	void Shader::compileAndLinkShaders()
	{
		//Creamos el programa
		_program = glCreateProgram();

		if (!RenderEngine::get().getIfCustomShaders()) 
		{
			addGenericShaders();
		}

		//Enlazamos los shaders en el programa
		for (unsigned int i = 0; i < _vertexShaders.size(); ++i) 
		{
			glAttachShader(_program, _vertexShaders[i]);
		}
		for (unsigned int i = 0; i < _tessellationControlShaders.size(); ++i) 
		{
			glAttachShader(_program, _tessellationControlShaders[i]);
		}
		for (unsigned int i = 0; i < _tessellationEvaluationShaders.size(); ++i) 
		{
			glAttachShader(_program, _tessellationEvaluationShaders[i]);
		}
		for (unsigned int i = 0; i < _geometryShaders.size(); ++i)
		{
			glAttachShader(_program, _geometryShaders[i]);
		}
		for (unsigned int i = 0; i < _fragmentShaders.size(); ++i)
		{
			glAttachShader(_program, _fragmentShaders[i]);
		}
		for (unsigned int i = 0; i < _computeShaders.size(); ++i) 
		{
			glAttachShader(_program, _computeShaders[i]);
		}

		//Y los enlazamos
		glLinkProgram(_program);

		//Comprobamos si hay errores en la fase de enlazado
		int linked;
		glGetProgramiv(_program, GL_LINK_STATUS, &linked);
		if (!linked)
		{ 
			//Calculamos una cadena de error 
			int logLen;
			glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLen);

			char *logString = new char[logLen];
			glGetProgramInfoLog(_program, logLen, NULL, logString);
			std::cerr << "Link Error: " << logString << std::endl;
			delete logString;

			glDeleteProgram(_program);
			_program = 0;

			exit(-1);
		}

		//Atributos
		loadAttributes();

		//Uniformes
		loadUniforms();

		//Creamos los identificadores de las variables uniformes de las matrices
		_uMatrixUniforms[0] = glGetUniformLocation(_program, "proj");
		_uMatrixUniforms[1] = glGetUniformLocation(_program, "view");
		_uMatrixUniforms[2] = glGetUniformLocation(_program, "model");
		_uMatrixUniforms[3] = glGetUniformLocation(_program, "modelView");
		_uMatrixUniforms[4] = glGetUniformLocation(_program, "modelViewProj");
		_uMatrixUniforms[5] = glGetUniformLocation(_program, "normal");

		//Uniformes de las propiedades escalares
		_uMaterialScalars[0] = glGetUniformLocation(_program, "shininess");
		_uMaterialScalars[1] = glGetUniformLocation(_program, "shininessStrength");
		_uMaterialScalars[2] = glGetUniformLocation(_program, "reflectivity");
		_uMaterialScalars[3] = glGetUniformLocation(_program, "refractIndex");
		_uMaterialScalars[4] = glGetUniformLocation(_program, "opacity");

		//Creamos los identificadores de las variables uniformes de los efectos
		_uAmbientOcclusion = glGetUniformLocation(_program, "ambientOcclusion");
		_uBumpMapping = glGetUniformLocation(_program, "bumpMapping");
		_uParallaxMapping[0] = glGetUniformLocation(_program, "parallaxMapping");
		_uParallaxMapping[1] = glGetUniformLocation(_program, "scale");
		_uParallaxMapping[2] = glGetUniformLocation(_program, "bias");

		_uCameraPosition = glGetUniformLocation(_program, "cameraPosition");
		_uBackgroundColor = glGetUniformLocation(_program, "backgroundColor");

		_uIlluminationModel = glGetUniformLocation(_program, "illuminationModel");
		_uShadingModel = glGetUniformLocation(_program, "shadingModel");

    _uLightsUniforms[0] = glGetUniformLocation(_program, "ambientIntensity");

		_uFogUniforms[0] = glGetUniformLocation(_program, "fogType");
		_uFogUniforms[1] = glGetUniformLocation(_program, "fogColor");
		_uFogUniforms[2] = glGetUniformLocation(_program, "fogDensity");
		_uFogUniforms[3] = glGetUniformLocation(_program, "fogStarts");
		_uFogUniforms[4] = glGetUniformLocation(_program, "fogEnds");

		deleteShaders();
	}

	void Shader::loadAttributes() 
	{
		std::vector<GLenum> properties = { GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };
		GLint size = static_cast<int>(properties.size());

		GLint numActiveAttribs = 0;
		glGetProgramInterfaceiv(_program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);

		std::map<std::string, Attribute*> attributes = RenderEngine::get().getAttributes();
		std::map<std::string, Attribute*>::iterator attributesIterator;

		for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
		{
			GLint values[4];
			glGetProgramResourceiv(_program, GL_PROGRAM_INPUT, attrib, size, &properties[0], size, NULL, values);
			std::vector<char> nameData(values[2]);
			glGetProgramResourceName(_program, GL_PROGRAM_INPUT, attrib, static_cast<GLsizei>(nameData.size()), NULL, &nameData[0]);
			std::string name = &nameData[0];

			attributesIterator = attributes.find(name.c_str());
			if (attributesIterator != attributes.end())
			{
				_attributeNames.insert(name);
			}
		}
	}

	void Shader::loadUniforms() 
	{
		std::vector<GLenum> properties = { GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };
		GLint size = static_cast<int>(properties.size());

		GLint numActiveUniforms = 0;
		glGetProgramInterfaceiv(_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

		std::map<std::string, Material*> materials = RenderEngine::get().getMaterials();
		std::map<std::string, Material*>::iterator materialsIterator;

		for (int uniform = 0; uniform < numActiveUniforms; ++uniform)
		{
			GLint values[4];
			glGetProgramResourceiv(_program, GL_UNIFORM, uniform, size, &properties[0], size, NULL, values);

			if (values[0] != -1)
			{
				continue; //Nos saltamos los uniformes que estén en un bloque
			}

			std::vector<char> nameData(values[2]);
			glGetProgramResourceName(_program, GL_UNIFORM, uniform, static_cast<GLsizei>(nameData.size()), NULL, &nameData[0]);
			std::string name = &nameData[0];

			materialsIterator = materials.find(name.c_str());
			if (materialsIterator != materials.end())
			{
				unsigned int id = glGetProgramResourceIndex(_program, GL_UNIFORM, name.c_str()); //Cojo el identificador del material

				//Separamos las texturas de los uniformes
				switch (materialsIterator->second->getSourceType()) 
				{
				case MaterialSourceType::TEXTURE:
					if (!_materials.insert(std::make_pair(name, new Material(materialsIterator->second->getLocation(), id, materialsIterator->second->getTextureType()))).second) 
					{
						std::cout << "Error - Duplicate material in shader!." << std::endl;
						exit(-1);
					}
					break;
				case MaterialSourceType::UNIFORM:
					if (!_materials.insert(std::make_pair(name, new Material(materialsIterator->second->getLocation(), id, materialsIterator->second->getUniformType()))).second)
					{
						std::cout << "Error - Duplicate material in shader!." << std::endl;
						exit(-1);
					}
					break;
				}
			}
		}
	}

	void Shader::loadAmbientIntensity() 
	{		
		_uLightsUniforms[0] = glGetUniformLocation(_program, "ambientIntensity");
		glUniform3fv(_uLightsUniforms[0], 1, glm::value_ptr(
			RenderEngine::get().getScene().getAmbientIntensity()->getAmbientIntensity())
		);
	}

	void Shader::loadPointLights(std::vector<PointLight*> pls) 
	{
		//Índices del array de luces puntuales
		int pointLightIndex = 0;

		//Nº de campos que tiene cada tipo de luz puntual (por su estructura)
		const int pointLightFields = 7;

		//Índice del bloque
		GLuint blockIndex = glGetUniformBlockIndex(_program, "PointLightBlock");

		//Le asignamos un binding point al bloque (para luego utilizarlo en BufferBase/BufferRange)
		int bindingPoint;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &bindingPoint);
		bindingPoint -= 1; //Se coge la primera posición empezando por detrás
		glUniformBlockBinding(_program, blockIndex, bindingPoint);

		//Tamaño del bloque y reserva del buffer (para todo el array de luces!!!)
		GLint blockSize;
		glGetActiveUniformBlockiv(_program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		GLubyte* blockBuffer = new GLubyte[blockSize];

		for (unsigned int light = 0; light < pls.size(); ++light)
		{
			//Consulta a las variables de cada bloque, nos interesa hacerlo manualmente en lugar de con un for
			//porque el bloque no asegura ningún orden en las variables y luego no se sabría donde subir cada valor

			//Estructuras de las luces puntuales
			std::string pointLights = "pointLights[" + std::to_string(pointLightIndex) + "].";
			std::string lightPosition = pointLights + "lightPosition";
			std::string constantAttenuation = pointLights + "constantAttenuation";
			std::string linearAttenuation = pointLights + "linearAttenuation";
			std::string quadraticAttenuation = pointLights + "quadraticAttenuation";
			std::string ambientIntensity = pointLights + "ambientIntensity";
			std::string diffuseIntensity = pointLights + "diffuseIntensity";
			std::string specularIntensity = pointLights + "specularIntensity";

			//Nombres
			const GLchar *names[] = { lightPosition.c_str(), constantAttenuation.c_str(), linearAttenuation.c_str(),
				quadraticAttenuation.c_str(), ambientIntensity.c_str(), diffuseIntensity.c_str(), specularIntensity.c_str() };

			//Índices
			GLuint indices[pointLightFields];
			glGetUniformIndices(_program, pointLightFields, names, indices);

			//Desplazamientos
			GLint offset[pointLightFields];
			glGetActiveUniformsiv(_program, pointLightFields, indices, GL_UNIFORM_OFFSET, offset);

			//Ponemos los datos en el buffer con sus tamaños adecuados
			//PointLight *pointLight = RenderEngine::get().getScene().getPointLights().at(pointLightIndex);
			PointLight *pointLight = pls.at(pointLightIndex);

			float ca = pointLight->getConstantAttenuation();
			float la = pointLight->getLinearAttenuation();
			float qa = pointLight->getQuadraticAttenuation();

			std::memcpy(blockBuffer + offset[0], &pointLight->getLightPosition(), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[1], &ca, sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[2], &la, sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[3], &qa, sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[4], &pointLight->getAmbientIntensity(), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[5], &pointLight->getDiffuseIntensity(), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[6], &pointLight->getSpecularIntensity(), 4 * sizeof(GLfloat));

			pointLightIndex += 1;
		}

		//Creamos el objeto de OpenGL y subimos los datos del buffer
		glGenBuffers(1, &_uboPointLights);
		glBindBuffer(GL_UNIFORM_BUFFER, _uboPointLights);
		glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//Enlazamos el objeto al buffer de las uniformes
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _uboPointLights);

		//Creamos el identificador de la variable uniform del número de luces
		_uLightsUniforms[1] = glGetUniformLocation(_program, "nPointLights");

		//Y subimos el nº de luces puntuales
		glUniform1i(_uLightsUniforms[1], static_cast<unsigned int>(pls.size()));

		delete[] blockBuffer;
	}

	void Shader::loadDirectionalLights(std::vector<DirectionalLight*> dls) 
	{
		//Índices del array de luces direccionales
		int directionalLightIndex = 0;

		//Nº de campos que tiene cada tipo de luz direccional (por su estructura)
		const int directionalLightFields = 4;

		//Índice del bloque
		GLuint blockIndex = glGetUniformBlockIndex(_program, "DirectionalLightBlock");

		//Le asignamos un binding point al bloque (para luego utilizarlo en BufferBase/BufferRange)
		int bindingPoint;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &bindingPoint);
		bindingPoint -= 2; //Se coge la segunda posición empezando por detrás
		glUniformBlockBinding(_program, blockIndex, bindingPoint);

		//Tamaño del bloque y reserva del buffer (para todo el array de luces!!!)
		GLint blockSize;
		glGetActiveUniformBlockiv(_program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		GLubyte* blockBuffer = new GLubyte[blockSize];

		for (unsigned int light = 0; light < dls.size(); ++light)
		{
			//Consulta a las variables de cada bloque, nos interesa hacerlo manualmente en lugar de con un for
			//porque el bloque no asegura ningún orden en las variables y luego no se sabría donde subir cada valor

			//Estructuras de las luces direccionales
			std::string directionalLights = "directionalLights[" + std::to_string(directionalLightIndex) + "].";
			std::string lightDirection = directionalLights + "lightDirection";
			std::string ambientIntensity = directionalLights + "ambientIntensity";
			std::string diffuseIntensity = directionalLights + "diffuseIntensity";
			std::string specularIntensity = directionalLights + "specularIntensity";

			//Nombres
			const GLchar *names[] = { lightDirection.c_str(), ambientIntensity.c_str(), diffuseIntensity.c_str(), specularIntensity.c_str() };

			//Índices
			GLuint indices[directionalLightFields];
			glGetUniformIndices(_program, directionalLightFields, names, indices);

			//Desplazamientos
			GLint offset[directionalLightFields];
			glGetActiveUniformsiv(_program, directionalLightFields, indices, GL_UNIFORM_OFFSET, offset);

			//Ponemos los datos en el buffer con sus tamaños adecuados
			//DirectionalLight *directionalLight = RenderEngine::get().getScene().getDirectionalLights().at(directionalLightIndex);
			DirectionalLight *directionalLight = dls.at(directionalLightIndex);

			std::memcpy(blockBuffer + offset[0], glm::value_ptr(directionalLight->getLightDirection()), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[1], glm::value_ptr(directionalLight->getAmbientIntensity()), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[2], glm::value_ptr(directionalLight->getDiffuseIntensity()), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[3], glm::value_ptr(directionalLight->getSpecularIntensity()), 4 * sizeof(GLfloat));

			directionalLightIndex += 1;
		}

		//Creamos el objeto de OpenGL y subimos los datos del buffer
		glGenBuffers(1, &_uboDirectionalLights);
		glBindBuffer(GL_UNIFORM_BUFFER, _uboDirectionalLights);
		glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//Enlazamos el objeto al buffer de las uniformes
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _uboDirectionalLights);

		//Creamos el identificador de la variable uniform del número de luces
		_uLightsUniforms[2] = glGetUniformLocation(_program, "nDirectionalLights");

		//Y subimos el nº de luces direccionales
		glUniform1i(_uLightsUniforms[2], static_cast<unsigned int>(dls.size()));

		delete[] blockBuffer;
	}

	void Shader::loadSpotLights(std::vector<SpotLight*> sls)
	{
		//Índices del array de luces focales
		int spotLightIndex = 0;

		//Nº de campos que tiene cada tipo de luz focal (por su estructura)
		const int spotLightFields = 9;

		//Índice del bloque
		GLuint blockIndex = glGetUniformBlockIndex(_program, "SpotLightBlock");

		//Le asignamos un binding point al bloque (para luego utilizarlo en BufferBase/BufferRange)
		int bindingPoint;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &bindingPoint);
		bindingPoint -= 3; //Se coge la tercera posición empezando por detrás
		glUniformBlockBinding(_program, blockIndex, bindingPoint);

		//Tamaño del bloque y reserva del buffer (para todo el array de luces!!!)
		GLint blockSize;
		glGetActiveUniformBlockiv(_program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		GLubyte* blockBuffer = new GLubyte[blockSize];

		for (unsigned int light = 0; light < sls.size(); ++light)
		{
			//Consulta a las variables de cada bloque, nos interesa hacerlo manualmente en lugar de con un for
			//porque el bloque no asegura ningún orden en las variables y luego no se sabría donde subir cada valor

			//Estructuras de las luces focales
			std::string spotLights = "spotLights[" + std::to_string(spotLightIndex) + "].";
			std::string lightPosition = spotLights + "lightPosition";
			std::string coneDirection = spotLights + "coneDirection";
			std::string coneAngle = spotLights + "coneAngle";
			std::string constantAttenuation = spotLights + "constantAttenuation";
			std::string linearAttenuation = spotLights + "linearAttenuation";
			std::string quadraticAttenuation = spotLights + "quadraticAttenuation";
			std::string ambientIntensity = spotLights + "ambientIntensity";
			std::string diffuseIntensity = spotLights + "diffuseIntensity";
			std::string specularIntensity = spotLights + "specularIntensity";

			//Nombres
			const GLchar *names[] = { lightPosition.c_str(), coneDirection.c_str(), coneAngle.c_str(), constantAttenuation.c_str(), 
				linearAttenuation.c_str(), quadraticAttenuation.c_str(), ambientIntensity.c_str(), diffuseIntensity.c_str(), specularIntensity.c_str() };

			//Índices
			GLuint indices[spotLightFields];
			glGetUniformIndices(_program, spotLightFields, names, indices);

			//Desplazamientos
			GLint offset[spotLightFields];
			glGetActiveUniformsiv(_program, spotLightFields, indices, GL_UNIFORM_OFFSET, offset);

			//Ponemos los datos en el buffer con sus tamaños adecuados
			//SpotLight *spotLight = RenderEngine::get().getScene().getSpotLights().at(spotLightIndex);
			SpotLight *spotLight = sls.at(spotLightIndex);

			GLfloat angle = spotLight->getConeAngle();
			GLfloat ca = spotLight->getConstantAttenuation();
			GLfloat la = spotLight->getLinearAttenuation();
			GLfloat qa = spotLight->getQuadraticAttenuation();

			std::memcpy(blockBuffer + offset[0], &spotLight->getLightPosition(), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[1], &spotLight->getConeDirection(), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[2], &angle, sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[3], &ca, sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[4], &la, sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[5], &qa, sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[6], &spotLight->getAmbientIntensity(), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[7], &spotLight->getDiffuseIntensity(), 4 * sizeof(GLfloat));
			std::memcpy(blockBuffer + offset[8], &spotLight->getSpecularIntensity(), 4 * sizeof(GLfloat));

			spotLightIndex += 1;
		}

		//Creamos el objeto de OpenGL y subimos los datos del buffer
		//GLuint _uboPointLights;
		glGenBuffers(1, &_uboSpotLights);
		glBindBuffer(GL_UNIFORM_BUFFER, _uboSpotLights);
		glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//Enlazamos el objeto al buffer de las uniformes
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _uboSpotLights);	

		//Creamos el identificador de la variable uniform del número de luces
		_uLightsUniforms[3] = glGetUniformLocation(_program, "nSpotLights");
		
		//Y subimos el nº de luces focales
		glUniform1i(_uLightsUniforms[3], static_cast<unsigned int>(sls.size()));

		delete[] blockBuffer;
	}

	void Shader::use() 
	{
		glUseProgram(_program);
	}
	
	std::set<std::string> &Shader::getAttributeNames() 
	{
		return _attributeNames;
	}

	std::map<std::string, Material*> &Shader::getMaterials() 
	{
		return _materials;
	}

	int Shader::getIlluminationModelUniform() 
	{
		return _uIlluminationModel;
	}

	int Shader::getShadingModelUniform() 
	{
		return _uShadingModel;
	}

	std::array<int, N_MATRIX_UNIFORMS> Shader::getMatrixUniforms()
	{
		return _uMatrixUniforms;
	}

	std::array<int, N_SCALAR_UNIFORMS> Shader::getMaterialScalarUniforms()
	{
		return _uMaterialScalars;
	}

	int Shader::getAmbientOcclusionUniform()
	{
		return _uAmbientOcclusion;
	}

	int Shader::getBumpMappingUniform() 
	{
		return _uBumpMapping;
	}

	std::array<int, N_PARALLAX_UNIFORMS> Shader::getParallaxMappingUniforms()
	{
		return _uParallaxMapping;
	}

	std::array<int, N_LIGHTS_UNIFORMS> Shader::getLightsUniforms()
	{
		return _uLightsUniforms;
	}
	std::array<int, N_FOG_UNIFORMS> Shader::getFogUniforms()
	{
		return _uFogUniforms;
	}

	int Shader::getCameraPositionUniform()
	{
		return _uCameraPosition;
	}

	int Shader::getBackgroundColorUniform()
	{
		return _uBackgroundColor;
	}

	unsigned int Shader::getUboPointLights()
	{
		return _uboPointLights;
	}

	unsigned int Shader::getUboDirectionalLights() 
	{
		return _uboDirectionalLights;
	}

	unsigned int Shader::getUboSpotLights() 
	{
		return _uboSpotLights;
	}

	unsigned int Shader::getProgram() 
	{
		return _program;
	}

	//Private
	void Shader::deleteShaders()
	{
		//Eliminamos los shaders
		if (!_vertexShaders.empty())
		{
			for (unsigned int i = 0; i < _vertexShaders.size(); ++i)
			{
				glDeleteShader(_vertexShaders[i]);
				glDetachShader(_program, _vertexShaders[i]);
			}
		}
		if (!_tessellationControlShaders.empty())
		{
			for (unsigned int i = 0; i < _tessellationControlShaders.size(); ++i)
			{
				glDeleteShader(_tessellationControlShaders[i]);
				glDetachShader(_program, _tessellationControlShaders[i]);
			}
		}
		if (!_tessellationEvaluationShaders.empty()) 
		{
			for (unsigned int i = 0; i < _tessellationEvaluationShaders.size(); ++i) 
			{
				glDeleteShader(_tessellationEvaluationShaders[i]);
				glDetachShader(_program, _tessellationEvaluationShaders[i]);
			}
		}
		if (!_geometryShaders.empty()) 
		{
			for (unsigned int i = 0; i < _geometryShaders.size(); ++i) 
			{
				glDeleteShader(_geometryShaders[i]);
				glDetachShader(_program, _geometryShaders[i]);
			}
		}
		if (!_fragmentShaders.empty())
		{
			for (unsigned int i = 0; i < _fragmentShaders.size(); ++i) 
			{
				glDeleteShader(_fragmentShaders[i]);
				glDetachShader(_program, _fragmentShaders[i]);
			}
		}
		if (!_computeShaders.empty()) 
		{
			for (unsigned int i = 0; i < _computeShaders.size(); ++i) 
			{
				glDeleteShader(_computeShaders[i]);
				glDetachShader(_program, _computeShaders[i]);
			}
		}

		//Liberamos los vectores de los shaders
		_vertexShaders.clear();
		std::vector<unsigned int>().swap(_vertexShaders);
		_tessellationControlShaders.clear();
		std::vector<unsigned int>().swap(_tessellationControlShaders);
		_tessellationEvaluationShaders.clear();
		std::vector<unsigned int>().swap(_tessellationEvaluationShaders);
		_geometryShaders.clear();
		std::vector<unsigned int>().swap(_geometryShaders);
		_fragmentShaders.clear();
		std::vector<unsigned int>().swap(_fragmentShaders);
		_computeShaders.clear();
		std::vector<unsigned int>().swap(_computeShaders);
	}

	void Shader::freeResources() 
	{
		glDeleteProgram(_program);

		for (auto material : _materials)
		{
			//material.first es un string
			delete material.second;
		}
	}

	std::string Shader::createShaderHeader()
	{
		std::string shaderHeader;
		shaderHeader += "#version 330 core" +
			shaderHeader += "\n#extension GL_ARB_shading_language_420pack : enable";
		shaderHeader += "\n#extension GL_ARB_uniform_buffer_object : require";
		shaderHeader += "\n#extension GL_ARB_explicit_uniform_location : enable";
		shaderHeader += "\n";
		return shaderHeader;
	}

	std::string Shader::createShaderDefines(MeshProperties* meshProperties)
	{
		std::string shaderDefines;

		shaderDefines += "\n#define N_MAX_POINTLIGHTS " + std::to_string(N_MAX_POINTLIGHTS);
		shaderDefines += "\n#define N_MAX_DIRECTIONALLIGHTS " + std::to_string(N_MAX_DIRECTIONALLIGHTS);
		shaderDefines += "\n#define N_MAX_SPOTLIGHTS " + std::to_string(N_MAX_SPOTLIGHTS);

		if (meshProperties->getMaterialProperties()->getAmbientSource() > 0) 
		{
			shaderDefines += "\n#define AMBIENT_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getAmbientSource());
		}
		if (meshProperties->getMaterialProperties()->getDiffuseSource() > 0) 
		{
			shaderDefines += "\n#define DIFFUSE_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getDiffuseSource());
		}
		if (meshProperties->getMaterialProperties()->getSpecularSource() > 0)
		{
			shaderDefines += "\n#define SPECULAR_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getSpecularSource());
		}
		if (meshProperties->getMaterialProperties()->getEmissiveSource() > 0) 
		{
			shaderDefines += "\n#define EMISSIVE_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getEmissiveSource());
		}
		if (meshProperties->getMaterialProperties()->getOpacitySource() > 0)
		{
			shaderDefines += "\n#define OPACITY_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getOpacitySource());
		}
		if (meshProperties->getMaterialProperties()->getShininessSource() > 0)
		{
			shaderDefines += "\n#define SHININESS_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getShininessSource());
		}
		if (meshProperties->getMaterialProperties()->getNormalSource() > 0) 
		{
			shaderDefines += "\n#define NORMAL_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getNormalSource());
		}
		if (meshProperties->getMaterialProperties()->getLightmapSource() > 0)
		{
			shaderDefines += "\n#define LIGHTMAP_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getLightmapSource());
		}
		if (meshProperties->getMaterialProperties()->getHeightSource() > 0) 
		{
			shaderDefines += "\n#define HEIGHT_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getHeightSource());
		}
		if (meshProperties->getMaterialProperties()->getDisplacementSource() > 0)
		{
			shaderDefines += "\n#define DISPLACEMENT_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getDisplacementSource());
		}
		if (meshProperties->getMaterialProperties()->getAuxiliarSource() > 0)
		{
			shaderDefines += "\n#define AUXILIAR_SOURCE " + std::to_string(meshProperties->getMaterialProperties()->getAuxiliarSource());
		}
		if (meshProperties->getMaterialProperties()->getAmbientTexBinding() > -1)
		{
			shaderDefines += "\n#define AMBIENT_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getAmbientTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getDiffuseTexBinding() > -1) 
		{
			shaderDefines += "\n#define DIFFUSE_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getDiffuseTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getSpecularTexBinding() > -1)
		{
			shaderDefines += "\n#define SPECULAR_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getSpecularTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getEmissiveTexBinding() > -1) 
		{
			shaderDefines += "\n#define EMISSIVE_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getEmissiveTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getNormalTexBinding() > -1)
		{
			shaderDefines += "\n#define NORMAL_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getNormalTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getLightmapTexBinding() > -1) 
		{
			shaderDefines += "\n#define LIGHTMAP_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getLightmapTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getHeightTexBinding() > -1) 
		{
			shaderDefines += "\n#define HEIGHT_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getHeightTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getShininessTexBinding() > -1)
		{
			shaderDefines += "\n#define SHININESS_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getShininessTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getOpacityTexBinding() > -1)
		{
			shaderDefines += "\n#define OPACITY_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getOpacityTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getDisplacementTexBinding() > -1) 
		{
			shaderDefines += "\n#define DISPLACEMENT_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getDisplacementTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getAuxiliarTexBinding() > -1)
		{
			shaderDefines += "\n#define AUXILIAR_TEX_BINDING " + std::to_string(meshProperties->getMaterialProperties()->getAuxiliarTexBinding());
		}
		if (meshProperties->getMaterialProperties()->getAmbientKLocation() > -1) 
		{
			shaderDefines += "\n#define AMBIENT_K_LOCATION " + std::to_string(meshProperties->getMaterialProperties()->getAmbientKLocation());
		}
		if (meshProperties->getMaterialProperties()->getDiffuseKLocation() > -1) 
		{
			shaderDefines += "\n#define DIFFUSE_K_LOCATION " + std::to_string(meshProperties->getMaterialProperties()->getDiffuseKLocation());
		}
		if (meshProperties->getMaterialProperties()->getSpecularKLocation() > -1) 
		{
			shaderDefines += "\n#define SPECULAR_K_LOCATION " + std::to_string(meshProperties->getMaterialProperties()->getSpecularKLocation());
		}
		if (meshProperties->getMaterialProperties()->getEmissiveKLocation() > -1) 
		{
			shaderDefines += "\n#define EMISSIVE_K_LOCATION " + std::to_string(meshProperties->getMaterialProperties()->getEmissiveKLocation());
		}
		if (meshProperties->getMaterialProperties()->getShininessLocation() > -1) 
		{
			shaderDefines += "\n#define SHININESS_LOCATION " + std::to_string(meshProperties->getMaterialProperties()->getShininessLocation());
		}
		if (meshProperties->getMaterialProperties()->getOpacityKLocation() > -1)
		{
			shaderDefines += "\n#define OPACITY_K_LOCATION " + std::to_string(meshProperties->getMaterialProperties()->getOpacityKLocation());
		}
		if (meshProperties->getGeometryProperties()->getPosLocation() > -1)
		{
			shaderDefines += "\n#define POS_LOCATION " + std::to_string(meshProperties->getGeometryProperties()->getPosLocation());
		}
		if (meshProperties->getGeometryProperties()->getColorLocation() > -1) 
		{
			shaderDefines += "\n#define COLOR_LOCATION " + std::to_string(meshProperties->getGeometryProperties()->getColorLocation());
		}
		if (meshProperties->getGeometryProperties()->getNormalLocation() > -1) 
		{
			shaderDefines += "\n#define NORMAL_LOCATION " + std::to_string(meshProperties->getGeometryProperties()->getNormalLocation());
		}
		if (meshProperties->getGeometryProperties()->getTexCoordLocation() > -1) 
		{
			shaderDefines += "\n#define TEXCOORD_LOCATION " + std::to_string(meshProperties->getGeometryProperties()->getTexCoordLocation());
		}
		if (meshProperties->getGeometryProperties()->getTangentLocation() > -1) 
		{
			shaderDefines += "\n#define TANGENT_LOCATION " + std::to_string(meshProperties->getGeometryProperties()->getTangentLocation());
		}
		if (meshProperties->getGeometryProperties()->getBinormalLocation() > -1) 
		{
			shaderDefines += "\n#define BINORMAL_LOCATION " + std::to_string(meshProperties->getGeometryProperties()->getBinormalLocation());
		}
		shaderDefines += "\n";

		return shaderDefines;
	}

	unsigned int Shader::loadShader(const char *fileName, GLenum type)
	{
		unsigned int fileLen;
		char *source = loadStringFromFile(fileName, fileLen);

		//Se suman las cabeceras y los defines al resto del shader genérico
		std::string shaderString = _shaderString + source;

		const char *c_str = shaderString.c_str();
		int size = static_cast<int>(shaderString.size());

		//Creación y compilación del Shader 
		unsigned int shader;
		shader = glCreateShader(type);
		glShaderSource(shader, 1, (const GLchar **)&c_str, (const int *)&size);
		glCompileShader(shader);
		delete source;

		//Comprobamos que se compiló bien 
		int compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) 
		{
			//Calculamos una cadena de error 
			int logLen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
			char *logString = new char[logLen];
			glGetShaderInfoLog(shader, logLen, NULL, logString);
			std::cerr << fileName << std::endl;
			std::cerr << "Error: " << logString << std::endl;
			delete logString;
			glDeleteShader(shader);
			exit(-1);
		}
		return shader;
	}

	char * Shader::loadStringFromFile(const char *fileName, unsigned int &fileLen)
	{
		//Se carga el fichero
		std::ifstream file;
		file.open(fileName, std::ios::in);
		if (!file) return 0;

		//Se calcula la longitud del fichero
		file.seekg(0, std::ios::end);
		fileLen = unsigned int(file.tellg());
		file.seekg(std::ios::beg);

		//Se lee el fichero
		char *source = new char[fileLen + 1];

		int i = 0;
		while (file.good())
		{
			source[i] = char(file.get());
			if (!file.eof()) ++i;
			else fileLen = i;
		}
		source[fileLen] = '\0';
		file.close();

		return source;
	}
}