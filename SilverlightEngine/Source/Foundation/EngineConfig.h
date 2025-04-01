#pragma once

#include "Platform.h"
#include <string>

namespace Silverlight
{
	class EngineConfig
	{
	public:
		EngineConfig() noexcept :
			m_WindowWidth{ 400 },
			m_WindowHeight{ 300 },
			m_WindowTitle{ "Untitled" }
		{};

		uint32 m_WindowWidth;
		uint32 m_WindowHeight;
		std::string m_WindowTitle;
	};
} // End of namespace