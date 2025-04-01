#pragma once

#include "Graphics/PrimitiveShapeEnum.h"
#include "Graphics/Vertex.h"
#include <vector>

namespace Silverlight
{
	class ShapeFactory
	{
	public:
		static void GetShapeData(const PrimitiveShapeEnum _shape, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices);
	};
} // End of namespace