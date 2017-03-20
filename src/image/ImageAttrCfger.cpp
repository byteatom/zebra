#include "ImageAttrCfger.h"
#include "ui_ImageAttrCfger.h"

#include <QFileDialog>
#include <QMessageBox>

#include "TextItem.h"
#include "PictureItem.h"
#include "SheetItem.h"
#include "WordItem.h"
#include "PptItem.h"
#include "ExcelItem.h"
#include "UtilQt.h"

ImageAttrCfger::ImageAttrCfger(QWidget *parent, ImageAttr* attr) :
	QGroupBox(parent),
	ui(new Ui::ImageAttrCfger),
	attr{attr}
{
	ui->setupUi(this);
	ui->word->hide();
	ui->excel->hide();
	ui->ppt->hide();

	ui->remove->setEnabled(false);
	ui->up->setEnabled(false);
	ui->down->setEnabled(false);
	connect(ui->text, &QPushButton::clicked,
			this, &ImageAttrCfger::addText);
	connect(ui->picture, &QPushButton::clicked,
			this, &ImageAttrCfger::addFile<PictureItem>);
	connect(ui->sheet, &QPushButton::clicked,
			this, &ImageAttrCfger::addSheet);
	connect(ui->word, &QPushButton::clicked,
			this, &ImageAttrCfger::addFile<WordItem>);
	connect(ui->ppt, &QPushButton::clicked,
			this, &ImageAttrCfger::addFile<PptItem>);
	connect(ui->excel, &QPushButton::clicked,
			this, &ImageAttrCfger::addFile<ExcelItem>);
	connect(ui->remove, &QPushButton::clicked,
			this, &ImageAttrCfger::remove);
	connect(ui->up, &QPushButton::clicked,
			this, &ImageAttrCfger::up);
	connect(ui->down, &QPushButton::clicked,
			this, &ImageAttrCfger::down);
	connect(attr->items, &QListWidget::itemDoubleClicked,
			this, &ImageAttrCfger::edit);
	connect(attr->items, &QListWidget::currentRowChanged,
			this, &ImageAttrCfger::currentRowChanged);
	connect(attr->items, &QListWidget::itemSelectionChanged,
			this, &ImageAttrCfger::itemSelectionChanged);

	attr->items->setSelectionMode(QListWidget::ExtendedSelection);
	attr->items->setEditTriggers(QAbstractItemView::EditKeyPressed);
	attr->items->setTextElideMode(Qt::ElideLeft);
	attr->items->setGeometry(ui->text->x(),
							 ui->text->geometry().bottom() + 11,
							 ui->down->geometry().right() - ui->text->x() + 1,
							 height() - 80);
	attr->items->setParent(this);
	if (attr->items->count() > 0) attr->items->setCurrentRow(0);
	connect(attr->items->itemDelegate(), &QAbstractItemDelegate::closeEditor,
			this, &ImageAttrCfger::nameChanged);
}

ImageAttrCfger::~ImageAttrCfger()
{
	delete ui;
}

void ImageAttrCfger::addText()
{
	TextItem* item = new TextItem(attr, nullptr);
	if (item->edit()) {
		attr->items->setCurrentItem(item);
	} else
		delete item;
}

void ImageAttrCfger::addSheet()
{
	SheetItem* item = new SheetItem(attr, nullptr);
	if (item->edit()) {
		attr->items->setCurrentItem(item);
	} else
		delete item;
}

template <class ItemClass>
void ImageAttrCfger::addFile()
{
	QStringList files = QFileDialog::getOpenFileNames(
							this,
							ImageAttrCfger::tr("Select Files"),
							"",
							ItemClass::filter(),
							nullptr,
							QFileDialog::ReadOnly);
	ItemClass* first = nullptr;
	for (auto& file: files) {
		ItemClass* item = new ItemClass(attr, file, nullptr);
		item->print();
		if (!first) first = item;
	}
	if (first) attr->items->setCurrentItem(first);
}

void ImageAttrCfger::remove()
{
	if (QMessageBox::Yes != QMessageBox::question(this,
											tr("Confirm"),
											tr("Delete selected items?"))) {
		return;
	}

	QList<QListWidgetItem*> items = attr->items->selectedItems();
	int minRow = attr->items->count() - 1;
	attr->items->blockSignals(true);
	for (QListWidgetItem* item: items) {
		int row = attr->items->row(item);
		if (row < minRow) minRow = row;
		static_cast<AttrItem*>(item)->clearCaches();
		delete item;
	}
	if (--minRow < 0 && attr->items->count() > 0) minRow = 0;
	attr->items->setCurrentRow(minRow);
	attr->items->blockSignals(false);
	currentRowChanged(minRow);
	itemSelectionChanged();
}

void ImageAttrCfger::up()
{
	UtilQt::raiseQListWidgetCurrentItem(attr->items);
}

void ImageAttrCfger::down()
{
	UtilQt::lowerQListWidgetCurrentItem(attr->items);
}

void ImageAttrCfger::edit(QListWidgetItem* item)
{
	AttrItem* attrItem = static_cast<AttrItem*>(item);
	if (attrItem->edit())
		attr->changed();
}

void ImageAttrCfger::currentRowChanged(int row)
{
	AttrItem* attrItem = nullptr;
	if (attr->items->count() > 0)
		attrItem = static_cast<AttrItem*>(attr->items->item(row));

	ui->up->setEnabled(row > 0);
	ui->down->setEnabled(row >= 0 && row < attr->items->count() - 1);

	if (attrItem) attrItem->editingPage = 0;

	for (int i = 0; i < ui->fadeBox->count(); ++i) {
		QWidget* fadeCfger = ui->fadeBox->widget(i);
		ui->fadeBox->removeWidget(fadeCfger);
		delete fadeCfger;
	}
	if (attrItem) {
		ui->fadeBox->addWidget(attrItem->fade->newCfger());
	}

	attr->changed();
}

void ImageAttrCfger::itemSelectionChanged()
{
	ui->remove->setEnabled(attr->items->selectedItems().count() > 0);
}

void ImageAttrCfger::nameChanged(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
	AttrItem* item = static_cast<AttrItem*>(attr->items->currentItem());
	item->name = item->text();
}
