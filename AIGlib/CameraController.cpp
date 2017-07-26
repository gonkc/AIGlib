#include "CameraController.h"

namespace AIGlib 
{
	CameraController::CameraController(Camera* camera, 
		ProjMatrixType projMatrixType, CameraType cameraType) :
		_camera(camera),
		_projMatrixType(projMatrixType),
		_cameraType(cameraType)
	{
		glm::mat4 proj;
		switch (projMatrixType)
		{
			case ProjMatrixType::PERSPECTIVE:
				break;
			case ProjMatrixType::ORTHOGRAPHIC:
				break;
		}



		glm::mat4 view;		
		switch (_cameraType)
		{
			case CameraType::BASE:
				view = glm::lookAt(_camera->getPosition(), _camera->getLookAt(), _camera->getUpVector());
				break;
			case CameraType::ARCBALL:
				//view = glm::
				break;
		}
	}

	CameraController::~CameraController()
	{

	}

	Camera* CameraController::getCamera() const
	{
		return _camera;
	}

	ProjMatrixType CameraController::getProjMatrixType() const
	{
		return _projMatrixType;
	}
	
	CameraType CameraController::getCameraType() const
	{
		return _cameraType;
	}
}