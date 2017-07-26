#ifndef __RENDERENGINE_H__
#define __RENDERENGINE_H__

#include "AIGlibMacros.h"
#include "Scene.h"
#include "GeometryProperties.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Attribute.h"
#include "Enums.h"
#include <map>
#include <memory>
#include <mutex>
#include <algorithm>
#include <pugixml/pugixml.hpp>

namespace AIGlib 
{
	class AIGLIB_API RenderEngine
	{
	public:
		//Getters
		static RenderEngine& get();
		Scene& getScene() const;
		GeometryProperties& getGeometryProperties(unsigned int index) const;
		MaterialProperties& getMaterialProperties(unsigned int index) const;
		Shader& getShader(MeshProperties* meshProperties) const;
		Texture& getTexture(unsigned int index) const;
		std::vector<MaterialProperties*> getMaterialProperties() const;

		//Setters
		void setScene(Scene *scene);
    unsigned int addGeometry(GeometryProperties *geometryProperties);
		unsigned int addMaterialProperties(MaterialProperties* materialProperties);
		void addShader(MeshProperties* meshProperties, Shader* shader);
		int addCustomShader(Shader* shader);
		void useCustomShaders();
		bool getIfCustomShaders();
		unsigned int registerTexture(std::string texturePath);

		//Attributes
		void registerGenericAttribute(unsigned int location, int size, AttributeDataType type, bool normalized, AttributeType attributeType);
		void registerCustomAttribute(unsigned int location, std::string name, int size, AttributeDataType type, bool normalized, AttributeType attributeType);

		void registerAttributesFromXML(std::string filename);
		std::map<std::string, Attribute*> getAttributes();

		//Materials
		void registerGenericTextureMaterial(unsigned int index, TextureMaterialType textureMaterialType);
		void registerCustomTextureMaterial(unsigned int index, std::string materialName, TextureMaterialType textureMaterialType);

		void registerGenericUniformMaterial(unsigned int index, UniformMaterialType uniformMaterialType);
		void registerCustomUniformMaterial(unsigned int index, std::string materialName, UniformMaterialType uniformMaterialType);

		void registerMaterialsFromXML(std::string filename);
		std::map<std::string, Material*> getMaterials();
		int getTextureMaterialBinding(TextureMaterialType textureMaterialType);
		int getUniformMaterialLocation(UniformMaterialType uniformMaterialType);

		void activeAnisotropicFilterIfSupported();
		void initOpenGLsettings();
		glm::vec4 getBackgroundColor();
		void setBackgroundColor(glm::vec4 backgroundColor);

		//Common
		void freeResources();

		//Info
		void printOpenGLVersionInfo();
		void printNumExtensions();
		void printUBOLimits();
		void printFBOLimits();

		std::map<MeshProperties*, Shader*> getMeshProperties();
	private:
		RenderEngine();
		RenderEngine(const RenderEngine&) = delete;
		RenderEngine& operator=(const RenderEngine&) = delete;

		void registerAttribute(unsigned int id, std::string name, int size, GLenum type, bool normalized, AttributeType attributeType);
		void registerTextureMaterial(unsigned int index, std::string materialName, TextureMaterialType textureMaterialType);
		void registerUniformMaterial(unsigned int index, std::string materialName, UniformMaterialType uniformMaterialType);
    GLenum getOpenGLDataType(AttributeDataType type);
    AttributeDataType getOpenGLDataType(std::string type);

		//static RenderEngine* _instance;
		static std::unique_ptr<RenderEngine> _instance;
		static std::once_flag _onceFlag;
		Scene* _scene;
		std::vector<GeometryProperties*> _geometries;
		std::vector<MaterialProperties*> _materialProperties;
		std::map<MeshProperties*, Shader*> _shaders;
		bool _useCustomShaders;
		std::vector<Shader*> _customShaders;
		std::vector<Texture*> _textures;
		std::map<std::string, Material*> _materials;
		std::map<std::string, Attribute*> _attributes;

	};
}
#endif