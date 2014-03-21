#ifndef JL_SHADERPROGRAM_HPP
#define JL_SHADERPROGRAM_HPP

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <unordered_map>

#include "Shader.hpp"

namespace jl
	{

	class ShaderProgram
	{
	private:

		GLuint m_shaderProgram;

		struct UniformBlock
		{
			GLuint buffer;
			GLuint bindingPoint;
		};

		std::unordered_map<std::string, UniformBlock> m_uniformBlocks;
		int m_currentBindingPoint;

	public:

		ShaderProgram();
		~ShaderProgram();

		void attachShader(Shader &shader);
		void linkProgram();
		void useProgram();

		void writeUniformBlock(const std::string &blockName, const std::string &uniformName, void* data, std::size_t dataSize);

		GLint getUniformLoc(const std::string &name) const;
		GLint getUniformBlockLoc(const std::string &name) const;
		GLint getVertexAttribute(const std::string &name) const;
		GLuint getProgram() const;
	};
};
#endif