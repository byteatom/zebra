#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QtGlobal>

class IPlugin
{
public:
	virtual ~IPlugin() = default;
};

extern "C" typedef IPlugin* (*PluginCreator)();

/*
#if defined(_MSC_VER)
	#define LIB_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
	#define LIB_EXPORT __attribute__((visibility("default")))
#else
	#define LIB_EXPORT
	__pragma("Unsupported compiler!")
#endif
*/

#define DEF_PLUGIN_CREATOR(PluginClass) \
extern "C" Q_DECL_EXPORT IPlugin* createKpg()\
{\
	return new PluginClass;\
}\

#endif // IPLUGIN_H
