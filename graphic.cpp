#include "graphic.h"
#include "qutil.h"
#include "mst/Prim.h"

#include <QMessageBox>
#include <QPainter>
#include <QPointF>
#include <QDebug>
#include <QWheelEvent>

Graphic::Graphic(QWidget *parent) :
    QWidget(parent)
{
    dragging = false;
    this->setMouseTracking(true);
    this->setCursor(Qt::ArrowCursor);

    // slider
    slider = new QSlider(Qt::Vertical, this);
    slider->setCursor(Qt::ArrowCursor);
    slider->setGeometry(20, 20, 20, 120);
    slider->setMaximum(100);
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(scaleChanged(int)));
    this->scaleChanged(50);

    // init
    dx = dy = 0.0;
    areaMode = qutil::MODE_NULL;
}

/**************** global ****************/
int Graphic::sliderPos = 50;
int Graphic::viewType = qutil::VIEW_SAMPLES;
int Graphic::areaMode = qutil::MODE_NORMAL;

float Graphic::weightPrim = 0;
float Graphic::weight = 0;
int Graphic::currSelection = -1;

std::vector< mst::Point<float> > Graphic::samples;
mst::Graph<float> Graphic::tree = mst::Graph<float>();
mst::Graph<float> Graphic::graph = mst::Graph<float>();
mst::Graph<float> Graphic::treePrim = mst::Graph<float>();
std::vector<int> Graphic::currSelections = std::vector<int>();

mst::Point<float> Graphic::trans(int x, int y)
{
    mst::Point<float> p;
    p.x = ((double)x - dx) / scale;
    p.y = ((double)y - dy) / scale;
    return p;
}

int Graphic::find(mst::Point<float> &point)
{
    for (int i = 0; i < samples.size(); ++i)
        if (samples[i].near(point, scale))
            return i;
    return -1;
}


/**************** events ****************/
void Graphic::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.translate(dx, dy);
    p.scale(scale, scale);

    QPen pen;

    // graph or tree
    switch (viewType) {
    case qutil::VIEW_PRIM:
        qutil::setPen(&pen, qutil::VIEW_PRIM, scale);
        p.setPen(pen);
        for (int i = 0; i < treePrim.edge_list.size(); ++i)
        {
            QPointF p1(treePrim.edge_list[i].s.x, treePrim.edge_list[i].s.y);
            QPointF p2(treePrim.edge_list[i].e.x, treePrim.edge_list[i].e.y);
            p.drawLine(p1, p2);
        }
        break;
    case qutil::VIEW_TREE:
        qutil::setPen(&pen, qutil::VIEW_TREE, scale);
        p.setPen(pen);
        for (int i = 0; i < tree.edge_list.size(); ++i)
        {
            QPointF p1(tree.edge_list[i].s.x, tree.edge_list[i].s.y);
            QPointF p2(tree.edge_list[i].e.x, tree.edge_list[i].e.y);
            p.drawLine(p1, p2);
        }
        break;
    case qutil::VIEW_GRAPH:
        qutil::setPen(&pen, qutil::VIEW_GRAPH, scale);
        p.setPen(pen);
        for (int i = 0; i < graph.edge_list.size(); ++i)
        {
            QPointF p1(graph.edge_list[i].s.x, graph.edge_list[i].s.y);
            QPointF p2(graph.edge_list[i].e.x, graph.edge_list[i].e.y);
            p.drawLine(p1, p2);
        }
        break;
    case qutil::VIEW_TREE_WITH_GRAPH:
        qutil::setPen(&pen, qutil::VIEW_GRAPH, scale);
        p.setPen(pen);
        for (int i = 0; i < graph.edge_list.size(); ++i)
        {
            QPointF p1(graph.edge_list[i].s.x, graph.edge_list[i].s.y);
            QPointF p2(graph.edge_list[i].e.x, graph.edge_list[i].e.y);
            p.drawLine(p1, p2);
        }
        qutil::setPen(&pen, qutil::VIEW_TREE, scale);
        p.setPen(pen);
        for (int i = 0; i < tree.edge_list.size(); ++i)
        {
            QPointF p1(tree.edge_list[i].s.x, tree.edge_list[i].s.y);
            QPointF p2(tree.edge_list[i].e.x, tree.edge_list[i].e.y);
            p.drawLine(p1, p2);
        }
        break;
    default:
        break;
    }

    // samples - normal
    qutil::setPen(&pen, qutil::VIEW_SAMPLES, scale);
    p.setPen(pen);
    for (int i = 0; i < samples.size(); ++i)
    {
        QPointF q(samples[i].x, samples[i].y);
        p.drawPoint(q);
    }

    // samples - selected
    qutil::setPen(&pen, qutil::VIEW_SAMPLES_SELECTED, scale);
    p.setPen(pen);
    for (int i = 0; i < currSelections.size(); ++i)
    {
        QPointF q(samples[currSelections[i]].x,
                  samples[currSelections[i]].y);
        p.drawPoint(q);
    }

    // samples - latest selected
    if (currSelection == -1)    // undef
        return;
    qutil::setPen(&pen, qutil::VIEW_SAMPLES_CURR, scale);
    p.setPen(pen);
    QPointF q(samples[currSelection].x, samples[currSelection].y);
    p.drawPoint(q);
}

void Graphic::wheelEvent(QWheelEvent *event)
{
    int newPos = sliderPos + event->delta();
    slider->setValue(newPos);
    scaleChanged(newPos);
}

void Graphic::mouseReleaseEvent(QMouseEvent *event)
{
    mst::Point<float> p = trans(event->x(), event->y());

    if (areaMode == qutil::MODE_NORMAL && dragging)
    {
        samples[currSelection] = p;
        samples.erase(samples.begin() + currSelection);
        if (find(p) != -1)  // repeat: go back
            samples.insert(samples.begin() + currSelection, bufPoint);
        else
            samples.insert(samples.begin() + currSelection, p);
        emit movePoint(currSelection);
        this->setCursor(Qt::CrossCursor);
        this->update();
    }

    if (areaMode == qutil::MODE_DELETE)
    {
        if (currSelection != -1)
        {
            samples.erase(samples.begin() + currSelection);
            emit deletePoint(currSelection);
            currSelection = -1;
            this->update();
        }
    }

    if (areaMode == qutil::MODE_MOVE && dragging)
    {
        float x = event->x();
        float y = event->y();
        dx += x - bufPoint.x;
        dy += y - bufPoint.y;
        this->update();
    }

    dragging = false;
}

void Graphic::mousePressEvent(QMouseEvent *event)
{
    mst::Point<float> p = trans(event->x(), event->y());

    if (areaMode == qutil::MODE_NORMAL)
    {
        currSelection = find(p);
        if (currSelection != -1)    // move
        {
            emit changePoint(currSelection);
            bufPoint = samples[currSelection];
            dragging = true;
        }
        else    // add
        {
            samples.push_back(p);
            emit addPoint(samples.size() - 1);
            currSelection = samples.size() - 1;
            this->update();
        }
    }

    if (areaMode == qutil::MODE_MOVE)
    {
        float x = event->x();
        float y = event->y();
        bufPoint = mst::Point<float>(x, y);
        dragging = true;
    }

    if (areaMode == qutil::MODE_LOCALE)
    {
        if (currSelection != -1)
        {
            // get maximum & minimum value
            float max = 0;
            float min = mst::Prim::INF;
            for (int i = 1; i < samples.size(); ++i)
            {
                if (samples[i].x > max)
                    max = samples[i].x;
                if (samples[i].x < min)
                    min = samples[i].x;
                if (samples[i].y > max)
                    max = samples[i].y;
                if (samples[i].y < min)
                    min = samples[i].y;
            }

            double k = (max - min) / (sqrt(samples.size())) / 30.0;
            if (k > 1.0)
                k = 1.0;
            dx = (this->width() * 0.5 - samples[currSelection].x) * (1 / k);
            dy = (this->height() * 0.5 - samples[currSelection].y) * (1 / k);
            scaleChanged(qutil::toPos(1 / k * 100));
            update();
            emit exitLocale();
        }
        else
        {
            QMessageBox::information(this, NULL,
                "Please select a point to continue, or exit by selecting other modes.",
                                     QMessageBox::Ok, QMessageBox::Ok);
        }
    }
}

void Graphic::mouseMoveEvent(QMouseEvent *event)
{
    mst::Point<float> p = trans(event->x(), event->y());

    if (areaMode == qutil::MODE_NORMAL && dragging)
    {
        this->setCursor(Qt::ArrowCursor);
        samples[currSelection] = p;
        emit movePoint(currSelection);
        this->update();
    }

    if (areaMode == qutil::MODE_DELETE || areaMode == qutil::MODE_LOCALE)
    {
        currSelection = find(p);
        emit changePoint(currSelection);
        this->update();
    }

    if (areaMode == qutil::MODE_MOVE && dragging == true)
    {
        float x = event->x();
        float y = event->y();
        dx += x - bufPoint.x;
        dy += y - bufPoint.y;
        this->update();
        bufPoint = mst::Point<float>(x, y);
    }
}

void Graphic::magnifyScale()
{
    scaleChanged(slider->value() + 1);
}

void Graphic::narrowScale()
{
    scaleChanged(slider->value() - 1);
}

void Graphic::changeTrans(double delta)
{
    dx = dy = delta;
    this->update();
}

/************** slots **************/
void Graphic::scaleChanged(int pos)
{
    if (pos > 100)
        pos = 100;
    if (pos < 0)
        pos = 0;
    slider->setValue(pos);
    sliderPos = pos;
    scale = qutil::toScale(pos);
    this->update();
}


/************* slider *************/
bool Graphic::changeSliderStatus()
{
    if (slider->isHidden())
    {
        slider->show();
        return true;
    }
    else
    {
        slider->hide();
        return false;
    }
}

void Graphic::setSliderStatus(bool show)
{
    if (show)
        slider->show();
    else
        slider->hide();
}
