#ifndef __FPSCAMERA_H__
#define __FPSCAMERA_H__

#include "AIGlibMacros.h"
#include "Camera.h"

namespace AIGlib
{
	class AIGLIB_API FPSCamera : public Camera
	{
	public:

		FPSCamera(ProjMatrixType projMatrixType, const glm::mat4& proj, const glm::mat4& view, const unsigned int& width,
			const unsigned int& height, const std::string& name = std::string());
		FPSCamera(const unsigned int& width, const unsigned int& height, const float& near,
			const float& far, const float& fovDegrees, const glm::vec3& position, const std::string& name = std::string());
		FPSCamera(const unsigned int& width, const unsigned int& height, const float& left, const float& right,
			const float& bottom, const float& top, const glm::vec3& position, const std::string& name = std::string());
		FPSCamera(const unsigned int& width, const unsigned int& height, const float& near, const float& far,
			const float& left, const float& right, const float& bottom, const float& top, const glm::vec3& position,
			const std::string& name = std::string());

		~FPSCamera();

		void setTurboSpeed();
		void setNormalSpeed();

		void mouseRightButtonPressed(int x, int y); //Se pulsa el botón
		void mouseRightButtonUnpressed(); //Se suelta el botón derecho del ratón
		void mouseRotate(int x, int y); //Rotaciones desde ratón

		void keyboardTranslate(float dx, float dy, float dz); //Traslaciones desde teclado
		void keyboardRotate(float dx, float dy, float dz); //Rotaciones desde teclado

		void zoomIn();
		void zoomOut();

		void updateView();

		void keyDown(unsigned char key);
		void keyUp(unsigned char key);
		void handleKeys();
		void handleMouse(int button, int state, int x, int y);

	private:
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