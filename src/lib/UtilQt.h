#ifndef UTILQT_H
#define UTILQT_H

#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QString>
#include <QHash>

class QListWidget;
class QListWidgetItem;

namespace UtilQt
{

struct QStringHash {
	size_t operator()(const QString& key) const {
		return qHash(key);
	}
};

typedef enum {
	STRECH,
	FILL,
	FIT,
	TILE,
	CENTER,
	SCALE_MAX
}Scale;

extern const QString scaleText[SCALE_MAX];

QPixmap scale(QPixmap &source, Scale scale, const QSize targetSize);

typedef enum {
	TOP,
	LEFT,
	RIGHT,
	BOTTOM,
	HORIZON,
	VERTICAL,
	ROTATE_MAX
}Rotate;

extern const QString rotateText[ROTATE_MAX];

QPixmap rotate(QPixmap &source, Rotate rotate);

typedef enum {
	MONO,
	BI,
	FULL,
	FILTER_MAX
}Filter;

extern const QString filterText[FILTER_MAX];

QPixmap filter(QPixmap &source, Filter filter);

QString imageFilters();

void raiseQListWidgetItem(QListWidget* list, QListWidgetItem* item);
void raiseQListWidgetCurrentItem(QListWidget* list);
void lowerQListWidgetItem(QListWidget* list, QListWidgetItem* item);
void lowerQListWidgetCurrentItem(QListWidget* list);
QString uuidDir(const QString& prefix);
QString uuidFile(const QString& prefix, const QString& suffix);

}

#endif // UTILQT_H
