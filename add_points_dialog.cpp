#include "add_points_dialog.h"
#include "ui_add_points_dialog.h"
#include "qutil.h"

#include <QMessageBox>
#include <QDebug>

AddPointsDialog::AddPointsDialog(QWidget *parent, std::vector< mst::Point<float> >& currentPoints) :
    QDialog(parent),
    ui(new Ui::AddPointsDialog),
    curr(currentPoints)
{
    ui->setupUi(this);
    this->setWindowTitle("Add points");
    this->setFixedSize(this->width(), this->height());

    // connections
    connect(ui->btn_add, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->btn_reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->btn_remove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->btn_clear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->btn_save, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(quit()));
}

AddPointsDialog::~AddPointsDialog()
{
    delete ui;
}

std::vector< mst::Point<float> > AddPointsDialog::getPoints()
{
    return points;
}

void AddPointsDialog::add()
{
    QString x = ui->lineEdit_x->text();
    QString y = ui->lineEdit_y->text();
    if (!qutil::validNumber(x) || !qutil::validNumber((y)))
    {
        QMessageBox::information(this, NULL, "Please input valid numbers for coordinates!",
                                 QMessageBox::Ok, QMessageBox::Ok);
        reset();
        return;
    }
    mst::Point<float> point;
    point.x = x.toFloat();
    point.y = y.toFloat();
    for (int i = 0; i < curr.size(); ++i)
    {
        if (curr[i] == point)
        {
            QMessageBox::information(this, NULL, "Sample repeated!",
                                     QMessageBox::Ok, QMessageBox::Ok);
            reset();
            return;
        }
    }
    for (int i = 0; i < points.size(); ++i)
    {
        if (points[i] == point)
        {
            QMessageBox::information(this, NULL, "Sample repeated!",
                                     QMessageBox::Ok, QMessageBox::Ok);
            reset();
            return;
        }
    }
    points.push_back(point);
    ui->list_points->addItem(QString::fromStdString(point.to_string()));
    reset();
}

void AddPointsDialog::reset()
{
    ui->lineEdit_x->setText(NULL);
    ui->lineEdit_y->setText(NULL);
}

void AddPointsDialog::remove()
{
    int index = ui->list_points->currentRow();
    if (index == -1)    // not found
    {
        QMessageBox::information(this, NULL, "Please select a point to delete!",
                                 QMessageBox::Ok, QMessageBox::Ok);
        ui->list_points->setCurrentRow(0);
        return;
    }
    points.erase(points.begin() + index);
    ui->list_points->takeItem(index);
}

void AddPointsDialog::clear()
{
    int res = QMessageBox::warning(this, NULL, "This will clear all new added points. Continue?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (res == QMessageBox::Yes)
    {
        ui->list_points->clear();
        points.clear();
    }
}

void AddPointsDialog::quit()
{
    if (points.size() != 0 || ui->lineEdit_x->text() != NULL || ui->lineEdit_y->text() != NULL)
    {
        int res = QMessageBox::warning(this, NULL, "Are you sure you want to quit now?",
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (res == QMessageBox::Yes)
            this->close();
    }
    this->close();
}
