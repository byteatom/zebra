#include "GlowCfger.h"
#include "ui_GlowCfger.h"

#include <QFileDialog>

#include "Glow.h"
#include "GlowEditor.h"

GlowCfger::GlowCfger(Glow* glow_):
	ui(new Ui::GlowCfger),
	glow{glow_}
{
	ui->setupUi(this);

	connect(ui->bg, &QCheckBox::toggled, [this](bool checked) {
		ui->bgEdit->setEnabled(checked);
		glow->effectBg.enabled = checked;
		glow->changed();
	});
	connect(ui->bgEdit, &QPushButton::clicked, [this]() {
		GlowEditor editor{this, &(glow->effectBg)};
		if (QDialog::Accepted == editor.exec())
			glow->changed();
	});
	connect(ui->fg, &QCheckBox::toggled, [this](bool checked) {
		ui->fgEdit->setEnabled(checked);
		glow->effectFg.enabled = checked;
		glow->changed();
	});
	connect(ui->fgEdit, &QPushButton::clicked, [this]() {
		GlowEditor editor{this, &(glow->effectFg)};
		if (QDialog::Accepted == editor.exec())
			glow->changed();
	});

	ui->bg->setChecked(glow->effectBg.enabled);
	ui->fg->setChecked(glow->effectFg.enabled);
}

GlowCfger::~GlowCfger()
{
	delete ui;
}
