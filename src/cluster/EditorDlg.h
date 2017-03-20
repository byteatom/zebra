#ifndef EDITORDLG_H
#define EDITORDLG_H

#include <unordered_set>

#include <QDialog>
#include <QTableWidget>

#include "UtilQt.h"

namespace Ui {
class EditorDlg;
}

class ClusterDlg;

class EditorDlg : public QDialog
{
	Q_OBJECT

public:
	explicit EditorDlg(ClusterDlg *parent, bool add);
	virtual ~EditorDlg();

private:
	Ui::EditorDlg *ui;
	ClusterDlg *parent;
	bool add;
	QList<QTableWidgetItem*> items;

	virtual void accept() override;
};

#endif // EDITORDLG_H
