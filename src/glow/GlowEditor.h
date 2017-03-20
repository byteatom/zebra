#ifndef EDITORDLG_H
#define EDITORDLG_H

#include <QDialog>

#include "GlowEffect.h"

namespace Ui {
class GlowEditor;
}

class GlowEditor : public QDialog
{
	Q_OBJECT

public:
	explicit GlowEditor(QWidget *parent, GlowEffect* effect);
	~GlowEditor();

private:
	Ui::GlowEditor *ui;
	GlowEffect* effect;

	virtual void accept() override;
};

#endif // EDITORDLG_H
