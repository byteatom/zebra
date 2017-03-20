#ifndef IPROJECT_H
#define IPROJECT_H

#include <QString>

class IProject
{
public:
	virtual QString filePath() = 0;
	virtual bool close() = 0;
protected:
	virtual ~IProject() = default;
};

#endif // IPROJECT_H
