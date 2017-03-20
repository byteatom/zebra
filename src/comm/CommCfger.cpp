#include "CommCfger.h"
#include "ui_CommCfger.h"

#include "Message.h"

CommCfger::CommCfger(QWidget *parent, IProtocol* protocol) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::CommCfger),
	protocol{protocol},
	typeNames{protocol->typeNames()}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/comm_64.png")));

	for (ICarrier::TypeName& typeName: typeNames) {
		ui->types->addItem(typeName.name);
	}
	ui->types->setCurrentText(protocol->typeName().name);
	showCfger();

	connect(ui->types, static_cast<void (QComboBox::*)(int)>
						(&QComboBox::currentIndexChanged),
			this, &CommCfger::typeChanged);
	connect(ui->test, &QPushButton::clicked,
			this, &CommCfger::test);
	connect(ui->close, &QPushButton::clicked,
			this, &CommCfger::accept);
}

CommCfger::~CommCfger()
{
	delete ui;
}

void CommCfger::showCfger()
{
	if (cfger) {
		delete cfger;
	}
	cfger = protocol->newCfger();
	if (!cfger) return;
	cfger->setParent(this);
	cfger->move(10, ui->types->y() + ui->types->height() + 20);
	cfger->show();
}

void CommCfger::typeChanged(int index)
{
	IProtocol::Session session;
	QString name = ui->types->currentText();
	for (ICarrier::TypeName& typeName: typeNames) {
		if (name == typeName.name) {
			session.type = typeName.type;
			protocol->bind(session);
			showCfger();
			break;
		}
	}
}

void CommCfger::test()
{
	ui->result->setText(tr("Testing......"));
	ui->test->setEnabled(false);
	IMsgSp req{new InfoGetReqMsg};
	protocol->close();
	protocol->open();
	protocol->send(req, [this](bool ok) {
		ui->test->setEnabled(true);
		ui->result->setText(ok ? tr("OK.") : tr("Error!"));
		protocol->close();
	});
}

void CommCfger::done(int r)
{
	protocol->close();
	QDialog::done(r);
}
