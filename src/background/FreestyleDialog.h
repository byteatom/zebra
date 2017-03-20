#ifndef FREESTYLEDIALOG_H
#define FREESTYLEDIALOG_H

#include <QDialog>

namespace Ui {
class FreestyleDialog;
}

class FreestyleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FreestyleDialog(QWidget *parent = 0);
    ~FreestyleDialog();

private:
    Ui::FreestyleDialog *ui;
};

#endif // FREESTYLEDIALOG_H
