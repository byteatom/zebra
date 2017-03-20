#include "PictureEditor.h"
#include "ui_PictureEditor.h"

#include <QPalette>

PictureEditor::PictureEditor(QWidget *parent, QSize size, PictureItem* item) :
	QDialog(parent),
	ui(new Ui::PictureEditor),
	item{item}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(ui->ok, &QPushButton::clicked, this, &PictureEditor::accept);
	connect(ui->cancel, &QPushButton::clicked, this, &PictureEditor::reject);
	ui->picture->resize(size);
	source.load(item->fileItem.file);
	refresh();
}

PictureEditor::~PictureEditor()
{
	delete ui;
}

void PictureEditor::refresh()
{
	QPalette pal{ui->picture->palette()};
	pal.setBrush(ui->picture->backgroundRole(), QBrush{source});
	ui->picture->setPalette(pal);
}

void PictureEditor::accept()
{
	QDialog::accept();
}
