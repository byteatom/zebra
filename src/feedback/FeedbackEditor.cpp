#include "FeedbackEditor.h"

#include <QImageReader>
#include <QFile>

#include "Feedback.h"
#include "UtilQt.h"

FeedbackEditor::FeedbackEditor(QWidget *parent) : QTextEdit{parent}
{
}

FeedbackEditor::~FeedbackEditor()
{
	for (HtmlImage& image: tmpImages) {
		QFile::remove(image.path);
	}
}

void FeedbackEditor::exportImages(QHash<QString, QString>& images)
{
	for (HtmlImage& image: extImages) {
		images[image.name] = image.path;
	}
	for (HtmlImage& image: tmpImages) {
		images[image.name] = image.path;
	}
}

bool FeedbackEditor::hasImagePath(const QMimeData *source) const
{
	if (!source->hasUrls()) return false;

	QList<QUrl> urls = source->urls();
	for (auto& url: urls) {
		if (!url.isLocalFile()) continue;
		if (!QImageReader::imageFormat(url.toLocalFile()).isEmpty()) {
			return true;
		}
	}

	return false;
}

QStringList FeedbackEditor::imagePaths(const QMimeData *source) const
{
	QStringList paths;

	QList<QUrl> urls = source->urls();
	for (auto& url: urls) {
		QString path = url.toLocalFile();
		if (!QImageReader::imageFormat(path).isEmpty())
			paths.push_back(path);
	}

	return paths;
}

QString FeedbackEditor::imageName()
{
	QString name{"cid:image"};
	name.append(QString::number(imageIndex++));
	return name;
}

void FeedbackEditor::addImage(const QImage& image)
{
	QString name = imageName();

	textCursor().insertImage(image, name);

	QString path = UtilQt::uuidFile(Feedback::setting->tempDir(), ".png");
	image.save(path);

	tmpImages.push_back({name, path});
}

void FeedbackEditor::addImage(const QString& path)
{
	QString name = imageName();

	textCursor().insertImage(QImage{path}, name);

	extImages.push_back({name, path});
}

bool FeedbackEditor::canInsertFromMimeData(const QMimeData *source) const
{
	if (source->hasImage() || hasImagePath(source))
		return true;
	else
		return QTextEdit::canInsertFromMimeData(source);
}

void FeedbackEditor::insertFromMimeData(const QMimeData *source)
{
	if (source->hasImage())	{
		QImage image = qvariant_cast<QImage>(source->imageData());
		addImage(image);
	} else if (hasImagePath(source)) {
		QStringList paths = imagePaths(source);
		for (auto& path: paths)
			addImage(path);
	} else
		QTextEdit::insertFromMimeData(source);
}
