#ifndef GROUPDELEGATE_H
#define GROUPDELEGATE_H

#include <set>

#include <QStyledItemDelegate>
#include <QComboBox>

class GroupDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit GroupDelegate(std::set<QString>& groups, QObject* parent = 0);
private:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
							const QModelIndex &index) const override;
	//void destroyEditor(QWidget *editor, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
							  const QModelIndex &index) const override;


	std::set<QString>& groups;
};

#endif // GROUPDELEGATE_H
