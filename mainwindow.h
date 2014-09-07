#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QCursor>
#include "graphic.h"
//#include "qutil.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void newPoint(int index);
    void deletePoint(int index);
    void updatePoint(int index);

    void selectedPoint(int index);

private slots:
    // for sample list
    void pointsRemoved();
    void selectionChanged();
    void exitRemoving();

    void on_action_load_sample_file_triggered();
    void on_action_load_random_sample_triggered();
    void on_action_quit_triggered();
    void on_action_m1_triggered(bool checked);
    void on_action_m2_triggered(bool checked);
    void on_action_solve_triggered();
    void on_action_generate_random_sample_triggered();
    void on_action_add_a_point_triggered();
    void on_action_add_points_triggered();
    void on_action_remove_a_point_triggered();
    void on_action_remove_points_triggered();
    void on_action_load_manually_triggered();
    void on_action_edit_triggered();
    void on_action_drawing_add_triggered();
    void on_action_drawing_remove_triggered();
    void on_action_clear_all_points_triggered();
    void on_action_drawing_move_triggered();
    void on_action_sample_view_triggered();
    void on_action_mst_view_triggered();
    void on_action_graph_view_triggered();
    void on_action_mst_with_graph_view_triggered();
    void on_action_set_to_normal_scale_100_triggered();
    void on_action_set_to_suitable_scale_triggered();
    void on_action_show_results_triggered();
    void on_action_save_results_triggered();
    void on_action_set_scale_triggered();
    void on_action_scale_slider_hide_or_show_triggered();
    void on_action_preference_triggered();
    void on_action_about_triggered();
    void on_action_magnify_scale_triggered();
    void on_action_narrow_scale_triggered();
    void on_action_locale_mode_triggered();

private:
    Ui::MainWindow *ui;
    Graphic *graphic;

    QLabel *lbl_count;
    QLabel *lbl_status;
    QListWidget *list_sample;
    QPushButton *btn_remove;
    QPushButton *btn_cancel;

    QCursor cs_pencil;
    QCursor cs_eraser;

    // algorithm setting
    bool isPrim;
    bool isPrimWithDelaunay;

    // test results
    double timePrimOnly;
    double timeDelaunay;
    double timePrim;
    QString report;

    void loadOptions();
    void updateLabel();
    void afterSampleLoading();
    void clearAll();
    void solvePrimOnly();
    void solveDelaunayWithPrim();

    // override
    void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
