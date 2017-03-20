#include "ClusterDlg.h"
#include "ui_ClusterDlg.h"

#include <algorithm>
#include <climits>
#include <chrono>
namespace std { namespace chrono {typedef duration<int, ratio<86400> > days;}}

#include <boost/multiprecision/cpp_int.hpp>

#include <QStyleFactory>
#include <QMessageBox>
#include <QProgressBar>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QDir>
#include <QDirIterator>
#include <QUuid>
#include <QSet>

#include "EditorDlg.h"
#include "xlsxdocument.h"
#include "xlsxworksheet.h"
#include "Message.h"
#include "Json.h"
#include "StatusItem.h"
#include "INode.h"
#include "SelGroupsDlg.h"
#include "ImportInfo.h"
#include "UtilQt.h"

Q_DECLARE_METATYPE(QTableWidgetItem*)

ClusterDlg::ClusterDlg(QWidget *parent, INodeBox* project, ISetting* setting) :
	QDialog(parent),
	ui(new Ui::ClusterDlg),
	setting{setting},
	idDelegate{this},
	groupDelegate{groups},
	screenDelegate{screens}
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowIcon(QIcon(QPixmap("image/icon/cluster_512.png")));

	ui->table->setStyle(QStyleFactory::create("Fusion"));
	ui->table->setColumnWidth(COL_ID, 270);
	ui->table->setItemDelegateForColumn(COL_ID, &idDelegate);
	ui->table->setColumnWidth(COL_NAME, 150);
	ui->table->setColumnWidth(COL_GROUP, 150);
	ui->table->setItemDelegateForColumn(COL_GROUP, &groupDelegate);
	ui->table->setColumnWidth(COL_SCREEN, 150);
	ui->table->setItemDelegateForColumn(COL_SCREEN, &screenDelegate);
	ui->table->verticalHeader()->setFixedWidth(40);
	ui->table->horizontalHeader()->setSectionResizeMode(COL_COMMENT, QHeaderView::Stretch);

	for (std::shared_ptr<void> pos; INode* node = project->next(pos);) {
		screens[node->name()] = dynamic_cast<IScreen*>(node);
	}

	loadFile(dataPath);

	//nam.setProxy(proxy);

	connect(ui->load, &QPushButton::clicked, this, &ClusterDlg::load);
	connect(ui->save, &QPushButton::clicked, this, &ClusterDlg::save);
	connect(ui->add, &QPushButton::clicked, this, &ClusterDlg::add);
	connect(ui->edit, &QPushButton::clicked, this, &ClusterDlg::edit);
	connect(ui->del, &QPushButton::clicked, this, &ClusterDlg::del);
	connect(ui->selGroups, &QPushButton::clicked, this, &ClusterDlg::selGroups);
	connect(ui->selAll, &QPushButton::clicked, this, &ClusterDlg::selAll);
	connect(ui->selLast, &QPushButton::clicked, this, &ClusterDlg::selLast);
	connect(ui->state, &QPushButton::clicked, this, &ClusterDlg::state);
	connect(ui->on, &QPushButton::clicked, this, &ClusterDlg::on);
	connect(ui->off, &QPushButton::clicked, this, &ClusterDlg::off);
	connect(ui->sync, &QPushButton::clicked, this, &ClusterDlg::sync);
	connect(ui->syncDiff, &QPushButton::clicked, this, &ClusterDlg::syncDiff);
	connect(ui->send, &QPushButton::clicked, this, &ClusterDlg::send);
	connect(ui->upgrade, &QPushButton::clicked, this, &ClusterDlg::upgrade);
	connect(ui->stop, &QPushButton::clicked, this, &ClusterDlg::stop);
	connect(ui->close, &QPushButton::clicked, this, &ClusterDlg::reject);

	netUi(false);

	QNetworkRequest request{QUrl{host + sLogin}};
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	Json json;
	json["username"] = user;
	QNetworkReply* reply = nam.post(request, json.dump().c_str());
	watchReply(reply);
}

ClusterDlg::~ClusterDlg()
{
	delete ui;
}

void ClusterDlg::cmdUi(bool enable)
{
	ui->table->setEditTriggers(enable
			? QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed
			: QAbstractItemView::NoEditTriggers);
	ui->del->setEnabled(enable);
	ui->edit->setEnabled(enable);
	ui->add->setEnabled(enable);
	ui->load->setEnabled(enable);
	netUi(enable);
}

void ClusterDlg::netUi(bool enable)
{
	ui->state->setEnabled(enable);
	ui->on->setEnabled(enable);
	ui->off->setEnabled(enable);
	ui->sync->setEnabled(enable);
	ui->syncDiff->setEnabled(enable);
	ui->send->setEnabled(enable);
	ui->syncDiff->setEnabled(enable);
}

void ClusterDlg::selectedIdItemVars()
{
	idItemVars.clear();
	QList<QTableWidgetItem*> items = ui->table->selectedItems();
	for (auto item: items) {
		if (item->column() == COL_ID) idItemVars.push_back(QVariant::fromValue(item));
	}
}

void ClusterDlg::setRow(QTableWidgetItem* anchorItem, QString cols[])
{
	for (int col = 0; col < COL_STATUS; ++col) {
		QTableWidgetItem* item = ui->table->item(anchorItem->row(), col);
		if (col == COL_GROUP) groups.insert(cols[COL_GROUP]);
		if (col == COL_ID) {
			ids.remove(item->text());
			ids.insert(cols[COL_ID]);
		}
		if (col == COL_SCREEN && screens.find(cols[col]) == screens.end())
			item->setText("");
		else
			item->setText(cols[col]);
	}
}

void ClusterDlg::addRow(QString cols[], int row)
{
	StatusItem* anchorItem = new StatusItem;
	ui->table->setItem(row, COL_STATUS, anchorItem);

	for (int col = 0; col < COL_STATUS; ++col) {
		ui->table->setItem(anchorItem->row(), col , new QTableWidgetItem);
	}

	setRow(anchorItem, cols);
}

void ClusterDlg::addRow(QString cols[])
{
	int row = ui->table->rowCount();
	ui->table->insertRow(row);
	addRow(cols, row);
}

bool ClusterDlg::idIllformed(const QString& id)
{
	QUuid uuid{id};
	return uuid.isNull();
}

bool ClusterDlg::idExist(const QString& id)
{
	return ids.contains(id);
	/*for (int row = 0; row < ui->table->rowCount(); ++row) {
		if (row == skipRow) continue;
		if (ui->table->item(row, COL_ID)->text() == id) {
			return true;
		}
	}
	return false;*/
}

bool ClusterDlg::checkId(const QString& id)
{
	if (id.isEmpty()) {
		QMessageBox::warning(this, tr("Warning"),
						tr("Some LED ID of selected items are empty"));
		return false;
	}
	return true;
}

bool ClusterDlg::checkScreen(const QString& screen)
{
	if (screen.isEmpty() || screens.find(screen) == screens.end()) {
		QMessageBox::warning(this, tr("Warning"),
						tr("The Screen of some selected items doesn't exists"));
		return false;
	}
	return true;
}

QString ClusterDlg::loadFile(const QString& path)
{
	ui->table->setSortingEnabled(false);
	ui->table->blockSignals(true);

	QXlsx::Document xlsx{path};
	QXlsx::Worksheet* sheet = xlsx.currentWorksheet();
	if (!sheet) return QString();
	QString result{tr("The format of LED ID should be:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx")};
	result.append("\r\n");
	QXlsx::CellRange range = sheet->dimension();
	int existRows = ui->table->rowCount();
	ui->table->setRowCount(existRows + range.rowCount());
	int tableRow = existRows;
	int readedRows = 0;
	for (int excelRow = range.firstRow(); excelRow <= range.lastRow(); ++excelRow) {
		QString cols[COL_STATUS];
		for (int col = 0; col < COL_STATUS; ++col)
			cols[col] = sheet->read(excelRow, col + 1).toString().trimmed();
		if (idIllformed(cols[COL_ID])) {
			result.append("\r\n").append(tr("Line %1:	invalid format").arg(excelRow));
			continue;
		}
		if (idExist(cols[COL_ID])) {
			result.append("\r\n").append(tr("Line %1:	duplicated").arg(excelRow));
			continue;
		}
		addRow(cols, tableRow);
		++tableRow;
		++readedRows;
	}

	ui->table->setRowCount(existRows + readedRows);
	ui->table->setSortingEnabled(true);
	ui->table->blockSignals(false);
	ui->table->setCurrentCell(ui->table->rowCount() - 1, 0);
	QString stat{readedRows < 2 ?
					tr("%1 device has been imported.")
				  : tr("%1 devices have been imported.")};
	result.prepend("\r\n").prepend(stat.arg(readedRows));
	return result;
}

void ClusterDlg::load()
{
	QString path = QFileDialog::getOpenFileName(this,
										tr("Import"),
										"",
										tr("Microsoft Excel (*.xlsx)"));
	if (path.isEmpty()) return;
	QString ret = loadFile(path);
	if (ret.isEmpty()) {
		QMessageBox::critical(this, tr("Error"),
						tr("Failed to read file"));
	} else {
		ImportInfo dlg{this, ret};
		dlg.exec();
	}
	ui->table->setFocus();
	return;
}

bool ClusterDlg::saveFile(const QString& path)
{
	ui->table->setCurrentItem(nullptr);
	QXlsx::Document xlsx;
	for (int row = 0; row < ui->table->rowCount(); ++row) {
		int xlsxRow = row + 1;
		for (int col = 0; col < COL_STATUS; ++col) {
			int xlsxCol = col + 1;
			xlsx.write(xlsxRow, xlsxCol, ui->table->item(row, col)->text());
		}
	}
	xlsx.saveAs(path);
	return true;
}

void ClusterDlg::save()
{
	QString path = QFileDialog::getSaveFileName(this,
										tr("Import"),
										"",
										tr("Microsoft Excel (*.xlsx)"));
	if (path.isEmpty()) return;
	if (!saveFile(path)) {
		QMessageBox::critical(this, tr("Error"),
						tr("Failed to save file"));
	}
	ui->table->setFocus();
	return;
}


void ClusterDlg::add()
{
	EditorDlg editor{this, true};
	editor.exec();
	ui->table->setFocus();
}

void ClusterDlg::edit()
{
	if (ui->table->selectedRanges().isEmpty()) {
		return;
	}
	EditorDlg editor{this, false};
	editor.exec();
	ui->table->setFocus();
}

void ClusterDlg::del()
{
	selectedIdItemVars();

	if (idItemVars.empty() || QMessageBox::Yes != QMessageBox::question(this,
							tr("Confirm"),
							tr("Delete selected %1 items?").arg(idItemVars.size()))) {
		ui->table->setFocus();
		return;
	}

	for (auto& idItemVar: idItemVars) {
		int row = idItemVar.value<QTableWidgetItem*>()->row();
		ids.remove(ui->table->item(row, COL_ID)->text());
		ui->table->removeRow(row);
	}
	ui->table->setFocus();
}

void ClusterDlg::selGroups()
{
	SelGroupsDlg dlg{this};
	dlg.exec();
	ui->table->setFocus();
}

void ClusterDlg::selAll()
{
	ui->table->selectAll();
	ui->table->setFocus();
}

void ClusterDlg::selLast()
{
	ui->table->clearSelection();
	QAbstractItemView::SelectionMode mode = ui->table->selectionMode();
	ui->table->setSelectionMode(QAbstractItemView::MultiSelection);
	for (auto& idItemVar: idItemVars) {
		ui->table->selectRow(idItemVar.value<QTableWidgetItem*>()->row());
	}
	ui->table->setSelectionMode(mode);
	ui->table->setFocus();
}

bool ClusterDlg::start(bool screenCheck)
{
	replies.clear();

	selectedIdItemVars();

	if (idItemVars.isEmpty()) {
		QMessageBox::warning(this, tr("Warning"),
						tr("Please select some items to operate on"));
		return false;
	}

	for (auto& idItemVar: idItemVars) {
		QTableWidgetItem* idItem = idItemVar.value<QTableWidgetItem*>();
		if (!checkId(idItem->text())) {
			return false;
		}
		if (screenCheck && !checkScreen(ui->table->item(idItem->row(), COL_SCREEN)->text())) {
			return false;
		}
	}

	cmdUi(false);

	for (auto& idItemVar: idItemVars) {
		setStatus(idItemVar.value<QTableWidgetItem*>(), STATUS_MIN);
	}

	return true;
}

void ClusterDlg::state()
{
	if (!start(false)) return;

	Json json;
	json["username"] = user;

	for (auto& idItemVar: idItemVars) {
		QNetworkRequest request{QUrl{host + sState}};
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
		json["ledid"] = idItemVar.value<QTableWidgetItem*>()->text();
		QNetworkReply* reply = nam.post(request, json.dump().c_str());
		watchReply(reply);
		replies.insert(reply);
		reply->setProperty(idItemVarsProp, QList<QVariant>{idItemVar});
	}
	if (replies.empty()) cmdUi(true);
	ui->table->setFocus();
}

void ClusterDlg::onOff(bool on)
{
	ui->table->setFocus();
	if (!start(false)) return;

	DisplaySetReqMsg msg;
	msg.setOpenClose(on);

	upload(idItemVars, {{cmdFileName, QByteArray{msg.buf(), msg.size()}}});

	if (replies.empty()) cmdUi(true);
}

void ClusterDlg::on()
{
	onOff(true);
}

void ClusterDlg::off()
{
	onOff(false);
}

void ClusterDlg::sync()
{
	ui->table->setFocus();
	if (!start(true)) return;

	auto hasher = [](const IScreen::SyncTime& key) -> size_t const {
		return qHash(key.date.toString() + key.time.toString());
	};
	auto equaler = [](const IScreen::SyncTime& first, const IScreen::SyncTime& second) -> bool const {
		return first.date == second.date && first.time == second.time;
	};

	QList<QVariant> autoIdItemVars;
	std::unordered_map<IScreen::SyncTime, QList<QVariant>, decltype(hasher), decltype(equaler)> manualIdItemVarsMap{10, hasher, equaler};

	for (auto& idItemVar: idItemVars) {
		QString scrrenName = ui->table->item(idItemVar.value<QTableWidgetItem*>()->row(), COL_SCREEN)->text();
		IScreen* screen = screens.find(scrrenName)->second;
		IScreen::SyncTime& sync = screen->syncTime();
		if (sync.systemSel)
			autoIdItemVars.push_back(idItemVar);
		else
			manualIdItemVarsMap[sync].push_back(idItemVar);
	}

	DisplaySetReqMsg msgAuto;
	msgAuto.setDateTimeNow();
	upload(autoIdItemVars, {{cmdFileName, QByteArray{msgAuto.buf(), msgAuto.size()}}});

	for (auto& manualIdItemVars: manualIdItemVarsMap) {
		DisplaySetReqMsg msgManual;
		msgManual.setDate(manualIdItemVars.first.date);
		msgManual.setTime(manualIdItemVars.first.time);
		upload(manualIdItemVars.second, {{cmdFileName, QByteArray{msgManual.buf(), msgManual.size()}}});
	}

	if (replies.empty()) cmdUi(true);
}

void ClusterDlg::syncDiff()
{
	ui->table->setFocus();
	if (!start(true)) return;

	auto hasher = [](const IScreen::SyncTime& key) -> size_t const {
		return qHash(key.date.toString() + key.time.toString());
	};
	auto equaler = [](const IScreen::SyncTime& first, const IScreen::SyncTime& second) -> bool const {
		return first.date == second.date && first.time == second.time;
	};

	QList<QVariant> autoIdItemVars;
	std::unordered_map<IScreen::SyncTime, QList<QVariant>, decltype(hasher), decltype(equaler)> manualIdItemVarsMap{10, hasher, equaler};

	for (auto& idItemVar: idItemVars) {
		QString scrrenName = ui->table->item(idItemVar.value<QTableWidgetItem*>()->row(), COL_SCREEN)->text();
		IScreen* screen = screens.find(scrrenName)->second;
		IScreen::SyncTime& sync = screen->syncTime();
		if (sync.systemSel)
			autoIdItemVars.push_back(idItemVar);
		else
			manualIdItemVarsMap[sync].push_back(idItemVar);
	}

	RelativeTimeSetReqMsg msgAuto;
	msgAuto.setTime(ui->diff->time());
	upload(autoIdItemVars, {{cmdFileName, QByteArray{msgAuto.buf(), msgAuto.size()}}});

	for (auto& manualIdItemVars: manualIdItemVarsMap) {
		DisplaySetReqMsg msgManual;
		msgManual.setDate(manualIdItemVars.first.date);
		msgManual.setTime(manualIdItemVars.first.time);
		upload(manualIdItemVars.second, {{cmdFileName, QByteArray{msgManual.buf(), msgManual.size()}}});
	}

	if (replies.empty()) cmdUi(true);
}

void ClusterDlg::send()
{
	ui->table->setFocus();
	if (!start(true)) return;

	std::unordered_map<QString, QList<QVariant>, UtilQt::QStringHash> screensIdItemVars;

	for (auto& idItemVar: idItemVars) {
		QString scrrenName = ui->table->item(idItemVar.value<QTableWidgetItem*>()->row(), COL_SCREEN)->text();
		screensIdItemVars[scrrenName].push_back(idItemVar);
	}

	for (auto& screenIdItemVars: screensIdItemVars) {
		IScreen* screen = screens.find(screenIdItemVars.first)->second;

		QString dir = UtilQt::uuidDir(setting->tempDir());

		QString ret = screen->buildFiles(dir, false);
		if (!ret.isEmpty()) {
			setStatus(screenIdItemVars.second, STATUS_FAIL);
			continue;
		}

		std::list<FileInfo> fileInfos;
		QDirIterator dirIt(dir, QDir::Files);
		while (dirIt.hasNext()) {
			QString path{dirIt.next()};
			QString name = dirIt.fileName();
			if (FileMsgFactory::isScf(name)
				|| FileMsgFactory::isSef(name)
				|| FileMsgFactory::isDcf(name)
				|| FileMsgFactory::isRcf(name))
			{
				fileInfos.push_back({path, nullptr});
			}
		}
		upload(screenIdItemVars.second, fileInfos);
	}

	if (replies.empty()) cmdUi(true);
}

void ClusterDlg::upgrade()
{
	QStringList paths = QFileDialog::getOpenFileNames(this,
										tr("Upgrade"));
	if (paths.isEmpty()) return;

	ui->table->setFocus();
	if (!start(false)) return;

	std::list<FileInfo> fileInfos;
	for (auto& path: paths)
		fileInfos.push_back({path, nullptr});

	upload(idItemVars, fileInfos);

	if (replies.empty()) cmdUi(true);
}

void ClusterDlg::stop()
{
	ui->table->setFocus();
	for (QNetworkReply* reply: replies) {
		reply->abort();
	}
}

qint64 ClusterDlg::msecsFrom(const QString& datetime)
{
	QDateTime from = QDateTime::fromString(datetime, Qt::ISODate);
	//from.setTimeSpec(Qt::LocalTime);
	QDateTime now = QDateTime::currentDateTime();

	return from.msecsTo(now);
}

void ClusterDlg::setStatus(QTableWidgetItem* idItem, Status status, const QString& text)
{
	int row = idItem->row();
	if (nullptr == ui->table->cellWidget(row, COL_STATUS)) {
		QProgressBar *bar = new QProgressBar;
		bar->setMinimum(STATUS_MIN);
		bar->setMaximum(STATUS_MAX);
		ui->table->setCellWidget(row, COL_STATUS, bar);
	}

	QTableWidgetItem* item = ui->table->item(row, COL_STATUS);
	QProgressBar* bar = qobject_cast<QProgressBar*>(ui->table->cellWidget(item->row(), COL_STATUS));

	QColor bgColor{Qt::white};
	if (status == STATUS_FAIL) {
		bgColor = Qt::red;
	} else if (status == STATUS_STATE) {
		bgColor = Qt::green;
	}

	QColor chunkColor{Qt::gray};
	if (status == STATUS_MAX || status == STATUS_STATE) {
		chunkColor = Qt::green;
	} else if (status == STATUS_FAIL) {
		chunkColor = Qt::red;
	}

	QString style = QString("QProgressBar {border:none; text-align: center; background-color: rgb(%1, %2, %3);}"
				 "QProgressBar::chunk {background-color: rgb(%4, %5, %6);}")
					.arg(QString::number(bgColor.red())
						,QString::number(bgColor.green())
						,QString::number(bgColor.blue())
						,QString::number(chunkColor.red())
						,QString::number(chunkColor.green())
						,QString::number(chunkColor.blue()));
	bar->setStyleSheet(style);

	QString format{"%p%"};
	if (status == STATUS_FAIL) {
		format = tr("error");
	} else if (status == STATUS_STATE) {
		std::chrono::milliseconds mils{msecsFrom(text)};
		std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(mils);
		std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);
		std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(minutes);
		std::chrono::days days = std::chrono::duration_cast<std::chrono::days>(hours);

		if (days.count() > 1) {
			format = QString::number(days.count()).append(tr(" days "));
		} else if (days.count() == 1) {
			format = QString::number(days.count()).append(tr(" day "));
		} else if (hours.count() > 1) {
			format = QString::number(hours.count()).append(tr(" hours "));
		} else if (hours.count() == 1) {
			format = QString::number(hours.count()).append(tr(" hour "));
		} else if (minutes.count() > 1) {
			format = QString::number(minutes.count()).append(tr(" minutes "));
		} else if (minutes.count() == 1) {
			format = QString::number(minutes.count()).append(tr(" minute "));
		} else if (seconds.count() > 1) {
			format = QString::number(seconds.count()).append(tr(" seconds "));
		} else if (seconds.count() <= 1) {
			format = QString::number(seconds.count()).append(tr(" second "));
		}
		format.append(tr("ago"));
	}
	bar->setFormat(format);

	bar->setMaximum(STATUS_MAX);
	if (status >= STATUS_MIN && status <= STATUS_MAX) {
		bar->setValue(status);
	} else if (status == STATUS_BUSY) {
		bar->setMaximum(STATUS_MIN);
	}

	if (status == STATUS_BUSY) {
		item->setText(QString::number(STATUS_MAX/2));
	} else if (status == STATUS_STATE) {
		item->setText(QString::number(msecsFrom(text)));
	} else {
		item->setText(QString::number(status));
	}
}

void ClusterDlg::setStatus(const QList<QVariant>& idItemVars, Status status, const QString& text)
{
	for (const QVariant& idItemVar: idItemVars) {
		setStatus(idItemVar.value<QTableWidgetItem*>(), status, text);
	}
}

QHttpPart ClusterDlg::formPart(const QString& name, const QByteArray& body)
{
	QHttpPart part;
	part.setHeader(QNetworkRequest::ContentDispositionHeader,
				   "form-data; name=\"" + name +"\"");
	part.setBody(body);
	return part;
}

QHttpPart ClusterDlg::filePart(const QString& fileName)
{
	QHttpPart part;
	part.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	part.setHeader(QNetworkRequest::ContentDispositionHeader,
				   "form-data; name=\"InputFiles\"; filename=\"" + fileName + "\"");
	return part;
}

void ClusterDlg::watchReply(QNetworkReply* reply)
{
	connect(reply, &QNetworkReply::uploadProgress,
			this, &ClusterDlg::uploadProgress);
	connect(reply, &QNetworkReply::finished,
			this, &ClusterDlg::finished);
	connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
			this, &ClusterDlg::error);
	connect(reply, &QNetworkReply::sslErrors,
			this, &ClusterDlg::sslErrors);
}

void ClusterDlg::upload(const QList<QVariant>& idItemVars, const std::list<FileInfo>& fileInfos)
{
	if (idItemVars.empty() || fileInfos.empty()) return;

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	multiPart->append(formPart("username", user.toLatin1()));

	QByteArray idsBody;
	for (const QVariant& idItemVar: idItemVars) {
		idsBody.append(idItemVar.value<QTableWidgetItem*>()->text().toLatin1()).append(',');
	}
	idsBody.chop(1);
	multiPart->append(formPart("ledlist", idsBody));

	for (const FileInfo& info: fileInfos) {
		QHttpPart part = filePart(info.data.isEmpty() ? QFileInfo(info.path).fileName() : info.path);
		if (info.data.isEmpty()) {
			QFile* file = new QFile{info.path, multiPart};
			if (!file->open(QIODevice::ReadOnly)) {
				delete file;
				continue;
			}
			part.setBodyDevice(file);
		} else {
			part.setBody(info.data);
		}
		multiPart->append(part);
	}

	QNetworkRequest request{QUrl{host + "/mediaUpload"}};
	QNetworkReply* reply = nam.post(request, multiPart);
	watchReply(reply);
	replies.insert(reply);
	reply->setProperty(idItemVarsProp, idItemVars);
	multiPart->setParent(reply);
}

void ClusterDlg::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (!reply) return;
	QList<QVariant> idItemVars = reply->property(idItemVarsProp).toList();

	int status;
	if (bytesTotal == -1) {
		status = STATUS_BUSY;
	} else if (bytesTotal == bytesSent) {
		status = STATUS_MAX;
	} else if (bytesTotal == 0) {
		status = STATUS_MAX;
	} else if (bytesSent <= 92233720368547758){
		status = bytesSent * 100 / bytesTotal;
	} else {
		status = (int)(boost::multiprecision::int128_t(bytesSent) * 100 / bytesTotal);
	}
	setStatus(idItemVars, (Status)status);
}

void ClusterDlg::finished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (!reply) return;

	QString path = reply->url().path();
	QNetworkReply::NetworkError error = reply->error();
	QString strError = reply->errorString();
	QByteArray body = reply->readAll();
	QStr resultError;
	Json resultJson;
	if (!body.isEmpty()) {
		Json json = Json::parse(body.constData());
		Json::iterator it = json.find("result");
		if (it != json.end()) {
			if (error != QNetworkReply::NoError) {
				resultError = *it;
				strError.append(" [").append(resultError).append("]");
			} else {
				resultJson = *it;
			}
		}
	}

	if (path == sLogin) {
		if (error != QNetworkReply::NoError && resultError != "The user has logged in, please log out first") {
			QMessageBox::critical(this, tr("Error"), strError);
		} else {
			netUi(true);
		}
	} else {
		QList<QVariant> idItemVars = reply->property(idItemVarsProp).toList();
		if (error != QNetworkReply::NoError) {
			QMessageBox::critical(this, tr("Error"), strError);
			setStatus(idItemVars, STATUS_FAIL);
		} else {
			if (path == sState) {
				Json::iterator it = resultJson.find("heartbeat_at");
				if (it->is_string()) {
					setStatus(idItemVars, STATUS_STATE, QStr(*it));
				} else
					setStatus(idItemVars, STATUS_FAIL);
			} else
				setStatus(idItemVars, STATUS_MAX);
		}

		replies.erase(reply);
		if (replies.empty()) {
			cmdUi(true);
		}
	}

	reply->deleteLater();
}

void ClusterDlg::error(QNetworkReply::NetworkError)
{
}

void ClusterDlg::sslErrors(const QList<QSslError> &errors)
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (!reply) return;
	reply->ignoreSslErrors(errors);
	reply->ignoreSslErrors();
}

void ClusterDlg::reject()
{
	QMessageBox::StandardButton btn = QMessageBox::question(this,
				tr("Save"),
				tr("Save changes?"),
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	if (btn == QMessageBox::Save)
		saveFile(dataPath);
	if (btn != QMessageBox::Cancel)
		QDialog::reject();
}
