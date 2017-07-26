#include "Camera.h"

namespace AIGlib
{

	Camera::Camera(ProjMatrixType projMatrixType, const glm::mat4& proj, const glm::mat4& view, const unsigned int& width,
		const unsigned int& height, const std::string& name) :
		_projMatrixType(ProjMatrixType::PERSPECTIVE),
		_proj(proj),
		_view(view),
		_width(width),
		_height(height),
		_position(-glm::vec3(view[3].xyz))
	{
		setName(name);
		setNodeType(NodeType::CAMERA);
		activateCamera();
	}

	//Default perspective constructor
	Camera::Camera(const unsigned int& width, const unsigned int& height, const float& near,
		const float& far, const float& fovDegrees, const glm::vec3& position, const std::string& name) :
		_projMatrixType(ProjMatrixType::PERSPECTIVE),
		_width(width),
		_height(height),
		_position(position)
	{
		_proj = glm::perspective(glm::radians(fovDegrees), (float)width / (float)height, near, far);

		_view = glm::mat4(1.0f);
		_view[3].xyz = -position;

		setName(name);
		setNodeType(NodeType::CAMERA);
		activateCamera();
	}

	//Default orthographic constructors
	Camera::Camera(const unsigned int& width, const unsigned int& height, const float& left, const float& right,
		const float& bottom, const float& top, const glm::vec3& position, const std::string& name) :
		_projMatrixType(ProjMatrixType::ORTHOGRAPHIC),
		_width(width),
		_height(height),
		_position(position)
	{
		_proj = glm::ortho(left, right, bottom, top);
		_view = glm::mat4(1.0f);
		_view[3].xyz = -position;

		setName(name);
		setNodeType(NodeType::CAMERA);
		activateCamera();
	}

	Camera::Camera(const unsigned int& width, const unsigned int& height, const float& near, const float& far,
		const float& left, const float& right, const float& bottom, const float& top, const glm::vec3& position, 
		const std::string& name) :
		_projMatrixType(ProjMatrixType::ORTHOGRAPHIC),
		_width(width),
		_height(height),
		_position(position)
	{
		_proj = glm::ortho(left, right, bottom, top, near, far);
		_view = glm::mat4(1.0f);
		_view[3].xyz = -position;


		setName(name);
		setNodeType(NodeType::CAMERA);
		activateCamera();
	}

	Camera::~Camera()
	{

	}

	glm::mat4 Camera::getProj() const 
	{
		return _proj;
	}

	glm::mat4 Camera::getView() const 
	{
		return _view;
	}

	void Camera::setProj(const glm::mat4& proj) 
	{
		_proj = proj;
	}

	void Camera::setView(const glm::mat4& view)
	{
		_view = view;
	}

	void Camera::setWidth(unsigned int width)
	{
		_width = width;
	}

	void Camera::setHeight(unsigned int height) 
	{
		_height = height;
	}

	void Camera::setViewport(const unsigned int& x, const unsigned int& y, 
		const unsigned int& width, const unsigned int& height)
	{
		glViewport(x, y, width, height);		
	}

	glm::vec3 Camera::getLookAt() const 
	{
		return glm::vec3(_view[0][2], _view[1][2], _view[2][2]);
	}

	glm::vec3 Camera::getUpVector() const 
	{
		return glm::vec3(_view[0][1], _view[1][1], _view[2][1]);
	}

	unsigned int Camera::getWidth() const 
	{
		return _width;
	}

	unsigned int Camera::getHeight() const 
	{
		return _height;
	}

	unsigned int Camera::getViewportX() const
	{
		return _xViewport;
	}

	unsigned int Camera::getViewportY() const 
	{
		return _yViewport;
	}

	float Camera::getNear() const
	{
		float near = 0.0f;
		switch (_projMatrixType) 
		{
		case ProjMatrixType::PERSPECTIVE:
			near = _proj[3][2] / (_proj[2][2] - 1.0f);
			break;
		case ProjMatrixType::ORTHOGRAPHIC:
			near = (1.0f + _proj[3][2]) / _proj[2][2];
			break;
		}
		return near;
	}

	float Camera::getFar() const 
	{
		float far = 0.0f;
		switch (_projMatrixType)
		{
		case ProjMatrixType::PERSPECTIVE:
			far = _proj[3][2] / (_proj[2][2] + 1.0f);
			break;
		case ProjMatrixType::ORTHOGRAPHIC:
			far = -(1.0f - _proj[3][2]) / _proj[2][2];
			break;
		}
		return far;
	}

	float Camera::getBottom() const
	{
		float bottom = 0.0f;
		float near;
		switch (_projMatrixType) 
		{
		case ProjMatrixType::PERSPECTIVE:
			near = getNear();
			bottom = near * (_proj[2][1] - 1.0f) / _proj[1][1];
			break;
		case ProjMatrixType::ORTHOGRAPHIC:
			bottom = (1.0f - _proj[3][1]) / _proj[1][1];
			break;
		}
		return bottom;
	}
	float Camera::getTop() const 
	{
		float top = 0.0f;
		float near = 0.0f;
		switch (_projMatrixType) 
		{
		case ProjMatrixType::PERSPECTIVE:
			near = getNear();
			top = near * (_proj[2][1] + 1.0f) / _proj[1][1];
			break;
		case ProjMatrixType::ORTHOGRAPHIC:
			top = -(1.0f + _proj[3][1]) / _proj[1][1];
			break;
		}
		return top;
	}
	float Camera::getLeft() const 
	{
		float left = 0.0f;
		float near = 0.0f;
		switch (_projMatrixType) 
		{
		case ProjMatrixType::PERSPECTIVE:
			near = getNear();
			left = near * (_proj[2][0] - 1.0f) / _proj[0][0];
			break;
		case ProjMatrixType::ORTHOGRAPHIC:
			left = -(1.0f + _proj[3][0]) / _proj[0][0];
			break;
		}
		return left;
	}
	float Camera::getRight() const
	{
		float right = 0.0f;
		float near = 0.0f;
		switch (_projMatrixType)
		{
		case ProjMatrixType::PERSPECTIVE:
			near = getNear();
			right = near * (_proj[2][0] + 1.0f) / _proj[0][0];
			break;
		case ProjMatrixType::ORTHOGRAPHIC:
			right = (1.0f - _proj[3][0]) / _proj[0][0];
			break;
		}
		return right;
	}


	float Camera::getFieldOfView() const
	{
		float result = -1.0f; //invalid for ortho
		switch (_projMatrixType)
		{
			case ProjMatrixType::PERSPECTIVE :
				result = glm::degrees(atan(1.0f / _proj[1][1]) * 2.0f);
				break;
		}
		return result;
	}

	glm::vec3 Camera::getPosition() const
	{
		return glm::vec3(_view[3].xyz);
	}

	void Camera::resize(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;

		switch (_projMatrixType)
		{
			case ProjMatrixType::PERSPECTIVE:
				_proj = glm::perspective(glm::radians(getFieldOfView()), 
					(float)_width / (float)_height, getNear(), getFar());
				break;
			case ProjMatrixType::ORTHOGRAPHIC:
				float right = _width * 0.5f;
				float top = _height * 0.5f;
				_proj[0][0] = 1.0f / right;
				_proj[1][1] = 1.0f / top;
				break;
		}
	}

	Camera* Camera::getActiveCamera()
	{
		return _active ? this : nullptr;
	}

	bool Camera::getIfActiveCamera()
	{
		return _active;
	}

	void Camera::activateCamera()
	{		
		//Desactivamos todas las cámaras de la escena
		GroupNode* root = getRootNode();
		root->deactivateCameras();

		//Y activamos la actual
		_active = true;
	}

	void Camera::deactivateCameras()
	{
		_active = false;
	}

	void Camera::draw(SceneState ss)
	{
		ss.proj = _proj;
		ss.view = _view * glm::inverse(ss.model);
	}
}