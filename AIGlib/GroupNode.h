#ifndef __GROUPNODE_H__
#define __GROUPNODE_H__

#include "AIGlibMacros.h"
#include "Enums.h"
#include "SceneState.h"
#include <gl/glew.h>
#include <gl/gl.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <string>
#include <vector>

namespace AIGlib 
{
	class AIGLIB_API GroupNode
	{
	public:		
		GroupNode(std::string name = std::string());
		~GroupNode();

		virtual void addChildNode(GroupNode* node);
		void removeChildNode(GroupNode* node);

		GroupNode* getParent() const;
		std::string getName() const;
		unsigned int getIndex() const;
		NodeType getNodeType() const;
		std::vector<GroupNode*> getChildNodes() const;
		GroupNode* getChildNode(unsigned int index) const;

		void translate(glm::vec3 translation);
		void translate(float xAxis, float yAxis, float zAxis);
		void rotate(glm::vec3 angles);
		void rotate(float xAngle, float yAngle, float zAngle);
		void rotate(float angle, glm::vec3 axis);
		void rotate(float angle, float xAxis, float yAxis, float zAxis);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		void rotateXY(float angle);
		void rotateXZ(float angle);
		void rotateYZ(float angle);
		void rotateXYZ(float angle);

		GroupNode* getNodeByName(std::string name, bool recursive = true);
		std::vector<GroupNode*> getNodes();

		void printTree(std::string indent = std::string());

		virtual GroupNode* getActiveCamera();
		virtual void deactivateCameras();
		GroupNode* getRootNode();
		virtual void resize(unsigned int width, unsigned int height);

		virtual void setIlluminationModel(IlluminationModelType illuminationModel);
		virtual void setShadingModel(ShadingModelType shadingModel);

		virtual void setTwoSided(bool twoSided);
		virtual void setWireframe(WireframeType wireframe);

		virtual void enableAmbientOcclusion();
		virtual void disableAmbientOcclusion();
		virtual void enableBumpMapping();
		virtual void disableBumpMapping();
		virtual void enableParallaxMapping(float scale, float bias);
		virtual void disableParallaxMapping();

		//virtual void precalculateLights();
		virtual void loadUniformBlocks();

    void setModel(const glm::mat4& model);
    glm::mat4 getModel() const;

    void render(SceneState ss);

    std::vector<GroupNode*> getPointLights();
    std::vector<GroupNode*> getDirectionalLights();
    std::vector<GroupNode*> getSpotLights();
    void setName(std::string name);
	protected:

    glm::mat4 _model;
    glm::quat _orientation;
		glm::vec3 _translation;
		glm::vec3 _rotation;
		

		void setNodeType(NodeType nodeType);
		void setIndex(unsigned int index);
		std::string getNodeTypeString(NodeType nodeType);
    virtual void draw(SceneState ss);
	private:
		GroupNode* _parent;
		std::string _name;
		unsigned int _index;
		NodeType _nodeType;
		std::vector<GroupNode*> _nodes;
		void freeResources();
		
	};
}

#endif