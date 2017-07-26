#include "MeshProperties.h"

namespace AIGlib 
{
	MeshProperties::MeshProperties() :
		_geometryProperties(nullptr),
		_materialProperties(nullptr),
		_customShaderId(-1),
		_isFBOShader(false)
	{

	}	

	MeshProperties::~MeshProperties()
	{
		freeResources();
	}

	GeometryProperties* MeshProperties::getGeometryProperties() const
	{
		return _geometryProperties;
	}
	MaterialProperties* MeshProperties::getMaterialProperties() const
	{
		return _materialProperties;
	}

	void MeshProperties::setGeometryProperties(GeometryProperties* geometryProperties) 
	{
		_geometryProperties = geometryProperties;
	}

	void MeshProperties::setMaterialProperties(MaterialProperties* materialProperties)
	{
		_materialProperties = materialProperties;
	}

	int MeshProperties::getCustomShaderId()
	{
		return _customShaderId;
	}

	void MeshProperties::setCustomShaderId(int customShaderId) 
	{
		_customShaderId = customShaderId;
	}

	bool MeshProperties::getIsFBOShader()
	{
		return _isFBOShader;
	}

	void MeshProperties::setFBOShader()
	{
		_isFBOShader = true;
	}

	void MeshProperties::freeResources()
	{
		//Eliminamos la geometría
		delete _geometryProperties;

		//Eliminamos las propiedades del material
		delete _materialProperties;
	}
}