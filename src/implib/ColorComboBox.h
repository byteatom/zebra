#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <vector>

#include <QComboBox>
#include <QColor>

#include "IScreen.h"

class ColorComboBox : public QComboBox
{
public:
	explicit ColorComboBox(QWidget *parent):
		QComboBox{parent}
	{
	}

	virtual ~ColorComboBox() {
		sig.disconnect();
	}

	QRgb index2Argb(int index)
	{
		static const std::vector<QRgb> map {
			QColor(Qt::red).rgb(),
			QColor(Qt::green).rgb(),
			QColor(Qt::yellow).rgb(),
			QColor(Qt::blue).rgb(),
			QColor(Qt::cyan).rgb(),
			QColor(Qt::magenta).rgb(),
			QColor(Qt::white).rgb(),
			QColor(Qt::black).rgb(),
		};

		return map[index];
	}

	void setMode(int mode)
	{
		if (mode > IScreen::BaseColor::THREE || mode == primary)
			return;

		primary = mode;

		int indexPre = currentIndex();
		QRgb rgbPre;
		if (indexPre >= 0) rgbPre = getCurrentArgb();

		blockSignals(true);

		for (int i = count(); i >= 0 ; --i)
			removeItem(i);

		int iconWidth = width()*4/5;
		int iconHeight = height()*3/5;
		QPixmap pixmap(QSize(iconWidth, iconHeight));
		setIconSize(QSize(iconWidth, iconHeight));

		int loop;
		if (mode == IScreen::BaseColor::ONE) {
			loop = 1;
		} else if (mode == IScreen::BaseColor::TWO) {
			loop = 3;
		} else if (mode == IScreen::BaseColor::THREE) {
			loop = 8;
		}

		int newIndex = 0;
		bool preRbgExist = false;
		for (int i = 0; i < loop; ++i) {
			QRgb rgb = index2Argb(i);
			if (indexPre >= 0 && rgb == rgbPre) {
				newIndex = i;
				preRbgExist = true;
			}
			pixmap.fill(rgb);
			addItem(QIcon(pixmap), "");
		}

		setCurrentIndex(newIndex);
		blockSignals(false);
		if (!preRbgExist) emit currentIndexChanged(newIndex);
	}

	void setMode(Attr<int>& mode)
	{
		sig.disconnect();
		auto slot = [this, &mode]() {
			setMode(mode.get());
		};
		sig = mode.connect(slot);
		slot();
	}

	QRgb getCurrentArgb()
	{
		return index2Argb(currentIndex());
	}

	void setCurrentArgb(const QRgb& argb)
	{
		for (int i = 0; i < count(); ++i) {
			if (index2Argb(i) == argb) {
				setCurrentIndex(i);
				break;
			}
		}
	}
private:
	int primary = -1;
	boost::signals2::connection sig;
};

#endif // COLORCOMBOBOX_H
