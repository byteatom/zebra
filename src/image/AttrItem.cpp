#include "AttrItem.h"

#include "IFadeFactory.h"

extern IFadeFactory* fadeFactory;

AttrItem::AttrItem(ImageAttr* attr, const Type type, const QString& name_, Json* jnode):
	QListWidgetItem{name_, attr->items},
	name{name_},
	fade{fadeFactory->create(jnode, 5, false)},
	attr{attr},
	type{type}
{
	setFlags(flags() | Qt::ItemIsEditable);
	if (jnode) {
		jtov(name);
		setText(name);
	}
}

AttrItem::~AttrItem()
{
	delete fade;
}

bool AttrItem::edit()
{
	bool modified = editImpl();
	if (modified) {
		print();
	}
	return modified;
}

void AttrItem::print()
{
	printImpl();
}

void AttrItem::clearCaches()
{
}

void AttrItem::exportProjJson(Json& jnode)
{
	vtoj(type);
	vtoj(name);

	IProjJson* fadeJson = dynamic_cast<IProjJson*>(fade);
	if (fadeJson) fadeJson->exportProjJson(jnode);
}

QString AttrItem::exportCoreJson(Json& jnode)
{
	ICoreJson* fadeJson = dynamic_cast<ICoreJson*>(fade);
	if (fadeJson) {
		QString ret = fadeJson->exportCoreJson(jnode);
		if (!ret.isEmpty()) return ret;
	}
	return QString();
}
