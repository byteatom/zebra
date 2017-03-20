#ifndef CLOUDDLG_H
#define CLOUDDLG_H

#include <set>
#include <unordered_map>

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QIODevice>
#include <QHttpPart>
#include <QNetworkProxy>
#include <QTableWidgetItem>

#include "UtilQt.h"
#include "GroupDelegate.h"
#include "ScreenDelegate.h"
#include "INodeBox.h"
#include "IScreen.h"
#include "ISetting.h"
#include "IdDelegate.h"

namespace Ui {
class ClusterDlg;
}

class EditorDlg;

class ClusterDlg : public QDialog
{
	Q_OBJECT

public:
	explicit ClusterDlg(QWidget *parent, INodeBox* project, ISetting* setting);
	virtual ~ClusterDlg();

private:
	enum {
		COL_ID,
		COL_NAME,
		COL_GROUP,
		COL_SCREEN,
		COL_COMMENT,
		COL_STATUS,
		COL_MAX
	};

	enum Status {
		STATUS_MIN = 0,
		STATUS_MAX = 100,
		STATUS_BUSY,
		STATUS_FAIL,
		STATUS_STATE
	};

	struct FileInfo{
		const QString path;
		QByteArray data;
	};

	void netUi(bool edit);
	void cmdUi(bool edit);
	void selectedIdItemVars();
	void setRow(QTableWidgetItem* anchorItem, QString cols[]);
	void addRow(QString cols[], int row);
	void addRow(QString cols[]);
	bool idIllformed(const QString& id);
	bool idExist(const QString& id);
	bool checkId(const QString& id);
	bool checkScreen(const QString& screen);
	QString loadFile(const QString& path);
	void load();
	bool saveFile(const QString& path);
	void save();
	void add();
	void edit();
	void del();
	void selGroups();
	void selAll();
	void selLast();
	bool start(bool screenCheck);
	void state();
	void onOff(bool on);
	void on();
	void off();
	void sync();
	void syncDiff();
	void send();
	void upgrade();
	void stop();

	qint64 msecsFrom(const QString& datetime);
	void setStatus(QTableWidgetItem* idItem, Status value, const QString& text = QString());
	void setStatus(const QList<QVariant>& idItemVars, Status value, const QString& text = QString());
	QHttpPart formPart(const QString& name, const QByteArray& body);
	QHttpPart filePart(const QString& fileName);
	void watchReply(QNetworkReply* reply);
	void upload(const QList<QVariant>& idItemVars, const std::list<FileInfo>& fileInfos);
	void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
	void finished();
	void error(QNetworkReply::NetworkError);
	void sslErrors(const QList<QSslError> &errors);

	virtual void reject() override;

	Ui::ClusterDlg *ui;
	ISetting* setting;
	IdDelegate idDelegate;
	std::set<QString> groups;
	GroupDelegate groupDelegate;
	std::unordered_map<QString, IScreen*, UtilQt::QStringHash> screens;
	QSet<QString> ids;
	ScreenDelegate screenDelegate;
	const QString dataPath{"data/cluster.xlsx"};
	const QString host{"http://121.40.110.183"};
	const QString sLogin{"/appLogin"};
	const QString sState{"/getLEDStatus"};
	const QString user{"889fe394-5fbb-4520-9a7f-5c3f369e1e7d"};
	const char* idItemVarsProp = "rows";
	QList<QVariant> idItemVars;
	const QString cmdFileName{"ALL.cmd"};
	QNetworkAccessManager  nam;
	QNetworkProxy proxy{QNetworkProxy::HttpProxy, "127.0.0.1", 8888};
	std::set<QNetworkReply*> replies;

	friend class EditorDlg;
	friend class SelGroupsDlg;
	friend class IdDelegate;
};

#endif // CLOUDDLG_H
