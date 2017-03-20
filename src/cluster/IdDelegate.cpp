#include "IdDelegate.h"

#include <QLineEdit>
#include <QMessageBox>

#include "ClusterDlg.h"

IdDelegate::IdDelegate(ClusterDlg* dlg, QObject* parent) :
	QStyledItemDelegate{parent},
	dlg{dlg}
{
}

void IdDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
								   const QModelIndex& index) const
{
	QLineEdit* edit = static_cast<QLineEdit*>(editor);
	QString value = edit->text().trimmed();
	QString valueOld = index.model()->data(index, Qt::EditRole).toString();
	if (value == valueOld) return;
	if (dlg->idIllformed(value)) {
		QMessageBox::warning(dlg, tr("Warning"),
					tr("LED ID's format should be xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"));
		return;
	}
	if (dlg->idExist(value)) {
		QMessageBox::warning(dlg, tr("Warning"),
					tr("LED ID has existed"));
		return;
	}
	dlg->ids.remove(valueOld);
	dlg->ids.insert(value);
	QStyledItemDelegate::setModelData(editor, model, index);
}
