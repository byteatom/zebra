#include "UtilQt.h"

#include <QCoreApplication>
#include <QPainter>
#include <QImageReader>
#include <QFileDialog>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>
#include <QUuid>

namespace UtilQt
{

const QString scaleText[SCALE_MAX] = {
	QT_TRANSLATE_NOOP("Graph", "Strech"),
	QT_TRANSLATE_NOOP("Graph", "Fill"),
	QT_TRANSLATE_NOOP("Graph", "Fit"),
	QT_TRANSLATE_NOOP("Graph", "Tile"),
	QT_TRANSLATE_NOOP("Graph", "Center")
};

const QString rotateText[ROTATE_MAX] = {
	QT_TRANSLATE_NOOP("Graph", "None"),
	QT_TRANSLATE_NOOP("Graph", "Left"),
	QT_TRANSLATE_NOOP("Graph", "Right"),
	QT_TRANSLATE_NOOP("Graph", "Botton"),
	QT_TRANSLATE_NOOP("Graph", "Horizon"),
	QT_TRANSLATE_NOOP("Graph", "Vertical")
};

const QString filterText[FILTER_MAX] = {
	QT_TRANSLATE_NOOP("Graph", "Monochrome"),
	QT_TRANSLATE_NOOP("Graph", "BiColor"),
	QT_TRANSLATE_NOOP("Graph", "Full")
};

QPixmap scale(QPixmap &source, Scale scale, const QSize targetSize) {
	QPixmap scaled;

	switch (scale) {
	case STRECH:
		scaled = source.scaled(
			targetSize,
			Qt::IgnoreAspectRatio,
			Qt::SmoothTransformation /*Qt::FastTransformation*/);
		break;
	case FILL:
		scaled = source.scaled(targetSize,
							   Qt::KeepAspectRatioByExpanding,
							   Qt::SmoothTransformation);
		break;
	case FIT:
		scaled = source.scaled(
			targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		break;
	case TILE:
		scaled = source;
		break;
	case CENTER:
		scaled = source;
		break;
	default:
		break;
	}

	QPixmap target(targetSize);
	target.fill(QColor::fromRgba(Qt::transparent));
	QPainter painter(&target);

	if (TILE == scale) {
		painter.drawTiledPixmap(target.rect(), scaled);
	} else {
		QSizeF offset = (targetSize - scaled.size()) / 2;
		QPointF pos = QPointF(offset.rwidth(), offset.rheight());
		painter.drawPixmap(pos, scaled);
	}

	return target;
}

QPixmap rotate(QPixmap &source, Rotate rotate)
{
	return QPixmap();
}

QPixmap filter(QPixmap &source, Filter filter)
{
	return QPixmap();
}

QString imageFilters()
{
	QList<QByteArray> formats = QImageReader::supportedImageFormats();
	QString filter{QCoreApplication::translate("UtilQt", "Image")};
	filter.append(" (");
	for (auto& format: formats) {
		filter.append("*.").append(format).append(" ");
	}
	filter.append(")");
	return filter;
}

void raiseQListWidgetItem(QListWidget* list, QListWidgetItem* item)
{
	if (!list || !item) return;
	if (list->count() < 2) return;

	int row = list->row(item);
	if (row == 0) return;
	list->takeItem(row);
	--row;
	list->insertItem(row, item);
}

void raiseQListWidgetCurrentItem(QListWidget* list)
{
	QListWidgetItem* item = list->currentItem();
	if (!item) return;
	raiseQListWidgetItem(list, item);
	list->setCurrentItem(item);
}

void lowerQListWidgetItem(QListWidget* list, QListWidgetItem* item)
{
	if (!list || !item) return;
	if (list->count() < 2) return;

	int row = list->row(item);
	if (row == list->count() - 1) return;
	list->takeItem(row);
	++row;
	list->insertItem(row, item);
}

void lowerQListWidgetCurrentItem(QListWidget* list)
{
	QListWidgetItem* item = list->currentItem();
	if (!item) return;
	lowerQListWidgetItem(list, item);
	list->setCurrentItem(item);
}

QString uuidDir(const QString& prefix)
{
	QString path;
	do {
		path = prefix;
		path.append(QUuid::createUuid().toString());
	} while(QFileInfo(path).exists());
	QDir().mkpath(path);
	return  path;
}

QString uuidFile(const QString& prefix, const QString& suffix)
{
	QString path;
	do {
		path = prefix;
		path.append(QUuid::createUuid().toString()).append(suffix);
	} while(QFileInfo(path).exists());
	QDir().mkpath(QFileInfo(path).absolutePath());
	return path;
}


}
