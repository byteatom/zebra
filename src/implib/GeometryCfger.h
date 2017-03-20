#ifndef REGIONBASECFGER_H
#define REGIONBASECFGER_H

#include <QGroupBox>

#include "RegionAttr.h"

namespace Ui {
class GeometryCfger;
}

class GeometryCfger : public QGroupBox
{
	Q_OBJECT
public:
	explicit GeometryCfger(QWidget *parent);
	~GeometryCfger();

	void init(RegionAttr* attr);

private:
	void load();
	void save();
	void xChanged(int x);
	void yChanged(int y);
	void widthChanged(int width);
	void heightChanged(int height);

	Ui::GeometryCfger *ui;
	RegionAttr* attr = nullptr;
	boost::signals2::connection boxRectConn;
};

#endif // REGIONBASECFGER_H
