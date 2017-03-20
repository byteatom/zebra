#ifndef SCREENDELEGATE_H
#define SCREENDELEGATE_H

#include <set>
#include <unordered_map>

#include <QStyledItemDelegate>
#include <QComboBox>

#include "IScreen.h"
#include "UtilQt.h"

class ScreenDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit ScreenDelegate(std::unordered_map<QString, IScreen*, UtilQt::QStringHash>& screens, QObject* parent = 0);
private:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
							const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
							  const QModelIndex &index) const override;


	std::unordered_map<QString, IScreen*, UtilQt::QStringHash>& screens;
};

#endif // SCREENDELEGATE_H
