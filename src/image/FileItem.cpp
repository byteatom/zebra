#include "FileItem.h"

FileItem::FileItem(const QString& path, Json* jnode):
	file{path}
{
	if (jnode) {
		jtov(file);
	}
}

void FileItem::exportProjJson(Json& jnode)
{
	vtoj(file);
}
