#include "WebItemEditor.h"
#include "ui_WebItemEditor.h"

#include <QLineEdit>
#include <algorithm>

#include "WebManager.h"

WebItemEditor::WebItemEditor(QWidget *parent, const QSize& size, WebItem& item) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::WebItemEditor),
	item{item}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	lang = setting->language().toStdString();

	ui->webBox->resize(size);
	ui->web->setContextMenuPolicy(Qt::NoContextMenu);
	QWebEnginePage* page = ui->web->page();
	page->setBackgroundColor(Qt::transparent);
	ui->web->load(item.uri());

	Json jQuery = item.queryInfo();
	if (jQuery.is_object()) {
		for (Json::iterator it = jQuery.begin(); it != jQuery.end(); ++it) {
			addQuery(it);
		}
	}

	int space  = ui->formLayout->verticalSpacing();
	ui->paraBox->move(ui->webBox->geometry().right() + space, ui->webBox->y());

	int width = ui->paraBox->geometry().right() + space;

	ui->btnBox->move((width - ui->btnBox->width()) / 2,
					 std::max(ui->webBox->geometry().bottom(), ui->paraBox->geometry().bottom()) + space);

	resize(width, ui->btnBox->geometry().bottom() + space);

	connect(ui->refresh, &QPushButton::clicked, this, &WebItemEditor::refresh);
	connect(ui->ok, &QPushButton::clicked, this, &WebItemEditor::ok);
	connect(ui->cancel, &QPushButton::clicked, this, &WebItemEditor::reject);
}

WebItemEditor::~WebItemEditor()
{
	delete ui;
}

void WebItemEditor::addQuery(Json::iterator it)
{
	Json value = it.value();
	if (!value.is_object()) return;
	QString key = QString::fromStdString(it.key());
	Json::iterator langIt = value.find(lang);
	QString name;
	if (langIt != value.end())
		name = QString::fromStdString(langIt.value());
	else
		name = key;

	static const int rowHeight = 30;

	QLabel* label = new QLabel{name + ": "};
	label->setMinimumHeight(rowHeight);
	QLineEdit* field = new QLineEdit;
	field->setProperty("key", key);
	field->setMinimumHeight(rowHeight);
	ui->formLayout->addRow(label, field);

	if (item.query.hasQueryItem(key))
		field->setText(item.query.queryItemValue(key));

	ui->webBox->resize(ui->webBox->width(), ui->webBox->height() + rowHeight + ui->formLayout->verticalSpacing());
}

QUrlQuery WebItemEditor::getQuery()
{
	QUrlQuery query;
	for (int row = 1; row < ui->formLayout->rowCount(); ++row) {
		QLineEdit* field = static_cast<QLineEdit*>(ui->formLayout->itemAt(row, QFormLayout::FieldRole)->widget());
		QString value = field->text();
		if (value.isEmpty()) continue;
		query.addQueryItem(field->property("key").toString(), value);
	}
	return query;
}

void WebItemEditor::refresh()
{
	QUrlQuery query = getQuery();
	QUrl uri = item.url();
	uri.setQuery(query);
	ui->web->load(uri);
}

void WebItemEditor::ok()
{
	item.query = getQuery();
	QDialog::accept();
}
