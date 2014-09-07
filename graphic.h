#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QWidget>
#include <QSlider>
#include <QMouseEvent>
#include "mst/util.h"

class Graphic : public QWidget
{
    Q_OBJECT
public:
    explicit Graphic(QWidget *parent = 0);

    // slider hiden or show
    bool changeSliderStatus();  // return current status after changing
    void setSliderStatus(bool show);

    // global status
    static int sliderPos;
    static int viewType;
    static int areaMode;

    // global data
    static float weightPrim;               // for Prim only
    static float weight;
    static std::vector< mst::Point<float> > samples;
    static mst::Graph<float> tree;
    static mst::Graph<float> graph;
    static mst::Graph<float> treePrim;     // for Prim only

    // global exchanger
    static int currSelection;
    static std::vector<int> currSelections;

    void magnifyScale();
    void narrowScale();
    void changeTrans(double delta);

signals:
    void addPoint(int);
    void deletePoint(int);
    void movePoint(int);
    void changePoint(int);
    void exitLocale();

public slots:
    void scaleChanged(int pos);

private:
    QSlider *slider;
    double scale;
    double dx, dy;

    // status
    bool dragging;

    // buffers
    mst::Point<float> bufPoint;

    // override
    void paintEvent(QPaintEvent*);
    void wheelEvent(QWheelEvent* event);

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    // utils
    mst::Point<float> trans(int x, int y);
    int find(mst::Point<float>& point);
};

#endif // GRAPHIC_H
