#ifndef SCALE_SET_DIALOG_H
#define SCALE_SET_DIALOG_H

#include <QDialog>
#include "qutil.h"

namespace Ui {
class ScaleSetDialog;
}

class ScaleSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleSetDialog(QWidget *parent = 0);
    ~ScaleSetDialog();
    int getPos();

private slots:
    void updateLabel(int pos);
    void setPos(int percent);

private:
    Ui::ScaleSetDialog *ui;
};

#endif // SCALE_SET_DIALOG_H
