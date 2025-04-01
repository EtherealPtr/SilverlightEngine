#pragma once

#include "Foundation/Platform.h"
#include "Graphics/Vertex.h"
#include <vector>

namespace Silverlight
{
	class Shape
	{
	public:
		virtual ~Shape() = default;
		virtual void GetShapeData(std::vector<Vertex>& _vertices, std::vector<uint32>& _indices) const = 0;
	};
} // End of namespace