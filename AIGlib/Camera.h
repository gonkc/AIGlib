#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "AIGlibMacros.h"
#include "GroupNode.h"
#include "Enums.h"
#include <gl/glew.h>
#include <gl/gl.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#define M_PI 3.141592653589793238462643383279502884L

namespace AIGlib 
{
	class AIGLIB_API Camera : public GroupNode
	{
	public:

		//Matrices
		Camera(ProjMatrixType projMatrixType, const glm::mat4& proj, const glm::mat4& view, const unsigned int& width, 
			const unsigned int& height, const std::string& name = std::string());

		//Perspective default constructor
		Camera(const unsigned int& width, const unsigned int& height, const float& near,
			const float& far, const float& fovDegrees, const glm::vec3& position, const std::string& name = std::string());

		//Orthographic default constructors
		Camera(const unsigned int& width, const unsigned int& height, const float& left, const float& right, 
			const float& bottom, const float& top, const glm::vec3& position, const std::string& name = std::string());
		Camera(const unsigned int& width, const unsigned int& height, const float& near, const float& far,
			const float& left, const float& right, const float& bottom, const float& top, const glm::vec3& position, 
			const std::string& name = std::string());
		~Camera();


		glm::mat4 getProj() const;
		glm::mat4 getView() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		unsigned int getViewportX() const;
		unsigned int getViewportY() const;
		float getNear() const;
		float getFar() const;
		float getBottom() const;
		float getTop() const;
		float getLeft() const;
		float getRight() const;
		float getFieldOfView() const; //solo perspective
		glm::vec3 getPosition() const;
		glm::vec3 getUpVector() const;
		glm::vec3 getLookAt() const;

		void setProj(const glm::mat4& proj);
		void setView(const glm::mat4& view);
		
		void setWidth(unsigned int width);
		void setHeight(unsigned int height);
		void setViewport(const unsigned int& x, const unsigned int& y,
			const unsigned int& width, const unsigned int& height);
		void resize(unsigned int width, unsigned int height);

		//Virtuales para poder usar otras cámaras
		virtual void mouseRotate(int x, int y) {};

		virtual void keyDown(unsigned char key) {};
		virtual void keyUp(unsigned char key) {};
		virtual void handleKeys() {};
		virtual void handleMouse(int button, int state, int x, int y) {};

		Camera* getActiveCamera();
		bool getIfActiveCamera();
		void activateCamera();
		void deactivateCameras();

		void draw(SceneState ss);

	protected:
		
		ProjMatrixType _projMatrixType;
		bool _active;
		glm::mat4 _view;
		glm::mat4 _proj; 
		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _lookAt;
		glm::vec3 _right;
		unsigned int _width;
		unsigned int _height;
		unsigned int _xViewport;
		unsigned int _yViewport;

	};
}
#endif