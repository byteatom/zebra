#ifndef QIPADDREDITOR_H
#define QIPADDREDITOR_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFont>
#include <QLabel>
#include <QKeyEvent>
#include <QRegExpValidator>
#include <QHostAddress>
#include <QStringBuilder>

class QIpAddrEditor : public QFrame
{
	Q_OBJECT
public:
	QIpAddrEditor(QWidget *parent) : QFrame(parent)
	{
		setFrameShape(QFrame::StyledPanel);
		setFrameShadow(QFrame::Plain);
		QPalette pal(palette());
		pal.setColor(QPalette::Active, backgroundRole(), pal.color(QPalette::Base));
		pal.setColor(QPalette::Inactive, backgroundRole(), pal.color(QPalette::Base));
		setPalette(pal);
		setAutoFillBackground(true);

		QHBoxLayout* layout = new QHBoxLayout(this);
		setLayout(layout);
		layout->setContentsMargins(0, 0, 4, 0);
		layout->setSpacing(0);

		for (int i = 0; i < IP_DIGIT_COUNT; ++i)
		{
			if (i != 0)
			{
				QLabel* dot = new QLabel(".", this);
				layout->addWidget(dot);
				layout->setStretch(layout->count(), 0);
				dot->setAlignment(Qt::AlignCenter);
				QFont font = dot->font();
				font.setBold(true);
				dot->setFont(font);
			}

			QLineEdit* digit = new QLineEdit(this);
			digits[i]  = digit;
			layout->addWidget(digit);
			layout->setStretch(layout->count(), 1);
			digit->setFrame(false);
			digit->setAlignment(Qt::AlignCenter);

			QFont font = digit->font();
			font.setStyleHint(QFont::Monospace);
			font.setFixedPitch(true);
			digit->setFont(font);

			QRegExp regexp( "^(0|[1-9]|[1-9][0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))$");
			QValidator *validator = new QRegExpValidator(regexp, digit);
			digit->setValidator(validator);

			connect(digit, &QLineEdit::textChanged,
					this, &QIpAddrEditor::textChanged,
					Qt::QueuedConnection);

			digit->installEventFilter(this);
		}
		digits[0]->setFocus();
	}

	virtual ~QIpAddrEditor() = default;

	void setAddress(const QHostAddress& addr)
	{
		QList<QString> digitList = addr.toString().split('.');
		for (int i = 0; i < IP_DIGIT_COUNT; ++i) {
			digits[i]->setText(digitList[i]);
		}
	}

signals:
	void addrChanged(const QString& addr);

private:
	enum {IP_DIGIT_COUNT = 4};

	virtual bool eventFilter(QObject *obj, QEvent *event){
		bool ret = QFrame::eventFilter(obj, event);

		if (event->type() != QEvent::KeyPress) return ret;
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

		QLineEdit* digit = nullptr;
		int current = 0;
		for (; current < IP_DIGIT_COUNT; ++current)
		{
			if (digits[current] == obj) {
				digit = digits[current];
				break;
			}
		}
		if (!digit) return ret;

		switch (keyEvent->key())
		{
		case Qt::Key_Left:
			if (digit->cursorPosition() == 0) {
				MoveBackward(current);
			}
			break;
		case Qt::Key_Right:
			if (digit->text().isEmpty() || (digit->text().size() == digit->cursorPosition())) {
				moveForward(current);
			}
			break;
		case Qt::Key_0:
			if (digit->text().isEmpty() || digit->text() == "0") {
				digit->setText("0");
				moveForward(current);
			}
			break;
		case Qt::Key_Backspace:
			if (digit->text().isEmpty() || digit->cursorPosition() == 0) {
				MoveBackward(current);
			}
			break;
		case Qt::Key_Comma:
		case Qt::Key_Period:
			if (!digit->text().isEmpty()) {
				moveForward(current);
			}
			break;
		default:
			break;
		}

		return ret;
	}

	void textChanged(const QString& text)
	{
		QString addr;
		addr.reserve(16);
		for (int i = 0; i < IP_DIGIT_COUNT; ++i) {
			QString digit = digits[i]->text();
			addr.append(digit.isEmpty() ? "0" : digit);
			if (i < IP_DIGIT_COUNT - 1) addr.append('.');
		}
		emit addrChanged(addr);

		QLineEdit* digit = static_cast<QLineEdit*>(sender());
		int current = 0;
		for (; current < IP_DIGIT_COUNT; ++current)
		{
			if (digit == digits[current]) break;
		}
		if (current >= IP_DIGIT_COUNT - 1) return;

		if ((digit->text().size() == 3
			 &&  digit->cursorPosition() == 3)
			|| (digit->text() == "0"))
		{
			moveForward(current);
		}
	}

	void moveForward(int current)
	{
		int next = current + 1;
		if (next > IP_DIGIT_COUNT - 1) return;
		digits[next]->setFocus();
		//digits[next]->setCursorPosition(0);
		digits[next]->selectAll();
	}

	void MoveBackward(int current)
	{
		int pre = current - 1;
		if (pre < 0) return;
		digits[pre]->setFocus();
		//digits[pre]->setCursorPosition(digits[pre]->text().size());
		digits[pre]->selectAll();
	}

	QLineEdit* digits[IP_DIGIT_COUNT];
};

#endif // QIPADDREDITOR_H
