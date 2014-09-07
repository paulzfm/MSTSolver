#ifndef ADD_POINT_DIALOG_H
#define ADD_POINT_DIALOG_H

#include <QDialog>
#include "mst/util.h"

namespace Ui {
class AddPointDialog;
}

class AddPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPointDialog(QWidget *parent, std::vector< mst::Point<float> >& currentPoints);
    ~AddPointDialog();

    mst::Point<float> getPoint();

private slots:
    void add();
    void clear();

private:
    Ui::AddPointDialog *ui;
    mst::Point<float> point;
    std::vector< mst::Point<float> > curr;
};

#endif // ADD_POINT_DIALOG_H
