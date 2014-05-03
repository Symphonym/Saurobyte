#ifndef JL_VERTEX_HPP
#define JL_VERTEX_HPP

#include <SDL2/SDL_opengl.h>
#include <Saurobyte/Math.hpp>

namespace jl
{
	class Vertex
	{
	public:

		// Vertex position
		Vector3f position;

		// Vertex color
		Vector3f color;

		// Vertex texture cordinates
		Vector2f texCoords;

		// Default constructor made through default arguments
		explicit Vertex(
			const Vector3f &position = {0,0,0},
			const Vector3f &color = {1,1,1},
			const Vector2f &texCoords = {2,2});

	};
};

#endif