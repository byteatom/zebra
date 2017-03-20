#ifndef SHEETEDITOR_H
#define SHEETEDITOR_H

#include <QDialog>
#include <QButtonGroup>

#include "SheetItem.h"

namespace Ui {
class SheetEditor;
}

class SheetCell;
class SheetCellEditor;

class SheetEditor : public QDialog
{
	Q_OBJECT
public:
	explicit SheetEditor(QWidget *parent, QSize size, SheetItem* item);
	~SheetEditor();

private:
	SheetCell* currentCell();
	SheetCellEditor* currentCellEditor();

	void sectionResized(int index, int oldWidth, int newWidth);
	void setColTitle();

	void addCol();
	void delCol();
	void addRow();
	void delRow();
	void family(const QFont &f);
	void point(int point);
	void letterSpace(double space);
	void lineSpace(double space);
	void bold(bool checked);
	void italic(bool checked);
	void underLine(bool checked);
	void bgColor();
	void textBgColor();
	void textColor();
	void left(bool checked);
	void center(bool checked);
	void right(bool checked);
	void justify(bool checked);
	void top(bool checked);
	void middle(bool checked);
	void bottom(bool checked);
	void wrap(bool checked);

	virtual void accept() override;

	Ui::SheetEditor *ui;
	QSize size;
	int borderSize = 1;
	SheetItem* item;
	QButtonGroup horAlign;
	QButtonGroup verAlign;
};

#endif // SHEETEDITOR_H
