#ifndef __MESHPROPERTIES_H__
#define __MESHPROPERTIES_H__

#include "AIGlibMacros.h"
#include "MaterialProperties.h"
#include "GeometryProperties.h"

namespace AIGlib 
{
	class MeshProperties 
	{
	public:

    bool operator==(const MeshProperties& rhs) const
    {
      //std::cout << "DEBUG: comparing mesh properties..." << std::endl;
      return
        this->getMaterialProperties() == rhs.getMaterialProperties()
        && this->getGeometryProperties() == rhs.getGeometryProperties()
        ;
    }

    MeshProperties();
		~MeshProperties();
		/*friend bool operator==(MeshProperties& lhs, MeshProperties& rhs)
		{
			bool result = false;
			if ((lhs.getMaterialProperties() == rhs.getMaterialProperties()) &&
				(lhs.getGeometryProperties() == rhs.getGeometryProperties()))
			{
				result = true;
			}
			return result;
		}*/

		GeometryProperties* getGeometryProperties() const;
		MaterialProperties* getMaterialProperties() const;

		void setGeometryProperties(GeometryProperties* geometryProperties);
		void setMaterialProperties(MaterialProperties* materialProperties);

		int getCustomShaderId();
		void setCustomShaderId(int customShaderId);

		bool getIsFBOShader();
		void setFBOShader();

	private:

		GeometryProperties* _geometryProperties;
		MaterialProperties* _materialProperties;
		int _customShaderId;
		bool _isFBOShader;
		void freeResources();
	};
}

#endif