#include "add_point_dialog.h"
#include "ui_add_point_dialog.h"
#include "qutil.h"
#include <QMessageBox>

AddPointDialog::AddPointDialog(QWidget *parent, std::vector< mst::Point<float> >& currentPoints) :
    QDialog(parent),
    ui(new Ui::AddPointDialog),
    curr(currentPoints)
{
    ui->setupUi(this);
    this->setWindowTitle("Add a point");
    this->setFixedSize(this->width(), this->height());

    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->btn_clear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(close()));
}

AddPointDialog::~AddPointDialog()
{
    delete ui;
}

void AddPointDialog::add()
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
        if (curr[i] == point)
        {
            QMessageBox::information(this, NULL, "Sample repeated!",
                                     QMessageBox::Ok, QMessageBox::Ok);
            clear();
            return;
        }
    }
    this->accept();
}

void AddPointDialog::clear()
{
    ui->lineEdit_x->setText(NULL);
    ui->lineEdit_y->setText(NULL);
}

mst::Point<float> AddPointDialog::getPoint()
{
    float x = ui->lineEdit_x->text().toFloat();
    float y = ui->lineEdit_y->text().toFloat();
    return mst::Point<float>(x, y);
}
