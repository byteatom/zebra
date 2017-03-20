#include "GroupDelegate.h"

#include <QComboBox>

GroupDelegate::GroupDelegate(std::set<QString>& groups, QObject* parent) :
	QStyledItemDelegate{parent},
	groups{groups}
{
}

QWidget *GroupDelegate::createEditor(QWidget *parent,
									const QStyleOptionViewItem& option,
									const QModelIndex& index) const
{
	QComboBox *combo = new QComboBox(parent);
	combo->setEditable(true);
	combo->setInsertPolicy(QComboBox::InsertAtTop);
	for (auto& group: groups) {
		combo->addItem(group);
	}
	return combo;
}

void GroupDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::EditRole).toString();
	QComboBox *combo = static_cast<QComboBox*>(editor);
	combo->setCurrentText(value);
}

void GroupDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
								   const QModelIndex& index) const
{
	QComboBox *combo = static_cast<QComboBox*>(editor);
	QString value = combo->currentText();
	model->setData(index, value, Qt::EditRole);
	groups.insert(value);
}

void GroupDelegate::updateEditorGeometry(QWidget* editor,
										const QStyleOptionViewItem& option,
										const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}
