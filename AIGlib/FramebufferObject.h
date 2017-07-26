#ifndef __FRAMEBUFFEROBJECT_H__
#define __FRAMEBUFFEROBJECT_H__

#include "AIGlibMacros.h"
#include "Shader.h"
#include "MeshProperties.h"

namespace AIGlib
{
	enum MSAAType
	{
		NONE = 0,
		MSAA2X = 2,
		MSAA4X = 4,
		MSAA8X = 8,
		MSAA16X = 16,
		MSAA32X = 32
	};

	class AIGLIB_API FramebufferObject
	{
	public:
		FramebufferObject(const unsigned int& nColorTex = 1);
		~FramebufferObject();
		void generate(const unsigned int& width,
			const unsigned int& height);
		virtual void bind(const unsigned int& width,
			const unsigned int& height);
		virtual void unbind(const unsigned int& width,
			const unsigned int& height);
		void render(const unsigned int& width,
			const unsigned int& height);
		virtual void effectLoadUniforms() { }
		void draw();
		void setMSAAType(MSAAType MSAAType, const unsigned int& width,
			const unsigned int& height);
    void setShader(Shader* shader);

	protected:


		Shader& getShader();		


	private:
		unsigned int _fbo;
		unsigned int _MSAAfbo;
		MSAAType _MSAAType;
		unsigned int _vao;
		unsigned int _renderBuffer;
		std::vector<unsigned int> _colorTex;
		unsigned int _nColorTex;
		unsigned int _depthTex;
		unsigned int _depthRenderBuffer;
		unsigned int _quadPosVBO;
		unsigned int _quadTexCoordVBO;
		unsigned int _quadTriangleIndexVBO;

		Shader* _shader;

		std::vector<unsigned int> _uColorTex;
		unsigned int _uNColorTex;
		unsigned int _uDepthTex;
		unsigned int _uWidth;
		unsigned int _uHeight;

		void initRenderToTextureQuad();
		void setUniformLocations();
		void freeResources();

	};
}

#endif