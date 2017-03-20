#include "SheetCellEditor.h"

#include "SheetCell.h"

SheetCellEditor::SheetCellEditor(SheetCell* cell):
	RichEditor{nullptr},
	cell{cell}
{
	setFrameShape(QFrame::NoFrame);
}

void SheetCellEditor::setBackgroundColor(const QColor& color)
{
	QString styleSheet = QString("background-color: rgba(%1, %2, %3, %4)")
						 .arg(color.red())
						 .arg(color.green())
						 .arg(color.blue())
						 .arg(color.alpha());
	viewport()->setStyleSheet(styleSheet);
}

void SheetCellEditor::focusInEvent(QFocusEvent *event)
{
	cell->tableWidget()->setCurrentItem(cell);
	RichEditor::focusInEvent(event);
}
