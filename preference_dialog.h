#ifndef PREFERENCE_DIALOG_H
#define PREFERENCE_DIALOG_H

#include <QDialog>

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferenceDialog(QWidget *parent = 0);
    ~PreferenceDialog();

private slots:
    void save();

private:
    Ui::PreferenceDialog *ui;

    bool show;
    int algorithm;
};

#endif // PREFERENCE_DIALOG_H
