#ifndef SAMPLE_GENERATOR_DIALOG_H
#define SAMPLE_GENERATOR_DIALOG_H

#include <QDialog>

namespace Ui {
class SampleGeneratorDialog;
}

class SampleGeneratorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SampleGeneratorDialog(QWidget *parent = 0);
    ~SampleGeneratorDialog();

    QString getPath();
    bool getType();

public slots:
    // for buttons
    void generate();
    void clear();
    void quit();

    // for path selection
    void selectPath();

private:
    Ui::SampleGeneratorDialog *ui;
};

#endif // SAMPLE_GENERATOR_DIALOG_H
