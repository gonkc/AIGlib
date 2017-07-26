#include "MaterialProperties.h"
#include "RenderEngine.h"
#include "Material.h"

namespace AIGlib 
{
	MaterialProperties::MaterialProperties() :
		_name(std::string()),
		_illuminationModel(IlluminationModelType::PHONG),
		_shadingModel(ShadingModelType::PHONG),
		_twoSided(true),
		_wireframe(WireframeType::FILL),
		_shininess(0.0f),
		_shininessStrength(1.0f),
		_reflectivity(0.0f),
		_refractIndex(1.0f),
		_opacity(1.0f),
		_ambientSource(0),
		_diffuseSource(0),
		_specularSource(0),
		_emissiveSource(0),
		_normalSource(0),
		_lightmapSource(0),
		_heightSource(0),
		_opacitySource(0),
		_shininessSource(0),
		_displacementSource(0),
		_auxiliarSource(0),
		_ambientTexBinding(-1),
		_diffuseTexBinding(-1),
		_specularTexBinding(-1),
		_emissiveTexBinding(-1),
		_normalTexBinding(-1),
		_lightmapTexBinding(-1),
		_heightTexBinding(-1),
		_opacityTexBinding(-1),
		_shininessTexBinding(-1),
		_displacementTexBinding(-1),
		_auxiliarTexBinding(-1),
		_ambientKLocation(-1),
		_diffuseKLocation(-1),
		_specularKLocation(-1),
		_emissiveKLocation(-1),
		_opacityKLocation(-1),
		_shininessLocation(-1)
	{
	}

	MaterialProperties::~MaterialProperties()
	{
		freeResources();
	}

	//getters
	std::string MaterialProperties::getName()
	{
		return _name;
	}

	IlluminationModelType MaterialProperties::getIlluminationModel() 
	{
		return _illuminationModel;
	}

	ShadingModelType MaterialProperties::getShadingModel() 
	{
		return _shadingModel;
	}

	bool MaterialProperties::getTwoSided() 
	{
		return _twoSided;
	}

	WireframeType MaterialProperties::getWireframe() 
	{
		return _wireframe;
	}

	float MaterialProperties::getShininess()
	{
		return _shininess;
	}

	float MaterialProperties::getShininessStrength() 
	{
		return _shininessStrength;
	}

	float MaterialProperties::getReflectivity() 
	{
		return _reflectivity;
	}

	float MaterialProperties::getRefractIndex() 
	{
		return _refractIndex;
	}

	float MaterialProperties::getOpacity()
	{
		return _opacity;
	}

	std::array<float, N_SCALAR_UNIFORMS> MaterialProperties::getMaterialScalars()
	{
		return { _shininess, _shininessStrength, _reflectivity, _refractIndex, _opacity };
	}

	int MaterialProperties::getAmbientSource() const
	{
		return _ambientSource;
	}

	int MaterialProperties::getDiffuseSource() const
	{
		return _diffuseSource;
	}

	int MaterialProperties::getSpecularSource() const
	{
		return _specularSource;
	}

	int MaterialProperties::getEmissiveSource() const
	{
		return _emissiveSource;
	}

	int MaterialProperties::getOpacitySource() const
	{
		return _opacitySource;
	}

	int MaterialProperties::getShininessSource() const
	{
		return _shininessSource;
	}

	int MaterialProperties::getNormalSource() const
	{
		return _normalSource;
	}

	int MaterialProperties::getLightmapSource() const
	{
		return _lightmapSource;
	}

	int MaterialProperties::getHeightSource() const
	{
		return _heightSource;
	}

	int MaterialProperties::getDisplacementSource() const
	{
		return _displacementSource;
	}

	int MaterialProperties::getAuxiliarSource() const
	{
		return _auxiliarSource;
	}

	int MaterialProperties::getAmbientTexBinding() 
	{
		return _ambientTexBinding;
	}

	int MaterialProperties::getDiffuseTexBinding() 
	{
		return _diffuseTexBinding;
	}

	int MaterialProperties::getSpecularTexBinding() 
	{
		return _specularTexBinding;
	}

	int MaterialProperties::getEmissiveTexBinding()
	{
		return _emissiveTexBinding;
	}

	int MaterialProperties::getOpacityTexBinding() 
	{
		return _opacityTexBinding;
	}

	int MaterialProperties::getShininessTexBinding()
	{
		return _shininessTexBinding;
	}

	int MaterialProperties::getNormalTexBinding()
	{
		return _normalTexBinding;
	}

	int MaterialProperties::getLightmapTexBinding()
	{
		return _lightmapTexBinding;
	}

	int MaterialProperties::getHeightTexBinding()
	{
		return _heightTexBinding;
	}

	int MaterialProperties::getDisplacementTexBinding()
	{
		return _displacementTexBinding;
	}

	int MaterialProperties::getAuxiliarTexBinding() 
	{
		return _auxiliarTexBinding;
	}

	int MaterialProperties::getAmbientKLocation() 
	{
		return _ambientKLocation;
	}

	int MaterialProperties::getDiffuseKLocation()
	{
		return _diffuseKLocation;
	}

	int MaterialProperties::getSpecularKLocation()
	{
		return _specularKLocation;
	}

	int MaterialProperties::getEmissiveKLocation()
	{
		return _emissiveKLocation;
	}

	int MaterialProperties::getOpacityKLocation()
	{
		return _opacityKLocation;
	}

	int MaterialProperties::getShininessLocation()
	{
		return _shininessLocation;
	}

	UniformMaterial* MaterialProperties::getUniformMaterial(UniformMaterialType uniformMaterialType) 
	{
		UniformMaterial* uniformMaterial = nullptr;
		std::map<UniformMaterialType, UniformMaterial*>::iterator uniformMaterialsIterator;
		uniformMaterialsIterator = _uniformMaterials.find(uniformMaterialType);
		if (uniformMaterialsIterator != _uniformMaterials.end()) 
		{
			uniformMaterial = uniformMaterialsIterator->second;
		}
		return uniformMaterial;
	}
	
	TextureMaterial* MaterialProperties::getTextureMaterial(TextureMaterialType textureMaterialType) 
	{
		TextureMaterial* textureMaterial = nullptr;
		std::map<TextureMaterialType, TextureMaterial*>::iterator textureMaterialsIterator;
		textureMaterialsIterator = _textureMaterials.find(textureMaterialType);
		if (textureMaterialsIterator != _textureMaterials.end()) 
		{
			textureMaterial = textureMaterialsIterator->second;
		}
		return textureMaterial;
	}

	//setters
	void MaterialProperties::setName(std::string name) 
	{
		_name = name;
	}

	void MaterialProperties::setIlluminationModel(IlluminationModelType illuminationModel) 
	{
		_illuminationModel = illuminationModel;
	}

	void MaterialProperties::setShadingModel(ShadingModelType shadingModel) {
		_shadingModel = shadingModel;
	}
	void MaterialProperties::setTwoSided(bool twoSided)
	{
		_twoSided = twoSided;
	}

	void MaterialProperties::setWireframe(WireframeType wireframe)
	{
		_wireframe = wireframe;
	}

	void MaterialProperties::setShininess(float shininess) 
	{
		_shininessSource = 2;
		_shininess = shininess;
	}

	void MaterialProperties::setShininessStrength(float shininessStrength)
	{
		_shininessStrength = shininessStrength;
	}

	void MaterialProperties::setReflectivity(float reflectivity) 
	{
		_reflectivity = reflectivity;
	}

	void MaterialProperties::setRefractIndex(float refractIndex) 
	{
		_refractIndex = refractIndex;
	}

	void MaterialProperties::setOpacity(float opacity)
	{
		_opacity = opacity;
	}

	void MaterialProperties::setAmbientSource(int ambientSource) 
	{
		_ambientSource = ambientSource;
	}

	void MaterialProperties::setDiffuseSource(int diffuseSource)
	{
		_diffuseSource = diffuseSource;
	}

	void MaterialProperties::setSpecularSource(int specularSource) 
	{
		_specularSource = specularSource;
	}

	void MaterialProperties::setEmissiveSource(int emissiveSource) 
	{
		_emissiveSource = emissiveSource;
	}

	void MaterialProperties::setOpacitySource(int opacitySource)
	{
		_opacitySource = opacitySource;
	}

	void MaterialProperties::setShininessSource(int shininessSource) 
	{
		_shininessSource = shininessSource;
	}

	void MaterialProperties::setNormalSource(int normalSource)
	{
		_normalSource = normalSource;
	}

	void MaterialProperties::setLightmapSource(int lightmapSource)
	{
		_lightmapSource = lightmapSource;
	}

	void MaterialProperties::setHeightSource(int heightSource)
	{
		_heightSource = heightSource;
	}

	void MaterialProperties::setDisplacementSource(int displacementSource) 
	{
		_displacementSource = displacementSource;
	}

	void MaterialProperties::setAuxiliarSource(int auxiliarSource) 
	{
		_auxiliarSource = auxiliarSource;
	}

	void MaterialProperties::setAmbientTexBinding(int ambientTexBinding)
	{
		_ambientTexBinding = ambientTexBinding;
	}

	void MaterialProperties::setDiffuseTexBinding(int diffuseTexBinding)
	{
		_diffuseTexBinding = diffuseTexBinding;
	}

	void MaterialProperties::setSpecularTexBinding(int specularTexBinding) 
	{
		_specularTexBinding = specularTexBinding;
	}

	void MaterialProperties::setEmissiveTexBinding(int emissiveTexBinding)
	{
		_emissiveTexBinding = emissiveTexBinding;
	}

	void MaterialProperties::setOpacityTexBinding(int opacityTexBinding)
	{
		_opacityTexBinding = opacityTexBinding;
	}

	void MaterialProperties::setShininessTexBinding(int shininessTexBinding)
	{
		_shininessTexBinding = shininessTexBinding;
	}

	void MaterialProperties::setNormalTexBinding(int normalTexBinding) 
	{
		_normalTexBinding = normalTexBinding;
	}

	void MaterialProperties::setLightmapTexBinding(int lightmapTexBinding) 
	{
		_lightmapTexBinding = lightmapTexBinding;
	}

	void MaterialProperties::setHeightTexBinding(int heightTexBinding) 
	{
		_heightTexBinding = heightTexBinding;
	}

	void MaterialProperties::setDisplacementTexBinding(int displacementTexBinding) 
	{
		_displacementTexBinding = displacementTexBinding;
	}

	void MaterialProperties::setAuxiliarTexBinding(int auxiliarTexBinding)
	{
		_auxiliarTexBinding = auxiliarTexBinding;
	}

	void MaterialProperties::setAmbientKLocation(int ambientKLocation) 
	{
		_ambientKLocation = ambientKLocation;
	}

	void MaterialProperties::setDiffuseKLocation(int diffuseKLocation) 
	{
		_diffuseKLocation = diffuseKLocation;
	}

	void MaterialProperties::setSpecularKLocation(int specularKLocation)
	{
		_specularKLocation = specularKLocation;
	}

	void MaterialProperties::setEmissiveKLocation(int emissiveKLocation)
	{
		_emissiveKLocation = emissiveKLocation;
	}

	void MaterialProperties::setOpacityKLocation(int opacityKLocation)
	{
		_opacityKLocation = opacityKLocation;
	}

	void MaterialProperties::setShininessLocation(int shininessLocation)
	{
		_shininessLocation = shininessLocation;
	}

	void MaterialProperties::setAmbientK(glm::vec3 ambientK) 
	{
		//Source y location
		_ambientSource = 2;
		if (_ambientKLocation == -1) 
		{
			_ambientKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::AMBIENT);
		}

		//Añadimos o actualizamos este tipo de material
		UniformMaterial* uniformMaterial = new UniformMaterial(_ambientKLocation, ambientK);
		_uniformMaterials[UniformMaterialType::AMBIENT] = uniformMaterial;
	}

	void MaterialProperties::setDiffuseK(glm::vec3 diffuseK) 
	{
		//Source y location
		_diffuseSource = 2;
		if (_diffuseKLocation == -1)
		{
			_diffuseKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::DIFFUSE);
		}

		//Añadimos o actualizamos este tipo de material
		UniformMaterial* uniformMaterial = new UniformMaterial(_diffuseKLocation, diffuseK);
		_uniformMaterials[UniformMaterialType::DIFFUSE] = uniformMaterial;
	}

	void MaterialProperties::setAmbientKAndDiffuseK(glm::vec3 colorK) 
	{
		setAmbientK(colorK);
		setDiffuseK(colorK);
	}

	void MaterialProperties::setSpecularK(glm::vec3 specularK) 
	{
		//Source y location
		_specularSource = 2;
		if (_specularKLocation == -1) 
		{
			_specularKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::SPECULAR);
		}

		//Añadimos o actualizamos este tipo de material
		UniformMaterial* uniformMaterial = new UniformMaterial(_specularKLocation, specularK);
		_uniformMaterials[UniformMaterialType::SPECULAR] = uniformMaterial;
	}

	void MaterialProperties::setEmissiveK(glm::vec3 emissiveK)
	{
		//Source y location
		_emissiveSource = 2;
		if (_emissiveKLocation == -1) 
		{
			_emissiveKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::EMISSIVE);
		}

		//Añadimos o actualizamos este tipo de material
		UniformMaterial* uniformMaterial = new UniformMaterial(_emissiveKLocation, emissiveK);
		_uniformMaterials[UniformMaterialType::EMISSIVE] = uniformMaterial;
	}

	void MaterialProperties::setOpacityK(glm::vec3 opacityK)
	{
		//Source y location
		_opacitySource = 2;
		if (_opacityKLocation == -1)
		{
			_opacityKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::OPACITY);
		}

		//Añadimos o actualizamos este tipo de material
		UniformMaterial* uniformMaterial = new UniformMaterial(_opacityKLocation, opacityK);
		_uniformMaterials[UniformMaterialType::OPACITY] = uniformMaterial;
	}

	void MaterialProperties::setAmbientTexId(unsigned int ambientTexId)
	{
		//Source y Binding
		_ambientSource = 1;
		if (_ambientTexBinding == -1)
		{
			_ambientTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::AMBIENT);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_ambientTexBinding, ambientTexId);
		_textureMaterials[TextureMaterialType::AMBIENT] = textureMaterial;
	}

	void MaterialProperties::setDiffuseTexId(unsigned int diffuseTexId)
	{
		//Source y Binding
		_diffuseSource = 1;
		if (_diffuseTexBinding == -1) 
		{
			_diffuseTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::DIFFUSE);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_diffuseTexBinding, diffuseTexId);
		_textureMaterials[TextureMaterialType::DIFFUSE] = textureMaterial;
	}

	void MaterialProperties::setAmbientTexIdAndDiffuseTexId(unsigned int colorTexId) 
	{
		setAmbientTexId(colorTexId);
		setDiffuseTexId(colorTexId);
	}

	void MaterialProperties::setSpecularTexId(unsigned int specularTexId)
	{
		//Source y Binding
		_specularSource = 1;
		if (_specularTexBinding == -1)
		{
			_specularTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::SPECULAR);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_specularTexBinding, specularTexId);
		_textureMaterials[TextureMaterialType::SPECULAR] = textureMaterial;
	}

	void MaterialProperties::setEmissiveTexId(unsigned int emissiveTexId) 
	{
		//Source y Binding
		_emissiveSource = 1;
		if (_emissiveTexBinding == -1)
		{
			_emissiveTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::EMISSIVE);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_emissiveTexBinding, emissiveTexId);
		_textureMaterials[TextureMaterialType::EMISSIVE] = textureMaterial;
	}

	void MaterialProperties::setNormalTexId(unsigned int normalTexId) 
	{
		//Source y Binding
		_normalSource = 1;
		if (_normalTexBinding == -1) 
		{
			_normalTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::NORMAL);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_normalTexBinding, normalTexId);
		_textureMaterials[TextureMaterialType::NORMAL] = textureMaterial;
	}

	void MaterialProperties::setLightmapTexId(unsigned int lightmapTexId) 
	{
		//Source y Binding
		_lightmapSource = 1;
		if (_lightmapTexBinding == -1) 
		{
			_lightmapTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::LIGHTMAP);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_lightmapTexBinding, lightmapTexId);
		_textureMaterials[TextureMaterialType::LIGHTMAP] = textureMaterial;
	}

	void MaterialProperties::setHeightTexId(unsigned int heightTexId)
	{
		//Source y Binding
		_heightSource = 1;
		if (_heightTexBinding == -1)
		{
			_heightTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::HEIGHT);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_heightTexBinding, heightTexId);
		_textureMaterials[TextureMaterialType::HEIGHT] = textureMaterial;
	}

	void MaterialProperties::setShininessTexId(unsigned int shininessTexId) 
	{
		//Source y Binding
		_shininessSource = 1;
		if (_shininessTexBinding == -1) 
		{
			_shininessTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::SHININESS);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_shininessTexBinding, shininessTexId);
		_textureMaterials[TextureMaterialType::SHININESS] = textureMaterial;
	}

	void MaterialProperties::setOpacityTexId(unsigned int opacityTexId) 
	{
		//Source y Binding
		_opacitySource = 1;
		if (_opacityTexBinding == -1) 
		{
			_opacityTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::OPACITY);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_opacityTexBinding, opacityTexId);
		_textureMaterials[TextureMaterialType::OPACITY] = textureMaterial;
	}

	void MaterialProperties::setDisplacementTexId(unsigned int displacementTexId)
	{
		//Source y Binding
		_displacementSource = 1;
		if (_displacementTexBinding == -1)
		{
			_displacementTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::DISPLACEMENT);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_displacementTexBinding, displacementTexId);
		_textureMaterials[TextureMaterialType::DISPLACEMENT] = textureMaterial;
	}

	void MaterialProperties::setAuxiliarTexId(unsigned int auxiliarTexId) 
	{
		//Source y Binding
		_auxiliarSource = 1;
		if (_auxiliarTexBinding == -1) 
		{
			_auxiliarTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::AUXILIAR);
		}

		//Añadimos o actualizamos este tipo de material
		TextureMaterial* textureMaterial = new TextureMaterial(_auxiliarTexBinding, auxiliarTexId);
		_textureMaterials[TextureMaterialType::AUXILIAR] = textureMaterial;
	}

	void  MaterialProperties::setMaterialFromAttributes() 
	{
		//Solo sources, los bindings y locations en este caso dan igual 
		//porque el material viene dado por los atributos de los vértices
		_ambientSource = 3;
		_diffuseSource = 3;
		_specularSource = 3;
		_emissiveSource = 3;
		_opacitySource = 3;
		_shininessSource = 3;
		_normalSource = 3;
		_lightmapSource = 3;
		_heightSource = 3;
		_displacementSource = 3;
		_auxiliarSource = 3;
	}

	void MaterialProperties::mixAmbientTexAndAmbientK(MixType mixType) 
	{		
		//Source
		switch (mixType)
		{
		case MixType::MUL:
			_ambientSource = 4;
			break;
		case MixType::SUM:
			_ambientSource = 5;
			break;
		}

		//Bindings y Locations
		if (_ambientTexBinding == -1) 
		{
			_ambientTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::AMBIENT);
		}
		if (_ambientKLocation == -1) 
		{
			_ambientKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::AMBIENT);
		}
	}

	void MaterialProperties::mixDiffuseTexAndDiffuseK(MixType mixType)
	{
		//Source
		switch (mixType)
		{
		case MixType::MUL:
			_diffuseSource = 4;
			break;
		case MixType::SUM:
			_diffuseSource = 5;
			break;
		}

		//Bindings y Locations
		if (_diffuseTexBinding == -1) 
		{
			_diffuseTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::DIFFUSE);
		}
		if (_diffuseKLocation == -1) 
		{
			_diffuseKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::DIFFUSE);
		}
	}

	void MaterialProperties::mixSpecularTexAndSpecularK(MixType mixType)
	{
		//Source
		switch (mixType)
		{
		case MixType::MUL:
			_specularSource = 4;
			break;
		case MixType::SUM:
			_specularSource = 5;
			break;
		}

		//Bindings y Locations
		if (_specularTexBinding == -1) 
		{
			_specularTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::SPECULAR);
		}
		if (_specularKLocation == -1)
		{
			_specularKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::SPECULAR);
		}
	}

	void MaterialProperties::mixEmissiveTexAndEmissiveK(MixType mixType) 
	{
		//Source
		switch (mixType)
		{
		case MixType::MUL:
			_emissiveSource = 4;
			break;
		case MixType::SUM:
			_emissiveSource = 5;
			break;
		}

		//Bindings y Locations
		if (_emissiveTexBinding == -1) 
		{
			_emissiveTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::EMISSIVE);
		}
		if (_emissiveKLocation == -1) 
		{
			_emissiveKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::EMISSIVE);
		}
	}

	void MaterialProperties::mixOpacityTexAndOpacityK(MixType mixType)
	{
		//Source
		switch (mixType)
		{
		case MixType::MUL:
			_opacitySource = 4;
			break;
		case MixType::SUM:
			_opacitySource = 5;
			break;
		}

		//Bindings y Locations
		if (_opacityTexBinding == -1) 
		{
			_opacityTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::OPACITY);
		}
		if (_opacityKLocation == -1)
		{
			_opacityKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::OPACITY);
		}
	}
	void MaterialProperties::mixShininessTexAndShininess(MixType mixType) 
	{
		//Source
		switch (mixType) 
		{
		case MixType::MUL:
			_shininessSource = 4;
			break;
		case MixType::SUM:
			_shininessSource = 5;
			break;
		}

		//Bindings y Locations
		if (_shininessTexBinding == -1) 
		{
			_shininessTexBinding = RenderEngine::get().getTextureMaterialBinding(TextureMaterialType::SHININESS);
		}
		/*if (_shininessKLocation == -1) 
		{
			_shininessKLocation = RenderEngine::get().getUniformMaterialLocation(UniformMaterialType::SHININESS);
		}*/
	}

	void MaterialProperties::freeResources()
	{
		// Eliminamos los uniformes como material
		for (auto uniformMaterial : _uniformMaterials)
		{
			//uniformMaterial.first es un enum
			delete uniformMaterial.second;
		}

		// Eliminamos las texturas como material
		for (auto textureMaterial : _textureMaterials)
		{
			//textureMaterial.first es un enum
			delete textureMaterial.second;
		}
	}

}