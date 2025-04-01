#pragma once

namespace Silverlight
{
	class Observer
	{
	public:
		virtual void OnNotify() = 0;
	};
} // End of namespace