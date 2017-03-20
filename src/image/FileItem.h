#ifndef FILEITEM_H
#define FILEITEM_H

#include "Json.h"

class FileItem
{
public:
	explicit FileItem(const QString& path, Json* jnode);
	virtual ~FileItem() = default;

	void exportProjJson(Json& jnode);

	QStr file;
};

#endif // FILEITEM_H
