#ifndef __MESH_H__
#define __MESH_H__

#include "AIGlibMacros.h"
#include "Texture.h"
#include "Shader.h"
#include "GroupNode.h"
#include "Enums.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <array>

namespace AIGlib 
{
	class AIGLIB_API Mesh : public GroupNode
	{
	public:

		Mesh(std::string name = std::string());
		~Mesh();

    bool getIsTransparent();

		void scale(glm::vec3 scale);
		void scale(float xAxis, float yAxis, float zAxis);

		MeshProperties* getMeshProperties() const;
		void setMaterialProperties(unsigned int index);
		void setGeometryProperties(unsigned int index);

		int getCustomShaderId();
		void setCustomShaderId(int customShaderId);

		void setMaterialFromAttributes();

		void setIlluminationModel(IlluminationModelType illuminationModel);
		void setShadingModel(ShadingModelType shadingModel);

		void enableAmbientOcclusion();
		void disableAmbientOcclusion();
		void enableBumpMapping();
		void disableBumpMapping();
		void enableParallaxMapping(float scale, float bias);
		void disableParallaxMapping();

		void setTwoSided(bool twoSided);
		void setWireframe(WireframeType wireframe);

		int getAmbientOcclusion();
		int getBumpMapping();
		int getNormalMapping();
		int getParallaxMapping();
		float getPMScale();
		float getPMBias();

		glm::vec3 getCenterPosition();
		void getCenterPosition(glm::vec3 centerPosition);

		void setDepth(float depth);
		float getDepth();

    void draw(SceneState ss);
    void render(SceneState ss);


	private:

    //Properties
		MeshProperties* _meshProperties;
    glm::vec3 _scale;
    glm::vec3 _centerPosition;
    float _depth;

		//Effects
		int _ambientOcclusion;
		int _bumpMapping;
		int _normalMapping;
		int _parallaxMapping;
		float _pmscale;
		float _pmbias;

    //Private methods
		void createOrsetGenericShader();
		void freeResources();
	};
}
#endif