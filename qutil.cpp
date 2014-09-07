#include "qutil.h"
#include <QRegExp>
#include <QColor>

namespace qutil {

bool validNumber(QString number)
{
    if (number.lastIndexOf(QRegExp("^[1-9]\\d*$")) != -1 ||
        number.lastIndexOf(QRegExp("^([1-9]\\d*|0).\\d*[1-9]$")) != -1)
        return true;
    return false;
}

int toPos(int percent)
{
    if (percent == 1)
        return 0;
    return (int)(percent / 100.0 * 50.0);
}

int toPercent(int pos)
{
    if (pos == 0)
        return 1;
    return (int)(pos / 50.0 * 100);
}

double toScale(int pos)
{
    if (pos == 0)
        return 0.01;
    return (double)(pos / 50.0);
}


void setPen(QPen *pen, int viewType, double scale)
{
    switch (viewType)
    {
    case VIEW_SAMPLES:
        pen->setColor(Qt::black);
        pen->setStyle(Qt::SolidLine);
        pen->setCapStyle(Qt::RoundCap);
        pen->setWidthF(6.0 / scale);
        break;
    case VIEW_SAMPLES_CURR:
        pen->setColor(QColor(255, 0, 255));
        pen->setStyle(Qt::SolidLine);
        pen->setCapStyle(Qt::RoundCap);
        pen->setWidthF(8.0 / scale);
        break;
    case VIEW_SAMPLES_SELECTED:
        pen->setColor(Qt::yellow);
        pen->setStyle(Qt::SolidLine);
        pen->setCapStyle(Qt::RoundCap);
        pen->setWidthF(7.0 / scale);
        break;
    case VIEW_GRAPH:
        pen->setColor(Qt::gray);
        pen->setStyle(Qt::DashLine);
        pen->setWidthF(2.0 / scale);
        break;
    case VIEW_PRIM:
    case VIEW_TREE:
        pen->setColor(Qt::red);
        pen->setStyle(Qt::SolidLine);
        pen->setWidthF(4.0 / scale);
        break;
    default:
        break;
    }
}

} // namespace qutil
