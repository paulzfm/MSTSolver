#include "edit_point_dialog.h"
#include "ui_edit_point_dialog.h"
#include "qutil.h"

EditPointDialog::EditPointDialog(QWidget *parent, std::vector< mst::Point<float> >& currentPoints,
                                 int index) :
    QDialog(parent),
    curr(currentPoints),
    currPoint(index),
    ui(new Ui::EditPointDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle("Edit a point");

    ui->lineEdit_x->setText(QString::number(curr[index].x));
    ui->lineEdit_y->setText(QString::number(curr[index].y));

    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(edit()));
    connect(ui->btn_clear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(close()));
}

EditPointDialog::~EditPointDialog()
{
    delete ui;
}

void EditPointDialog::edit()
{
    QString x = ui->lineEdit_x->text();
    QString y = ui->lineEdit_y->text();

    if (!qutil::validNumber(x) || !qutil::validNumber(y))
    {
        QMessageBox::information(this, NULL, "Please input valid numbers for coordinates!",
                                 QMessageBox::Ok, QMessageBox::Ok);
        clear();
        return;
    }
    point.x = x.toFloat();
    point.y = y.toFloat();

    for (int i = 0; i < curr.size(); ++i)
    {
        if (i != currPoint && curr[i] == point)
        {
            QMessageBox::information(this, NULL, "Sample repeated!",
                                     QMessageBox::Ok, QMessageBox::Ok);
            clear();
            return;
        }
    }
    this->accept();
}

void EditPointDialog::clear()
{
    ui->lineEdit_x->setText(NULL);
    ui->lineEdit_y->setText(NULL);
}

mst::Point<float> EditPointDialog::getPoint()
{
    float x = ui->lineEdit_x->text().toFloat();
    float y = ui->lineEdit_y->text().toFloat();
    return mst::Point<float>(x, y);
}
