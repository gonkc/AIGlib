#ifndef __ASSIMPLOADER_H__
#define __ASSIMPLOADER_H__

#include "AIGlibMacros.h"
#include "RenderEngine.h"
#include "GeometryProperties.h"
#include "GroupNode.h"
#include "MaterialProperties.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h> 

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <string>
#include <vector>

namespace AIGlib 
{
	class AIGLIB_API AssimpLoader 
	{
	public:

		AssimpLoader(const unsigned int& width, const unsigned int& height);
		~AssimpLoader();

		bool loadFile(std::string filePath);
		GroupNode* createSceneGraph(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 translation =
			glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), int customShaderId = -1);
		std::vector<Mesh*> createAllMeshes(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 translation =
			glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), int customShaderId = -1);

	private:

		std::vector<Assimp::Importer*> _importers;
		const aiScene* _scene;
		std::string _basePath;
    glm::vec3 _scale;
		glm::vec3 _translation;
		glm::vec3 _rotation;
		std::vector<unsigned int> _materialPropertiesIndices;
		int _customShaderId;
		unsigned int _width;
		unsigned int _height;

		void getMaterials();
		std::vector<GroupNode*> getLights();
		std::vector<Camera*> getCameras();
		Mesh* createMesh(const aiMesh* mesh);
		GroupNode* createNode(const aiNode* node, GroupNode* root);
    void cleanResources();
		void freeResources();
	};
}

#endif