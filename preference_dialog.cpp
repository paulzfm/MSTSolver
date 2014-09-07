#include "preference_dialog.h"
#include "ui_preference_dialog.h"
#include <fstream>

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle("Preference");

    std::fstream in("options");
    if (!in)
    {
        // using default options
        ui->show->setChecked(true);
        ui->al1->setChecked(true);
    }
    else
    {
        // loading
        int num1, num2;
        in >> num1 >> num2;
        switch (num1)
        {
        case 0:
            ui->hide->setChecked(true);
            break;
        default:
            ui->show->setChecked(true);
        }

        switch (num2)
        {
        case 2:
            ui->al2->setChecked(true);
            break;
        case 3:
            ui->al3->setChecked(true);
            break;
        default:
            ui->al1->setChecked(true);
            break;
        }
    }
    in.close();

    connect(ui->btn_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->btn_discard, SIGNAL(clicked()), this, SLOT(close()));
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::save()
{
    std::ofstream out("options");
    if (ui->show->isChecked())
        out << "1 ";
    else
        out << "0 ";

    if (ui->al1->isChecked())
        out << "1";
    else if (ui->al2->isChecked())
        out << "2";
    else
        out << "3";

    out.close();
    this->accept();
}
