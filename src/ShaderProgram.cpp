#include "ShaderProgram.hpp"
#include <vector>
#include <cstring>
#include "Logger.hpp"

namespace jl
{

	ShaderProgram::ShaderProgram()
		:
		m_shaderProgram(0),
		m_currentBindingPoint(0)
	{
		m_shaderProgram = glCreateProgram();
	}
	ShaderProgram::~ShaderProgram()
	{
		if(glIsProgram(m_shaderProgram) == GL_TRUE)
			glDeleteProgram(m_shaderProgram);
	}

	void ShaderProgram::attachShader(Shader &shader)
	{
		shader.attachTo(m_shaderProgram);
	}
	void ShaderProgram::linkProgram()
	{
		glLinkProgram(m_shaderProgram);

		// Grab link status
		GLint linkStatus = 0;
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &linkStatus);

		// Check for faulty linking
		if(linkStatus == GL_FALSE)
		{
			GLint infoLogLength = 0;
			glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

			std::vector<char> logMessage(infoLogLength);

			glGetProgramInfoLog(m_shaderProgram, infoLogLength, NULL, &logMessage[0]);
			JL_ERROR_LOG("Shader linker error:\n%s", &logMessage[0]);
		}

		// Detach all shaders post-linking so it doesn't hold any future delete requests
		GLint attachedShaders = 0;
		glGetProgramiv(m_shaderProgram, GL_ATTACHED_SHADERS, &attachedShaders);

		std::vector<GLuint> linkedShaders(attachedShaders);
		glGetAttachedShaders(m_shaderProgram, attachedShaders, NULL, &linkedShaders[0]);

		for(std::size_t i = 0; i < linkedShaders.size(); i++)
			glDetachShader(m_shaderProgram, linkedShaders[i]);

	}
	void ShaderProgram::useProgram()
	{
		glUseProgram(m_shaderProgram);
	}

	void ShaderProgram::writeUniformBlock(const std::string &blockName, const std::string &uniformName, void* data, std::size_t dataSize)
	{
		/*UniformBlock block;
		GLuint blockIndex = glGetUniformBlockIndex(m_shaderProgram, blockName.c_str());

		if(m_uniformBlocks.find(blockName) != m_uniformBlocks.end())
		{
			block = m_uniformBlocks.find(blockName)->second;
			glBindBuffer(GL_UNIFORM_BUFFER, block.buffer);
		}
		else
		{
			// Store data for this block
			block.bindingPoint = m_currentBindingPoint++;
			glGenBuffers(1, &block.buffer);

			// Relate uniform block to binding point
			glUniformBlockBinding(m_shaderProgram, blockIndex, block.bindingPoint);

			// Allocate space for buffer object
			GLint blockByteSize = 0;
			glGetActiveUniformBlockiv(
				m_shaderProgram, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockByteSize);

			SDL_Log("SIIIIZE %i", blockByteSize);

			glBindBuffer(GL_UNIFORM_BUFFER, block.buffer);
			glBufferData(GL_UNIFORM_BUFFER, blockByteSize, NULL, GL_DYNAMIC_DRAW);

			m_uniformBlocks[blockName] = block;
		}


		// Relate buffer to binding point
		glBindBufferBase(GL_UNIFORM_BUFFER, block.bindingPoint, block.buffer);

		// Grab uniform block indice data
		std::vector<const char*> uniformNameProper
		{
			uniformName.c_str()
		};
		GLuint indice = 0;
		GLint offset = 0, size = 0, typeSize = 0;
		glGetUniformIndices(
			m_shaderProgram, 1, &uniformNameProper[0], &indice);
		glGetActiveUniformsiv(
			m_shaderProgram, 1, &indice, GL_UNIFORM_OFFSET, &offset);
		glGetActiveUniformsiv(
			m_shaderProgram, 1, &indice, GL_UNIFORM_SIZE, &size);
		glGetActiveUniformsiv(
			m_shaderProgram, 1, &indice, GL_UNIFORM_TYPE, &typeSize);

		SDL_Log("Offset %i", offset);
		SDL_Log("Size %i", size);
		SDL_Log("TypeSize %i", typeSize);

		void* bufferData = glMapBufferRange(GL_UNIFORM_BUFFER, offset, sizeof(GLfloat)*size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
		std::memcpy(bufferData, data, dataSize);*/

	}

	GLint ShaderProgram::getUniformLoc(const std::string &name) const
	{
		return glGetUniformLocation(m_shaderProgram, name.c_str());
	}
	GLint ShaderProgram::getUniformBlockLoc(const std::string &name) const
	{
		return glGetUniformBlockIndex(m_shaderProgram, name.c_str());
	}
	GLint ShaderProgram::getVertexAttribute(const std::string &name) const
	{
		return glGetAttribLocation(m_shaderProgram, name.c_str());
	}
	GLuint ShaderProgram::getProgram() const
	{
		return m_shaderProgram;
	}

};