#include "SheetCell.h"

#include "SheetCellEditor.h"

SheetCell::SheetCell(QTableWidget* table, int row, int col)
{
	setBackground(Qt::black);
	table->setItem(row, col, this);
	editor = new SheetCellEditor{this};
	table->setCellWidget(row, col, editor);
}

