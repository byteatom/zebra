#ifndef IDDELEGATE_H
#define IDDELEGATE_H

#include <QStyledItemDelegate>
#include <QTableView>

class ClusterDlg;

class IdDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit IdDelegate(ClusterDlg* dlg, QObject* parent = 0);
private:
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const override;

	ClusterDlg* dlg;
};

#endif // IDDELEGATE_H
