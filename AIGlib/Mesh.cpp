#include "Mesh.h"
#include "RenderEngine.h"
#include <memory>

namespace AIGlib 
{
	Mesh::Mesh(std::string name) :
		_scale(1.0f),
		_ambientOcclusion(0),
		_bumpMapping(0),
		_normalMapping(0),
		_parallaxMapping(0),
		_pmscale(0.0f),
		_pmbias(0.0f),
		_depth(0.0f),
    _centerPosition(glm::vec3(0.0f, 0.0f, 0.0f))
	{
		setName(name);
		setNodeType(NodeType::MESH);
		_meshProperties = new MeshProperties();
	}

	Mesh::~Mesh() 
	{

	}

  void Mesh::draw(SceneState ss)
  {
    _model = glm::mat4(1.0f);
    _model = glm::translate(_model, _translation);
    _model = glm::rotate(_model, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    _model = glm::rotate(_model, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    _model = glm::rotate(_model, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    _model = glm::scale(_model, _scale);

    _model = ss.model * _model;

    if (getIsTransparent())
    {
      RenderEngine::get().getScene().addMeshToTransparentMeshes(this, ss);
    }
    else
    {

      render(ss);
    }
  }

  void Mesh::render(SceneState ss)
  {
    MaterialProperties* materialProperties = _meshProperties->getMaterialProperties();
    Shader& shader = RenderEngine::get().getShader(_meshProperties);

    //Calculamos las matrices requeridas por el vertex shader
    glm::mat4 modelView = ss.view * _model;
    glm::mat4 modelViewProj = ss.proj * ss.view * _model;
    glm::mat4 normal = glm::transpose(glm::inverse(modelView));
    std::array<int, N_MATRIX_UNIFORMS> uMatrices = shader.getMatrixUniforms();
    //int uNPointLightsUniforms = shader.getMeshPointLightsUniforms();
    //int uNDirectionalLightsUniforms = shader.getMeshDirectionalLightsUniforms();
    //int uNSpotLightsUniforms = shader.getMeshSpotLightsUniforms();
    //Desenlazamos texturas entre objetos
    /*for (unsigned int i = 0; i < RenderEngine::get().getMaterials().size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, NULL);
    }*/
    //glBindTextures(0, RenderEngine::get().getMaterials().size(), NULL);

    //if (ss.program != shader.getProgram())
    //{
      //std::cout << "shader n: " << shader.getProgram() << std::endl;
      ss.program = (int)shader.getProgram();
      shader.use();

      int uBackgroundColor = shader.getBackgroundColorUniform();
      if (uBackgroundColor != -1) //background color
      {
        glUniform3fv(uBackgroundColor, 1, glm::value_ptr(RenderEngine::get().getScene().getBackgroundColor()));
      }

      int uCameraPosition = shader.getCameraPositionUniform();
      if (uCameraPosition != -1) //Posición de la cámara
      {
        glm::vec3 cameraPosition = glm::vec3(ss.view[3].x, ss.view[3].y, ss.view[3].z);
        glUniform3fv(uCameraPosition, 1, glm::value_ptr(cameraPosition));
      }

      shader.loadAmbientIntensity();

      //Matrices
      if (uMatrices[0] != -1) //Proj
      {
        glUniformMatrix4fv(uMatrices[0], 1, GL_FALSE, &(ss.proj[0][0]));
      }
      if (uMatrices[1] != -1) //View
      {
        glUniformMatrix4fv(uMatrices[1], 1, GL_FALSE, &(ss.view[0][0]));
      }

      int uIlluminationModel = shader.getIlluminationModelUniform();
      int uShadingModel = shader.getShadingModelUniform();

      //Modelo de iluminación
      if (uIlluminationModel != -1) {
        glUniform1i(uIlluminationModel, static_cast<int>(materialProperties->getIlluminationModel()));
      }

      //Modelo de sombreado
      if (uShadingModel != -1)
      {
        glUniform1i(uShadingModel, static_cast<int>(materialProperties->getShadingModel()));
      }

      //Material textures && material uniforms
      for (const auto& it : shader.getMaterials())
      {
        unsigned int location = it.second->getLocation();
        unsigned int id = it.second->getId();
        TextureMaterial* textureMaterial = nullptr;
        UniformMaterial* uniformMaterial = nullptr;
        switch (it.second->getSourceType())
        {
        case MaterialSourceType::TEXTURE:
          textureMaterial = materialProperties->getTextureMaterial(it.second->getTextureType());
          if (textureMaterial != nullptr)
          {
            glActiveTexture(GL_TEXTURE0 + location);
            glBindTexture(GL_TEXTURE_2D, textureMaterial->getTextureId());
            //glUniform1i(location, id); old correct
            //glUniform1i(id, location);
          }
          break;
        case MaterialSourceType::UNIFORM:
          uniformMaterial = materialProperties->getUniformMaterial(it.second->getUniformType());
          if (uniformMaterial != nullptr)
          {
            glUniform3fv(location, 1, glm::value_ptr(uniformMaterial->getColor()));
          }
          break;
        }
      }

      //Material scalars
      std::array<int, N_SCALAR_UNIFORMS> uMaterialScalars = shader.getMaterialScalarUniforms();
      std::array<float, N_SCALAR_UNIFORMS> materialScalars = materialProperties->getMaterialScalars();
      //Escalares del material
      if (uMaterialScalars[0] != -1) //Shininess
      {
        glUniform1f(uMaterialScalars[0], materialProperties->getShininess());
      }
      if (uMaterialScalars[1] != -1) //ShininessStrength
      {
        glUniform1f(uMaterialScalars[1], materialProperties->getShininessStrength());
      }
      if (uMaterialScalars[2] != -1) //Reflectivity
      {
        glUniform1f(uMaterialScalars[2], materialProperties->getReflectivity());
      }
      if (uMaterialScalars[3] != -1) //RefractIndex
      {
        glUniform1f(uMaterialScalars[3], materialProperties->getRefractIndex());
      }
      if (uMaterialScalars[4] != -1) //Opacity
      {
        glUniform1f(uMaterialScalars[4], materialProperties->getOpacity());
      }

      //Fog uniforms
      RenderEngine::get().getScene().getFog()->loadUniforms(&shader);
    //}

    //Matrices que dependen de la model
    if (uMatrices[2] != -1) //Model
    {
      glUniformMatrix4fv(uMatrices[2], 1, GL_FALSE, &(_model[0][0]));
    }
    if (uMatrices[3] != -1) //ModelView
    {
      glUniformMatrix4fv(uMatrices[3], 1, GL_FALSE, &(modelView[0][0]));
    }
    if (uMatrices[4] != -1) //ModelViewProj
    {
      glUniformMatrix4fv(uMatrices[4], 1, GL_FALSE, &(modelViewProj[0][0]));
    }
    if (uMatrices[5] != -1) //Normal
    {
      glUniformMatrix4fv(uMatrices[5], 1, GL_FALSE, &(normal[0][0]));
    }

    //Mesh effects
    int uAmbientOcclusion = shader.getAmbientOcclusionUniform();
    int uBumpMapping = shader.getBumpMappingUniform();
    std::array<int, N_PARALLAX_UNIFORMS> uParallaxMapping = shader.getParallaxMappingUniforms();
    if (uAmbientOcclusion != -1) //Ambient occlusion
    {
      glUniform1i(uAmbientOcclusion, _ambientOcclusion);
    }
    if (uBumpMapping != -1) //Bump Mapping
    {
      glUniform1i(uBumpMapping, _bumpMapping);
    }
    if (uParallaxMapping[0] != -1) //Parallax Mapping
    {
      glUniform1i(uParallaxMapping[0], _parallaxMapping);
    }
    if (uParallaxMapping[1] != -1) //Parallax Mapping (scale)
    {
      glUniform1f(uParallaxMapping[1], _pmscale);
    }
    if (uParallaxMapping[2] != -1) //Parallax Mapping (bias)
    {
      glUniform1f(uParallaxMapping[2], _pmbias);
    }

    //Two-sided
    if (!materialProperties->getTwoSided())
    {
      glDisable(GL_CULL_FACE);
    }
    else
    {
      glEnable(GL_CULL_FACE);
    }

    //Transparency
    //float opacity = materialProperties->getOpacity();
    //if (opacity < 1.0f)
    if (getIsTransparent())
    {
      glDisable(GL_LIGHTING);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    //Wireframe
    WireframeType wireframe = materialProperties->getWireframe();

    if (wireframe != WireframeType::FILL)
    {
      switch (wireframe)
      {
      case WireframeType::POINT:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
      case WireframeType::LINE:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
      }
    }

    //Activamos el vao y pintamos la lista de triángulos
    _meshProperties->getGeometryProperties()->render();

    if (wireframe != WireframeType::FILL)
    {
      //Dejamos el Wireframe por defecto a GL_FILL
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //if (transparentK != glm::vec3(0.0f)) {
    //if (opacity < 1.0f)
    if (getIsTransparent())
    {
      glDisable(GL_BLEND);
      glEnable(GL_LIGHTING);
    }
  }

  bool Mesh::getIsTransparent()
  {
    return _meshProperties->getMaterialProperties()->getOpacity() < 1.0f;
  }

	void Mesh::scale(glm::vec3 scale) 
	{
		_scale = scale;
	}

	void Mesh::scale(float xAxis, float yAxis, float zAxis)
	{
		_scale.x = xAxis;
		_scale.y = yAxis;
		_scale.z = zAxis;
	}

	void Mesh::setMaterialFromAttributes() 
	{
		_meshProperties->getMaterialProperties()->setMaterialFromAttributes();
	}

	MeshProperties* Mesh::getMeshProperties() const
	{
		return _meshProperties;
	}

	void Mesh::setMaterialProperties(unsigned int index) 
	{
		MaterialProperties& materialProperties = RenderEngine::get().getMaterialProperties(index);
		_meshProperties->setMaterialProperties(&materialProperties);

		if (!RenderEngine::get().getIfCustomShaders()) 
		{
			if (_meshProperties->getGeometryProperties() != nullptr) 
			{
				createOrsetGenericShader();
			}
		}
	}

	void Mesh::setGeometryProperties(unsigned int index)
	{
		GeometryProperties& geometryProperties = RenderEngine::get().getGeometryProperties(index);

    _centerPosition = geometryProperties.getVertexPosCenter();

		_meshProperties->setGeometryProperties(&geometryProperties);

		if (!RenderEngine::get().getIfCustomShaders()) 
		{
			if (_meshProperties->getMaterialProperties() != nullptr) 
			{
				createOrsetGenericShader();
			}
		}
	}

	int Mesh::getCustomShaderId()
	{
		if (!RenderEngine::get().getIfCustomShaders()) 
		{
			std::cerr << "Error - You can't use getCustomShaderId because You're using generic shaders." << std::endl;
			exit(-1);
		}
		return _meshProperties->getCustomShaderId();
	}
	void Mesh::setCustomShaderId(int customShaderId)
	{
		if (!RenderEngine::get().getIfCustomShaders()) 
		{
			std::cerr << "Error - You can't use setCustomShaderId because You're using generic shaders." << std::endl;
			exit(-1);
		}
		_meshProperties->setCustomShaderId(customShaderId);
	}

	void Mesh::createOrsetGenericShader()
	{	
		//Buscar shader con estas propiedades, si no existe se crea, y si existe se coge el index
		Shader *shader = &RenderEngine::get().getShader(_meshProperties);
		if (shader == nullptr) 
		{
			Shader* newShader = new Shader(_meshProperties);
			newShader->compileAndLinkShaders();
			RenderEngine::get().addShader(_meshProperties, newShader);
		}
	}

	void Mesh::setIlluminationModel(IlluminationModelType illuminationModel) 
	{
		_meshProperties->getMaterialProperties()->setIlluminationModel(illuminationModel);
	}
	void Mesh::setShadingModel(ShadingModelType shadingModel) 
	{
		_meshProperties->getMaterialProperties()->setShadingModel(shadingModel);
	}

	void Mesh::enableAmbientOcclusion()
	{
		if (_ambientOcclusion == 0)
		{
			if (_meshProperties->getMaterialProperties()->getTextureMaterial(TextureMaterialType::LIGHTMAP) != nullptr)
			{
				//Sólo se activa si tiene la textura adecuada
				_ambientOcclusion = 1;
			}
      else
      {
        std::cerr << "Error: Ambient Occlussion can't be enabled. Lightmap texture not found!" << std::endl;
      }
		}
	}

	void Mesh::disableAmbientOcclusion() 
	{
		if (_ambientOcclusion == 1)
		{
			_ambientOcclusion = 0;
		}
	}

	void Mesh::enableBumpMapping()
	{
		if (_bumpMapping == 0) 
		{
			if ((_meshProperties->getMaterialProperties()->getTextureMaterial(TextureMaterialType::NORMAL) != nullptr) ||
				(_meshProperties->getMaterialProperties()->getTextureMaterial(TextureMaterialType::HEIGHT) != nullptr))
			{
				//Sólo se activa si tiene la textura adecuada
        disableParallaxMapping();
				_bumpMapping = 1;
			}
			else
			{
				std::cerr << "Error: Bump mapping can't be enabled. Normal texture or Height texture not found!" << std::endl;
			}
		}
	}

	void Mesh::disableBumpMapping() 
	{
		if (_bumpMapping == 1)
		{
			_bumpMapping = 0;
		}
	}

	void Mesh::enableParallaxMapping(float scale, float bias)
	{
		if (_parallaxMapping == 0)
		{
			if (((_meshProperties->getMaterialProperties()->getTextureMaterial(TextureMaterialType::NORMAL) != nullptr) ||
				(_meshProperties->getMaterialProperties()->getTextureMaterial(TextureMaterialType::HEIGHT) != nullptr)) &&
			    (_meshProperties->getMaterialProperties()->getTextureMaterial(TextureMaterialType::DISPLACEMENT) != nullptr))
			{
        //Sólo se activa si tiene la textura adecuada
        disableBumpMapping();
				_pmscale = scale;
				_pmbias = bias;
				_parallaxMapping = 1;
			}
			else
			{
				std::cerr << "Error: Parallax mapping can't be enabled. Normal or Height textures not found, or Displacement texture not found!" << std::endl;
			}
		}
	}

	void Mesh::disableParallaxMapping()
	{
		if (_parallaxMapping == 1) 
		{
			_parallaxMapping = 0;
		}
	}

	void Mesh::setTwoSided(bool twoSided) 
	{
		_meshProperties->getMaterialProperties()->setTwoSided(twoSided);
	}

	void Mesh::setWireframe(WireframeType wireframe) 
	{
		_meshProperties->getMaterialProperties()->setWireframe(wireframe);
	}

	void Mesh::freeResources() 
	{
		//Eliminamos las propiedades del shader
		delete _meshProperties;
	}

	int Mesh::getAmbientOcclusion()
	{
		return _ambientOcclusion;
	}
	int Mesh::getBumpMapping()
	{
		return _bumpMapping;
	}
	int Mesh::getNormalMapping()
	{
		return _normalMapping;
	}
	int Mesh::getParallaxMapping()
	{
		return _parallaxMapping;
	}
	float Mesh::getPMScale()
	{
		return _pmscale;
	}
	float Mesh::getPMBias()
	{
		return _pmbias;
	}

	glm::vec3 Mesh::getCenterPosition()
	{
		return _centerPosition;
	}

  void Mesh::getCenterPosition(glm::vec3 centerPosition)
  {
    _centerPosition = centerPosition;
  }

	void Mesh::setDepth(float depth)
	{
		_depth = depth;
	}

  float Mesh::getDepth()
	{
		return _depth;
	}

}
