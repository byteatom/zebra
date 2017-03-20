#include "ScreenDelegate.h"

#include <QComboBox>

ScreenDelegate::ScreenDelegate(std::unordered_map<QString, IScreen*, UtilQt::QStringHash>& screens, QObject* parent) :
	QStyledItemDelegate{parent},
	screens{screens}
{
}

QWidget *ScreenDelegate::createEditor(QWidget *parent,
									const QStyleOptionViewItem& option,
									const QModelIndex& index) const
{
	QComboBox *combo = new QComboBox(parent);
	for (auto& screen: screens) {
		combo->addItem(screen.first);
	}
	return combo;
}

void ScreenDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::EditRole).toString();
	QComboBox *combo = static_cast<QComboBox*>(editor);
	combo->setCurrentText(value);
}

void ScreenDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
								   const QModelIndex& index) const
{
	QComboBox *combo = static_cast<QComboBox*>(editor);
	QString value = combo->currentText();
	model->setData(index, value, Qt::EditRole);
}

void ScreenDelegate::updateEditorGeometry(QWidget* editor,
										const QStyleOptionViewItem& option,
										const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}
