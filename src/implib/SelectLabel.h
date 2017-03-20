#ifndef SELECTLABEL_H
#define SELECTLABEL_H

#include <functional>
#include <QLabel>
#include <QMouseEvent>
#include <QVariant>

class SelectLabel : public QLabel
{
    Q_OBJECT

public:
    using SelectCall = std::function<bool (SelectLabel*)>;

    explicit SelectLabel(QWidget *parent) : SelectLabel("", parent)
    {
    }

    explicit SelectLabel(const QString &text, QWidget *parent) : QLabel(text, parent)
    {
        setLineWidth(3);
        setFrameShape(QFrame::Box);
        setAlignment(Qt::AlignCenter);
    }

    virtual ~SelectLabel() = default;

    void setSelected(bool selected)
    {
        this->selected = selected;
        changeState();
    }

    bool getSelected()
    {
        return selected;
    }

    void setVariant(const QVariant &variant)
    {
        this->variant = variant;
    }

    QVariant getVariant()
    {
        return variant;
    }


    void setID(int id)
    {
        this->id = id;
    }

    int getID()
    {
        return id;
    }

    void setSelectCall(SelectCall call)
    {
        this->selectCall = call;
    }

protected:
    virtual void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            bool over = false;
            if (selectCall)
                over = selectCall(this);
            if (!over) {
                selected = !selected;
                changeState();
            }
        }
        QLabel::mousePressEvent(event);
        event->ignore();
    }

private:
    void changeState()
    {
        if (selected)
            setStyleSheet("color:rgb(0, 255, 0)");
        else
            setStyleSheet("color:rgb(0, 0, 0)");
    }

    SelectCall selectCall;
    bool selected = false;
    int id;
    QVariant variant;
};

#endif // SELECTLABEL_H
