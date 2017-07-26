#include "RenderPass.h"

namespace AIGlib
{
	RenderPass::RenderPass(const unsigned int& width, const unsigned int& height) :
		_width(width),
		_height(height),
    _msaaType(MSAAType::NONE)
	{

	}

	RenderPass::~RenderPass()
	{

	}

	void RenderPass::addRenderPass(FramebufferObject* framebufferObject)
	{
    if (_msaaType != MSAAType::NONE)
    {
      framebufferObject->setMSAAType(_msaaType, _width, _height);
    }
		_framebufferObjects.push_back(framebufferObject);
	}

	std::vector<FramebufferObject*> RenderPass::getFramebufferObjects()
	{
		return _framebufferObjects;
	}

	FramebufferObject* RenderPass::getFramebufferObject(const unsigned int& index)
	{
		return _framebufferObjects.at(index);
	}

	void RenderPass::resize(const unsigned int& width, const unsigned int& height)
	{
		_width = width;
		_height = height;
		for (unsigned int i = 0; i < _framebufferObjects.size(); ++i)
		{
			_framebufferObjects.at(i)->generate(_width, _height);
		}
	}

	void RenderPass::bindFirst()
	{
		if (_framebufferObjects.size() != 0)
		{
			_framebufferObjects.at(0)->bind(_width, _height);
		}
	}

	void RenderPass::renderAll()
	{
		unsigned int size = static_cast<unsigned int>(_framebufferObjects.size());
		for (unsigned int i = 0; i < size; ++i)
		{
			if (i + 1 < size)
			{
				_framebufferObjects.at(i + 1)->bind(_width, _height); //bind next fbo
			}
			else
			{
				_framebufferObjects.at(i)->unbind(_width, _height); //bind default fbo
			}
			_framebufferObjects.at(i)->render(_width, _height); //render fbo
			_framebufferObjects.at(i)->effectLoadUniforms(); //Upload effect uniforms
			_framebufferObjects.at(i)->draw();
		}
	}

	void RenderPass::setMSAAType(MSAAType MSAAType)
	{
    _msaaType = MSAAType;
		switch (MSAAType)
		{
		case MSAAType::NONE:
			std::cout << "MSAA changed to 'None'." << std::endl;
			break;
		case MSAAType::MSAA2X:
			std::cout << "MSAA changed to '2x'." << std::endl;
			break;
		case MSAAType::MSAA4X:
			std::cout << "MSAA changed to '4x'." << std::endl;
			break;
		case MSAAType::MSAA8X:
			std::cout << "MSAA changed to '8x'." << std::endl;
			break;
		case MSAAType::MSAA16X:
			std::cout << "MSAA changed to '16x'." << std::endl;
			break;
		case MSAAType::MSAA32X:
			std::cout << "MSAA changed to '32x'." << std::endl;
			break;
		}
		for (unsigned int i = 0; i < _framebufferObjects.size(); ++i)
		{
			_framebufferObjects.at(i)->setMSAAType(MSAAType, _width, _height);
		}
	}
}