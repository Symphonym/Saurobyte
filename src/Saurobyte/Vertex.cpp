#include <Saurobyte/Vertex.hpp>

namespace jl
{
	Vertex::Vertex(
		const Vector3f &newPosition,
		const Vector3f &newColor,
		const Vector2f &newTexCoords)
		:
		position(newPosition),
		color(newColor),
		texCoords(newTexCoords)
	{

	}
};