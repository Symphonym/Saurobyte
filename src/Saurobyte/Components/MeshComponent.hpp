#ifndef JL_MESHCOMPONENT_HPP
#define JL_MESHCOMPONENT_HPP

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <string>
#include <Saurobyte/Component.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Saurobyte
{

	class MeshComponent : public Component<MeshComponent>
	{
	public:
		
		// Triangle data
		struct Triangle
		{	
			std::vector<GLfloat> vertices;
			std::vector<GLfloat> verticeColors;
			std::vector<GLfloat> textureCoords;
		};

	private:

		GLuint m_vertexBuffer;

		std::size_t m_vertexCount;

		// Offsets for where in memory data is stored
		std::size_t m_colorOffset, m_textureOffset;

		
		// If the triangles has been loaded into OpenGL memory
		bool m_loadedTriangles;

		std::vector<Triangle> m_triangles;


	public:

		GLuint vertexArray;

		GLuint meshTexture;
		GLuint meshSampler;

		MeshComponent(const std::vector<Triangle> &triangles, const std::string &texturePath);
		~MeshComponent();

		void loadTriangles(const std::vector<Triangle> &triangles);
		void loadTexture(const std::string &path);

		void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

		virtual void onLuaSet(const std::string& valueName, LuaEnvironment &env);
		virtual std::string getName() const;

		void storeData();

		std::size_t getVertexCount() const;
		bool isLoaded() const;
	};
};

#endif