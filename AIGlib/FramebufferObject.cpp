#include "FramebufferObject.h"
#include "RenderEngine.h"

namespace AIGlib
{
	FramebufferObject::FramebufferObject(const unsigned int& nColorTex) :
		_fbo(invalidValue),
		_MSAAfbo(invalidValue),
		_MSAAType(MSAAType::NONE),
		_nColorTex(nColorTex)
	{
		_colorTex.resize(nColorTex, invalidValue);
		_uColorTex.resize(nColorTex, invalidValue);

		//Inicializamos el VAO que va a renderizar el quad a pantalla
		initRenderToTextureQuad();
	}

	FramebufferObject::~FramebufferObject()
	{
		freeResources();
	}

	void FramebufferObject::initRenderToTextureQuad()
	{

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		//Posiciones de los vértices
		static const float quadVertexPos[] =
		{
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,

			//-0.3f,  0.7f,  0.0f, 
			//0.3f,  0.7f,  0.0f, 
			//-0.3f,  1.0f,  0.0f, 
			//0.3f,  1.0f,  0.0f
		};

		/*static const float quadVertexTexCoord[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
		};
		static const unsigned int quadTriangleIndex[]{
		0, 1, 2,
		2, 3, 0
		};*/

		glGenBuffers(1, &_quadPosVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadPosVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexPos), quadVertexPos, GL_STATIC_DRAW);

		/*glGenBuffers(1, &_quadTexCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadTexCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexTexCoord), quadVertexTexCoord, GL_STATIC_DRAW);

		glGenBuffers(1, &_quadTriangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadTriangleIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadTriangleIndex), quadTriangleIndex, GL_STATIC_DRAW);
		*/
	}
	void FramebufferObject::setShader(Shader* shader)
	{
		_shader = shader;

		//Establecemos un Shader genérico para todo el FB
		setUniformLocations();
	}
	Shader& FramebufferObject::getShader()
	{
		return *_shader;
	}

	void FramebufferObject::generate(const unsigned int& width,
		const unsigned int& height)
	{
		if (_MSAAType != MSAAType::NONE)
		{
			GLuint rboColorId;
			glGenRenderbuffers(1, &rboColorId);
			glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, (GLsizei)_MSAAType, GL_RGBA8, width, height);

			GLuint rboDepthId;
			glGenRenderbuffers(1, &rboDepthId);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, (GLsizei)_MSAAType, GL_DEPTH_COMPONENT, width, height);

			glGenFramebuffers(1, &_MSAAfbo);
			glBindFramebuffer(GL_FRAMEBUFFER, _MSAAfbo);

			// Enlazamos el buffer de color y el de profundidad al fbo
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboColorId);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepthId);
		}

		//Creamos el buffer y lo enlazamos
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		//Creamos la textura de color
		for (unsigned int i = 0; i < _nColorTex; ++i)
		{
			glGenTextures(1, &_colorTex[i]);
			glBindTexture(GL_TEXTURE_2D, _colorTex[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		//Creamos la textura de profundidad
		glGenTextures(1, &_depthTex);
		glBindTexture(GL_TEXTURE_2D, _depthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//Y ponemos renderedTexture como attachement0
		for (unsigned int i = 0; i < _nColorTex; ++i)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorTex[i], 0);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTex, 0);

		// Comprobamos que el buffer se haya creado correctamente
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "¡Error: incomplete framebuffer!" << std::endl;
			exit(-1);
		}

		//Desenlazamos el fbo
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FramebufferObject::bind(const unsigned int& width,
		const unsigned int& height)
	{
		//Enlazamos el frameBuffer
		if (_MSAAType == MSAAType::NONE)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		}
		else
		{
			//MSAA 
			glBindFramebuffer(GL_FRAMEBUFFER, _MSAAfbo);
		}

		//Renderizamos en todo el frameBuffer
		glViewport(0, 0, width, height);

		//Limpiamos los bufferes 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Activamos el buffer de profundidad
		glEnable(GL_DEPTH_TEST);
	}

	void FramebufferObject::unbind(const unsigned int& width,
		const unsigned int& height)
	{
		if (_MSAAType == MSAAType::NONE)
		{
			//Enlazamos al buffer principal
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			//MSAA
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _MSAAfbo);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void FramebufferObject::render(const unsigned int& width,
		const unsigned int& height)
	{
		Scene& scene = RenderEngine::get().getScene();

		glBindVertexArray(_vao);

		//Renderizamos en todo el frameBuffer
		glViewport(0, 0, width, height);

		//Limpiamos la pantalla
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Usamos el shader del FBO
		glUseProgram(_shader->getProgram());

		//Ahora depth antes que colors
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _depthTex);

		//Enlazamos las texturas
		for (unsigned int i = 0; i < _nColorTex; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + 1 + i);
			glBindTexture(GL_TEXTURE_2D, _colorTex[i]);
		}

		//Subimos las variables uniformes
		glUniform1i(_uDepthTex, 0);
		for (unsigned int i = 0; i < _nColorTex; ++i)
		{
			glUniform1i(_uColorTex[i], 1 + i);
		}
		glUniform1i(_uWidth, width);
		glUniform1i(_uHeight, height);
		//glUniform3fv(_uBackgroundColor, 1, glm::value_ptr(scene.getBackgroundColor()));
		//glUniformMatrix4fv(_uView, 1, GL_FALSE, &(scene.getCamera().getView()[0][0]));
	}	

	/*void FramebufferObject::effectRender()
	{

	}*/

	void FramebufferObject::draw()
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _quadPosVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableVertexAttribArray(0);
	}

	void FramebufferObject::freeResources()
	{
		glDeleteFramebuffers(1, &_fbo);
		for (unsigned int i = 0; i < _nColorTex; ++i) 
		{
			glDeleteTextures(1, &_colorTex[i]);
		}
		glDeleteTextures(1, &_depthTex);
		glDeleteBuffers(1, &_quadPosVBO);
		delete _shader;
	}

	void FramebufferObject::setUniformLocations()
	{
		for (unsigned int i = 0; i < _nColorTex; ++i)
		{
			std::string name = "colorTex[" + std::to_string(i) + "]";
			_uColorTex[i] = glGetUniformLocation(_shader->getProgram(), name.c_str());
		}

		_uNColorTex = glGetUniformLocation(_shader->getProgram(), "nColorTex");
		_uDepthTex = glGetUniformLocation(_shader->getProgram(), "depthTex");

		_uWidth = glGetUniformLocation(_shader->getProgram(), "width");
		_uHeight = glGetUniformLocation(_shader->getProgram(), "height");

		//_uBackgroundColor = glGetUniformLocation(_shader->getProgram(), "backgroundColor");
		//_uView = glGetUniformLocation(_shader->getProgram(), "view");
	}

	void FramebufferObject::setMSAAType(MSAAType MSAAType, const unsigned int& width,
		const unsigned int& height)
	{
		_MSAAType = MSAAType;
		generate(width, height);
	}
}