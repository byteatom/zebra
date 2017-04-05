#ifndef NODEATTR_H
#define NODEATTR_H

#include <boost/signals2.hpp>

#include <QtGlobal>
#include <QCoreApplication>

#include "INode.h"
#include "INodeBox.h"
#include "QStr.h"
#include "Json.h"
#include "Attr.h"

#define UI_SPACE 10

class NodeAttr
{
public:
	explicit NodeAttr(INodeBox* box, int type, Json* jnode):
		box{box},
		type{type}
	{
		if (jnode) {
			jtov(name);
		}
		if (name.isEmpty())
			name = box->buildName(typeName());
	}

	static const char* typeStr(int type)
	{
		static const char* typeStrs[INode::Type::MAX_TYPE] = {
			QT_TR_NOOP("Screen"),
			QT_TR_NOOP("Program"),
			QT_TR_NOOP("Text"),
			QT_TR_NOOP("Image"),
			QT_TR_NOOP("Clock"),
			QT_TR_NOOP("Calendar"),
			QT_TR_NOOP("Timer"),
			QT_TR_NOOP("Flash Text"),
			QT_TR_NOOP("Humiture"),
			QT_TR_NOOP("Variable"),
			QT_TR_NOOP("Web"),
		};

		return typeStrs[type];
	}
	static int str2Type(const std::string& str)
	{
		for (int i = 0; i < INode::Type::MAX_TYPE; ++i) {
			if (str == typeStr(i)) return i;
		}
		return INode::Type::MAX_TYPE;
	}
	static const QString typeName(int type)
	{
		return QCoreApplication::translate("NodeAttr", typeStr(type));
	}

	const QString typeStr()
	{
		return typeStr(type);
	}
	const QString typeName()
	{
		return typeName(type);
	}

	void exportProjJson(Json& jnode)
	{
		jnode["type"] = typeStr();
		vtoj(name);
	}

	INodeBox* box = nullptr;
	bool active = false;
	int type;
	QStr name;

	boost::signals2::signal<void ()> changed;
};

#endif // NODEATTR_H
