#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class  MyGraphicsView: public QGraphicsView
{
    Q_OBJECT

public:
    MyGraphicsView(QWidget *parent = 0);

protected:
    void wheelEvent(QWheelEvent *event)Q_DECL_OVERRIDE;


};

#endif
