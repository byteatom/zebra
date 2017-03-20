#ifndef PROGRAMATTR_H
#define PROGRAMATTR_H

#include <QCoreApplication>
#include <QDateTime>

#include "NodeAttr.h"

#include "IBackground.h"
#include "IBorder.h"
#include "QStr.h"
#include "Attr.h"

class ProgramAttr : public NodeAttr
{
	Q_DECLARE_TR_FUNCTIONS(ProgramAttr)
public:
	enum Mode {
		SEQUENTIAL,
		SPECIFIED,
		FIXED,
		EXTERNAL,
		MODE_MAX
	};

	explicit ProgramAttr(INodeBox* box, Json* jnode, IBackground* background, IBorder* border);
	~ProgramAttr();

	Attr<KRect> contentRect;

	int mode{Mode::SEQUENTIAL};
	int fixedValue{600};
	int externalValue{600};
	QDateTime start{QDateTime::currentDateTime()};
	QDateTime stop{QDateTime::currentDateTime().addDays(1)};
	bool monday{true};
	bool tuesday{true};
	bool wednesday{true};
	bool thursday{true};
	bool friday{true};
	bool saturday{true};
	bool sunDay{true};

	IBackground* background = nullptr;
	IBorder* border = nullptr;

	void exportProjJson(Json& jnode);
	QString exportCoreJson(Json& jnode);

private:
	void updateContentRect();
	boost::signals2::connection boxRectConn;
};

#endif // PROGRAMATTR_H
