#include "GlowEditor.h"
#include "ui_GlowEditor.h"

#include <QDirIterator>
#include <QFileInfo>
#include <UtilQt.h>

GlowEditor::GlowEditor(QWidget *parent, GlowEffect* effect_) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::GlowEditor),
	effect{effect_}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	ui->overlap->setVisible(!effect->bg);

	#define UI_SPACE 10
	QSize iconSize{128, 128};
	ui->list->setIconSize(iconSize);
	QSize itemSize = iconSize + QSize{22, 22}/*checkbox*/;
	QSize listSize{itemSize.width() * 5 + 30/*scrollbar*/, itemSize.height() * 4};
	resize(listSize.width() + UI_SPACE * 2, listSize.height() + ui->ok->height() + UI_SPACE * 3);
	ui->list->resize(listSize);
	//ui->list->setGridSize(QSize{128, 128});
	int toolY = ui->list->y() + ui->list->height() + UI_SPACE;
	ui->speedTag->move(UI_SPACE, toolY);
	ui->speed->move(ui->speedTag->x() + ui->speedTag->width() + UI_SPACE, toolY);
	ui->overlap->move(ui->speed->x() + ui->speed->width() + UI_SPACE * 2, toolY);
	ui->cancel->move(width() - UI_SPACE - ui->cancel->width(), toolY);
	ui->ok->move(ui->cancel->x() - UI_SPACE - ui->ok->width(), toolY);

	ui->list->setSelectionMode(QAbstractItemView::MultiSelection);
	ui->list->setMovement(QListView::Static);

	QString libPath{"image/glow/"};
	if (effect->bg) {
		libPath.append("background");
	} else
		libPath.append("foreground");
	QDirIterator dirIt{libPath, QDir::Files};
	QDir dir;
	while (dirIt.hasNext()) {
		QString file = dir.relativeFilePath(dirIt.next());
		QListWidgetItem* item = new QListWidgetItem{QIcon{file}, QFileInfo(file).completeBaseName(), ui->list};
		//item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
		item->setData(Qt::UserRole, file);
		item->setSizeHint(itemSize);
		for (auto& path: effect->paths) {
			if (path == file) {
				item->setSelected(true);
				break;
			}
		}
	}
	ui->speed->setValue(effect->speed);
	if (!effect->bg)
		ui->overlap->setChecked(static_cast<GlowEffectFg*>(effect)->overlap);

	connect(ui->ok, &QPushButton::clicked, this, &GlowEditor::accept);
	connect(ui->cancel, &QPushButton::clicked, this, &GlowEditor::reject);
	connect(ui->speed, static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged),
			[this](int value) {
		effect->speed = value;
	});
	connect(ui->overlap, &QCheckBox::toggled, [this](bool checked) {
		static_cast<GlowEffectFg*>(effect)->overlap = checked;
	});
	connect(ui->list, &QListWidget::itemSelectionChanged, [this]() {
		if (ui->list->selectedItems().size() > 5) {
			ui->list->currentItem()->setSelected(false);
		}
	});
}

GlowEditor::~GlowEditor()
{
	delete ui;
}

void GlowEditor::accept()
{
	effect->paths.clear();
	QList<QListWidgetItem*> items = ui->list->selectedItems();
	for (auto item: items) {
		effect->paths.push_back(item->data(Qt::UserRole).toString());
	}
	effect->speed = ui->speed->value();
	if (ui->overlap->isVisible())
		static_cast<GlowEffectFg*>(effect)->overlap = ui->overlap->isChecked();

	QDialog::accept();
}
