#ifndef MODULEWIZARD_H
#define MODULEWIZARD_H

#include <QString>
#include <QWizard>
#include <QModelIndex>
#include <QTableWidgetItem>
#include <QTimer>
#include "ISetting.h"
#include "IScreen.h"
#include "IProtocol.h"

#define CTRLINFO_DECODEMODE     (1ul<<7)
#define CTRLINFO_OEPOLARITY     (1ul<<6)
#define CTRLINFO_DATAPOLARITY   (1ul<<5)
#define CTRLINFO_STRPOLARITY    (1ul<<4)

struct ScanDotSend {
	uint16_t dotIndex;
	uint8_t display;
	uint8_t type;
	uint8_t currentLine;
	uint8_t bitMask;
	uint8_t linesInterface;
	uint8_t wrapTimes;
	uint8_t baseColor;
};

namespace Ui {
class ModuleWizard;
}

class ModuleWizard : public QWizard
{
	Q_OBJECT

public:
	enum {
		ScanDotWizardPageStart = 0,
		ScanDotWizardPageBasePara,
		ScanDotWizardPageDataPolar,
		ScanDotWizardPageOePolar,
		ScanDotWizardPageModuleLines,
		ScanDotWizardPageWarpTimes,
		ScanDotWizardPageDotCoord,
		ScanDotWizardPageComplete,
	};

	explicit ModuleWizard(const ISetting &setting_,
						   IScreen::Module &ledModule_,
						   IProtocol *protocol_,
						   bool newModule_,
						   QWidget *parent = 0);
	~ModuleWizard();

	void showEmptyDotConut(int count);
	void showPageCompleteHint(const QString &str, Qt::GlobalColor color);
	void saveScanDataFile(void);

	void clearDotCoordData();
	void initScanData();
	void setScanDataToScanDotWizard();
	void dotCoordCellSelected(int row, int column);
	void sendScanDotSend(ScanDotSend *send);

private slots:
	void scanDotWizardCurrentIdChanged(int id);
	void on_pushButton_pageDotCoordReset_clicked();
	void on_pushButton_pageDotCoordCancel_clicked();
	void on_pushButton_pageDotCoordZoom_clicked();
	void on_checkBox_pageBaseParaEnableEmptyDot_toggled(bool checked);
	void on_tableWidget_pageDotCoord_cellPressed(int row, int column);
	void on_tableWidget_pageDotCoord_cellEntered(int row, int column);
	void on_pushButton_pageDotCoordEmpty_clicked();
	void on_ScanDotWizard_finished(int result);
	void on_pushButton_pageCompleteSava_clicked();
	void on_comboBox_pageBaseParaDecodeMode_currentIndexChanged(int index);
	void on_comboBox_pageBaseParaBaseColor_currentIndexChanged(int index);
	void on_comboBox_pageDataPolarSelect_currentIndexChanged(int index);
	void on_radioButton_pageDataPolarTestAllBright_toggled(bool checked);
	void on_comboBox_pageOePolarSelect_currentIndexChanged(int index);
	void on_radioButton_pageOePolarBright_toggled(bool checked);
	void on_tableWidget_pageDotCoord_itemSelectionChanged();

protected:
	bool eventFilter(QObject *obj, QEvent *event);

private:
	const ISetting &setting;
	IScreen::Module &ledModule;
	IProtocol *protocol;
	ScanDotSend scanDotSend;
	bool newModule;
	bool isSave;
	bool leftButtonPress;

	Ui::ModuleWizard *ui;
};

#endif // MODULEWIZARD_H
