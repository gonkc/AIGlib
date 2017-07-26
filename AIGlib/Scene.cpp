#include "Scene.h"
#include "RenderEngine.h"
#include "Enums.h"
#include "Definitions.h"

namespace AIGlib
{
	Scene::Scene() :
		_camera(nullptr),
		_renderPass(nullptr),
		_worldNode(nullptr),
		_backgroundColor(glm::vec4(0.0f)),
		_ambientIntensity(new AmbientIntensity(glm::vec3(0.0f))),
		_fog(new Fog())/*,
		_currentProgram(-1)*/
	{

	}

	Scene::~Scene() 
	{
		freeResources();
	}

	//Getters

	Camera& Scene::getCamera() const
	{
		if (_worldNode != nullptr) 
		{
			return *static_cast<Camera*>(_worldNode->getActiveCamera());
		}
		else 
		{
			return *_camera;
		}
	}
	Camera* Scene::getCameraPtr() const
	{
		if (_worldNode != nullptr)
		{
			return static_cast<Camera*>(_worldNode->getActiveCamera());
		}
		else
		{
			return _camera;
		}
	}
	void Scene::setCamera(Camera* camera)
	{
		_camera = camera;
	}

	RenderPass* Scene::getRenderPass()
	{
		return _renderPass;
	}

	void Scene::setRenderPass(RenderPass* renderPass)
	{
		_renderPass = renderPass;
	}

	bool Scene::isRenderPassloaded() 
	{
		return _renderPass != nullptr;
	}

	Mesh* Scene::getMesh(unsigned int index) 
	{
		return _meshes[index];
	}

	std::vector<Mesh*> Scene::getMeshes() 
	{
		return _meshes;
	}

	unsigned int Scene::addMesh(Mesh* mesh)
	{
		Shader* meshShader = &RenderEngine::get().getShader(mesh->getMeshProperties());
		if (meshShader == nullptr)
		{
			std::cout << "You must set a shader in order to add mesh to scene." << std::endl;
			exit(-1);
		}
		if (mesh->getMeshProperties()->getGeometryProperties() == nullptr)
		{
			std::cout << "You must set a geometryProperties in order to add mesh to scene." << std::endl;
			exit(-1);
		}
		if (mesh->getMeshProperties()->getMaterialProperties() == nullptr)
		{
			std::cout << "You must set a material properties in order to add mesh to scene." << std::endl;
			exit(-1);
		}

		//mesh->precalculateLights();
		_meshes.push_back(mesh);
		return static_cast<unsigned int>(_meshes.size() - 1);
	}

	AmbientIntensity* Scene::getAmbientIntensity()
	{
		return _ambientIntensity;
	}

	void Scene::setAmbientIntensity(AmbientIntensity* ambientIntensity)
	{
		_ambientIntensity = ambientIntensity;
	}

	std::vector<PointLight*> Scene::getPointLights() 
	{
    std::vector<PointLight*> pointLights = _pointLights;
    if (_worldNode != nullptr)
    {
      std::vector<GroupNode*> scenePointLights = _worldNode->getPointLights();
      for (const auto& pointLight : scenePointLights)
      {
        pointLights.push_back(static_cast<PointLight*>(pointLight));
      }
    }
    return pointLights;
	}

	void Scene::addPointLight(PointLight* pointLight) 
	{
		_pointLights.push_back(pointLight);
	}

	std::vector<DirectionalLight*> Scene::getDirectionalLights()
	{
    std::vector<DirectionalLight*> directionalLights = _directionalLights;
    if (_worldNode != nullptr)
    {
      std::vector<GroupNode*> sceneDirectionalLights = _worldNode->getDirectionalLights();
      for (const auto& directionalLight : sceneDirectionalLights)
      {
        directionalLights.push_back(static_cast<DirectionalLight*>(directionalLight));
      }
    }
		return directionalLights;
	}

	void Scene::addDirectionalLight(DirectionalLight* directionalLight)
	{
		_directionalLights.push_back(directionalLight);
	}

	std::vector<SpotLight*> Scene::getSpotLights() 
	{
    std::vector<SpotLight*> spotLights = _spotLights;
    if (_worldNode != nullptr)
    {
      std::vector<GroupNode*> sceneSpotLights = _worldNode->getSpotLights();
      for (const auto& spotLight : sceneSpotLights)
      {
        spotLights.push_back(static_cast<SpotLight*>(spotLight));
      }
    }
    return spotLights;
	}

	void Scene::addSpotLight(SpotLight* spotLight) 
  {
		_spotLights.push_back(spotLight);
	}

	struct less_depth
	{
		inline bool operator() (Mesh* lhs, Mesh* rhs)
		{
			return (lhs->getDepth() < rhs->getDepth());
		}
	};

	void Scene::render()
	{
		if (isRenderPassloaded())
		{
			_renderPass->bindFirst();
			renderScene();
			_renderPass->renderAll();
		}
		else
		{
			renderScene();
		}
	}

	void Scene::renderScene()
	{

    Camera& camera = getCamera();
    if (&camera == nullptr)
    {
      std::cerr << "Error: No active camera." << std::endl;
      exit(-1);
    }

    glm::mat4& proj = camera.getProj();
    glm::mat4& view = camera.getView();

    SceneState ss;
    ss.model = glm::mat4(1.0f);
    ss.view = view;
    ss.proj = proj;
		ss.program = -1;

		//Limpiamos los bufferes 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Activamos el buffer de profundidad
		glEnable(GL_DEPTH_TEST);

		//Single meshes
		for (const auto& mesh : _meshes)    
		{
		  mesh->draw(ss);
		}

		//Scene tree
    //ss.model = glm::mat4(1.0f);
    //ss.view = camera.getView();
    //ss.proj = camera.getProj();
    //ss.view = glm::mat4(1.0f);
    //ss.proj = glm::mat4(1.0f);
    //ss.program = -1;

		_worldNode->render(ss);

		//Sort transparent meshes >
		std::sort(std::begin(_transparentMeshes), std::end(_transparentMeshes), less_depth());

		//Render transparent meshes
		for (const auto& mesh : _transparentMeshes)
		{			
		  mesh->render(ss);
		}
    
		//Clean transparent meshes
		_transparentMeshes.clear();
		std::vector<Mesh*>().swap(_transparentMeshes);
    
		//Desenlazamos el shader
		glUseProgram(NULL);
	}

	void Scene::setRootNode(GroupNode* rootNode) 
	{
		_worldNode = rootNode;

		for (const auto& meshProperties : RenderEngine::get().getMeshProperties())
		{
			if (meshProperties.second != nullptr)
			{
				meshProperties.second->use();
				meshProperties.second->loadAmbientIntensity();
				meshProperties.second->loadPointLights(getPointLights());
				meshProperties.second->loadDirectionalLights(getDirectionalLights());
				meshProperties.second->loadSpotLights(getSpotLights());		
			}
		}
		glUseProgram(NULL);	
    
    if (_renderPass != nullptr)
    {
      if (_renderPass->getFramebufferObjects().size() == 0)
      {

        Shader* shader = new Shader(nullptr);
        shader->addVertexShader("../shaders/shader.fbo.vert");
        shader->addFragmentShader("../shaders/shader.fbo.frag");
        shader->compileAndLinkShaders();
        FramebufferObject* framebufferObject = new FramebufferObject();
        framebufferObject->setShader(shader);
        _renderPass->addRenderPass(framebufferObject);
      }
    }
	}

	GroupNode* Scene::getRootNode() 
	{
		return _worldNode;
	}

	void Scene::changeIlluminationModel(IlluminationModelType illuminationModel)
	{
		switch (illuminationModel) 
		{
		case IlluminationModelType::PHONG:
			std::cout << "Illumination Model changed to 'Phong'" << std::endl;
			break;
		case IlluminationModelType::BLINN_PHONG:
			std::cout << "Illumination Model changed to 'Blinn-Phong'" << std::endl;
			break;
		}
		for (unsigned int i = 0; i < _meshes.size(); ++i)
		{
			_meshes[i]->setIlluminationModel(illuminationModel);
		}
		if (_worldNode != nullptr)
		{
			_worldNode->setIlluminationModel(illuminationModel);
		}
	}

	void Scene::changeShadingModel(ShadingModelType shadingModel) 
	{
		switch (shadingModel) 
		{
		case ShadingModelType::FLAT:
			std::cout << "Shading Model changed to 'Flat'" << std::endl;
			break;
		case ShadingModelType::GOURAUD:
			std::cout << "Shading Model changed to 'Gouraud'" << std::endl;
			break;
		case ShadingModelType::PHONG:
			std::cout << "Shading Model changed to 'Phong'" << std::endl;
			break;
		}
		for (unsigned int i = 0; i < _meshes.size(); ++i) 
		{
			_meshes[i]->setShadingModel(shadingModel);
		}
		if (_worldNode != nullptr) 
		{
			_worldNode->setShadingModel(shadingModel);
		}
	}

	void Scene::setTwoSided(bool twoSided) 
	{
		if (twoSided) 
		{
			std::cout << "'TwoSided' enabled."<< std::endl;
		}
		else 
		{
			std::cout << "'TwoSided' disabled." << std::endl;
		}
		for (unsigned int i = 0; i < _meshes.size(); ++i)
		{
			_meshes[i]->setTwoSided(twoSided);
		}
		if (_worldNode != nullptr)
		{
			_worldNode->setTwoSided(twoSided);
		}
	}

	void Scene::setWireframe(WireframeType wireframe) 
	{
		switch (wireframe) 
		{
		case WireframeType::POINT:
			std::cout << "'Wireframe' changed to point." << std::endl;
			for (unsigned int i = 0; i < _meshes.size(); ++i)
			{
				_meshes[i]->setWireframe(wireframe);
			}
			if (_worldNode != nullptr)
			{
				_worldNode->setWireframe(wireframe);
			}
			break;
		case WireframeType::LINE:
			std::cout << "'Wireframe' changed to line." << std::endl;
			for (unsigned int i = 0; i < _meshes.size(); ++i)
			{
				_meshes[i]->setWireframe(wireframe);
			}
			if (_worldNode != nullptr)
			{
				_worldNode->setWireframe(wireframe);
			}
			break;
		case WireframeType::FILL:
			std::cout << "'Wireframe' changed to fill." << std::endl;
			for (unsigned int i = 0; i < _meshes.size(); ++i) 
			{
				_meshes[i]->setWireframe(wireframe);
			}
			if (_worldNode != nullptr) 
			{
				_worldNode->setWireframe(wireframe);
			}
			break;
		}
	}

	void Scene::enableAmbientOcclusion()
	{
		std::cout << "'Ambient Occlusion' enabled." << std::endl;
		for (unsigned int i = 0; i < _meshes.size(); ++i)
		{
			_meshes[i]->enableAmbientOcclusion();
		}
		if (_worldNode != nullptr) 
		{
			_worldNode->enableAmbientOcclusion();
		}
	}

	void Scene::disableAmbientOcclusion() 
	{
		std::cout << "'Ambient Occlusion' disabled." << std::endl;
		for (unsigned int i = 0; i < _meshes.size(); ++i) 
		{
			_meshes[i]->disableAmbientOcclusion();
		}
		if (_worldNode != nullptr) 
		{
			_worldNode->disableAmbientOcclusion();
		}
	}

	void Scene::enableBumpMapping() 
	{
		std::cout << "'Bump Mapping' enabled." << std::endl;
		for (unsigned int i = 0; i < _meshes.size(); ++i) 
		{
			_meshes[i]->enableBumpMapping();
		}
		if (_worldNode != nullptr) 
		{
			_worldNode->enableBumpMapping();
		}
	}

	void Scene::disableBumpMapping() 
	{
		std::cout << "'Bump Mapping' disabled." << std::endl;
		for (unsigned int i = 0; i < _meshes.size(); ++i) 
		{
			_meshes[i]->disableBumpMapping();
		}
		if (_worldNode != nullptr) 
		{
			_worldNode->disableBumpMapping();
		}
	}

	void Scene::enableParallaxMapping(float scale, float bias)
	{
		std::cout << "'Parallax Mapping' enabled." << std::endl;
		for (unsigned int i = 0; i < _meshes.size(); ++i) 
		{
			_meshes[i]->enableParallaxMapping(scale, bias);
		}
		if (_worldNode != nullptr)
		{
			_worldNode->enableParallaxMapping(scale, bias);
		}
	}

	void Scene::disableParallaxMapping() 
	{
		std::cout << "'Parallax Mapping' disabled." << std::endl;
		for (unsigned int i = 0; i < _meshes.size(); ++i) 
		{
			_meshes[i]->disableParallaxMapping();
		}
		if (_worldNode != nullptr) 
		{
			_worldNode->disableParallaxMapping();
		}
	}

	void Scene::resize(unsigned int width, unsigned int height) 
	{
		glViewport(0, 0, width, height);

		//Resize para las cámaras del grafo
		if (_worldNode != nullptr)
		{		
			_worldNode->resize(width, height);
		}

		//Resize para la cámara global
		if (_camera != nullptr)
		{
			_camera->resize(width, height);
		}

		if (isRenderPassloaded())
		{
			_renderPass->resize(width, height);
		}
	}

	void Scene::freeResources() 
	{
		//Eliminamos la cámara
		delete _camera;

		//Eliminamos la niebla
		delete _fog;

		//Eliminamos los fbos
		delete _renderPass;

		//Eliminamos las mallas independientes
		for (unsigned int i = 0; i < _meshes.size(); ++i)
		{
			delete _meshes[i];
		}

		//Eliminamos las mallas del árbol de escena
		delete _worldNode;

		//Eliminamos las luces puntuales
		for (unsigned int i = 0; i < _pointLights.size(); ++i)
		{
			delete _pointLights[i];
		}

		//Eliminamos las luces direccionales
		for (unsigned int i = 0; i < _directionalLights.size(); ++i)
		{
			delete _directionalLights[i];
		}

		//Eliminamos las luces focales
		for (unsigned int i = 0; i < _spotLights.size(); ++i)
		{
			delete _spotLights[i];
		}

		//Eliminamoz la intensidad ambiental
		delete _ambientIntensity;
	}

	glm::vec4 Scene::getBackgroundColor()
	{
		return _backgroundColor;
	}

	void Scene::setBackgroundColor(glm::vec4 backgroundColor)
	{
		_backgroundColor = backgroundColor;
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	}

	Fog* Scene::getFog()
	{
		return _fog;
	}

	void Scene::setFog(Fog* fog)
	{
		_fog = fog;
	}

  void Scene::addMeshToTransparentMeshes(Mesh* mesh, SceneState ss)
  {
	  glm::vec3 cameraLookAt = glm::vec3(ss.view[0][2], ss.view[1][2], ss.view[2][2]);
	  glm::vec3 cameraPosition = glm::vec3(ss.view[3].xyz);

    //ModelView
    glm::mat4 modelView = ss.view * mesh->getModel();

    //Center position (local coordinates)
    glm::mat4 posModelSpace = glm::translate(glm::mat4(1.0f), mesh->getCenterPosition());

    //Center position (camera coordinates)
    glm::mat4 posCameraSpace = posModelSpace * modelView;

    float depth = glm::dot(glm::vec3(posCameraSpace[3].xyz) - cameraPosition, cameraLookAt);

    //Set distance value
    mesh->setDepth(depth);

    _transparentMeshes.push_back(mesh);
  }
}