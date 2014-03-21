#ifndef JL_MESH_HPP
#define JL_MESH_HPP

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace jl
{

	class Mesh
	{
	private:

		GLuint m_vertexArray;
		GLuint m_vertexBuffer;

		GLuint m_meshTexture;
		GLuint m_meshSampler;

		glm::mat4 m_meshTranslation, m_meshRotation;

		std::size_t m_vertexCount;

		// Offsets for where in memory data is stored
		std::size_t m_colorOffset, m_textureOffset;

		
		// If the triangles has been loaded into OpenGL memory
		bool m_loadedTriangles;

		// Triangle data
		struct Triangle
		{	
			std::vector<GLfloat> vertices;
			std::vector<GLfloat> verticeColors;
			std::vector<GLfloat> textureCoords;
		};

		std::vector<Triangle> m_triangles;

		void storeData();

	public:

		Mesh();
		~Mesh();

		void loadTriangles(const std::vector<Triangle> &triangles);
		void loadTexture(const std::string &path);

		void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		void setPosition(GLfloat x, GLfloat y, GLfloat z);
		void setRotation(GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);

		void move(GLfloat xOffset, GLfloat yOffset = 0, GLfloat zOffset = 0);
		void rotate(GLfloat xRotation, GLfloat yRotation = 0, GLfloat zRotation = 0);

		void draw();

		std::size_t getVertexCount() const;
		glm::mat4 getTransform() const;
	};
};

#endif