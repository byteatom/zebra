#ifndef IMPORTINFO_H
#define IMPORTINFO_H

#include <QDialog>

namespace Ui {
class ImportInfo;
}

class ImportInfo : public QDialog
{
	Q_OBJECT

public:
	explicit ImportInfo(QWidget *parent, const QString& info);
	~ImportInfo();

private:
	Ui::ImportInfo *ui;

	void copy();
};

#endif // IMPORTINFO_H
