#include "sample_generator_dialog.h"
#include "ui_sample_generator_dialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include "mst/SampleGenerator.h"

SampleGeneratorDialog::SampleGeneratorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SampleGeneratorDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle("Random sample");

    // init widgets
    ui->cmb_type->addItem("Integer");
    ui->cmb_type->addItem("Float Numbers");
    ui->cmb_type->setCurrentIndex(0);

    ui->sp_size->setMaximum(20000);
    ui->sp_size->setMinimum(5);
    ui->sp_size->setValue(100);
    ui->lineEdit_path->setText(NULL);
    ui->lineEdit_path->setEnabled(false);   // uneditable

    // connections
    connect(ui->btn_gen, SIGNAL(clicked()), this, SLOT(generate()));
    connect(ui->btn_reset, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->btn_quit, SIGNAL(clicked()), this, SLOT(quit()));
    connect(ui->btn_open, SIGNAL(clicked()), this, SLOT(selectPath()));
}

SampleGeneratorDialog::~SampleGeneratorDialog()
{
    delete ui;
}


QString SampleGeneratorDialog::getPath()
{
    return ui->lineEdit_path->text();
}

bool SampleGeneratorDialog::getType()
{
    if (ui->cmb_type->currentIndex() == 0)
        return true;
    else
        return false;
}

void SampleGeneratorDialog::generate()
{
    // parameters
    QString fileName;
    bool isInt;
    int size;

    // get paramters
    switch (ui->cmb_type->currentIndex())
    {
    case 0:
        isInt = true;
        break;
    case 1:
        isInt = false;
        break;
    default:    // shouldn't be here
        QMessageBox::information(this, NULL, "Please select a data type!",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (ui->sp_size->value() == 0)
    {
        QMessageBox::information(this, NULL, "The sample size cannnot be 0!",
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    else
    {
        size = ui->sp_size->value();
    }

    if (ui->lineEdit_path->text() == NULL)
    {
        QMessageBox::information(this, NULL, "Please select a path for the sample file!",
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    else
    {
        fileName = ui->lineEdit_path->text();
    }

    // now generate the sample
    mst::SampleGenerator(isInt, size, fileName.toStdString());

    // success information
    QMessageBox::information(this, NULL, "Sample has been generated successfully: " + fileName,
                             QMessageBox::Ok, QMessageBox::Ok);

    // exit with success info
    this->accept();
}

void SampleGeneratorDialog::clear()
{
    ui->cmb_type->setCurrentIndex(0);
    ui->sp_size->setValue(0);
    ui->lineEdit_path->setText(NULL);
}

void SampleGeneratorDialog::quit()
{
    if (ui->cmb_type->currentIndex() == 0 && ui->sp_size->value() == 0 &&
        ui->lineEdit_path->text() == NULL)
    {
        this->close();
    }
    else
    {
        int res = QMessageBox::information(this, "Quit", "Are you sure you want to quit sample generating?",
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (res == QMessageBox::Yes)
            this->close();
    }
}

void SampleGeneratorDialog::selectPath()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Select a path to save sample file", QDir::homePath(),
                                                    "data file(*.dat);;text file(*.txt);;all files(*.*)");
    ui->lineEdit_path->setText(fileName);
}
