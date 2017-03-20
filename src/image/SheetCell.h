#ifndef SHEETCELL_H
#define SHEETCELL_H

#include <QTableWidget>
#include <QTableWidgetItem>

class SheetCellEditor;

class SheetCell : public QTableWidgetItem
{
public:
	SheetCell(QTableWidget* table, int row, int col);

	SheetCellEditor* editor = nullptr;
};

#endif // SHEETCELL_H
