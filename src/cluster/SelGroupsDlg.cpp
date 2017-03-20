#include "SelGroupsDlg.h"
#include "ui_SelGroupsDlg.h"

#include <climits>

#include "ClusterDlg.h"
#include "ui_ClusterDlg.h"

SelGroupsDlg::SelGroupsDlg(ClusterDlg *parent) :
	QDialog(parent),
	ui(new Ui::SelGroupsDlg),
	parent{parent}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(ui->ok, &QPushButton::clicked, this, &SelGroupsDlg::accept);
	connect(ui->cancel, &QPushButton::clicked, this, &SelGroupsDlg::reject);

	for (auto& group: parent->groups) {
		new QListWidgetItem{group, ui->groups};
	}
	ui->groups->sortItems(Qt::AscendingOrder);
}

SelGroupsDlg::~SelGroupsDlg()
{
	delete ui;
}

void SelGroupsDlg::accept()
{
	QList<QListWidgetItem*> items = ui->groups->selectedItems();
	if (items.empty()) return;

	parent->ui->table->blockSignals(true);
	parent->ui->table->clearSelection();
	QAbstractItemView::SelectionMode mode = parent->ui->table->selectionMode();
	parent->ui->table->setSelectionMode(QAbstractItemView::MultiSelection);

	parent->ui->table->sortByColumn(ClusterDlg::COL_GROUP, Qt::AscendingOrder);
	QList<QListWidgetItem*>::iterator groupIt = items.begin();
	QString group = (*groupIt)->text();
	int top = INT_MAX;
	int bottom = INT_MIN;
	auto match = [&](int row) ->bool {
		QTableWidgetItem* tableItem = parent->ui->table->item(row, ClusterDlg::COL_GROUP);;
		bool matched = (group == tableItem->text());
		if (matched) {
			if (top > row)
				top = row;
			if (bottom < row)
				bottom = row;
		}
		return matched;
	};
	int rowCount = parent->ui->table->rowCount();
	for (int row = 0; row < rowCount; ++row) {
		bool matched = match(row);
		if ((!matched || row == rowCount - 1) && top != INT_MAX && bottom != INT_MIN) {
			parent->ui->table->setRangeSelected(QTableWidgetSelectionRange
				{top, ClusterDlg::COL_ID, bottom, ClusterDlg::COL_STATUS}, true);
			top = INT_MAX;
			bottom = INT_MIN;
			if (++groupIt == items.end()) break;
			group = (*groupIt)->text();
			match(row);
		}
	}

	parent->ui->table->setSelectionMode(mode);
	parent->ui->table->blockSignals(false);
	QDialog::accept();
}
