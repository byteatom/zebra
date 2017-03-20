#ifndef FEEDBACKEDITOR_H
#define FEEDBACKEDITOR_H

#include <list>

#include <QTextEdit>
#include <QMimeData>
#include <QStringList>
#include <QHash>

class FeedbackEditor : public QTextEdit
{
	struct HtmlImage{
		QString name;
		QString path;
	};

public:
	explicit FeedbackEditor(QWidget *parent = nullptr);
	virtual ~FeedbackEditor();

	void exportImages(QHash<QString, QString>& images);

private:
	bool hasImagePath(const QMimeData *source) const;
	QStringList imagePaths(const QMimeData *source) const;
	QString imageName();
	void addImage(const QImage& image);
	void addImage(const QString& path);
	bool canInsertFromMimeData(const QMimeData *source) const override;
	void insertFromMimeData(const QMimeData *source) override;

	int imageIndex = 0;
	std::list<HtmlImage> extImages;
	std::list<HtmlImage> tmpImages;
};

#endif // FEEDBACKEDITOR_H
