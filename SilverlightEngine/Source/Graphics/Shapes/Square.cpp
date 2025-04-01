#include "Square.h"

namespace Silverlight
{
	void Square::GetShapeData(std::vector<Vertex>& _vertices, std::vector<uint32>& _indices) const
	{
		_vertices =
		{
			Vertex{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
			Vertex{ 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
			Vertex{ 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
			Vertex{-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}
		};

		_indices = { 0, 1, 2, 2, 3, 0 };
	}
} // End of namespace