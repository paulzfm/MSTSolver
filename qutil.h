#ifndef QUTIL_H
#define QUTIL_H

#include <QString>
#include <QMessageBox>
#include <QPen>
#include "mst/util.h"

namespace qutil {

// tell the type of a number given in form of QString
bool validNumber(QString number);

// pen style settings
const static int VIEW_SAMPLES            = 0;
const static int VIEW_TREE               = 1;
const static int VIEW_GRAPH              = 2;
const static int VIEW_TREE_WITH_GRAPH    = 3;
const static int VIEW_PRIM               = 4;
const static int VIEW_SAMPLES_CURR       = 5;
const static int VIEW_SAMPLES_SELECTED   = 6;

void setPen(QPen* pen, int viewType, double scale);

// graphic area modes
const static int MODE_NORMAL            = 7;
const static int MODE_DELETE            = 8;
const static int MODE_MULTI_SELECTION   = 9;
const static int MODE_MOVE              = 10;
const static int MODE_NULL              = 11;
const static int MODE_LOCALE            = 12;

// file Names
const static QString ico_pencil = ":/ico/pencil";
const static QString ico_eraser = ":/ico/eraser";

// tranformer
int toPos(int percent);
int toPercent(int pos);
double toScale(int pos);


}   // namespace qutil



#endif // QUTIL_H
