#ifndef __SCENESTATE_H__
#define __SCENESTATE_H__

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

namespace AIGlib
{
	struct SceneState 
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		int program;
		//lights vector

	};
}

#endif
