#ifndef IMAGEATTRCFGER_H
#define IMAGEATTRCFGER_H

#include <QGroupBox>

#include "ImageAttr.h"

namespace Ui {
class ImageAttrCfger;
}

class ImageAttrCfger : public QGroupBox
{
	Q_OBJECT

public:
	explicit ImageAttrCfger(QWidget *parent, ImageAttr* attr);
	~ImageAttrCfger();

private:
	void addText();
	void addSheet();
	template <class ItemClass>
	void addFile();
	void remove();
	void up();
	void down();
	void edit(QListWidgetItem* item);
	void currentRowChanged(int row);
	void itemSelectionChanged();
	void nameChanged(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);

	Ui::ImageAttrCfger *ui;
	ImageAttr* attr = nullptr;
};

#endif // IMAGEATTRCFGER_H
