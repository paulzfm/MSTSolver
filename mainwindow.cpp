#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sample_generator_dialog.h"
#include "add_point_dialog.h"
#include "add_points_dialog.h"
#include "edit_point_dialog.h"
#include "scale_set_dialog.h"
#include "preference_dialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QLabel>
#include <QPen>
#include <QAbstractItemView>
#include <QList>
#include <QListWidgetItem>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPixmap>

#include "qutil.h"
#include "mst/Prim.h"
#include "mst/SampleReader.h"
#include "delaunay/Clarkson-Delaunay.h"

#include <ctime>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MST Solver");
    this->setWindowState(Qt::WindowMaximized);

    // graphic
    graphic = new Graphic(this);
    QGridLayout *layout_center = new QGridLayout(ui->centralWidget);
    layout_center->setMargin(0);
    layout_center->addWidget(graphic);

    // dock
    ui->dock->setWindowTitle("samples");
    ui->dock->setFixedWidth(120);
    QGridLayout *layout_dock = new QGridLayout(ui->dockWidgetContents);
    layout_dock->setMargin(0);

    list_sample = new QListWidget();
    list_sample->setAutoScroll(true);
    list_sample->setFixedWidth(120);
    layout_dock->addWidget(list_sample, 0, 0);
    connect(list_sample, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
    connect(list_sample, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_action_edit_triggered()));

    btn_remove = new QPushButton("Remove");
    layout_dock->addWidget(btn_remove, 1, 0);
    btn_remove->hide();
    connect(btn_remove, SIGNAL(clicked()), this, SLOT(pointsRemoved()));

    btn_cancel = new QPushButton("Cancel");
    layout_dock->addWidget(btn_cancel, 2, 0);
    btn_cancel->hide();
    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(exitRemoving()));

    lbl_count = new QLabel("0 point");
    lbl_count->setMargin(6);
    lbl_count->setAlignment(Qt::AlignHCenter);
    layout_dock->addWidget(lbl_count, 3, 0);

    // status bar
    lbl_status = new QLabel("Please load a sample.");
    lbl_status->setMinimumSize(lbl_status->sizeHint());
    lbl_status->setAlignment(Qt::AlignHCenter);
    ui->statusBar->addWidget(lbl_status);

    // loading options
    loadOptions();

    // disable some actions
    ui->action_solve->setDisabled(true);
    ui->menu_results->setDisabled(true);
    ui->action_add_a_point->setDisabled(true);
    ui->action_add_points->setDisabled(true);
    ui->action_add_points->setDisabled(true);
    ui->action_remove_a_point->setDisabled(true);
    ui->action_remove_points->setDisabled(true);
    ui->action_clear_all_points->setDisabled(true);
    ui->action_edit->setDisabled(true);
    ui->action_drawing_multi_select->setDisabled(true);
    ui->action_drawing_multi_select->setChecked(false);
    ui->action_drawing_add->setDisabled(true);
    ui->action_drawing_move->setDisabled(true);
    ui->action_drawing_remove->setDisabled(true);
    ui->action_locale_mode->setDisabled(true);
    ui->action_graph_view->setDisabled(true);
    ui->action_mst_view->setDisabled(true);
    ui->action_mst_with_graph_view->setDisabled(true);
    ui->action_sample_view->setDisabled(true);

    // connections with graphics
    connect(graphic, SIGNAL(addPoint(int)), this, SLOT(newPoint(int)));
    connect(graphic, SIGNAL(movePoint(int)), this, SLOT(updatePoint(int)));
    connect(graphic, SIGNAL(deletePoint(int)), this, SLOT(deletePoint(int)));
    connect(graphic, SIGNAL(changePoint(int)), this, SLOT(selectedPoint(int)));
    connect(graphic, SIGNAL(exitLocale()), this, SLOT(on_action_drawing_move_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadOptions()
{
    std::ifstream in("options");
    if (!in)
    {
        // default
        graphic->setSliderStatus(true);
        ui->action_scale_slider_hide_or_show->setText("Hide slider");

        ui->action_m1->setChecked(true);
        ui->action_m2->setChecked(false);
        isPrim = true;
        isPrimWithDelaunay = false;
    }
    else
    {
        // loading
        int num1, num2;
        in >> num1 >> num2;
        switch (num1)
        {
        case 0:
            graphic->setSliderStatus(false);
            ui->action_scale_slider_hide_or_show->setText("Show slider");
            break;
        default:
            graphic->setSliderStatus(true);
            ui->action_scale_slider_hide_or_show->setText("Hide slider");
            break;
        }

        switch (num2)
        {
        case 2:
            ui->action_m1->setChecked(false);
            ui->action_m2->setChecked(true);
            isPrim = false;
            isPrimWithDelaunay = true;
            break;
        case 3:
            ui->action_m1->setChecked(true);
            ui->action_m2->setChecked(true);
            isPrim = true;
            isPrimWithDelaunay = true;
            break;
        default:
            ui->action_m1->setChecked(true);
            ui->action_m2->setChecked(false);
            isPrim = true;
            isPrimWithDelaunay = false;
            break;
        }
    }
    in.close();
}

void MainWindow::afterSampleLoading()
{
    lbl_status->setText("Sample loaded.");

    // show samples
    for (int i = 0; i < Graphic::samples.size(); ++i)
        this->list_sample->addItem(QString::fromStdString(Graphic::samples[i].to_string()));
    this->list_sample->setCurrentRow(Graphic::samples.size() - 1);
    Graphic::currSelection = Graphic::samples.size() - 1;
    this->updateLabel();
    this->on_action_set_to_suitable_scale_triggered();

    // enable actions
    ui->action_add_a_point->setEnabled(true);
    ui->action_add_points->setEnabled(true);
    ui->action_remove_a_point->setEnabled(true);
    ui->action_remove_points->setEnabled(true);
    ui->action_edit->setEnabled(true);
    ui->action_clear_all_points->setEnabled(true);

    ui->menu_view->setEnabled(true);
    ui->action_mst_view->setEnabled(false);
    ui->action_mst_view->setChecked(false);
    ui->action_graph_view->setEnabled(false);
    ui->action_graph_view->setChecked(false);
    ui->action_sample_view->setEnabled(false);
    ui->action_sample_view->setChecked(true);
    ui->action_mst_with_graph_view->setEnabled(false);
    ui->action_mst_with_graph_view->setChecked(false);
    this->on_action_drawing_add_triggered();

    ui->action_solve->setEnabled(true);
    ui->menu_results->setEnabled(false);
}

void MainWindow::clearAll()
{
    Graphic::samples.clear();
    this->list_sample->clear();
    Graphic::currSelection = -1;
    Graphic::currSelections.clear();
    Graphic::tree.edge_list.clear();
    Graphic::treePrim.edge_list.clear();
    Graphic::graph.edge_list.clear();
    this->updateLabel();
    graphic->update();
}

void MainWindow::updateLabel()
{
    int count = Graphic::samples.size();
    QString text;
    switch (count)
    {
    case 0:
        text = "No points";
        break;
    case 1:
        text = "1 point";
        break;
    default:
        text = QString::number(count) + " points";
        break;
    }
    this->lbl_count->setText(text);
}

void MainWindow::solvePrimOnly()
{
    int n = Graphic::samples.size();
    Graphic::treePrim.edge_list.clear();

    float** matrix = new float*[n];
    for (int i = 0; i < n; ++i)
        matrix[i] = new float[n];

    clock_t start = clock();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            matrix[i][j] = mst::euclid_distance(Graphic::samples[i], Graphic::samples[j]);

    mst::Prim prim(matrix, n);
    clock_t end = clock();
    timePrimOnly = (double)(end - start) / CLOCKS_PER_SEC;

    Graphic::weightPrim = prim.get_weight();
    std::vector<int> edges = prim.get_edges();
    Graphic::treePrim.set(Graphic::samples, edges);
    lbl_status->setText("Done. Weight: " + QString::number(Graphic::weightPrim));
}

void MainWindow::solveDelaunayWithPrim()
{
    int n = Graphic::samples.size();
    Graphic::graph.edge_list.clear();
    Graphic::tree.edge_list.clear();

    // parse samples for Delaunay use
    float* points = new float[n * 2];
    for (int i = 0; i < n; ++i)
    {
        points[i * 2] = Graphic::samples[i].x;
        points[i * 2 + 1] = Graphic::samples[i].y;
    }

    // call Delaunay
    WORD* triangleIndexList;
    int numTriangleVertices = 0;
    clock_t start = clock();
    triangleIndexList = delaunay::BuildTriangleIndexList(points, 100.0, n,
                                                         2, 0, &numTriangleVertices);
    clock_t end = clock();
    timeDelaunay = (double)(end - start) / CLOCKS_PER_SEC;

    // check if there is no triangles
    if (numTriangleVertices == 0)
    {
        QMessageBox::information(this, NULL, "The Clarkson-Delaunay algorithm doesn't work for this problem. Now Prim will be used instead.",
                                 QMessageBox::Ok, QMessageBox::Ok);

        ui->action_m1->setChecked(true);
        ui->action_m2->setChecked(false);
        this->isPrim = true;
        this->isPrimWithDelaunay = false;
        this->on_action_solve_triggered();
    }

    Graphic::graph.set(Graphic::samples, triangleIndexList, numTriangleVertices / 3);

    // generate weight matrix
    float** matrix = new float*[n];
    for (int i = 0; i < n; ++i)
        matrix[i] = new float[n];

    start = clock();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            matrix[i][j] = mst::Prim::INF;

    for (int i = 0; i < numTriangleVertices; i += 3)
    {
        unsigned int a = triangleIndexList[i];
        unsigned int b = triangleIndexList[i + 1];
        unsigned int c = triangleIndexList[i + 2];
        matrix[a][b] = matrix[b][a] =
                mst::euclid_distance(Graphic::samples[a], Graphic::samples[b]);
        matrix[a][c] = matrix[c][a] =
                mst::euclid_distance(Graphic::samples[a], Graphic::samples[c]);
        matrix[b][c] = matrix[c][b] =
                mst::euclid_distance(Graphic::samples[b], Graphic::samples[c]);
    }

    // Prim
    mst::Prim prim(matrix, n);
    end = clock();
    timePrim = (double)(end - start) / CLOCKS_PER_SEC;

    Graphic::weight = prim.get_weight();
    std::vector<int> edges = prim.get_edges();
    Graphic::tree.set(Graphic::samples, edges);
    lbl_status->setText("Done. Weight: " + QString::number(Graphic::weight));
}


/*********** public slots ***********/
void MainWindow::pointsRemoved()
{
    // removing
    for (int i = 0; i < Graphic::samples.size(); ++i)
    {
        if (list_sample->item(i)->isSelected())
        {
            Graphic::samples.erase(Graphic::samples.begin() + i);
            list_sample->takeItem(i);
            --i;
        }
    }
    exitRemoving();
}

void MainWindow::exitRemoving()
{
    // recover
    if (ui->action_drawing_add->isChecked())
        this->on_action_drawing_add_triggered();
    else if (ui->action_drawing_move->isChecked())
        this->on_action_drawing_move_triggered();
    else
        this->on_action_drawing_remove_triggered();

    this->list_sample->setSelectionMode(QAbstractItemView::SingleSelection);
    this->btn_remove->hide();
    this->btn_cancel->hide();
    this->list_sample->setCurrentRow(-1);
    Graphic::currSelection = -1;
    Graphic::currSelections.clear();

    ui->action_edit->setEnabled(true);
    ui->action_add_a_point->setEnabled(true);
    ui->action_add_points->setEnabled(true);

    ui->action_drawing_multi_select->setChecked(false);

    // update
    this->updateLabel();
    graphic->update();
}

void MainWindow::selectionChanged()
{
    if (Graphic::areaMode == qutil::MODE_MULTI_SELECTION)
    {
        int count = this->list_sample->selectedItems().size();
        this->lbl_count->setText(QString::number(count) + " selected");

        Graphic::currSelections.clear();
        for (int i = 0; i < Graphic::samples.size(); ++i)
            if (list_sample->item(i)->isSelected())
                Graphic::currSelections.push_back(i);
        graphic->update();
    }
    else
    {
        Graphic::currSelection = this->list_sample->currentRow();
        graphic->update();
    }
}

/**** slots for list ****/
void MainWindow::newPoint(int index)
{
    list_sample->addItem(QString::fromStdString(
                                         Graphic::samples[index].to_string()));
    list_sample->setCurrentRow(index);
    this->updateLabel();
}

void MainWindow::deletePoint(int index)
{
    list_sample->takeItem(index);
    this->updateLabel();
}

void MainWindow::updatePoint(int index)
{
    list_sample->item(index)->setText(QString::fromStdString(
                                          Graphic::samples[index].to_string()));
    list_sample->setCurrentRow(index);
}

void MainWindow::selectedPoint(int index)
{
    list_sample->setCurrentRow(index);
}



/**************** actions ***************/

// Sample Loading
void MainWindow::on_action_load_sample_file_triggered()
{
    this->clearAll();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Sample File"), QDir::homePath(),
                                            tr("data file(*.dat);;text file(*.txt);;all files(*.*)"));

    if (fileName == 0)
    {
        QMessageBox::information(this, NULL, "No file selected!",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    // read Graphic::samples to buffer
    mst::SampleReader reader(fileName.toStdString());
    if (reader.get_exceptions() == reader.FILE_OPEN_FAILED)
    {
        QString text = "File [" + fileName + "] loading failed!";
        QMessageBox::information(this, NULL, text, QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (reader.get_exceptions() == reader.SAMPLE_REPEATED)
    {
        QMessageBox::information(this, NULL, "Points cannot be repeated!",
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    Graphic::samples = reader.get_samples();
    this->afterSampleLoading();
}

void MainWindow::on_action_load_random_sample_triggered()
{
    this->clearAll();
    SampleGeneratorDialog *dlg = new SampleGeneratorDialog(this);
    if (dlg->exec() == QDialog::Accepted)
    {
        QString fileName = dlg->getPath();

        // read samples to buffer
        mst::SampleReader reader(fileName.toStdString());
        if (reader.get_exceptions() == reader.FILE_OPEN_FAILED)
        {
            QString text = "File [" + fileName + "] loading failed!";
            QMessageBox::information(this, NULL, text, QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        if (reader.get_exceptions() == reader.SAMPLE_REPEATED)
        {
            QMessageBox::information(this, NULL, "Points cannot be repeated!",
                                     QMessageBox::Ok, QMessageBox::Ok);
            return;
        }

        Graphic::samples = reader.get_samples();
        this->afterSampleLoading();
    }
}

void MainWindow::on_action_load_manually_triggered()
{
    this->clearAll();
    this->afterSampleLoading();
}

void MainWindow::on_action_quit_triggered()
{
    int value = QMessageBox::information(this, "Quit", "Are you sure you want to quit now?",
                                 QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (value == QMessageBox::Yes)
        this->close();
}

// Algorithm selection
void MainWindow::on_action_m1_triggered(bool checked)
{
    this->isPrim = checked;
}

void MainWindow::on_action_m2_triggered(bool checked)
{
    this->isPrimWithDelaunay = checked;
}

// Solve
void MainWindow::on_action_solve_triggered()
{
    lbl_status->setText("Solving...");

    if (!isPrim && !isPrimWithDelaunay) // no algorithm selected
    {
        QMessageBox::about(this, "Warning", "Please select at least one algorithm for solving!");
        return;
    }

    if (Graphic::samples.size() < 5)
    {
        QMessageBox::about(this, "Warning", "Please provide at least 5 points for solving!");
        return;
    }

    if (isPrim)
        this->solvePrimOnly();

    if (isPrimWithDelaunay)
        this->solveDelaunayWithPrim();

    // report recording
    report = "";
    if (isPrim)
    {
        report += "\nPrim Only\n";
        report += "  Sample size: " + QString::number(Graphic::samples.size()) + "\n";
        report += "  Weight: " + QString::number(Graphic::weightPrim) + "\n";
        report += "  Time cost: " + QString::number(timePrimOnly) + " s\n";
    }

    if (isPrimWithDelaunay)
    {
        report += "\nPrim with Delaunay\n";
        report += "  Sample size: " + QString::number(Graphic::samples.size()) + "\n";
        report += "  Weight: " + QString::number(Graphic::weight) + "\n";
        report += "  Delaunay time cost: " + QString::number(timeDelaunay) + " s\n";
        report += "  Prim time cost: " + QString::number(timePrim) + " s\n";
        report += "  Total time cost: " + QString::number(timeDelaunay + timePrim) + " s\n";
    }

    // after sloving
    ui->menu_results->setEnabled(true);
    this->on_action_set_to_suitable_scale_triggered();
    this->on_action_mst_view_triggered();
}

// Sample Operation
void MainWindow::on_action_generate_random_sample_triggered()
{
    SampleGeneratorDialog *dlg = new SampleGeneratorDialog(this);
    dlg->exec();
}

void MainWindow::on_action_add_a_point_triggered()
{
    AddPointDialog *dlg = new AddPointDialog(this, Graphic::samples);
    if (dlg->exec() == QDialog::Accepted)
    {
        mst::Point<float> point = dlg->getPoint();
        Graphic::samples.push_back(point);
        this->list_sample->addItem(QString::fromStdString(point.to_string()));
        this->list_sample->setCurrentRow(Graphic::samples.size() - 1);
        Graphic::currSelection = Graphic::samples.size() - 1;
        this->updateLabel();
        graphic->update();
    }
}

void MainWindow::on_action_add_points_triggered()
{
    AddPointsDialog *dlg = new AddPointsDialog(this, Graphic::samples);
    if (dlg->exec() == QDialog::Accepted)
    {
        std::vector< mst::Point<float> > points = dlg->getPoints();
        for (int i = 0; i < points.size(); ++i)
        {
            Graphic::samples.push_back(points[i]);
            this->list_sample->addItem(QString::fromStdString(points[i].to_string()));
        }
        this->list_sample->setCurrentRow(Graphic::samples.size() - 1);
        Graphic::currSelection = Graphic::samples.size() - 1;
        this->updateLabel();
        graphic->update();
    }
}

void MainWindow::on_action_remove_a_point_triggered()
{
    int index = this->list_sample->currentRow();
    if (index == -1)    // not found
    {
        QMessageBox::information(this, "No point selected", "Please select a point to delete!",
                                 QMessageBox::Ok, QMessageBox::Ok);
        this->list_sample->setCurrentRow(0);
        Graphic::currSelection = 0;
        return;
    }
    Graphic::samples.erase(Graphic::samples.begin() + index);
    this->list_sample->takeItem(index);
    this->list_sample->setCurrentRow(Graphic::samples.size() - 1);
    Graphic::currSelection = Graphic::samples.size() - 1;
    this->updateLabel();
    graphic->update();
}

void MainWindow::on_action_remove_points_triggered()
{
    Graphic::areaMode = qutil::MODE_MULTI_SELECTION;

    this->list_sample->setSelectionMode(QAbstractItemView::MultiSelection);
    this->btn_remove->show();
    this->btn_cancel->show();
    this->list_sample->setCurrentRow(-1);
    Graphic::currSelection = -1;

    ui->action_edit->setEnabled(false);
    ui->action_add_a_point->setEnabled(false);
    ui->action_add_points->setEnabled(false);

    ui->action_drawing_add->setEnabled(false);
    ui->action_drawing_move->setEnabled(false);
    ui->action_drawing_remove->setEnabled(false);
    ui->action_drawing_multi_select->setChecked(true);
}

void MainWindow::on_action_clear_all_points_triggered()
{
    if (Graphic::samples.size() == 0)
        return;
    int res = QMessageBox::information(this, "Clear", "This will clear all points and graphs. Do you want to continue?",
                                       QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (res == QMessageBox::Yes)
        this->on_action_load_manually_triggered();
}

void MainWindow::on_action_edit_triggered()
{
    int index = this->list_sample->currentRow();
    if (index == -1)    // not found
    {
        QMessageBox::information(this, "No point selected", "Please select a point to edit!",
                                 QMessageBox::Ok, QMessageBox::Ok);
        this->list_sample->setCurrentRow(0);
        return;
    }
    EditPointDialog *dlg = new EditPointDialog(this, Graphic::samples, index);
    if (dlg->exec() == QDialog::Accepted)
    {
        mst::Point<float> point = dlg->getPoint();
        Graphic::samples[index] = point;
        this->list_sample->item(index)->setText(QString::fromStdString(point.to_string()));
        this->updateLabel();
        graphic->update();
    }
}

// Mode
void MainWindow::on_action_drawing_add_triggered()
{
    graphic->setCursor(Qt::CrossCursor);
    ui->action_drawing_add->setEnabled(false);
    ui->action_drawing_add->setChecked(true);
    ui->action_drawing_remove->setEnabled(true);
    ui->action_drawing_remove->setChecked(false);
    ui->action_drawing_move->setEnabled(true);
    ui->action_drawing_move->setChecked(false);
    ui->action_locale_mode->setEnabled(true);
    ui->action_locale_mode->setChecked(false);
    Graphic::areaMode = qutil::MODE_NORMAL;
}

void MainWindow::on_action_drawing_remove_triggered()
{
    graphic->setCursor(Qt::UpArrowCursor);
    ui->action_drawing_remove->setEnabled(false);
    ui->action_drawing_remove->setChecked(true);
    ui->action_drawing_add->setEnabled(true);
    ui->action_drawing_add->setChecked(false);
    ui->action_drawing_move->setEnabled(true);
    ui->action_drawing_move->setChecked(false);
    ui->action_locale_mode->setEnabled(true);
    ui->action_locale_mode->setChecked(false);
    Graphic::areaMode = qutil::MODE_DELETE;
}

void MainWindow::on_action_drawing_move_triggered()
{
    graphic->setCursor(Qt::OpenHandCursor);
    ui->action_drawing_move->setEnabled(false);
    ui->action_drawing_move->setChecked(true);
    ui->action_drawing_add->setEnabled(true);
    ui->action_drawing_add->setChecked(false);
    ui->action_drawing_remove->setEnabled(true);
    ui->action_drawing_remove->setChecked(false);
    ui->action_locale_mode->setEnabled(true);
    ui->action_locale_mode->setChecked(false);
    Graphic::areaMode = qutil::MODE_MOVE;
}


void MainWindow::on_action_locale_mode_triggered()
{
    this->on_action_set_to_suitable_scale_triggered();
    graphic->setCursor(Qt::UpArrowCursor);
    ui->action_locale_mode->setEnabled(false);
    ui->action_locale_mode->setChecked(true);
    ui->action_drawing_add->setEnabled(true);
    ui->action_drawing_add->setChecked(false);
    ui->action_drawing_remove->setEnabled(true);
    ui->action_drawing_remove->setChecked(false);
    ui->action_drawing_move->setEnabled(true);
    ui->action_drawing_move->setChecked(false);
    Graphic::areaMode = qutil::MODE_LOCALE;
}

// View
void MainWindow::on_action_sample_view_triggered()
{
    Graphic::viewType = qutil::VIEW_SAMPLES;
    ui->action_sample_view->setChecked(true);
    ui->action_sample_view->setEnabled(false);
    ui->action_mst_view->setEnabled(true);
    ui->action_mst_view->setChecked(false);
    if (isPrimWithDelaunay)
    {
        ui->action_graph_view->setEnabled(true);
        ui->action_graph_view->setChecked(false);
        ui->action_mst_with_graph_view->setEnabled(true);
        ui->action_mst_with_graph_view->setChecked(false);
    }
    graphic->update();
}

void MainWindow::on_action_mst_view_triggered()
{
    if (isPrim)
        Graphic::viewType = qutil::VIEW_PRIM;
    else
        Graphic::viewType = qutil::VIEW_TREE;
    ui->action_mst_view->setChecked(true);
    ui->action_mst_view->setEnabled(false);
    ui->action_sample_view->setChecked(false);
    ui->action_sample_view->setEnabled(true);
    if (isPrimWithDelaunay)
    {
        ui->action_graph_view->setEnabled(true);
        ui->action_graph_view->setChecked(false);
        ui->action_mst_with_graph_view->setEnabled(true);
        ui->action_mst_with_graph_view->setChecked(false);
    }
    graphic->update();
}

void MainWindow::on_action_graph_view_triggered()
{
    Graphic::viewType = qutil::VIEW_GRAPH;
    ui->action_graph_view->setChecked(true);
    ui->action_graph_view->setEnabled(false);
    ui->action_mst_view->setChecked(false);
    ui->action_mst_view->setEnabled(true);
    ui->action_sample_view->setChecked(false);
    ui->action_sample_view->setEnabled(true);
    ui->action_mst_with_graph_view->setEnabled(true);
    ui->action_mst_with_graph_view->setChecked(false);
    graphic->update();
}

void MainWindow::on_action_mst_with_graph_view_triggered()
{
    Graphic::viewType = qutil::VIEW_TREE_WITH_GRAPH;
    ui->action_mst_with_graph_view->setChecked(true);
    ui->action_mst_with_graph_view->setEnabled(false);
    ui->action_mst_view->setChecked(false);
    ui->action_mst_view->setEnabled(true);
    ui->action_sample_view->setChecked(false);
    ui->action_sample_view->setEnabled(true);
    ui->action_graph_view->setChecked(false);
    ui->action_graph_view->setEnabled(true);
    graphic->update();
}

// Scale
void MainWindow::on_action_set_to_normal_scale_100_triggered()
{
    graphic->scaleChanged(50);
}

void MainWindow::on_action_set_to_suitable_scale_triggered()
{
    if (Graphic::samples.size() < 5)
    {
        this->on_action_set_to_normal_scale_100_triggered();
        return;
    }

    // get maximum & minimum value
    float max = 0;
    float min = mst::Prim::INF;
    for (int i = 1; i < Graphic::samples.size(); ++i)
    {
        if (Graphic::samples[i].x > max)
            max = Graphic::samples[i].x;
        if (Graphic::samples[i].x < min)
            min = Graphic::samples[i].x;
        if (Graphic::samples[i].y > max)
            max = Graphic::samples[i].y;
        if (Graphic::samples[i].y < min)
            min = Graphic::samples[i].y;
    }

    float win = MIN(ui->centralWidget->height(), ui->centralWidget->width());

    float scale = win / (max - min);
    int pos = qutil::toPos(scale * 100);
    graphic->scaleChanged(pos);

    double delta = 50.0 - min;
    graphic->changeTrans(delta);
}

void MainWindow::on_action_set_scale_triggered()
{
    ScaleSetDialog *dlg = new ScaleSetDialog(this);
    if (dlg->exec() == QDialog::Accepted)
        graphic->scaleChanged(dlg->getPos());
}

void MainWindow::on_action_scale_slider_hide_or_show_triggered()
{
    bool isShown = graphic->changeSliderStatus();
    if (isShown)
        ui->action_scale_slider_hide_or_show->setText("Hide slider");
    else
        ui->action_scale_slider_hide_or_show->setText("Show slider");
}

void MainWindow::on_action_magnify_scale_triggered()
{
    graphic->magnifyScale();
}

void MainWindow::on_action_narrow_scale_triggered()
{
    graphic->narrowScale();
}


// results
void MainWindow::on_action_show_results_triggered()
{
    QMessageBox::about(this, "Report", report);
}

void MainWindow::on_action_save_results_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
                this, "Select a path to save report file", QDir::homePath(),
                "text file(*.txt);;all files(*.*)");

    if (fileName != NULL)
    {
        std::ofstream out(fileName.toStdString().c_str());
        out << "# Report file auto-generated by MST Solver\n"
            << report.toStdString();
        out.close();
        QMessageBox::information(
                    this, NULL, "Report has been recorded into [" + fileName + "] successfully",
                    QMessageBox::Ok, QMessageBox::Ok);
    }
}

// Preference
void MainWindow::on_action_preference_triggered()
{
    PreferenceDialog *dlg = new PreferenceDialog(this);
    if (dlg->exec() == QDialog::Accepted)
        loadOptions();
}

void MainWindow::on_action_about_triggered()
{
    QString info = "Author: Paul Zhu\nEmail: zhufengminpaul@163.com\n\n2014-09-07";
    QMessageBox::about(this, "About", info);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    this->on_action_quit_triggered();
}
