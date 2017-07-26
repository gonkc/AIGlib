#ifndef __SCENE_H__
#define __SCENE_H__

#include "AIGlibMacros.h"
#include "Camera.h"
#include "Fog.h"
#include "Mesh.h"
#include "AmbientIntensity.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Shader.h"
#include "RenderPass.h"
#include "GeometryProperties.h"
#include "GroupNode.h"
#include "Definitions.h"
#include "SceneState.h"
#include <array>



namespace AIGlib
{
	class AIGLIB_API Scene
	{
	public:
		Scene();
		~Scene();

		//getters
		Camera& getCamera() const;
		Camera* getCameraPtr() const;
		Mesh* getMesh(unsigned int index);
		std::vector<Mesh*> getMeshes();
		AmbientIntensity* getAmbientIntensity();
		std::vector<PointLight*> getPointLights();
		std::vector<DirectionalLight*> getDirectionalLights();
		std::vector<SpotLight*> getSpotLights();
		GroupNode* getRootNode();
		Fog* getFog();

		//setters
		void setCamera(Camera* camera);
		unsigned int addMesh(Mesh* mesh);
		void setAmbientIntensity(AmbientIntensity* ambientIntensity);
		void addPointLight(PointLight *pointLight);
		void addDirectionalLight(DirectionalLight* directionalLight);
		void addSpotLight(SpotLight *spotLight);
		void setRootNode(GroupNode* rootNode);
		void setFog(Fog* fog);

		//Others
		bool isRenderPassloaded();
		void render();


		void changeIlluminationModel(IlluminationModelType illuminationModel);
		void changeShadingModel(ShadingModelType shadingModel);
		void setTwoSided(bool twoSided);
		void setWireframe(WireframeType wireframe);
		void enableAmbientOcclusion();
		void disableAmbientOcclusion();
		void enableBumpMapping();
		void disableBumpMapping();
		void enableParallaxMapping(float scale = 0.02f, float bias = -0.03f);
		void disableParallaxMapping();
		void resize(unsigned int width, unsigned int height);

		glm::vec4 getBackgroundColor();
		void setBackgroundColor(glm::vec4 backgroundColor);

		RenderPass* getRenderPass();
		void setRenderPass(RenderPass* renderPass);

    void addMeshToTransparentMeshes(Mesh* mesh, SceneState ss);

	private:

		Camera* _camera;
		Fog* _fog;
		RenderPass* _renderPass;
		std::vector<Mesh*> _meshes;
		GroupNode* _worldNode; 
    std::vector<Mesh*> _transparentMeshes;

		AmbientIntensity* _ambientIntensity;
		std::vector<PointLight*> _pointLights;
		std::vector<DirectionalLight*> _directionalLights;
		std::vector<SpotLight*> _spotLights;

		glm::vec4 _backgroundColor;

		void renderScene();
		void freeResources();

	};
}

#endif