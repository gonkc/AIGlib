#ifndef __AIGLIB_H__
#define __AIGLIB_H__

#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
//#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include "AmbientIntensity.h"
#include "Attribute.h"
#include "AssimpLoader.h"
#include "Camera.h"
#include "CameraController.h"
#include "Color.h"
#include "Definitions.h"
#include "DirectionalLight.h"
#include "FPSCamera.h"
#include "GeometryProperties.h"
#include "Material.h"
#include "Mesh.h"
#include "OrbitCamera.h"
#include "PointLight.h"
#include "RenderEngine.h"
#include "Scene.h"
#include "Shader.h"
#include "SpotLight.h"
#include "Texture.h"
#include "GroupNode.h"
#include "RenderPass.h"
#include "Fog.h"
#include "EffectGrayscale.h"
#include "EffectInversion.h"
#include "EffectKernel.h"
#include "EffectDepth.h"
#include "EffectGaussianBlur.h"
#include "EffectCrossStitching.h"


namespace AIGlib
{

}

#endif