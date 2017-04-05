#include "WebAttr.h"

#include "INode.h"
#include "ISetting.h"
#include "UtilQt.h"
#include "WebItem.h"

extern ISetting* setting;

WebAttr::WebAttr(INodeBox* box, Json* jnode):
	RegionAttr{box, INode::Type::WEB, jnode},
	editIndex{0}
{
	if (jnode) {
		Json::iterator jitemsIt = jnode->find("~items");
		if (jitemsIt != jnode->end())	{
			Json jitems = *jitemsIt;
			for (Json::iterator jItemIt = jitems.begin(); jItemIt != jitems.end(); ++jItemIt) {
				items.emplace_back(&(*jItemIt));
			}
		}
	}
}

boost::signals2::connection WebAttr::pageObserve(IRegion::PageObserver observer)
{
	auto slot = [this, observer](){
		observer(items.size(), editIndex);
	};
	slot();
	return changed.connect(slot);
}

void WebAttr::pageFirst()
{
	if (items.empty()) return;

	editIndex = 0;
	changed();
}

void WebAttr::pagePrevious()
{
	if (items.empty()) return;

	if (editIndex > 0)
		--editIndex;
	else
		editIndex = items.size() - 1;
	changed();
}

void WebAttr::pageNext()
{
	if (items.empty()) return;

	if (editIndex < items.size() - 1)
		++editIndex;
	else
		editIndex = 0;
	changed();
}

void WebAttr::pageLast()
{
	if (items.empty()) return;

	editIndex = items.size() - 1;
	changed();
}

void WebAttr::exportProjJson(Json& jnode)
{
	RegionAttr::exportProjJson(jnode);
	Json jitems;
	for (const WebItem& item: items) {
		Json jitem;
		item.exportProjJson(jitem);
		jitems.push_back(jitem);
	}
	jnode["~items"] = jitems;
}
