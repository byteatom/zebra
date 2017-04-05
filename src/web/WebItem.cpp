#include "WebItem.h"

#include <boost/filesystem/fstream.hpp>
namespace bfs = boost::filesystem;

#include <QImageReader>
#include <QPixmap>
#include <QDir>
#include <QFileInfo>

static const QString indexFile{"/index.html"};

WebItems WebItem::origin()
{
	WebItems items;

	QString base{"web/"};
	QDir baseDir{base};
	QStringList list = baseDir.entryList(QDir::QDir::AllDirs | QDir::NoDotAndDotDot);
	for (const QString& name: list) {
		items.emplace_back(base + name);
	}

	return items;
}

WebItem::WebItem(Json* jnode)
{
	if (jnode) {
		QStr path;
		jtov(path);
		jtov(duration);

		QUrl url{path};
		dir = QFileInfo(url.path()).dir().path();
		query = QUrlQuery{url};
	}
}

WebItem::WebItem(const QString& dir) : dir{dir}
{
}

void WebItem::exportProjJson(Json& jnode) const
{
	QStr path =  dir + indexFile;
	if (!query.isEmpty()) path.append("?").append(query.query());
	vtoj(path);
	vtoj(duration);
}

QIcon WebItem::thumb() const
{
	QList<QByteArray> formats = QImageReader::supportedImageFormats();
	for (auto& format: formats) {
		QString thumbPath = dir + "/thumb." + format;
		QPixmap pix{thumbPath};
		if (pix.isNull()) continue;
		return QIcon{pix.scaled(thumbWidth, thumbHeight)};
	}
}

QUrl WebItem::url() const
{
	return  QUrl::fromLocalFile(QDir(dir).absolutePath() + indexFile);
}

QUrl WebItem::uri() const
{
	QUrl uri = url();
	uri.setQuery(query);
	return uri;
}

Json WebItem::queryInfo() const
{
	Json json;

	bfs::ifstream ifs{bfs::path{static_cast<std::string>(dir.toStdString() + "/query.json")},
					  std::ios::in | std::ios::binary};
	if (ifs.is_open()) {
		ifs >> json;
	}

	return json;
}
