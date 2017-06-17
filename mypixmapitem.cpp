#include "mypixmapitem.h"


MyPixmapItem::MyPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(pixmap, parent) {

    DISTANCE_FOR_RESIZE = boundingRect().width()*0.05;
//    qDebug()<<DISTANCE_FOR_RESIZE;

    setFlags(QGraphicsItem::ItemSendsScenePositionChanges  | QGraphicsItem::ItemSendsGeometryChanges
             | QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);

    setFullRect( QGraphicsPixmapItem::boundingRect() );
    calculateEdgesRectangules();
    shiftRotationAngle = 0;
    loadCursors();

}


void MyPixmapItem::setCropRectangle(const QRectF &crop_rectangle)
{
    this->crop_rectangle = crop_rectangle;
    this->calculateEdgesRectangules();
    emit crop_rectangle_change(this->crop_rectangle);
}



void MyPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(!editMode){return;}
//    qDebug() << "mousePressEvent:"<<"initCropFlag: "<<initCropFlag;
    initCropFlag = true;
    setCursor( selectCursor( mousePresPosition=event->pos() ) );
    prepareGeometryChange();
    if(rectangleSelected==NULL){
        initCropFlag = false;
        // Aseguramos que se pase el evento a QGraphicsView, esto permite que funciones
        // el scroll con arrastre de mouse cuando se ha habilitado mediante:
        // QGraphicsView::setDragMode(ScrollHandDrag)
        event->ignore();
        return;
    }
    QGraphicsPixmapItem::mousePressEvent(event);

}

void MyPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "mouseReleaseEvent:"<<"initCropFlag: "<<initCropFlag;
    if(!editMode || !initCropFlag){return;}
    prepareGeometryChange();
    setCursor( Qt::ArrowCursor );
    rectangleSelected = NULL;
    initCropFlag = false;
    QGraphicsPixmapItem::mouseReleaseEvent(event);// IMPORTANTE. evita saltos de imagen.
}

void MyPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if(!editMode || !initCropFlag){return;}    
    prepareGeometryChange();
    edit_crop_rectangle(event->pos());
    QGraphicsPixmapItem::mouseMoveEvent(event);
//    qDebug() << "mouseMoveEvent:"<<"initCropFlag: "<<initCropFlag;
}

void MyPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
//    qDebug() << "hoverMoveEvent:"<<"initCropFlag: "<<initCropFlag;
    initCropFlag = false;
    if(!editMode){return;}
    prepareGeometryChange();
    setCursor( selectCursor( event->pos() ) );
    //        qDebug() << "cursor position:"<<event;
    QGraphicsPixmapItem::hoverMoveEvent(event);
}


void MyPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    qDebug()<<"\n---Paint event---\n"<<"\ncrop_rectangle: "<<crop_rectangle<<"\nnorth rectangule: "<<north_rectangle<<"\nsout_rectangule: "<<south_rectangle;


    QGraphicsPixmapItem::paint(painter,option,widget);


    if(!editMode){return;}


    qreal penSize = (boundingRect().width()/boundingRect().height());

    painter->setPen(QPen(QColor(0, 255, 0), penSize, Qt::SolidLine, Qt::RoundCap));
    painter->drawRect(crop_rectangle);



    QVector<QLineF> linesOfEdition, linesOfReference;
    // lineas usadas para edición
    qreal dist_lines = DISTANCE_FOR_RESIZE;
    linesOfEdition  <<QLineF(crop_rectangle.left(), crop_rectangle.top(),
                             crop_rectangle.left()+dist_lines, crop_rectangle.top())
                    <<QLineF(crop_rectangle.right(), crop_rectangle.top(),
                             crop_rectangle.right()-dist_lines, crop_rectangle.top())
                    <<QLineF(crop_rectangle.left(), crop_rectangle.bottom(),
                             crop_rectangle.left()+dist_lines, crop_rectangle.bottom())
                    <<QLineF(crop_rectangle.right(), crop_rectangle.bottom(),
                             crop_rectangle.right()-dist_lines, crop_rectangle.bottom())
                    <<QLineF(crop_rectangle.left(), crop_rectangle.top(),
                             crop_rectangle.left(), crop_rectangle.top()+dist_lines)
                    <<QLineF(crop_rectangle.right(), crop_rectangle.top(),
                             crop_rectangle.right(), crop_rectangle.top()+dist_lines)
                    <<QLineF(crop_rectangle.left(), crop_rectangle.bottom(),
                             crop_rectangle.left(), crop_rectangle.bottom()-dist_lines)
                    <<QLineF(crop_rectangle.right(), crop_rectangle.bottom(),
                             crop_rectangle.right(), crop_rectangle.bottom()-dist_lines);

    // lineas usadas para referencia vertical
    dist_lines = crop_rectangle.width()/3;
    linesOfReference  <<QLineF(crop_rectangle.left()+dist_lines, crop_rectangle.top(),
                               crop_rectangle.left()+dist_lines, crop_rectangle.bottom())
                     <<QLineF(crop_rectangle.left()+2*dist_lines, crop_rectangle.topLeft().y(),
                              crop_rectangle.topLeft().x()+2*dist_lines, crop_rectangle.bottom());

    // lineas usadas para referencia horizontal
    dist_lines = crop_rectangle.height()/3;
    linesOfReference  <<QLineF(crop_rectangle.left(), crop_rectangle.top()+dist_lines,
                               crop_rectangle.right(), crop_rectangle.top()+dist_lines)
                     <<QLineF(crop_rectangle.left(), crop_rectangle.top()+2*dist_lines,
                                                    crop_rectangle.right(), crop_rectangle.top()+2*dist_lines);


    painter->setPen(QPen(QColor(0, 255, 0), 2*penSize, Qt::SolidLine, Qt::RoundCap));
    painter->drawLines(linesOfEdition);
    painter->setPen(QPen(QColor(0, 255, 0), 2*penSize, Qt::DotLine, Qt::RoundCap));
    painter->drawLines(linesOfReference);

//    // Paint auxiliar rectangles
//            painter->setPen(Qt::white);
//            painter->drawRect(north_west_rectangle);
//            painter->drawRect(north_rectangle);
//            painter->drawRect(north_east_rectangle);
//            painter->drawRect(east_rectangle);
//            painter->drawRect(south_east_rectangle);
//            painter->drawRect(south_rectangle);
//            painter->drawRect(south_west_rectangle);
//            painter->drawRect(west_rectangle);
//            painter->setPen(Qt::yellow);
//            painter->drawRect(center_rectangle);



    QPainterPath path;
    path.moveTo(0, 0);
    QPolygonF polygon1, polygon2;
    path.addPolygon(polygon1<<boundingRect().topLeft()
                    <<crop_rectangle.topLeft()
                    <<crop_rectangle.bottomLeft()
                    <<boundingRect().bottomLeft());

    path.addPolygon(polygon1<<boundingRect().topLeft()
                    <<boundingRect().topRight()
                    <<crop_rectangle.topRight()
                    <<crop_rectangle.topLeft());
    path.addPolygon(polygon2<<boundingRect().topRight()
                    <<boundingRect().bottomRight()
                    <<crop_rectangle.bottomRight()
                    <<crop_rectangle.topRight());
    path.addPolygon(polygon2<<boundingRect().bottomRight()
                    <<boundingRect().bottomLeft()
                    <<crop_rectangle.bottomLeft()
                    <<crop_rectangle.bottomRight());

    painter->setPen(Qt::NoPen);
    path.setFillRule(Qt::WindingFill);
    painter->fillPath(path, QColor(0, 127, 127, 100));
//    painter->fillRect(boundingRect(), QColor(0, 127, 127, 100));


}



// reimplementamos shape() para garantizar el área donde se llevarán a cabo
// los eventos de mouse, de lo contrario, estos
// eventos sólo ocurrirían en la superficie en donde haya pixeles definidos
QPainterPath MyPixmapItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}



void MyPixmapItem::edit_crop_rectangle(const QPointF &p)
{
    // .
    qreal dx, dy;
    if(rectangleSelected == &north_rectangle )
    {
        p.y()<0||p.y()>boundingRect().y()+boundingRect().height() ? dy=0 : dy=p.y()-crop_rectangle.y();
        crop_rectangle.adjust(0,dy,0,0);
        if(crop_rectangle.height()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setY(crop_rectangle.bottom()-3*DISTANCE_FOR_RESIZE);}

        correct_crop_rectangle();
    }


    if(rectangleSelected == &south_rectangle )
    {
        p.y()<0||p.y()>boundingRect().y()+boundingRect().height() ? dy=0 : dy=crop_rectangle.y()+crop_rectangle.height()-p.y();
        crop_rectangle.adjust(0,0,0,-dy);
        if(crop_rectangle.height()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setHeight(3*DISTANCE_FOR_RESIZE);}

        correct_crop_rectangle();
    }


    if(rectangleSelected == &west_rectangle )
    {
        p.x()<0||p.x()>boundingRect().x()+boundingRect().width() ? dx=0 : dx=p.x()-crop_rectangle.x();
        crop_rectangle.adjust(dx,0,0,0);
        if(crop_rectangle.width()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setX(crop_rectangle.right()-3*DISTANCE_FOR_RESIZE);}

        correct_crop_rectangle();
    }

    if(rectangleSelected == &east_rectangle )
    {
        p.x()<0||p.x()>boundingRect().x()+boundingRect().width() ? dx=0 : dx=crop_rectangle.x()+crop_rectangle.width()-p.x();
        crop_rectangle.adjust(0,0,-dx,0);
        if(crop_rectangle.width()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setWidth(3*DISTANCE_FOR_RESIZE);}

        correct_crop_rectangle();
    }


    if(rectangleSelected == &north_west_rectangle)
    {
        p.x()<0||p.x()>boundingRect().x()+boundingRect().width() ? dx=0 : dx=p.x()-crop_rectangle.x();
        p.y()<0||p.y()>boundingRect().y()+boundingRect().height() ? dy=0 : dy=p.y()-crop_rectangle.y();
        crop_rectangle.adjust(dx,dy,0,0);
        if(crop_rectangle.height()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setY(crop_rectangle.bottom()-3*DISTANCE_FOR_RESIZE);}
        if(crop_rectangle.width()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setX(crop_rectangle.right()-3*DISTANCE_FOR_RESIZE);}

        correct_crop_rectangle();
    }

    if(rectangleSelected == &north_east_rectangle)
    {
        p.x()<0||p.x()>boundingRect().x()+boundingRect().width() ? dx=0 : dx=crop_rectangle.x()+crop_rectangle.width()-p.x();
        p.y()<0||p.y()>boundingRect().y()+boundingRect().height() ? dy=0 : dy=p.y()-crop_rectangle.y();
        crop_rectangle.adjust(0,dy,-dx,0);
        if(crop_rectangle.height()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setY(crop_rectangle.bottom()-3*DISTANCE_FOR_RESIZE);}
        if(crop_rectangle.width()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setWidth(3*DISTANCE_FOR_RESIZE);}

        correct_crop_rectangle();
    }


    if(rectangleSelected == &south_east_rectangle)
    {
        p.x()<0||p.x()>boundingRect().x()+boundingRect().width() ? dx=0 : dx=crop_rectangle.x()+crop_rectangle.width()-p.x();
        p.y()<0||p.y()>boundingRect().y()+boundingRect().height() ? dy=0 : dy=crop_rectangle.y()+crop_rectangle.height()-p.y();
        crop_rectangle.adjust(0,0,-dx,-dy);
        if(crop_rectangle.height()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setHeight(3*DISTANCE_FOR_RESIZE);}
        if(crop_rectangle.width()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setWidth(3*DISTANCE_FOR_RESIZE);}

        correct_crop_rectangle();
    }

    if(rectangleSelected == &south_west_rectangle)
    {
        p.x()<0||p.x()>boundingRect().x()+boundingRect().width() ? dx=0 : dx=p.x()-crop_rectangle.x();
        p.y()<0||p.y()>boundingRect().y()+boundingRect().height() ? dy=0 : dy=crop_rectangle.y()+crop_rectangle.height()-p.y();
        crop_rectangle.adjust(dx,0,0,-dy);
        if(crop_rectangle.height()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setHeight(3*DISTANCE_FOR_RESIZE);}
        if(crop_rectangle.width()<3*DISTANCE_FOR_RESIZE){crop_rectangle.setX(crop_rectangle.right()-3*DISTANCE_FOR_RESIZE);}

        correct_crop_rectangle();
    }

    if(rectangleSelected == &center_rectangle)
    {

        dx = p.x()-mousePresPosition.x();
        dy = p.y()-mousePresPosition.y();
        crop_rectangle.adjust(dx,dy,dx,dy);
        mousePresPosition = p;
        if(crop_rectangle.x()<0 || crop_rectangle.right()>boundingRect().right()){
            crop_rectangle.adjust(-dx,0,-dx,0);
        }
        if(crop_rectangle.y()<0 || crop_rectangle.bottom()>boundingRect().bottom()){
            crop_rectangle.adjust(-0,-dy,0,-dy);
        }

    }    
    calculateEdgesRectangules();
    emit crop_rectangle_change(this->crop_rectangle);
}

// cuando el rectangulo de corte queda muy proximo a los bordes,
// del rectangulo original de la imagen, se realiza un ajuste
// para igualar ambos rectangulos. Esto mejora la experiencia
// del usuario cuando se realiza edición del rectangulo "crop_rectangle.
void MyPixmapItem::correct_crop_rectangle()
{
    // coordenada y()
    if(qAbs(crop_rectangle.top()-boundingRect().top())<DISTANCE_FOR_RESIZE/5)
        crop_rectangle.setTop(boundingRect().top());
    // coordenada x()
    if(qAbs(crop_rectangle.bottom()-boundingRect().bottom())<DISTANCE_FOR_RESIZE/5)
        crop_rectangle.setBottom(boundingRect().bottom());
    // coordenada left()
    if(qAbs(crop_rectangle.left()-boundingRect().left())<DISTANCE_FOR_RESIZE/5)
        crop_rectangle.setLeft(boundingRect().left());
    // coordenada right()
    if(qAbs(crop_rectangle.right()-boundingRect().right())<DISTANCE_FOR_RESIZE/5)
        crop_rectangle.setRight(boundingRect().right());

    emit crop_rectangle_change(this->crop_rectangle);
}

QCursor MyPixmapItem::selectCursor(const QPointF &p)
{
//    QMatrix rm;
//    rm.rotate(0);
//    QCursor( QPixmap(":/icons/cursors/cursor_1.png").transformed(rm) );
    if( north_west_rectangle.contains(p) ){
        //            qDebug()<<"\nnorth_west_rectangle";
        rectangleSelected = &north_west_rectangle;
        return myCursors["F_diagonal"];
    }

    else if( north_rectangle.contains(p) ){
        //            qDebug()<<"\nnorth_rectangle";
        rectangleSelected = &north_rectangle;
        return myCursors["vertical"];
    }

    else if( north_east_rectangle.contains(p) ){
        //            qDebug()<<"\nnorth_east_rectangle";
        rectangleSelected = &north_east_rectangle;
        return myCursors["B_Diagonal"];
    }

    else if( east_rectangle.contains(p) ){
        //            qDebug()<<"\neast_rectangle";
        rectangleSelected = &east_rectangle;
        return myCursors["horizontal"];
    }

    else if( south_east_rectangle.contains(p) ){
        //            qDebug()<<"\nsouth_east_rectangle";
        rectangleSelected = &south_east_rectangle;
        return myCursors["F_diagonal"];
    }

    else if( south_rectangle.contains(p) ){
        //            qDebug()<<"\nsouth_rectangle";
        rectangleSelected = &south_rectangle;
        return myCursors["vertical"];
    }

    else if( south_west_rectangle.contains(p) ){
        //            qDebug()<<"\nsouth_west_rectangle";
        rectangleSelected = &south_west_rectangle;
        return myCursors["B_Diagonal"];
    }

    else if( west_rectangle.contains(p) ){
        //            qDebug()<<"\nwest_rectangle";
        rectangleSelected = &west_rectangle;
        return myCursors["horizontal"];
    }

    else if( center_rectangle.contains(p) && initCropFlag ){
        //            qDebug()<<"\ncenter_rectangle";
        rectangleSelected = &center_rectangle;
        return myCursors["allCursors"];
    }

    else{
        //            qDebug()<<"\nNULL";
        rectangleSelected = NULL;
        return QCursor(Qt::ArrowCursor);
    }
}

void MyPixmapItem::setFullRect(const QRectF &rect)
{
    if (rect == crop_rectangle)
        return;
    prepareGeometryChange();
    crop_rectangle = rect;
}

// Calcula los rectangulos auxiliares
void MyPixmapItem::calculateEdgesRectangules()
{
    north_west_rectangle = QRectF(crop_rectangle.x()-DISTANCE_FOR_RESIZE,
                                  crop_rectangle.y()-DISTANCE_FOR_RESIZE,
                                  2*DISTANCE_FOR_RESIZE, 2*DISTANCE_FOR_RESIZE);

    north_rectangle = QRectF(crop_rectangle.x()+DISTANCE_FOR_RESIZE,
                             crop_rectangle.y()-DISTANCE_FOR_RESIZE,
                             crop_rectangle.width()-2*DISTANCE_FOR_RESIZE, 2*DISTANCE_FOR_RESIZE);


    north_east_rectangle = QRectF(crop_rectangle.width()+crop_rectangle.x()-DISTANCE_FOR_RESIZE,
                                  crop_rectangle.y()-DISTANCE_FOR_RESIZE,
                                  2*DISTANCE_FOR_RESIZE, 2*DISTANCE_FOR_RESIZE);


    east_rectangle = QRectF(crop_rectangle.width()+crop_rectangle.x()-DISTANCE_FOR_RESIZE,
                            crop_rectangle.y()+DISTANCE_FOR_RESIZE,
                            2*DISTANCE_FOR_RESIZE, crop_rectangle.height()-2*DISTANCE_FOR_RESIZE);


    south_east_rectangle = QRectF(crop_rectangle.width()+crop_rectangle.x()-DISTANCE_FOR_RESIZE,
                                  crop_rectangle.y()+crop_rectangle.height()-DISTANCE_FOR_RESIZE,
                                  2*DISTANCE_FOR_RESIZE, 2*DISTANCE_FOR_RESIZE);


    south_rectangle = QRectF(crop_rectangle.x()+DISTANCE_FOR_RESIZE,
                             crop_rectangle.y()+crop_rectangle.height()-DISTANCE_FOR_RESIZE,
                             crop_rectangle.width()-2*DISTANCE_FOR_RESIZE, 2*DISTANCE_FOR_RESIZE);


    south_west_rectangle = QRectF(crop_rectangle.x()-DISTANCE_FOR_RESIZE,
                                  crop_rectangle.y()+crop_rectangle.height()-DISTANCE_FOR_RESIZE,
                                  2*DISTANCE_FOR_RESIZE, 2*DISTANCE_FOR_RESIZE);


    west_rectangle = QRectF(crop_rectangle.x()-DISTANCE_FOR_RESIZE,
                            crop_rectangle.y()+DISTANCE_FOR_RESIZE,
                            2*DISTANCE_FOR_RESIZE, crop_rectangle.height()-2*DISTANCE_FOR_RESIZE);

    center_rectangle = QRectF( north_west_rectangle.right(), north_west_rectangle.bottom(),
                               crop_rectangle.width()-2*DISTANCE_FOR_RESIZE, crop_rectangle.height()-2*DISTANCE_FOR_RESIZE );

}

void MyPixmapItem::loadCursors()
{

    QMatrix shiftRotationMatrix, rotate45_Matrix, rotate90_Matrix, rotate135_Matrix;
    shiftRotationMatrix.rotate(shiftRotationAngle);
    rotate45_Matrix.rotate(45);
    rotate90_Matrix.rotate(90);
    rotate135_Matrix.rotate(135);


    myCursors["vertical"] = QCursor(
                QPixmap(":/icons/cursors/vertical.png").transformed(shiftRotationMatrix) );

    myCursors["horizontal"] = QCursor(
                QPixmap(":/icons/cursors/vertical.png").transformed(shiftRotationMatrix).transformed(rotate90_Matrix) );

    myCursors["F_diagonal"] = QCursor(
                QPixmap(":/icons/cursors/vertical.png").transformed(shiftRotationMatrix).transformed(rotate135_Matrix) );

    myCursors["B_Diagonal"] = QCursor(
                QPixmap(":/icons/cursors/vertical.png").transformed(shiftRotationMatrix).transformed(rotate45_Matrix) );

    myCursors["allCursors"] = QCursor(
                QPixmap(":/icons/cursors/allCursors.png") );

}

void MyPixmapItem::rotateCursors(int shiftRotationAngle)
{
    this->shiftRotationAngle = shiftRotationAngle;
    qApp->processEvents();
    loadCursors();
}







