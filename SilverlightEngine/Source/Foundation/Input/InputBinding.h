#pragma once

#include "Foundation/Platform.h"
#include <functional>

namespace Silverlight
{
	struct InputBinding
	{
		int32 m_Key;
		std::function<void()> m_Callback;
	};
} // End of namespace