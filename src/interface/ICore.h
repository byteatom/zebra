#ifndef ICORE_H
#define ICORE_H

class ICore
{
public:
	virtual bool build(const char* json/**<utf8 json file buffer*/) = 0;
protected:
	virtual ~ICore() = default;
};

#endif // ICORE_H
