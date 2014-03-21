#ifndef JL_SHADER_HPP
#define JL_SHADER_HPP

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <string>

namespace jl
{

	class Shader
	{
	private:

		GLuint m_shaderHandle;

	public:

		Shader();
		explicit Shader(GLenum shaderType, const std::string &filePath);
		~Shader();

		void loadFromFile(const std::string &filePath);
		void loadFromFile(GLenum shaderType, const std::string &filePath);

		void attachTo(GLuint program);
	};

};

#endif