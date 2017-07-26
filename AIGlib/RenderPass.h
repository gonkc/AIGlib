#ifndef __RENDERPASS_H__
#define __RENDERPASS_H__

#include "AIGlibMacros.h"
#include "FramebufferObject.h"
#include <vector>

namespace AIGlib
{
	class AIGLIB_API RenderPass
	{
	public:
		RenderPass(const unsigned int& width, const unsigned int& height);
		~RenderPass();
		void addRenderPass(FramebufferObject* framebufferObject);
		std::vector<FramebufferObject*> getFramebufferObjects();
		FramebufferObject* getFramebufferObject(const unsigned int& index);
		void resize(const unsigned int& width, const unsigned int& height);
		void bindFirst();
		void renderAll();
		void setMSAAType(MSAAType MSAAType);
	private:
		std::vector<FramebufferObject*> _framebufferObjects;
		unsigned int _width;
		unsigned int _height;
    MSAAType _msaaType;
	};
}

#endif