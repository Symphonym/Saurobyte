#include "MeshComponent.hpp"
#include <cstring>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace jl
	{

	MeshComponent::MeshComponent(const std::vector<Triangle> &triangles, const std::string &texturePath)
		:
		Component<MeshComponent>("MeshComponent"),
		m_vertexBuffer(0),
		m_vertexCount(0),
		m_colorOffset(0),
		m_textureOffset(0),
		m_loadedTriangles(false),
		vertexArray(0),
		meshTexture(0),
		meshSampler(0)
	{
		glGenVertexArrays(1, &vertexArray);
		glGenBuffers(1, &m_vertexBuffer);

		loadTriangles(triangles);
		loadTexture(texturePath);
	}
	MeshComponent::~MeshComponent()
	{
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &m_vertexBuffer);

		if(glIsTexture(meshTexture) == GL_TRUE)
			glDeleteTextures(1, &meshTexture);
		if(glIsSampler(meshSampler) == GL_TRUE)
			glDeleteSamplers(1, &meshSampler);
	}

	void MeshComponent::storeData()
	{
		glBindVertexArray(vertexArray);

		std::vector<GLfloat> vertices;
		std::vector<GLfloat> verticeColors;
		std::vector<GLfloat> textureCoords;
		int vertexSize = 3, vertexColorSize = 4;

		// Grab all vertices and vertice colors from the triangles
		for(std::size_t i = 0; i < m_triangles.size(); i++)
		{
			// Fill missing colors with 1's
			while(m_triangles[i].verticeColors.size() < vertexColorSize)
				m_triangles[i].verticeColors.push_back(1.f);

			vertices.insert(vertices.end(), m_triangles[i].vertices.begin(), m_triangles[i].vertices.end());
			verticeColors.insert(verticeColors.end(), m_triangles[i].verticeColors.begin(), m_triangles[i].verticeColors.end());
			textureCoords.insert(textureCoords.end(), m_triangles[i].textureCoords.begin(), m_triangles[i].textureCoords.end());
		}

		// Values per vertex, grabbed from first vertice (since they should all be the same anyway)
		if(m_triangles.size() > 0)
			vertexSize = m_triangles[0].vertices.size();

		m_vertexCount = vertices.size() / vertexSize;

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(GLfloat) * vertices.size() + sizeof(GLfloat) * verticeColors.size() + sizeof(GLfloat) * textureCoords.size(),
			NULL,
			GL_DYNAMIC_DRAW);

		m_colorOffset = sizeof(GLfloat) * vertices.size();
		m_textureOffset = sizeof(GLfloat) * vertices.size() + sizeof(GLfloat) * verticeColors.size();

		// Store vertices and vertice colors in buffer
		glBufferSubData(GL_ARRAY_BUFFER,
			0,
			sizeof(GLfloat) * vertices.size(),
			&vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER,
			sizeof(GLfloat) * vertices.size(),
			sizeof(GLfloat) * verticeColors.size(),
			&verticeColors[0]);
		glBufferSubData(GL_ARRAY_BUFFER,
			m_colorOffset,
			sizeof(GLfloat) * verticeColors.size(),
			&verticeColors[0]);
		glBufferSubData(GL_ARRAY_BUFFER,
			m_textureOffset,
			sizeof(GLfloat) * textureCoords.size() ,
			&textureCoords[0]);


		// Connect data with shaders

		// Vertices
		glEnableVertexAttribArray(0); 
		glVertexAttribPointer(0, vertexSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		// Vertex colors
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,
			vertexColorSize,
			GL_FLOAT,
			GL_FALSE,
			0, (void*)(m_colorOffset));
		
		// Texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(m_textureOffset));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_loadedTriangles = true;
	}

	void MeshComponent::loadTriangles(const std::vector<Triangle> &triangles)
	{
		m_triangles.insert(m_triangles.end(), triangles.begin(), triangles.end());

		if(m_triangles.size() > 0)
			m_vertexCount = (m_triangles[0].vertices.size() * m_triangles.size()) / 3;
	}
	void MeshComponent::loadTexture(const std::string &path)
	{
		if(glIsTexture(meshTexture) == GL_FALSE)
			glGenTextures(1, &meshTexture);
		if(glIsSampler(meshSampler) == GL_FALSE)
			glGenSamplers(1, &meshSampler);

		// Setup sampler data
		glSamplerParameteri(meshSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(meshSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glSamplerParameteri(meshSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(meshSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		SDL_Surface* surface = IMG_Load(path.c_str());

		GLint readFormat = GL_RGB;

		if(surface->format->BytesPerPixel == 4)
		{
			if(surface->format->Rmask == 0xFF)
				readFormat = GL_RGBA;
			else if(surface->format->Bmask == 0xFF)
				readFormat = GL_BGRA;
		}
		else if(surface->format->BytesPerPixel == 3)
		{
			if(surface->format->Rmask == 0xFF)
				readFormat = GL_RGB;
			else if(surface->format->Bmask == 0xFF)
				readFormat = GL_BGR;
		}
			
		if(surface->format->Rmask)
		SDL_Log("Bytes/Pixel: %i", surface->format->BytesPerPixel);

		GLfloat texDat[] =
		{
			1.f,1.f,1.f,1.f,
			0.f,1.f, 0.f, 1.f,
			1.f,1.f,1.f,1.f,
			0.f,1.f, 0.f, 1.f,
		};

		// Setup texture data
		glBindTexture(GL_TEXTURE_2D, meshTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, readFormat, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_FreeSurface(surface);
	}

	void MeshComponent::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		// Update local memory so we can keep track of the mesh data easily
		for(std::size_t i = 0; i < m_triangles.size(); i++)
		{
			m_triangles[i].verticeColors[0] = r;
			m_triangles[i].verticeColors[1] = g;
			m_triangles[i].verticeColors[2] = g;
			m_triangles[i].verticeColors[3] = a;
		}

		// Update buffer memory by mapping to it
		if(m_loadedTriangles)
		{
			
			std::size_t colorSize = m_textureOffset - m_colorOffset;
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
			void *data = glMapBufferRange(
				GL_ARRAY_BUFFER,
				m_colorOffset,
				colorSize,
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

			// Modify color values of vertices
			GLfloat* dataPtr = static_cast<GLfloat*>(data);
			for(std::size_t i = 0; i < colorSize/sizeof(GLfloat)-3; i += 4)
			{
				dataPtr[i] = r;
				dataPtr[i+1] = g;
				dataPtr[i+2] = b;
				dataPtr[i+3] = a;
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	std::size_t MeshComponent::getVertexCount() const
	{
		return m_vertexCount;
	}
	bool MeshComponent::isLoaded() const
	{
		return m_loadedTriangles;
	}







	void MeshComponent::onLuaSet(const std::string& valueName, lua_State *state)
	{
		if(valueName == "color")
		{
			float r = 0, g = 0, b = 0;
			r = luaL_checknumber(state, 1);
			g = luaL_checknumber(state, 2);
			b = luaL_checknumber(state, 3);

			setColor(r, g, b, 1); 
		}
	}
}