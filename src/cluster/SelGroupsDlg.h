#ifndef SELGROUPSDLG_H
#define SELGROUPSDLG_H

#include <QDialog>

namespace Ui {
class SelGroupsDlg;
}

class ClusterDlg;

class SelGroupsDlg : public QDialog
{
	Q_OBJECT

public:
	explicit SelGroupsDlg(ClusterDlg *parent);
	~SelGroupsDlg();

private:
	Ui::SelGroupsDlg *ui;
	ClusterDlg *parent;

	virtual void accept() override;
};

#endif // SELGROUPSDLG_H
