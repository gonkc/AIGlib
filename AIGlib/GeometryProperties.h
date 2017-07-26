#ifndef __GEOMETRYPROPERTIES_H__
#define __GEOMETRYPROPERTIES_H__

#include "AIGlibMacros.h"
#include "Enums.h"
#include <gl/glew.h>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

namespace AIGlib 
{
	class AIGLIB_API GeometryProperties
	{

	public:

    bool operator==(const GeometryProperties& rhs) const
    {
      //std::cout << "DEBUG: comparing geometries..." << std::endl;
      return
        this->getVertexPos() == rhs.getVertexPos()
        && this->getVertexColor() == rhs.getVertexColor()
        && this->getVertexNormal() == rhs.getVertexNormal()
        && this->getVertexTexCoord() == rhs.getVertexTexCoord()
        && this->getVertexTangent() == rhs.getVertexTangent()
        && this->getVertexBinormal() == rhs.getVertexBinormal()
        ;
    }

    GeometryProperties(unsigned int nVertex = invalidValue);
		~GeometryProperties();
    void uploadGeometry();

    void addVertexPosAttribute(const std::vector<glm::vec3> vertexData);
    void addVertexColorAttribute(const std::vector<glm::vec4> vertexData);
    void addVertexNormalAttribute(const std::vector<glm::vec3> vertexData);
    void addVertexTexCoordAttribute(const std::vector<glm::vec2> vertexData);
    void addVertexTangentAttribute(const std::vector<glm::vec3> vertexData);
    void addVertexBinormalAttribute(const std::vector<glm::vec3> vertexData);
		void addIndicesList(unsigned int nIndices, const std::vector<unsigned int>& indices);

		void render();
		bool isInit();

		int getPosLocation() const;
		int getColorLocation() const;
		int getNormalLocation() const;
		int getTexCoordLocation() const;
		int getTangentLocation() const;
		int getBinormalLocation() const;

    std::vector<glm::vec3> getVertexPos() const;
    std::vector<glm::vec4> getVertexColor() const;
    std::vector<glm::vec3> getVertexNormal() const;
    std::vector<glm::vec2> getVertexTexCoord() const;
    std::vector<glm::vec3> getVertexTangent() const;
    std::vector<glm::vec3> getVertexBinormal() const;

    glm::vec3 getVertexPosCenter();

	private:
		unsigned int _vao;//Vertex array object
		std::vector<unsigned int> _vbos;//Vertex buffer object
		std::vector<unsigned int> _ibos;//Index buffer object
		unsigned int _nVertex;
    std::vector<glm::vec3> _vertexPos;
    std::vector<glm::vec4> _vertexColor;
    std::vector<glm::vec3> _vertexNormal;
    std::vector<glm::vec2> _vertexTexCoord;
    std::vector<glm::vec3> _vertexTangent;
    std::vector<glm::vec3> _vertexBinormal;
		std::vector<unsigned int> _nFaces;
    unsigned int _nIndices;
    std::vector<unsigned int> _indices;

		int _posLocation;
		int _colorLocation;
		int _normalLocation;
		int _texCoordLocation;
		int _tangentLocation;
		int _binormalLocation;
    void addVertexAttribute(AttributeType attributeType, std::vector<glm::vec2> vertexData);
    void addVertexAttribute(AttributeType attributeType, std::vector<glm::vec3> vertexData);
    void addVertexAttribute(AttributeType attributeType, std::vector<glm::vec4> vertexData);
    void addIndices();
		bool isLocationUsed(int location);
		void freeResources();
	};



}

#endif
