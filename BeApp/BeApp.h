#pragma once

#ifdef BEAPP_EXPORTS
#	define BE_APPLICATION_ITEM __declspec(dllexport)
#else
#	define BE_APPLICATION_ITEM __declspec(dllimport)
#endif
