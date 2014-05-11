#ifndef JL_VERTEX_HPP
#define JL_VERTEX_HPP

#include <SDL2/SDL_opengl.h>
#include <Saurobyte/Math/Vector3.hpp>
#include <Saurobyte/Math/Vector2.hpp>

namespace Saurobyte
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
			const Vector3f &position = Vector3f(0,0,0),
			const Vector3f &color = Vector3f(1,1,1),
			const Vector2f &texCoords = Vector2f(2,2));

	};
};

#endif