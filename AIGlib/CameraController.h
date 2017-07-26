#ifndef __CAMERACONTROLLER_H__
#define __CAMERACONTROLLER_H__

#include "AIGlibMacros.h"
#include "Enums.h"
#include "Camera.h"

namespace AIGlib
{
	class AIGLIB_API CameraController
	{
	public:
		CameraController(Camera* camera, 
			ProjMatrixType projMatrixType = ProjMatrixType::PERSPECTIVE, CameraType cameraType = CameraType::BASE);
		~CameraController();

		Camera* getCamera() const;
		ProjMatrixType getProjMatrixType() const;
		CameraType getCameraType() const;

	private:
		Camera* _camera;
		ProjMatrixType _projMatrixType;
		CameraType _cameraType;



		glm::quat _orientation; //Orientación de la cámara en coordenadas del mundo
		glm::vec3 _rotation; //Rotación temporal

		glm::vec2 _mousePosition; //Posición del ratón en coordenadas de ventana
		bool _ismouseRightButtonPressed = false;
		int _speed = 0;

		//Matriz proyección
		float _znear;
		float _zfar;
		float _aspect;

		//Campo de visión
		float _fov;
		const float _maxFov = 60.1f;
		const float _minFov = 1.0f;

		//Velocidades

		const float _keyboardTranslationSpeed[2] = { 0.10f, 0.20f };
		const float _mouseXRotationSpeed[2] = { 0.0020f, 0.0040f };
		const float _mouseYRotationSpeed[2] = { 0.0020f, 0.0040f };

		const float _keyboardXRotationSpeed[2] = { 0.0080f, 0.0160f };
		const float _keyboardYRotationSpeed[2] = { 0.0080f, 0.0160f };
		const float _keyboardZRotationSpeed[2] = { 0.0080f, 0.0160f };
		const float _zoomSpeed = 1.0f;

		bool _keys[256] = { false };

	};
}

#endif