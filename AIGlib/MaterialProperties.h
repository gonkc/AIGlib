#ifndef __MATERIALPROPERTIES_H__
#define __MATERIALPROPERTIES_H__

#include "AIGlibMacros.h"
#include "Enums.h"
#include "Definitions.h"
#include "UniformMaterial.h"
#include "TextureMaterial.h"
#include <gl/glew.h>
#include <gl/gl.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <map>

namespace AIGlib 
{
	class AIGLIB_API MaterialProperties 
	{
	public:

    bool operator==(const MaterialProperties& rhs) const
    {
      //std::cout << "DEBUG: comparing material properties..." << std::endl;
      return
        this->getAmbientSource() == rhs.getAmbientSource()
        && this->getDiffuseSource() == rhs.getDiffuseSource()
        && this->getSpecularSource() == rhs.getSpecularSource()
        && this->getEmissiveSource() == rhs.getEmissiveSource()
        && this->getOpacitySource() == rhs.getOpacitySource()
        && this->getShininessSource() == rhs.getShininessSource()
        && this->getNormalSource() == rhs.getNormalSource()
        && this->getLightmapSource() == rhs.getLightmapSource()
        && this->getHeightSource() == rhs.getHeightSource()
        && this->getDisplacementSource() == rhs.getDisplacementSource()
        && this->getAuxiliarSource() == rhs.getAuxiliarSource()
        ;
    }

		MaterialProperties();
		~MaterialProperties();
		/*friend bool operator==(MaterialProperties& lhs, MaterialProperties& rhs)
		{
			bool result = false;
			if ((lhs.getAmbientSource() == rhs.getAmbientSource()) &&
				(lhs.getDiffuseSource() == rhs.getDiffuseSource()) &&
				(lhs.getSpecularSource() == rhs.getSpecularSource()) &&
				(lhs.getEmissiveSource() == rhs.getEmissiveSource()) &&
				(lhs.getOpacitySource() == rhs.getOpacitySource()) &&
				(lhs.getShininessSource() == rhs.getShininessSource()) &&
				(lhs.getNormalSource() == rhs.getNormalSource()) &&
				(lhs.getLightmapSource() == rhs.getLightmapSource()) &&
				(lhs.getHeightSource() == rhs.getHeightSource()) &&
				(lhs.getDisplacementSource() == rhs.getDisplacementSource()) &&
				(lhs.getAuxiliarSource() == rhs.getAuxiliarSource()))
			{
				result = true;
			}
			return result;
		}*/
		std::string getName();
		IlluminationModelType getIlluminationModel();
		ShadingModelType getShadingModel();
		bool getTwoSided();
		WireframeType getWireframe();

		float getShininess();
		float getShininessStrength();
		float getReflectivity();
		float getRefractIndex();
		float getOpacity();
		std::array<float, N_SCALAR_UNIFORMS> getMaterialScalars();

		//getters
		int getAmbientSource() const;
		int getDiffuseSource() const;
		int getSpecularSource() const;
		int getEmissiveSource() const;
		int getOpacitySource() const;
		int getShininessSource() const;
		int getNormalSource() const;
		int getLightmapSource() const;
		int getHeightSource() const;
		int getDisplacementSource() const;
		int getAuxiliarSource() const;

		int getAmbientTexBinding();
		int getDiffuseTexBinding();
		int getSpecularTexBinding();
		int getEmissiveTexBinding();
		int getOpacityTexBinding();
		int getShininessTexBinding();
		//int getReflectiveTexBinding();
		int getNormalTexBinding();
		int getLightmapTexBinding();
		int getHeightTexBinding();
		int getDisplacementTexBinding();
		int getAuxiliarTexBinding();

		int getAmbientKLocation();
		int getDiffuseKLocation();
		int getSpecularKLocation();
		int getEmissiveKLocation();
		int getOpacityKLocation();
		int getShininessLocation();
		//int getReflectiveKLocation();

		UniformMaterial* getUniformMaterial(UniformMaterialType uniformMaterialType);
		TextureMaterial* getTextureMaterial(TextureMaterialType textureMaterialType);

		void setName(std::string name);
		void setIlluminationModel(IlluminationModelType illuminationModel);
		void setShadingModel(ShadingModelType shadingModel);
		void setTwoSided(bool twoSided);
		void setWireframe(WireframeType wireframe);

		void setShininess(float shininess);
		void setShininessStrength(float shininessStrength);
		void setReflectivity(float reflectivity);
		void setRefractIndex(float refractIndex);
		void setOpacity(float alpha);

		//setters
		void setAmbientSource(int ambientSource);
		void setDiffuseSource(int diffuseSource);
		void setSpecularSource(int specularSource);
		void setEmissiveSource(int emissiveSource);
		void setOpacitySource(int opacitySource);
		void setShininessSource(int shininessSource);
		//void setReflectiveSource(int reflectiveSource);
		void setNormalSource(int normalSource);
		void setLightmapSource(int lightmapSource);
		void setHeightSource(int heightSource);
		void setDisplacementSource(int displacementSource);
		void setAuxiliarSource(int auxiliarSource);

		void setAmbientTexBinding(int ambientTexBinding);
		void setDiffuseTexBinding(int diffuseTexBinding);
		void setSpecularTexBinding(int specularTexBinding);
		void setEmissiveTexBinding(int emissiveTexBinding);
		void setOpacityTexBinding(int opacityTexBinding);
		void setShininessTexBinding(int shininessTexBinding);
		//void setReflectiveTexBinding(int reflectiveTexBinding);
		void setNormalTexBinding(int normalTexBinding);
		void setLightmapTexBinding(int lightmapTexBinding);
		void setHeightTexBinding(int heightTexBinding);
		void setDisplacementTexBinding(int displacementTexBinding);
		void setAuxiliarTexBinding(int auxiliarTexBinding);

		void setAmbientKLocation(int ambientKLocation);
		void setDiffuseKLocation(int diffuseKLocation);
		void setSpecularKLocation(int specularKLocation);
		void setEmissiveKLocation(int emissiveKLocation);
		void setOpacityKLocation(int opacityKLocation);
		void setShininessLocation(int shininessLocation);
		//void setReflectiveKLocation(int reflectiveKLocation);

		void setAmbientK(glm::vec3 ambientK);
		void setDiffuseK(glm::vec3 diffuseK);
		void setAmbientKAndDiffuseK(glm::vec3 colorK);
		void setSpecularK(glm::vec3 specularK);
		void setEmissiveK(glm::vec3 emissiveK);
		void setOpacityK(glm::vec3 opacityK);
		//void setReflectiveK(glm::vec3 reflectiveK);

		void setAmbientTexId(unsigned int ambientTexId);
		void setDiffuseTexId(unsigned int diffuseTexId);
		void setAmbientTexIdAndDiffuseTexId(unsigned int colorTexId);
		void setSpecularTexId(unsigned int specularTexId);
		void setEmissiveTexId(unsigned int emissiveTexId);
		void setNormalTexId(unsigned int normalTexId);
		void setLightmapTexId(unsigned int lightmapTexId);
		void setHeightTexId(unsigned int heightTexId);
		void setShininessTexId(unsigned int shininessTexId);
		void setOpacityTexId(unsigned int opacityTexId);
		//void setReflectiveTexId(unsigned int reflectiveTexId);
		void setDisplacementTexId(unsigned int displacementTexId);
		void setAuxiliarTexId(unsigned int auxiliarTexId);

		void setMaterialFromAttributes();

		void mixAmbientTexAndAmbientK(MixType mixType);
		void mixDiffuseTexAndDiffuseK(MixType mixType);
		void mixSpecularTexAndSpecularK(MixType mixType);
		void mixEmissiveTexAndEmissiveK(MixType mixType);
		void mixOpacityTexAndOpacityK(MixType mixType);
		void mixShininessTexAndShininess(MixType mixType);
		//void mixReflectiveTexAndReflectiveK(MixType mixType);
		
	private:
		//Nombre
		std::string _name;

		//Modelo de iluminación
		IlluminationModelType _illuminationModel;

		//Modelo de sombreado
		ShadingModelType _shadingModel;
		
		//Doble cara
		bool _twoSided;
		
		//Modo de dibujo, point, line o fill
		WireframeType _wireframe;

		// Uniformes como material
		std::map<UniformMaterialType, UniformMaterial*> _uniformMaterials;

		// Texturas como material
		std::map<TextureMaterialType, TextureMaterial*> _textureMaterials;

		// Propiedades escalares del material
		float _shininess;
		float _shininessStrength;
		float _reflectivity;
		float _refractIndex;
		float _opacity;

		int _ambientSource;
		int _diffuseSource;
		int _specularSource;
		int _emissiveSource;
		int _opacitySource;
		int _shininessSource;
		//int _reflectiveSource;
		int _normalSource;
		int _lightmapSource;
		int _heightSource;
		int _displacementSource;
		int _auxiliarSource;

		int _ambientTexBinding;
		int _diffuseTexBinding;
		int _specularTexBinding;
		int _emissiveTexBinding;
		int _opacityTexBinding;
		int _shininessTexBinding;
		//int _reflectiveTexBinding;
		int _normalTexBinding;
		int _lightmapTexBinding;
		int _heightTexBinding;
		int _displacementTexBinding;
		int _auxiliarTexBinding;

		int _ambientKLocation;
		int _diffuseKLocation;
		int _specularKLocation;
		int _emissiveKLocation;
		int _opacityKLocation;
		int _shininessLocation;
		//int _reflectiveKLocation;

		void freeResources();
	};
}

#endif