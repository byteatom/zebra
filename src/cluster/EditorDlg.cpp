#include "EditorDlg.h"
#include "ui_EditorDlg.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

#include "ClusterDlg.h"
#include "ui_ClusterDlg.h"

EditorDlg::EditorDlg(ClusterDlg *parent, bool add) :
	QDialog(parent, Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::EditorDlg),
	parent{parent},
	add{add}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/cluster_512.png")));
	setWindowTitle(add ? tr("Add") : tr("Edit"));

	items = parent->ui->table->selectedItems();
	bool multi = (items.size() > parent->ui->table->columnCount());

	for (auto& group: parent->groups) {
		ui->groups->addItem(group);
	}
	ui->groups->setCurrentIndex(-1);

	for (auto& screen: parent->screens) {
		ui->screens->addItem(screen.first);
	}
	ui->screens->setCurrentIndex(-1);

	if (!add) {
		int srcRow = items.first()->row();
		if (!multi) {
			ui->id->setText(parent->ui->table->item(srcRow, ClusterDlg::COL_ID)->text());
		} else {
			ui->id->setEnabled(false);
		}
		ui->name->setText(parent->ui->table->item(srcRow, ClusterDlg::COL_NAME)->text());
		ui->groups->setCurrentText(parent->ui->table->item(srcRow, ClusterDlg::COL_GROUP)->text());
		ui->screens->setCurrentText(parent->ui->table->item(srcRow, ClusterDlg::COL_SCREEN)->text());
		ui->comment->setText(parent->ui->table->item(srcRow, ClusterDlg::COL_COMMENT)->text());
	}

	ui->nameAuto->setVisible(!add && multi);

	connect(ui->ok, &QPushButton::clicked, this, &EditorDlg::accept);
	connect(ui->cancel, &QPushButton::clicked, this, &EditorDlg::reject);
}

EditorDlg::~EditorDlg()
{
	delete ui;
}

void EditorDlg::accept()
{
	QString id = ui->id->text().trimmed();

	if (ui->id->isEnabled()) {
		if (id.isEmpty()) {
			QMessageBox::warning(this, tr("Warning"),
						tr("LED ID cannot be empty"));
			return;
		}
		if (parent->idIllformed(id)) {
			QMessageBox::warning(this, tr("Warning"),
						tr("LED ID's format should be xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"));
			return;
		}
		bool idChanged = false;
		if (!add)
			idChanged = (id != parent->ui->table->item(items.first()->row(), ClusterDlg::COL_ID)->text());
		if ((add || idChanged) &&  parent->idExist(id)) {
			QMessageBox::warning(this, tr("Warning"),
						tr("LED ID has existed"));
			return;
		}
	}

	QString cols[] = {id,
					  ui->name->text(),
					  ui->groups->currentText(),
					  ui->screens->currentText(),
					  ui->comment->text()};

	if (add) {
		parent->addRow(cols);
	} else {
		if (ui->id->isEnabled()) {
			parent->setRow(parent->ui->table->selectedItems().first(), cols);
		} else {
			int srcRow = items.first()->row();
			QList<QTableWidgetItem*>::Iterator it = items.begin();
			while (it != items.end()) {
				if ((*it)->column() != ClusterDlg::COL_ID)
					it = items.erase(it);
				else
					++it;
			}
			bool nameChanged = (ui->name->text() != parent->ui->table->item(srcRow, ClusterDlg::COL_NAME)->text());
			bool groupChanged = (ui->groups->currentText() != parent->ui->table->item(srcRow, ClusterDlg::COL_GROUP)->text());
			bool screenChanged = (ui->screens->currentText() != parent->ui->table->item(srcRow, ClusterDlg::COL_SCREEN)->text());
			bool commentChanged = (ui->comment->text() != parent->ui->table->item(srcRow, ClusterDlg::COL_COMMENT)->text());
			int nameNumber = 0;
			for (auto& item: items) {
				bool autoName = ui->nameAuto->isChecked();
				if (nameChanged || autoName) {
					QString name = ui->name->text();
					if (autoName)
						name.append(" ").append(QString::number(++nameNumber));
					parent->ui->table->item(item->row(), ClusterDlg::COL_NAME)->setText(name);
				}
				if (groupChanged) {
					QString group = ui->groups->currentText();
					parent->ui->table->item(item->row(), ClusterDlg::COL_GROUP)->setText(group);
					parent->groups.insert(group);
				}
				if (screenChanged) parent->ui->table->item(item->row(), ClusterDlg::COL_SCREEN)->setText(ui->screens->currentText());
				if (commentChanged) parent->ui->table->item(item->row(), ClusterDlg::COL_COMMENT)->setText(ui->comment->text());
			}
		}
	}
	QDialog::accept();
}
