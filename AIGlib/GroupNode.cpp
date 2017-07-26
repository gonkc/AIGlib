#include "GroupNode.h"
#include "RenderEngine.h"
#include "Camera.h"
#include <algorithm>
#include <assert.h>

namespace AIGlib 
{
	GroupNode::GroupNode(std::string name) :
	  _parent(nullptr),
	  _name(name),
	  _nodeType(NodeType::GROUP),
	  _model(glm::mat4(1.0f)),
      _orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
	  _translation(0.0f, 0.0f, 0.0f),
	  _rotation(0.0f, 0.0f, 0.0f)
	{

	}

	GroupNode::~GroupNode()
	{
		freeResources();
	}

	void GroupNode::loadUniformBlocks()
	{
		for (unsigned int i = 0; i < _nodes.size(); ++i)
		{
			_nodes.at(i)->loadUniformBlocks();
		}
	}

	void GroupNode::addChildNode(GroupNode* node) 
	{
		_nodes.push_back(node);
		node->_index = static_cast<unsigned int>(_nodes.size() - 1);
		node->_parent = this;
	}
	void GroupNode::removeChildNode(GroupNode* node) 
	{
		_nodes.erase(std::remove(_nodes.begin(), _nodes.end(), node), _nodes.end());
		delete node;
	}

	GroupNode* GroupNode::getParent() const
	{
		return _parent;
	}
	std::string GroupNode::getName() const
	{
		return _name;
	}
	unsigned int GroupNode::getIndex() const
	{
		return _index;
	}
	NodeType GroupNode::getNodeType() const
	{
		return _nodeType;
	}
	std::vector<GroupNode*> GroupNode::getChildNodes() const 
	{
		return _nodes;
	}
	GroupNode* GroupNode::getChildNode(unsigned int index) const
	{
    if (index >= _nodes.size())
    {
     std::cerr << "Error getting child node. Index don't exist."<< std::endl;
      exit(-1);
    }
		return _nodes.at(index);
	}


	void GroupNode::setName(std::string name) 
	{
		_name = name;
	}
	void GroupNode::setIndex(unsigned int index)
	{
		_index = index;
	}
	void GroupNode::setNodeType(NodeType nodeType) 
	{
		_nodeType = nodeType;
	}

	void GroupNode::translate(glm::vec3 translation)
	{
		_translation = translation;
	}
	void GroupNode::translate(float xAxis, float yAxis, float zAxis) 
	{
		_translation = glm::vec3(xAxis, yAxis, zAxis);
	}
	void GroupNode::rotate(glm::vec3 angles) 
	{
		_rotation = angles;
	}
	void GroupNode::rotate(float xAngle, float yAngle, float zAngle) 
	{
		_rotation = glm::vec3(xAngle, yAngle, zAngle);
	}
	void GroupNode::rotate(float angle, glm::vec3 axis)
	{
		_rotation = angle * axis;
	}
	void GroupNode::rotate(float angle, float xAxis, float yAxis, float zAxis)
	{
		_rotation.x = angle * xAxis;
		_rotation.y = angle * yAxis;
		_rotation.z = angle * zAxis;
	}
	void GroupNode::rotateX(float angle) 
	{
		_rotation.x = angle;
	}
	void GroupNode::rotateY(float angle)
	{
		_rotation.y = angle;
	}
	void GroupNode::rotateZ(float angle) 
	{
		_rotation.z = angle;
	}
	void GroupNode::rotateXY(float angle)
	{
		_rotation.x = angle;
		_rotation.y = angle;
	}
	void GroupNode::rotateXZ(float angle) 
	{
		_rotation.x = angle;
		_rotation.z = angle;
	}
	void GroupNode::rotateYZ(float angle)
	{
		_rotation.y = angle;
		_rotation.z = angle;
	}
	void GroupNode::rotateXYZ(float angle)
	{
		_rotation.x = angle;
		_rotation.y = angle;
		_rotation.z = angle;
	}

	void GroupNode::render(SceneState ss)
  {
    if (_parent != nullptr)
    {
      return;
    }
    draw(ss);
  }

  void printMatrix(glm::mat4 matrix)
  {
    std::cout << "|" << matrix[0][0] << "\t" << matrix[1][0] << "\t" << matrix[2][0] << "\t" << matrix[3][0] << "|" << std::endl;
    std::cout << "|" << matrix[0][1] << "\t" << matrix[1][1] << "\t" << matrix[2][1] << "\t" << matrix[3][1] << "|" << std::endl;
    std::cout << "|" << matrix[0][2] << "\t" << matrix[1][2] << "\t" << matrix[2][2] << "\t" << matrix[3][2] << "|" << std::endl;
    std::cout << "|" << matrix[0][3] << "\t" << matrix[1][3] << "\t" << matrix[2][3] << "\t" << matrix[3][3] << "|" << std::endl;
  }

  void GroupNode::draw(SceneState ss)
  {

    _model = glm::mat4(1.0f);
    _model = glm::translate(_model, _translation);
    _model = glm::rotate(_model, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    _model = glm::rotate(_model, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    _model = glm::rotate(_model, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

    ss.model *= _model;

	  for (const auto& child : _nodes)
	  {
		  child->draw(ss);
	  }
    ss.model *= glm::inverse(_model); 

  }

	std::vector<GroupNode*> GroupNode::getNodes() 
	{
		return _nodes;
	}

	GroupNode* GroupNode::getNodeByName(std::string name, bool recursive)
	{
		GroupNode* found = nullptr;
		if (_name == name)
		{
      found = this;
		}
		else
		{
			if (recursive)
			{
        for (const auto& node : _nodes)
				{
          found = node->getNodeByName(name, recursive);
					if (found != nullptr)
					{
						break;
					}
				}
			}
		}
		return found;
	};

	void GroupNode::setIlluminationModel(IlluminationModelType illuminationModel) 
	{
    for (const auto& node : _nodes)
		{
			node->setIlluminationModel(illuminationModel);
		}
	}

	void GroupNode::setShadingModel(ShadingModelType shadingModel) 
	{
    for (const auto& node : _nodes)
		{
			node->setShadingModel(shadingModel);
		}
	}

	void GroupNode::setTwoSided(bool twoSided) 
	{
    for (const auto& node : _nodes)
		{
			node->setTwoSided(twoSided);
		}
	}

	void GroupNode::setWireframe(WireframeType wireframe) 
	{
    for (const auto& node : _nodes)
		{
			node->setWireframe(wireframe);
		}
	}

	void GroupNode::enableAmbientOcclusion() 
	{
    for (const auto& node : _nodes)
		{
			node->enableAmbientOcclusion();
		}
	}

	void GroupNode::disableAmbientOcclusion() 
	{
    for (const auto& node : _nodes)
		{
			node->disableAmbientOcclusion();
		}
	}

	void GroupNode::enableBumpMapping()
	{
    for (const auto& node : _nodes)
		{
      node->enableBumpMapping();
		}
	}

	void GroupNode::disableBumpMapping() 
	{
    for (const auto& node : _nodes)
		{
			node->disableBumpMapping();
		}
	}

	void GroupNode::enableParallaxMapping(float scale, float bias)
	{
    for (const auto& node : _nodes)
		{
			node->enableParallaxMapping(scale, bias);
		}
	}

	void GroupNode::disableParallaxMapping()	
	{
    for (const auto& node : _nodes)
		{
			node->disableParallaxMapping();
		}
	}

	std::string GroupNode::getNodeTypeString(NodeType nodeType) 
	{
		std::string result;
		switch (nodeType) 
		{
		case NodeType::GROUP:
			result = "GROUP";
			break;
		case NodeType::MESH:
			result = "MESH";
			break;
		case NodeType::POINTLIGHT:
			result = "POINTLIGHT";
			break;
		case NodeType::DIRECTIONALLIGHT:
			result = "DIRECTIONALLIGHT";
			break;
		case NodeType::SPOTLIGHT:
			result = "SPOTLIGHT";
			break;
		default:
			result = "Error getting nodeType string!";
			break;
		}
		return result;
	}

	void GroupNode::printTree(std::string indent) 
	{
		std::cout << indent << _name << " (" << getNodeTypeString(_nodeType) << ") " << std::to_string(_index) << std::endl;

		indent += "  ";
    for (const auto& node : _nodes)
		{
			node->printTree(indent);
		}

	}

	void GroupNode::deactivateCameras() 
	{
    for (const auto& node : _nodes)
		{
      node->deactivateCameras();
		}
	}

	GroupNode* GroupNode::getRootNode() 
	{
		GroupNode* root = this;
		while (root->getParent() != nullptr)
		{
			root = root->getParent();
		}
		return root;
	}

	GroupNode* GroupNode::getActiveCamera()
	{
		GroupNode* camera = nullptr;
    for (const auto& node : _nodes)
		{
			camera = node->getActiveCamera();
			if (camera != nullptr) 
			{
				break;
			}
		}
		if (camera == nullptr)
		{
			std::cerr << "Error: Camera not found!" << std::endl;
			exit(-1);
		}
		return camera;
	}

	void GroupNode::resize(unsigned int width, unsigned int height) 
	{
		for (unsigned int i = 0; i < _nodes.size(); ++i) 
		{
			_nodes.at(i)->resize(width, height);
		}
	}

	void GroupNode::freeResources() 
	{
		for (auto node : _nodes)
		{
			delete node;
		}
	}

  void GroupNode::setModel(const glm::mat4& model)
  {
    _model = model;
  }

  glm::mat4 GroupNode::getModel() const
  {
    return _model;
  }

  std::vector<GroupNode*> GroupNode::getPointLights()
  {
    std::vector<GroupNode*> pointLights;
    for (const auto& node : _nodes)
    {
      if (node->getNodeType() == NodeType::POINTLIGHT)
      {
        pointLights.push_back(node);
      }
      else
      {
        std::vector<GroupNode*> nodepointLights = node->getPointLights();
        pointLights.insert(pointLights.end(), nodepointLights.begin(), nodepointLights.end());
      }
    }
    return pointLights;
  }

  std::vector<GroupNode*> GroupNode::getDirectionalLights()
  {
    std::vector<GroupNode*> directionalLights;
    for (const auto& node : _nodes)
    {
      if (node->getNodeType() == NodeType::DIRECTIONALLIGHT)
      {
        directionalLights.push_back(node);
      }
      else
      {
        std::vector<GroupNode*> nodeDirectionalLights = node->getDirectionalLights();
        directionalLights.insert(directionalLights.end(), nodeDirectionalLights.begin(), nodeDirectionalLights.end());
      }
    }
    return directionalLights;
  }

  std::vector<GroupNode*> GroupNode::getSpotLights()
  {
    std::vector<GroupNode*> spotLights;
    for (const auto& node : _nodes)
    {
      if (node->getNodeType() == NodeType::SPOTLIGHT)
      {
        spotLights.push_back(node);
      }
      else
      {
        std::vector<GroupNode*> nodeSpotLights = node->getSpotLights();
        spotLights.insert(spotLights.end(), nodeSpotLights.begin(), nodeSpotLights.end());
      }
    }
    return spotLights;
  }

}