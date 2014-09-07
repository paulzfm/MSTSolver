#ifndef EDIT_POINT_DIALOG_H
#define EDIT_POINT_DIALOG_H

#include <QDialog>
#include "mst/util.h"

namespace Ui {
class EditPointDialog;
}

class EditPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPointDialog(QWidget *parent, std::vector< mst::Point<float> >& currentPoints,
                             int index);
    ~EditPointDialog();

    mst::Point<float> getPoint();

private slots:
    void edit();
    void clear();

private:
    Ui::EditPointDialog *ui;
    int currPoint;
    mst::Point<float> point;
    std::vector< mst::Point<float> > curr;
};

#endif // EDIT_POINT_DIALOG_H
