#include "WebAttrCfger.h"
#include "ui_WebAttrCfger.h"

#include "WebItemStore.h"

WebAttrCfger::WebAttrCfger(QWidget *parent, WebAttr *attr_) :
	QGroupBox(parent),
	ui(new Ui::WebAttrCfger),
	attr{attr_}
{
	ui->setupUi(this);

	connect(ui->edit, &QPushButton::clicked, this, &WebAttrCfger::edit);
}

WebAttrCfger::~WebAttrCfger()
{
	delete ui;
}

void WebAttrCfger::edit()
{
	WebItemStore editor{this, attr};
	int ret = editor.exec();
	if (ret = QDialog::Accepted) {
		attr->editIndex = 0;
		attr->changed();
	}
}
