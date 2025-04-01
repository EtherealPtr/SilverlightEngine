#include "Pyramid.h"

namespace Silverlight
{
	void Pyramid::GetShapeData(std::vector<Vertex>& _vertices, std::vector<uint32>& _indices) const
	{
		_vertices =
		{
			Vertex{-0.5f, -0.5f,  0.0f, 0.0f, 1.0f, -0.5f, -0.5f,  0.0f},
			Vertex{ 0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.5f, -0.5f,  0.0f},
			Vertex{ 0.5f, -0.5f, -1.0f, 1.0f, 0.0f,  0.5f, -0.5f, -1.0f},
			Vertex{-0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -1.0f},
			Vertex{ 0.0f,  0.5f, -0.5f, 0.5f, 0.5f,  0.0f,  0.5f, -0.5f}
		};

		_indices =
		{
			0, 3, 4,
			3, 2, 4,
			2, 1, 4,
			1, 0, 4,
			1, 2, 3,
			1, 3, 0
		};
	}
} // End of namespace