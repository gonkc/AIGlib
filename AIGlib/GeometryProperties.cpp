#include "GeometryProperties.h"
#include "RenderEngine.h"
#include <algorithm>

namespace AIGlib {

  GeometryProperties::GeometryProperties(unsigned int nVertex) :
		_vao(invalidValue),
		_nVertex(nVertex),
		_posLocation(-1),
		_colorLocation(-1),
		_normalLocation(-1),
		_texCoordLocation(-1),
		_tangentLocation(-1),
		_binormalLocation(-1)
	{
		//Creamos el VAO
		//glGenVertexArrays(1, &_vao);
		//glBindVertexArray(_vao);
		//glBindVertexArray(0);
	}

  GeometryProperties::~GeometryProperties()
  {
	}

  void GeometryProperties::uploadGeometry()
  {
    //Creamos el VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindVertexArray(0);

    if (!_vertexPos.empty())
    {
      addVertexAttribute(AttributeType::POS, _vertexPos);
    }

    if (!_vertexColor.empty())
    {
      addVertexAttribute(AttributeType::COLOR, _vertexColor);
    }

    if (!_vertexNormal.empty())
    {
      addVertexAttribute(AttributeType::NORMAL, _vertexNormal);
    }

    if (!_vertexTexCoord.empty())
    {
      addVertexAttribute(AttributeType::TEXCOORD, _vertexTexCoord);
    }

    if (!_vertexTangent.empty())
    {
      addVertexAttribute(AttributeType::TANGENT, _vertexTangent);
    }

    if (!_vertexBinormal.empty())
    {
      addVertexAttribute(AttributeType::BINORMAL, _vertexBinormal);
    }

    if (!_indices.empty())
    {
      addIndices();
    }
  }

  void GeometryProperties::addVertexPosAttribute(const std::vector<glm::vec3> vertexData)
  {
    _vertexPos = vertexData;
    //addVertexAttribute(AttributeType::POS, _vertexPos);
  }

  void GeometryProperties::addVertexColorAttribute(const std::vector<glm::vec4> vertexData)
  {
    _vertexColor = vertexData;
    //addVertexAttribute(AttributeType::COLOR, _vertexColor);
  }

  void GeometryProperties::addVertexNormalAttribute(const std::vector<glm::vec3> vertexData)
  {
    _vertexNormal = vertexData;
    //addVertexAttribute(AttributeType::NORMAL, _vertexNormal);
  }

  void GeometryProperties::addVertexTexCoordAttribute(const std::vector<glm::vec2> vertexData)
  {
    _vertexTexCoord = vertexData;
    //addVertexAttribute(AttributeType::TEXCOORD, _vertexTexCoord);
  }

  void GeometryProperties::addVertexTangentAttribute(const std::vector<glm::vec3> vertexData)
  {
    _vertexTangent = vertexData;
    //addVertexAttribute(AttributeType::TANGENT, _vertexTangent);
  }

  void GeometryProperties::addVertexBinormalAttribute(const std::vector<glm::vec3> vertexData)
  {
    _vertexBinormal = vertexData;
    //addVertexAttribute(AttributeType::BINORMAL, _vertexBinormal);
  }

	void GeometryProperties::addIndicesList(unsigned int nFaces, const std::vector<unsigned int>& indices)
	{
    _nIndices = static_cast<unsigned int>(indices.size() / nFaces);
    _indices = indices;
		_nFaces.push_back(nFaces);

    //addIndices();
	}

  void GeometryProperties::addIndices()
  {
    _ibos.resize(_ibos.size() + 1);

    glBindVertexArray(_vao);

    glGenBuffers(1, &_ibos[_ibos.size() - 1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibos[_ibos.size() - 1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
  }

	void GeometryProperties::render()
	{
		glBindVertexArray(_vao);
    unsigned int ibosSize = static_cast<unsigned int>(_ibos.size());

		if (ibosSize != 0)
		{
			//Indexed
			for (unsigned int i = 0; i < ibosSize; ++i)
			{
				glEnable(_ibos[i]);
				glDrawElements(GL_TRIANGLES, _nFaces[i] * _nIndices, GL_UNSIGNED_INT, (void*)0);
			}
		}
		else 
		{
			//Non indexed
			//for (int i = 0; i < _indices.size(); ++i) {
				//glEnable(_indices[i]);
				glDrawArrays(GL_TRIANGLES, 0, _nVertex);
			//}
		}

		glBindVertexArray(0);

	}
  void GeometryProperties::addVertexAttribute(AttributeType attributeType, std::vector<glm::vec2> vertexData)
  {
    Attribute* attribute = nullptr;

    //Comprobamos que el atributo esté registrado
    for (const auto& iterator : RenderEngine::get().getAttributes())
    {
      if (iterator.second->getAttributeType() == attributeType)
      {
        attribute = iterator.second;
        break;
      }
    }
    if (attribute == nullptr)
    {
      std::string attributeName;
      switch (attribute->getAttributeType()) {
      case AttributeType::POS:
        attributeName = "POS";
        break;
      case AttributeType::COLOR:
        attributeName = "COLOR";
        break;
      case AttributeType::NORMAL:
        attributeName = "NORMAL";
        break;
      case AttributeType::TEXCOORD:
        attributeName = "TEXCOORD";
        break;
      case AttributeType::TANGENT:
        attributeName = "TANGENT";
        break;
      case AttributeType::BINORMAL:
        attributeName = "BINORMAL";
        break;
      }
      std::cout << "Error - '" << attributeName.c_str() << "' attribute type is not registered." << std::endl;
    }
    else {

      if (_nVertex == invalidValue) {
        std::cout << "Error - 'nVertex value is -1." << std::endl;
        exit(-1);
      }

      //Comprobamos que la ubicación no esté cogida ya
      if (isLocationUsed(attribute->getLocation())) {
        std::cout << "Error - Attribute location is already used ." << std::endl;
        exit(-1);
      }

      //Asignamos la posición
      switch (attribute->getAttributeType()) {
      case AttributeType::POS:
        _posLocation = attribute->getLocation();
        break;
      case AttributeType::COLOR:
        _colorLocation = attribute->getLocation();
        break;
      case AttributeType::NORMAL:
        _normalLocation = attribute->getLocation();
        break;
      case AttributeType::TEXCOORD:
        _texCoordLocation = attribute->getLocation();
        break;
      case AttributeType::TANGENT:
        _tangentLocation = attribute->getLocation();
        break;
      case AttributeType::BINORMAL:
        _binormalLocation = attribute->getLocation();
        break;
      }

      //Ampliamos el vector de vbos para añadir el nuevo
      _vbos.resize(_vbos.size() + 1);

      //Enlazamos el vao
      glBindVertexArray(_vao);

      //Creamos el vbo
      glGenBuffers(1, &_vbos[_vbos.size() - 1]);
      glBindBuffer(GL_ARRAY_BUFFER, _vbos[_vbos.size() - 1]);
      glBufferData(GL_ARRAY_BUFFER, _nVertex * sizeof(float) * attribute->getSize(), &vertexData[0], GL_STATIC_DRAW);
      glVertexAttribPointer(attribute->getLocation(), attribute->getSize(), attribute->getType(), attribute->getNormalized(), 0, 0);
      glEnableVertexAttribArray(attribute->getLocation());

      //Desenlazamos el vao
      glBindVertexArray(0);
    }
  }
  void GeometryProperties::addVertexAttribute(AttributeType attributeType, std::vector<glm::vec3> vertexData)
  {
    Attribute* attribute = nullptr;

    //Comprobamos que el atributo esté registrado
    for (const auto& iterator : RenderEngine::get().getAttributes())
    {
      if (iterator.second->getAttributeType() == attributeType)
      {
        attribute = iterator.second;
        break;
      }
    }
    if (attribute == nullptr)
    {
      std::string attributeName;
      switch (attribute->getAttributeType()) {
      case AttributeType::POS:
        attributeName = "POS";
        break;
      case AttributeType::COLOR:
        attributeName = "COLOR";
        break;
      case AttributeType::NORMAL:
        attributeName = "NORMAL";
        break;
      case AttributeType::TEXCOORD:
        attributeName = "TEXCOORD";
        break;
      case AttributeType::TANGENT:
        attributeName = "TANGENT";
        break;
      case AttributeType::BINORMAL:
        attributeName = "BINORMAL";
        break;
      }
      std::cout << "Error - '" << attributeName.c_str() << "' attribute type is not registered." << std::endl;
    }
    else {

      if (_nVertex == invalidValue) {
        std::cout << "Error - 'nVertex value is -1." << std::endl;
        exit(-1);
      }

      //Comprobamos que la ubicación no esté cogida ya
      if (isLocationUsed(attribute->getLocation())) {
        std::cout << "Error - Attribute location is already used ." << std::endl;
        exit(-1);
      }

      //Asignamos la posición
      switch (attribute->getAttributeType()) {
      case AttributeType::POS:
        _posLocation = attribute->getLocation();
        break;
      case AttributeType::COLOR:
        _colorLocation = attribute->getLocation();
        break;
      case AttributeType::NORMAL:
        _normalLocation = attribute->getLocation();
        break;
      case AttributeType::TEXCOORD:
        _texCoordLocation = attribute->getLocation();
        break;
      case AttributeType::TANGENT:
        _tangentLocation = attribute->getLocation();
        break;
      case AttributeType::BINORMAL:
        _binormalLocation = attribute->getLocation();
        break;
      }

      //Ampliamos el vector de vbos para añadir el nuevo
      _vbos.resize(_vbos.size() + 1);

      //Enlazamos el vao
      glBindVertexArray(_vao);

      //Creamos el vbo
      glGenBuffers(1, &_vbos[_vbos.size() - 1]);
      glBindBuffer(GL_ARRAY_BUFFER, _vbos[_vbos.size() - 1]);
      glBufferData(GL_ARRAY_BUFFER, _nVertex * sizeof(float) * attribute->getSize(), &vertexData[0], GL_STATIC_DRAW);
      glVertexAttribPointer(attribute->getLocation(), attribute->getSize(), attribute->getType(), attribute->getNormalized(), 0, 0);
      glEnableVertexAttribArray(attribute->getLocation());

      //Desenlazamos el vao
      glBindVertexArray(0);
    }
  }

  void GeometryProperties::addVertexAttribute(AttributeType attributeType, std::vector<glm::vec4> vertexData)
  {
    Attribute* attribute = nullptr;

    //Comprobamos que el atributo esté registrado
    for (const auto& iterator : RenderEngine::get().getAttributes())
    {
      if (iterator.second->getAttributeType() == attributeType)
      {
        attribute = iterator.second;
        break;
      }
    }
    if (attribute == nullptr)
    {
      std::string attributeName;
      switch (attribute->getAttributeType()) {
      case AttributeType::POS:
        attributeName = "POS";
        break;
      case AttributeType::COLOR:
        attributeName = "COLOR";
        break;
      case AttributeType::NORMAL:
        attributeName = "NORMAL";
        break;
      case AttributeType::TEXCOORD:
        attributeName = "TEXCOORD";
        break;
      case AttributeType::TANGENT:
        attributeName = "TANGENT";
        break;
      case AttributeType::BINORMAL:
        attributeName = "BINORMAL";
        break;
      }
      std::cerr << "Error - '" << attributeName.c_str() << "' attribute type is not registered." << std::endl;
      exit(-1);
    }
    else {

      if (_nVertex == invalidValue) {
        std::cerr << "Error - 'nVertex' value is invalid (" <<  _nVertex << ")." << std::endl;
        exit(-1);
      }

      //Comprobamos que la ubicación no esté cogida ya
      if (isLocationUsed(attribute->getLocation())) {
        std::cerr << "Error - Attribute location is already used ." << std::endl;
        exit(-1);
      }

      //Asignamos la posición
      switch (attribute->getAttributeType()) {
      case AttributeType::POS:
        _posLocation = attribute->getLocation();
        break;
      case AttributeType::COLOR:
        _colorLocation = attribute->getLocation();
        break;
      case AttributeType::NORMAL:
        _normalLocation = attribute->getLocation();
        break;
      case AttributeType::TEXCOORD:
        _texCoordLocation = attribute->getLocation();
        break;
      case AttributeType::TANGENT:
        _tangentLocation = attribute->getLocation();
        break;
      case AttributeType::BINORMAL:
        _binormalLocation = attribute->getLocation();
        break;
      }

      //Ampliamos el vector de vbos para añadir el nuevo
      _vbos.resize(_vbos.size() + 1);

      //Enlazamos el vao
      glBindVertexArray(_vao);

      //Creamos el vbo
      glGenBuffers(1, &_vbos[_vbos.size() - 1]);
      glBindBuffer(GL_ARRAY_BUFFER, _vbos[_vbos.size() - 1]);
      glBufferData(GL_ARRAY_BUFFER, _nVertex * sizeof(float) * attribute->getSize(), &vertexData[0], GL_STATIC_DRAW);
      glVertexAttribPointer(attribute->getLocation(), attribute->getSize(), attribute->getType(), attribute->getNormalized(), 0, 0);
      glEnableVertexAttribArray(attribute->getLocation());

      //Desenlazamos el vao
      glBindVertexArray(0);
    }
  }

	bool GeometryProperties::isInit() 
	{
		return (_vao != invalidValue);
	}

	int GeometryProperties::getPosLocation() const
	{
		return _posLocation;
	}

	int GeometryProperties::getColorLocation() const
	{
		return _colorLocation;
	}

	int GeometryProperties::getNormalLocation() const
	{
		return _normalLocation;
	}

	int GeometryProperties::getTexCoordLocation()  const
	{
		return _texCoordLocation;
	}

	int GeometryProperties::getTangentLocation()  const
	{
		return _tangentLocation;
	}

	int GeometryProperties::getBinormalLocation() const
	{
		return _binormalLocation;
	}

  std::vector<glm::vec3> GeometryProperties::getVertexPos() const
  {
    return _vertexPos;
  }

  std::vector<glm::vec4> GeometryProperties::getVertexColor() const
  {
    return _vertexColor;
  }

  std::vector<glm::vec3> GeometryProperties::getVertexNormal() const
  {
    return _vertexNormal;
  }

  std::vector<glm::vec2> GeometryProperties::getVertexTexCoord() const
  {
    return _vertexTexCoord;
  }

  std::vector<glm::vec3> GeometryProperties::getVertexTangent() const
  {
    return _vertexTangent;
  }

  std::vector<glm::vec3> GeometryProperties::getVertexBinormal() const
  {
    return _vertexBinormal;
  }


	bool GeometryProperties::isLocationUsed(int location)
	{
		return ((location == _posLocation) || (location == _colorLocation) || (location == _normalLocation) ||
			(location == _texCoordLocation) || (location == _tangentLocation) || (location == _binormalLocation));
	}

	void GeometryProperties::freeResources() 
	{
		//Desenlazamos los buffer de los atributos y de los índices
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Eliminamos los atributos de los vértices
		for (unsigned int i = 0; i < _vbos.size(); ++i) 
		{
			glDeleteBuffers(1, &_vbos[i]);
		}

		//Eliminamos las listas de índices
		for (unsigned int i = 0; i < _ibos.size(); ++i) 
		{
			glDeleteBuffers(1, &_ibos[i]);
		}

		//Desenlazamos el array de vértices
		glBindVertexArray(0);

		//Eliminamos el array de vértices
		glDeleteVertexArrays(1, &_vao);
	}

  glm::vec3 GeometryProperties::getVertexPosCenter()
  {
    glm::vec3 vertexPosCenter(0.0f);

    float count = 0.0f;
    for (const auto& vertexPos : _vertexPos)
    {
      vertexPosCenter += vertexPos;
      count+=1.0f;
    }
    vertexPosCenter /= count;
    return vertexPosCenter;
  }

  /*bool operator==(const GeometryProperties& lhs, const GeometryProperties& rhs)
  {
    std::cout << "comparing..." << std::endl;
    bool result = false;
    if ((lhs.getPosLocation() == rhs.getPosLocation()) &&
      (lhs.getColorLocation() == rhs.getColorLocation()) &&
      (lhs.getNormalLocation() == rhs.getNormalLocation()) &&
      (lhs.getTexCoordLocation() == rhs.getTexCoordLocation()) &&
      (lhs.getTangentLocation() == rhs.getTangentLocation()) &&
      (lhs.getBinormalLocation() == rhs.getBinormalLocation()) &&
      (lhs.getVertexPos() == rhs.getVertexPos()) &&
      (lhs.getVertexColor() == rhs.getVertexColor()) &&
      (lhs.getVertexNormal() == rhs.getVertexNormal()) &&
      (lhs.getVertexTexCoord() == rhs.getVertexTexCoord()) &&
      (lhs.getVertexTangent() == rhs.getVertexTangent()) &&
      (lhs.getVertexBinormal() == rhs.getVertexBinormal())
      )
    {
      result = true;
    }
    return result;
  }*/

  /*bool GeometryProperties::operator==(const GeometryProperties& other)
  {
    std::cout << "comparing1..." << std::endl;
    bool result = false;
    if ((this->getPosLocation() == other.getPosLocation()) &&
      (this->getColorLocation() == other.getColorLocation()) &&
      (this->getNormalLocation() == other.getNormalLocation()) &&
      (this->getTexCoordLocation() == other.getTexCoordLocation()) &&
      (this->getTangentLocation() == other.getTangentLocation()) &&
      (this->getBinormalLocation() == other.getBinormalLocation()) &&
      (this->getVertexPos() == other.getVertexPos()) &&
      (this->getVertexColor() == other.getVertexColor()) &&
      (this->getVertexNormal() == other.getVertexNormal()) &&
      (this->getVertexTexCoord() == other.getVertexTexCoord()) &&
      (this->getVertexTangent() == other.getVertexTangent()) &&
      (this->getVertexBinormal() == other.getVertexBinormal())
      )
    {
      result = true;
    }
    return result;
  }*/

  /*bool operator==(const GeometryProperties& lhs, const GeometryProperties& rhs)
  {
    std::cout << "entro" << std::endl;
    return true;
  }*/
}