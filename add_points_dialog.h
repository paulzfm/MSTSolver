#ifndef ADD_POINTS_DIALOG_H
#define ADD_POINTS_DIALOG_H

#include <QDialog>
#include <vector>
#include "mst/util.h"

namespace Ui {
class AddPointsDialog;
}

class AddPointsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPointsDialog(QWidget *parent, std::vector< mst::Point<float> >& currentPoints);
    ~AddPointsDialog();

    std::vector< mst::Point<float> > getPoints();

private slots:
    void add();
    void reset();
    void remove();
    void clear();
    void quit();

private:
    Ui::AddPointsDialog *ui;
    std::vector< mst::Point<float> > points;
    std::vector< mst::Point<float> > curr;
};

#endif // ADD_POINTS_DIALOG_H
