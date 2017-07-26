#include "FPSCamera.h"

namespace AIGlib
{
	FPSCamera::FPSCamera(ProjMatrixType projMatrixType, const glm::mat4& proj, const glm::mat4& view, 
		const unsigned int& width, const unsigned int& height, const std::string& name) :
		Camera(projMatrixType, proj, view, width, height, name),
		_fov(getFieldOfView())
	{
		
	}

	FPSCamera::FPSCamera(const unsigned int& width, const unsigned int& height, const float& near,
		const float& far, const float& fovDegrees, const glm::vec3& position, const std::string& name) :
		Camera(width, height, near, far, fovDegrees, position, name),
		_fov(getFieldOfView())
	{

	}
	FPSCamera::FPSCamera(const unsigned int& width, const unsigned int& height, const float& left, const float& right,
		const float& bottom, const float& top, const glm::vec3& position, const std::string& name) :
		Camera(width, height, left, right, bottom, top, position, name)
	{

	}

	FPSCamera::FPSCamera(const unsigned int& width, const unsigned int& height, const float& near, const float& far,
		const float& left, const float& right, const float& bottom, const float& top, const glm::vec3& position,
		const std::string& name) :
		Camera(width, height, near, far, left, right, bottom, top, position, name)
	{

	}

	FPSCamera::~FPSCamera() 
	{

	}

	void FPSCamera::updateView()
	{		
		_orientation = glm::quat(_rotation) * _orientation;
		_rotation = glm::vec3(0.0f);
		_orientation = glm::normalize(_orientation);
		glm::mat4 rotate = glm::toMat4(_orientation);
		glm::mat4 translate = glm::mat4(1.0f);
		translate = glm::translate(translate, -_position);
		_view = rotate * translate;
	}

	void FPSCamera::setTurboSpeed() 
	{
		_speed = 1;
	}

	void FPSCamera::setNormalSpeed()
	{
		_speed = 0;
	}

	void FPSCamera::mouseRightButtonPressed(int x, int y) 
	{
		_mousePosition.x = (float)x;
		_mousePosition.y = (float)y;
		_ismouseRightButtonPressed = true;
	}

	void FPSCamera::mouseRightButtonUnpressed()
	{
		_ismouseRightButtonPressed = false;
	}

	void FPSCamera::mouseRotate(int x, int y)
	{
		if (!_ismouseRightButtonPressed)
		{		
			return;
		}

		glm::vec2 mouseDisplacement = glm::vec2(x, y) - _mousePosition;

		_rotation.y = _mouseXRotationSpeed[_speed] * mouseDisplacement.x;
		_rotation.x = _mouseYRotationSpeed[_speed] * mouseDisplacement.y;

		updateView();

		_mousePosition = glm::vec2(x, y);
	}

	//Rotación
	void FPSCamera::keyboardRotate(float dx, float dy, float dz) 
	{
		_rotation.x = dx * _keyboardYRotationSpeed[_speed];
		_rotation.y = dy * _keyboardXRotationSpeed[_speed];
		_rotation.z = dz * _keyboardZRotationSpeed[_speed];

		updateView();
	}

	//Translación
	void FPSCamera::keyboardTranslate(float dx, float dy, float dz) 
	{
		glm::vec3 strafe = glm::vec3(_view[0][0], _view[1][0], _view[2][0]);
		glm::vec3 up = glm::vec3(_view[0][1], _view[1][1], _view[2][1]);
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 forward = glm::vec3(_view[0][2], _view[1][2], _view[2][2]);

		_position += (-dz * forward + dy * up + dx * strafe) * _keyboardTranslationSpeed[_speed];

		updateView();
	}

	void FPSCamera::zoomIn()
	{
		if (_fov >= _minFov && _fov <= _maxFov)
			_fov -= _zoomSpeed;
		if (_fov <= _minFov)
			_fov = _minFov;
		_proj = glm::perspective(glm::radians(_fov), (float)_width / (float)_height, getNear(), getFar());
	}

	void FPSCamera::zoomOut()
	{
		if (_fov >= _minFov && _fov <= _maxFov)
			_fov += _zoomSpeed;
		if (_fov >= _maxFov)
			_fov = _maxFov;
		_proj = glm::perspective(glm::radians(_fov), (float)_width / (float)_height, getNear(), getFar());
	}

	void FPSCamera::keyDown(unsigned char key) 
	{
		_keys[key] = true;
	}

	void FPSCamera::keyUp(unsigned char key)
	{
		_keys[key] = false;
	}

	void FPSCamera::handleKeys() 
	{
		float dx, dy, dz, rx, ry, rz;
		dx = dy = dz = rx = ry = rz = 0.0f;

		if ((_keys['w']) || (_keys['s']) || (_keys['a']) || (_keys['d']) || (_keys[' ']) || (_keys['x'])) 
		{
			if (_keys['w']) { dz = 1.0f; }
			else if (_keys['s']) { dz = -1.0f; }
			if (_keys['a']) { dx = -1.0f; }
			else if (_keys['d']) { dx = 1.0f; }
			if (_keys[' ']) { dy = 1.0f; }
			else if (_keys['x']) { dy = -1.0f; }
			keyboardTranslate(dx, dy, dz);
		}
		if ((_keys['y']) || (_keys['h']) || (_keys['g']) || (_keys['j']) || (_keys['b']) || (_keys['n'])) 
		{
			if (_keys['y']) { rx = -1.0f; }
			else if (_keys['h']) { rx = 1.0f; }
			if (_keys['g']) { ry = -1.0f; }
			else if (_keys['j']) { ry = 1.0f; }
			if (_keys['n']) { rz = 1.0f; }
			else if (_keys['b']) { rz = -1.0f; }
			keyboardRotate(rx, ry, rz);
		}
		if (_keys['q'])
		{
			setTurboSpeed();
		}
		else
		{ 
			setNormalSpeed();
		}
	}

	void FPSCamera::handleMouse(int button, int state, int x, int y) 
	{
		if (state == 0) 
		{
			if (button == 2)
			{
				mouseRightButtonPressed(x, y);
			}
			if (button == 3) 
			{
				zoomIn();
			}
			if (button == 4)
			{
				zoomOut();
			}
		}
		else 
		{
			if (button == 2) 
			{
				mouseRightButtonUnpressed();
			}
		}
	}
}