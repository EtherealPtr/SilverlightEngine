#pragma once

#ifdef SILVERLIGHT_EXPORTS
	#define SILVERLIGHT_ENGINE __declspec(dllexport)
#else
	#define SILVERLIGHT_ENGINE __declspec(dllimport)
#endif