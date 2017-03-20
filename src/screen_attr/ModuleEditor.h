#ifndef MODULEEDITOR_H
#define MODULEEDITOR_H

#include <QDialog>
#include <QStackedLayout>

#include "ISetting.h"
#include "IScreen.h"
#include "IProtocol.h"

namespace Ui {
class ModuleEditor;
}

class ModuleEditor : public QDialog
{
	Q_OBJECT

public:
	explicit ModuleEditor(const ISetting &setting_,
						  IScreen::Module &ledModule_,
						  IProtocol *protocol_,
						  bool newModule,
						  QWidget *parent = 0);
	~ModuleEditor();

private slots:
	void dispAttrSet(void);
	void dispScanDot(void);
	void saveAttrSet(void);
	void unlockAttrSet(void);

private:
	void setEnabledAttrSet(bool);
	const ISetting &setting;
	IScreen::Module &ledModule;
	IProtocol *protocol = nullptr;
	bool newModule;
	int hideCnt;

	Ui::ModuleEditor *ui;
};

#endif // MODULEEDITOR_H
