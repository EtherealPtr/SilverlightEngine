#pragma once

#include "Foundation/Platform.h"

namespace Silverlight
{
	struct Image
	{
		unsigned char* m_Pixels;
		int32 m_ImageWidth;
		int32 m_ImageHeight;
		uint32 m_ImageIndex;
		uint64 m_ImageSize;
	};
} // End of namespace