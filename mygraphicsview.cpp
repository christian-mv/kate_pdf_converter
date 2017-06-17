#include <QtWidgets>
#include <cmath>
#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setDragMode(ScrollHandDrag);
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    // scale
    const QPointF p0scene = mapToScene(event->pos());
    double factor = qPow(1.2, event->delta() / 115);
    scale(factor, factor);

    // positioned
    const QPointF p1mouse = mapFromScene(p0scene);
    const QPointF move = p1mouse - event->pos(); // The move
    horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
    verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());

}









