#ifndef __SHADER_H__
#define __SHADER_H__

#include "AIGlibMacros.h"
#include "Attribute.h"
#include "Material.h"
#include "Texture.h"
#include "MeshProperties.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <gl/glew.h>
#include <set>
#include <array>
#include "Definitions.h"

namespace AIGlib 
{
	class AIGLIB_API Shader
	{
	public:

		Shader(MeshProperties* meshProperties);
		~Shader();

		unsigned int getProgram();
		void use();

		void addVertexShader(const char *vshaderPath);
		void addTessellationControlShader(const char *tcshaderPath);
		void addTessellationEvalShader(const char *teshaderPath);
		void addGeometryShader(const char *gshaderPath);
		void addFragmentShader(const char *fshaderPath);
		void addComputeShader(const char *cshaderPath);

		void compileAndLinkShaders();

		std::set<std::string>& getAttributeNames();
		std::map<std::string, Material*>& getMaterials();

		int getIlluminationModelUniform();
		int getShadingModelUniform();
		std::array<int, N_MATRIX_UNIFORMS> getMatrixUniforms();
		std::array<int, N_SCALAR_UNIFORMS> getMaterialScalarUniforms();
		int getAmbientOcclusionUniform();
		int getBumpMappingUniform();
		std::array<int, N_PARALLAX_UNIFORMS> getParallaxMappingUniforms();
		std::array<int, N_LIGHTS_UNIFORMS> getLightsUniforms();
		std::array<int, N_FOG_UNIFORMS> getFogUniforms();
		//int getMeshPointLightsUniforms();
		//int getMeshDirectionalLightsUniforms();
		//int getMeshSpotLightsUniforms();
		int getCameraPositionUniform();
		int getBackgroundColorUniform();

		unsigned int getUboPointLights();
		unsigned int getUboDirectionalLights();
		unsigned int getUboSpotLights();

		void loadAmbientIntensity();
		void loadPointLights(std::vector<PointLight*> pls);
		void loadDirectionalLights(std::vector<DirectionalLight*> dls);
		void loadSpotLights(std::vector<SpotLight*> sls);
	private:

		unsigned int _program;
		std::string _shaderString;

		std::vector<unsigned int> _vertexShaders;
		std::vector<unsigned int> _tessellationControlShaders;
		std::vector<unsigned int> _tessellationEvaluationShaders;
		std::vector<unsigned int> _geometryShaders;
		std::vector<unsigned int> _fragmentShaders;
		std::vector<unsigned int> _computeShaders;

		void addGenericShaders();

		//Nombres de los atributos 
		std::set<std::string> _attributeNames;

		//Nombre de los materiales
		std::map<std::string, Material*> _materials;

		//Identificadores de las variables uniform de las matrices
		int _uProjMat;
		int _uViewMat;
		int _uModelViewMat;
		int _uModelMat;
		int _uModelViewProjMat;
		int _uNormalMat;

		//Bloques de uniformes
		unsigned int _uboPointLights;
		unsigned int _uboDirectionalLights;
		unsigned int _uboSpotLights;

		//Uniformes con el número de luces
		std::array<int, N_LIGHTS_UNIFORMS> _uLightsUniforms;

		//Uniformes para utilizar en el shader
		int _uAmbientOcclusion;
		int _uBumpMapping;
		int _uNormalMapping;
		std::array<int, N_PARALLAX_UNIFORMS> _uParallaxMapping;

		//Uniforme para la posición de la cámara
		int _uCameraPosition;

		//Uniforme para el color de fondo
		int _uBackgroundColor;

		//Uniforme para el modelo de iluminación
		int _uIlluminationModel;

		//Uniforme para el modelo de sombreado
		int _uShadingModel;

		//Uniformes de matrices
		std::array<int, N_MATRIX_UNIFORMS> _uMatrixUniforms;

		//Escalares del material
		std::array<int, N_SCALAR_UNIFORMS> _uMaterialScalars;

		//Uniformes para la niebla
		std::array<int, N_FOG_UNIFORMS> _uFogUniforms;

		std::string createShaderHeader();
		std::string createShaderDefines(MeshProperties* meshProperties);
		unsigned int loadShader(const char *filename, GLenum type);
		char* loadStringFromFile(const char *filename, unsigned int &fileLen);
		void loadAttributes();
		void loadUniforms();

		void deleteShaders();
		void freeResources();
	};
}

#endif