#ifndef SHEETCELLEDITOR_H
#define SHEETCELLEDITOR_H

#include "RichEditor.h"

class SheetCell;

class SheetCellEditor : public RichEditor
{
public:
	SheetCellEditor(SheetCell* cell);

	SheetCell* cell;
	void setBackgroundColor(const QColor& color);
private:
	virtual void focusInEvent(QFocusEvent *e) override;
};

#endif // SHEETCELLEDITOR_H
