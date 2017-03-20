#include "SheetEditor.h"
#include "ui_SheetEditor.h"

#include <QScrollBar>
#include <QPixmap>
#include <QColorDialog>

#include "SheetCell.h"
#include "SheetCellEditor.h"

SheetEditor::SheetEditor(QWidget *parent, QSize size, SheetItem* item) :
	QDialog(nullptr),
	ui(new Ui::SheetEditor),
	size{size},
	item{item}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon{"image/icon/sheet_128.png"});

	horAlign.addButton(ui->left);
	horAlign.addButton(ui->center);
	horAlign.addButton(ui->right);
	horAlign.addButton(ui->justify);

	verAlign.addButton(ui->top);
	verAlign.addButton(ui->middle);
	verAlign.addButton(ui->bottom);

	ui->table->verticalHeader()->setFixedWidth(30);
	ui->table->setFixedWidth(ui->table->verticalHeader()->width()
					  + size.width() - borderSize
					  + ui->table->verticalScrollBar()->width());
	ui->table->setStyleSheet("QTableWidget {background-color: black; gridline-color: red;}"
							 "QTableCornerButton::section {background-color: #4BACC6;}"
							 "QHeaderView {background-color: black;}"
							 "QHeaderView::section {background-color: #4BACC6; color: white; border:1px solid red;}"
							 //"QTableWidget::item {border:1px solid red;}"
							 );

	if (!item->rowHeight.empty()) {
		int col = 0;
		for (auto colWidth: item->colWidth) {
			ui->table->insertColumn(col);
			ui->table->setColumnWidth(col, colWidth);
			col++;
		}
		int row = 0;
		for (auto rowHeight: item->rowHeight) {
			ui->table->insertRow(row);
			ui->table->setRowHeight(row, rowHeight);
			row++;
		}

		row = 0;
		for (auto& htmlRow: item->html) {
			col = 0;
			for (auto& htmlCol: htmlRow) {
				SheetCell* cell = new SheetCell(ui->table, row, col);
				cell->editor->setHtml(htmlCol);
				col++;
			}
			row++;
		}
	} else {
		ui->table->insertColumn(0);
		ui->table->insertRow(0);
		new SheetCell(ui->table, 0, 0);
	}
	//setColTitle();

	connect(ui->table->horizontalHeader(), &QHeaderView::sectionResized,
			this, &SheetEditor::sectionResized);

	connect(ui->addCol, &QPushButton::clicked,
			this, &SheetEditor::addCol);
	connect(ui->delCol, &QPushButton::clicked,
			this, &SheetEditor::delCol);
	connect(ui->addRow, &QPushButton::clicked,
			this, &SheetEditor::addRow);
	connect(ui->delRow, &QPushButton::clicked,
			this, &SheetEditor::delRow);
	connect(ui->family, &QFontComboBox::currentFontChanged,
			this, &SheetEditor::family);
	connect(ui->point, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &SheetEditor::point);
	connect(ui->letterSpace, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
			this, &SheetEditor::letterSpace);
	connect(ui->lineSpace, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
			this, &SheetEditor::lineSpace);
	connect(ui->bold, &QPushButton::clicked,
			this, &SheetEditor::bold);
	connect(ui->italic, &QPushButton::clicked,
			this, &SheetEditor::italic);
	connect(ui->underLine, &QPushButton::clicked,
			this, &SheetEditor::underLine);
	connect(ui->bgColor, &QPushButton::clicked,
			this, &SheetEditor::bgColor);
	connect(ui->textBgColor, &QPushButton::clicked,
			this, &SheetEditor::textBgColor);
	connect(ui->textColor, &QPushButton::clicked,
			this, &SheetEditor::textColor);
	connect(ui->left, &QPushButton::clicked,
			this, &SheetEditor::left);
	connect(ui->center, &QPushButton::clicked,
			this, &SheetEditor::center);
	connect(ui->right, &QPushButton::clicked,
			this, &SheetEditor::right);
	connect(ui->justify, &QPushButton::clicked,
			this, &SheetEditor::justify);
	connect(ui->top, &QPushButton::clicked,
			this, &SheetEditor::top);
	connect(ui->middle, &QPushButton::clicked,
			this, &SheetEditor::middle);
	connect(ui->bottom, &QPushButton::clicked,
			this, &SheetEditor::bottom);
	connect(ui->wrap, &QPushButton::toggled,
			this, &SheetEditor::wrap);

	connect(ui->ok, &QPushButton::clicked,
			this, &SheetEditor::accept);
	connect(ui->cancel, &QPushButton::clicked,
			this, &SheetEditor::reject);
}

SheetEditor::~SheetEditor()
{
	delete ui;
}

SheetCell* SheetEditor::currentCell()
{
	return static_cast<SheetCell*>(ui->table->currentItem());
}

SheetCellEditor* SheetEditor::currentCellEditor()
{
	SheetCell* cell = currentCell();
	return cell ? cell->editor : nullptr;
}

void SheetEditor::sectionResized(int index, int oldWidth, int newWidth)
{
	if (newWidth <= oldWidth) return;
	int otherColWidth = 0;
	for (int col = 0; col < ui->table->columnCount(); ++col) {
		if (col != index) otherColWidth += ui->table->columnWidth(col);
	}
	int maxWidth = size.width() - borderSize - otherColWidth;
	if (newWidth > maxWidth) {
		ui->table->setColumnWidth(index, maxWidth);
	}
}

void SheetEditor::setColTitle()
{
	char title = 'A';
	QAbstractItemModel* model = ui->table->verticalHeader()->model();
	for (int col = 0; col < ui->table->columnCount(); ++col, ++title) {
		//QTableWidgetItem* item = ui->table->verticalHeaderItem(col);
		//item->setText(QString{title});
		model->setHeaderData(col, Qt::Vertical, QString{title}, Qt::DecorationRole);
	}
}

void SheetEditor::addCol()
{
	int col = ui->table->currentColumn();
	if (col < 0) {
		col = 0;
	}
	ui->table->insertColumn(col);
	if (ui->table->columnCount() == 0) {
		ui->table->setColumnWidth(col, size.width());
	} else {
		int widthAnchor = ui->table->columnWidth(col + 1);
		int widthNew = widthAnchor / 2;
		ui->table->setColumnWidth(col, widthNew);
		ui->table->setColumnWidth(col + 1, widthAnchor - widthNew);
	}
	for (int row = 0; row < ui->table->rowCount(); ++row) {
		new SheetCell(ui->table, row, col);
	}
}

void SheetEditor::delCol()
{
	int col = ui->table->currentColumn();
	if (col < 0) return;
	ui->table->removeColumn(col);
}

void SheetEditor::addRow()
{
	int row = ui->table->currentRow();
	if (row < 0) row = 0;
	ui->table->insertRow(row);
	for (int col = 0; col < ui->table->columnCount(); ++col) {
		new SheetCell(ui->table, row, col);
	}
}

void SheetEditor::delRow()
{
	int row = ui->table->currentRow();
	if (row < 0) return;
	ui->table->removeRow(row);
}

void SheetEditor::family(const QFont& font)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setFontFamily(font);
}

void SheetEditor::point(int point)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setFontPointSize(point);
}

void SheetEditor::letterSpace(double space)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setLetterSpace(space);
}

void SheetEditor::lineSpace(double space)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setLineSpace(space);
}

void SheetEditor::bold(bool checked)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setFontBold(checked);
}

void SheetEditor::italic(bool checked)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setFontItalic(checked);
}

void SheetEditor::underLine(bool checked)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setFontUnderline(checked);
}

void SheetEditor::bgColor()
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	QColor color = QColorDialog::getColor(Qt::black, this,
										  tr("Select Background Color"),
										  QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;
	/*QPalette pal(editor->viewport()->palette());
	pal.setColor(editor->viewport()->backgroundRole(), color);
	editor->viewport()->setPalette(pal);*/
	editor->setBackgroundColor(color);
}

void SheetEditor::textBgColor()
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	QColor color = QColorDialog::getColor(Qt::black, this,
										  tr("Select Text Background Color"),
										  QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;
	editor->setTextBackgroundColor(color);
}

void SheetEditor::textColor()
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	QColor color = QColorDialog::getColor(Qt::white, this,
										  tr("Select Text Color"),
										  QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;
	editor->setTextColor(color);
}

void SheetEditor::left(bool checked)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setAlignment(Qt::AlignLeft);
}

void SheetEditor::center(bool checked)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setAlignment(Qt::AlignCenter);
}

void SheetEditor::right(bool checked)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setAlignment(Qt::AlignRight);
}

void SheetEditor::justify(bool checked)
{
	SheetCellEditor* editor = currentCellEditor();
	if (!editor) return;
	editor->setAlignment(Qt::AlignJustify);
}

void SheetEditor::top(bool checked)
{

}

void SheetEditor::middle(bool checked)
{

}

void SheetEditor::bottom(bool checked)
{

}

void SheetEditor::wrap(bool checked)
{
	SheetCellEditor* editor = currentCellEditor();
	if (editor) editor->setLineWrapMode(checked ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
}

void SheetEditor::accept()
{
	//save attr
	item->html.clear();
	item->rowHeight.clear();
	item->colWidth.clear();
	for (int row = 0; row < ui->table->rowCount(); ++row) {
		item->html.emplace_back();
		item->rowHeight.push_back(ui->table->rowHeight(row));
		std::list<QStr>& rowHtml = item->html.back();
		for (int col = 0; col < ui->table->columnCount(); ++col){
			if (row == 0) item->colWidth.push_back(ui->table->columnWidth(col));
			SheetCell* cell = static_cast<SheetCell*>(ui->table->item(row, col));
			rowHtml.push_back(cell->editor->toHtml());
		}
	}

	//prepair snap
	item->clear();
	int snapHeight = borderSize;
	for (int row = 0; row < ui->table->rowCount(); ++row) {
		snapHeight += ui->table->rowHeight(row);
	}
	QPixmap snap{QSize{size.width(), snapHeight}};
	snap.fill(Qt::transparent);
	QPoint snapPos{0, 0};

	//prapair table
	QScrollBar* verScrollBar = ui->table->verticalScrollBar();
	verScrollBar->setValue(0);
	QRect tableRect;
	tableRect.setX(ui->table->verticalHeader()->width() - borderSize);
	tableRect.setWidth(size.width());

	//first page
	tableRect.setY(ui->table->horizontalHeader()->height() - borderSize);
	int firstHeight = ui->table->height() - tableRect.y();
	if (firstHeight > snapHeight) firstHeight = snapHeight;
	tableRect.setHeight(firstHeight);
	ui->table->render(&snap, snapPos, tableRect);

	//middle page
	int step = ui->table->height();
	int restHeight = verScrollBar->maximum();
	int rendHeight = 0;
	while (restHeight >= step) {
		verScrollBar->setValue(rendHeight += step);
		snapPos.setY(snapPos.y() + tableRect.height());
		tableRect.setY(0);
		tableRect.setHeight(step);
		ui->table->render(&snap, snapPos, tableRect);
		restHeight -= step;
	}

	//last page
	if (restHeight > 0) {
		verScrollBar->setValue(verScrollBar->maximum());
		snapPos.setY(snapPos.y() + tableRect.height());
		tableRect.setY(ui->table->height() - restHeight);
		tableRect.setHeight(restHeight);
		ui->table->render(&snap, snapPos, tableRect);
	}

	item->add(snap);

	QDialog::accept();
}
