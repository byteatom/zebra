#include "FadeCfger.h"
#include "ui_FadeCfger.h"

#include "Fade.h"

struct StyleCfg {
	const FadeStyle style;
	const char* name;
	bool show;
} styleCfg[] = {
	{FadeRandom, QT_TRANSLATE_NOOP("FadeCfger", "Random"), true},
	{FadeStatic, QT_TRANSLATE_NOOP("FadeCfger", "Static"), true},
	{FadeMoveRight, QT_TRANSLATE_NOOP("FadeCfger", "Move Right"), true},
	{FadeMoveLeft, QT_TRANSLATE_NOOP("FadeCfger", "Move Left"), true},
	{FadeMoveDown, QT_TRANSLATE_NOOP("FadeCfger", "Move Down"), true},
	{FadeMoveUp, QT_TRANSLATE_NOOP("FadeCfger", "Move Up"), true},
	{FadeOverlayRight, QT_TRANSLATE_NOOP("FadeCfger", "Overlay Right"), false},
	{FadeOverlayLeft, QT_TRANSLATE_NOOP("FadeCfger", "Overlay Left"), false},
	{FadeOverlayDown, QT_TRANSLATE_NOOP("FadeCfger", "Overlay Down"), false},
	{FadeOverlayUp, QT_TRANSLATE_NOOP("FadeCfger", "Overlay Up"), false},
	{FadeOpenHorizontal, QT_TRANSLATE_NOOP("FadeCfger", "Open Horizontal"), true},
	{FadeOpenVertical, QT_TRANSLATE_NOOP("FadeCfger", "Open Vertical"), true},
	{FadeCloseHorizontal, QT_TRANSLATE_NOOP("FadeCfger", "Close Horizontal"), true},
	{FadeCloseVertical, QT_TRANSLATE_NOOP("FadeCfger", "Close Vertical"), true},
	{FadeOutHorizontal, QT_TRANSLATE_NOOP("FadeCfger", "Out Horizontal"), true},
	{FadeOutVertical, QT_TRANSLATE_NOOP("FadeCfger", "Out Vertical"), true},
	{FadeShutterDown, QT_TRANSLATE_NOOP("FadeCfger", "Shutter Down"), false},
	{FadeShutterUp, QT_TRANSLATE_NOOP("FadeCfger", "Shutter Up"), false},
	{FadeBlink, QT_TRANSLATE_NOOP("FadeCfger", "Blink"), true},
	{FadeOutRight, QT_TRANSLATE_NOOP("FadeCfger", "Out Right"), false},
	{FadeOutLeft, QT_TRANSLATE_NOOP("FadeCfger", "Out Left"), false},
	{FadeOutDown, QT_TRANSLATE_NOOP("FadeCfger", "Out Down"), false},
	{FadeOutUp, QT_TRANSLATE_NOOP("FadeCfger", "Out Up"), false},
	{FadePageLeft, QT_TRANSLATE_NOOP("FadeCfger", "Page Left"), true},
	{FadePageRight, QT_TRANSLATE_NOOP("FadeCfger", "Page Right"), true},
	{FadeOpenFan, QT_TRANSLATE_NOOP("FadeCfger", "Open Fan"), true},
	{FadeCloseFan, QT_TRANSLATE_NOOP("FadeCfger", "Close Fan"), true},
	{FadeSpinLeft, QT_TRANSLATE_NOOP("FadeCfger", "Spin Left"), true},
	{FadeSpinRight, QT_TRANSLATE_NOOP("FadeCfger", "Spin Right"), true},
	{FadePullDown, QT_TRANSLATE_NOOP("FadeCfger", "Pull Down"), true},
	{FadePullUp, QT_TRANSLATE_NOOP("FadeCfger", "Pull Up"), true},
	{FadeRollDown, QT_TRANSLATE_NOOP("FadeCfger", "Roll Down"), false},
	{FadeRollRight, QT_TRANSLATE_NOOP("FadeCfger", "Roll Right"), false},
	{FadeGrid, QT_TRANSLATE_NOOP("FadeCfger", "Grid"), false},
	{FadeContrastBlink, QT_TRANSLATE_NOOP("FadeCfger", "Contrast Blink"), false},
	{FadeExpandRectangle, QT_TRANSLATE_NOOP("FadeCfger", "Expand Rectangle"), true},
	{FadeShrinkRectangle, QT_TRANSLATE_NOOP("FadeCfger", "Shrink Rectangle"), true},
	{FadePullRight, QT_TRANSLATE_NOOP("FadeCfger", "Pull Right"), true},
	{FadePullLeft, QT_TRANSLATE_NOOP("FadeCfger", "Pull Left"), true},
	{FadeExpandCross, QT_TRANSLATE_NOOP("FadeCfger", "Expand Cross"), true},
	{FadeShrinkCross, QT_TRANSLATE_NOOP("FadeCfger", "Shrink Cross"), true},
	{FadeRectangleTopLeft, QT_TRANSLATE_NOOP("FadeCfger", "Rectangle Top Left"), true},
	{FadeRectangleTopRight, QT_TRANSLATE_NOOP("FadeCfger", "Rectangle Top Right"), true},
	{FadeRectangleBottomLeft, QT_TRANSLATE_NOOP("FadeCfger", "Rectangle Bottom Left"), true},
	{FadeRectangleBottomRight, QT_TRANSLATE_NOOP("FadeCfger", "Rectangle Bottom Right"), true},
	{FadeDiagonalTopLeft, QT_TRANSLATE_NOOP("FadeCfger", "Diagonal Top Left"), false},
	{FadeDiagonalTopRight, QT_TRANSLATE_NOOP("FadeCfger", "Diagonal Top Right"), false},
	{FadeDiagonalBottomLeft, QT_TRANSLATE_NOOP("FadeCfger", "Diagonal Bottom Left"), false},
	{FadeDiagonalBottomRight, QT_TRANSLATE_NOOP("FadeCfger", "Diagonal Bottom Right"), false},
	{FadeSolidDown, QT_TRANSLATE_NOOP("FadeCfger", "Solid Down"), false},
	{FadeSolidUp, QT_TRANSLATE_NOOP("FadeCfger", "Solid Up"), false},
	{FadeMoveLeftContinuous, QT_TRANSLATE_NOOP("FadeCfger", "Move Left Continuous"), true},
	{FadeMoveUpContinuous, QT_TRANSLATE_NOOP("FadeCfger", "Move Up Continuous"), false},
	{FadeMoveRightContinuous, QT_TRANSLATE_NOOP("FadeCfger", "Move Right Continuous"), false},
	{FadeMoveDownContinuous, QT_TRANSLATE_NOOP("FadeCfger", "Move Down Continuous"), false}
};

FadeCfger::FadeCfger(Fade* fade_):
	ui(new Ui::FadeCfger),
	fade{fade_}
{
	ui->setupUi(this);

	int currentStyleIndex = 0;
	for (int i = 0; i < std::extent<decltype(styleCfg)>::value; ++i) {
		if (!styleCfg[i].show) continue;
		ui->style->addItem(tr(styleCfg[i].name), styleCfg[i].style);
		if (styleCfg[i].style == fade->style) currentStyleIndex = i;
	}
	ui->style->setCurrentIndex(currentStyleIndex);
	for (int i = 0; i <= 20; ++i) {
		if (i == 0)
			ui->speed->addItem(QString::number(i).append(tr("(Fastest)")));
		else if (i == 20)
			ui->speed->addItem(QString::number(i).append(tr("(Slowest)")));
		else
			ui->speed->addItem(QString::number(i));
	}
	ui->speed->setCurrentIndex(fade->speed);
	ui->stay->setValue(fade->stay);
	if (fade->hopsDisabled) {
		ui->hops->hide();
		ui->hopsTag->hide();
	} else
		ui->hops->setValue(fade->hops);

	connect(ui->style, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		fade->style = ui->style->itemData(index).toInt();
	});
	connect(ui->speed, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
			[this](int index) {
		fade->speed = index;
	});
	connect(ui->stay, static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged),
			[this](int value) {
		fade->stay = value;
	});
	connect(ui->hops, static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged),
			[this](int value) {
		fade->hops = value;
	});
}

FadeCfger::~FadeCfger()
{
	delete ui;
}
