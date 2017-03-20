#ifndef PICTUREEDITOR_H
#define PICTUREEDITOR_H

#include <QDialog>

#include "PictureItem.h"

namespace Ui {
class PictureEditor;
}

class PictureEditor : public QDialog
{
	Q_OBJECT

public:
	explicit PictureEditor(QWidget *parent, QSize size, PictureItem* item);
	~PictureEditor();

private:
	void refresh();
	virtual void accept() override;

	Ui::PictureEditor *ui;
	PictureItem* item;
	QPixmap source;
};

#endif // PICTUREEDITOR_H
