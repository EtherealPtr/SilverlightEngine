#include "ShapeFactory.h"
#include "Triangle.h"
#include "Square.h"
#include "Pyramid.h"
#include "Cube.h"

namespace Silverlight
{
	void ShapeFactory::GetShapeData(const PrimitiveShapeEnum _shape, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices)
	{
		switch (_shape)
		{
			case PrimitiveShapeEnum::TriangleShape:
			{
				Triangle triangle{};
				triangle.GetShapeData(_vertices, _indices);
				break;
			}
			case PrimitiveShapeEnum::SquareShape:
			{
				Square square{};
				square.GetShapeData(_vertices, _indices);
				break;
			}
			case PrimitiveShapeEnum::PyramidShape:
			{
				Pyramid pyramid{};
				pyramid.GetShapeData(_vertices, _indices);
				break;
			}
			case PrimitiveShapeEnum::CubeShape:
			{
				Cube cube{};
				cube.GetShapeData(_vertices, _indices);
				break;
			}
		}
	}
} // End of namespace