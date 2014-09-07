#include "scale_set_dialog.h"
#include "ui_scale_set_dialog.h"
#include <QSignalMapper>

ScaleSetDialog::ScaleSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleSetDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Scale");
    this->setFixedSize(this->width(), this->height());

    ui->slider->setValue(50);
    ui->slider->setMaximum(100);

    // connections
    connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(updateLabel(int)));
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(accept()));

    QSignalMapper *mapper = new QSignalMapper(this);
    connect(ui->rb_20, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(ui->rb_20, 20);
    connect(ui->rb_50, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(ui->rb_50, 50);
    connect(ui->rb_100, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(ui->rb_100, 100);
    connect(ui->rb_150, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(ui->rb_150, 150);
    connect(ui->rb_200, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(ui->rb_200, 200);

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setPos(int)));

    // init
    ui->rb_100->setChecked(true);
}

ScaleSetDialog::~ScaleSetDialog()
{
    delete ui;
}

void ScaleSetDialog::updateLabel(int pos)
{
    ui->lbl->setText(QString::number(qutil::toPercent(pos)) + "%");
    ui->rb_others->setChecked(true);
}

void ScaleSetDialog::setPos(int percent)
{
    ui->slider->setValue(qutil::toPos(percent));
    switch (percent)
    {
    case 20:
        ui->rb_20->setChecked(true);
        break;
    case 50:
        ui->rb_50->setChecked(true);
        break;
    case 100:
        ui->rb_100->setChecked(true);
        break;
    case 150:
        ui->rb_150->setChecked(true);
        break;
    case 200:
        ui->rb_200->setChecked(true);
        break;
    }
}

int ScaleSetDialog::getPos()
{
    return ui->slider->value();
}
