#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include <QtWidgets>
#include <QPainterPath>
#include <QObject>

class MyPixmapItem :  public QObject, public QGraphicsPixmapItem
{

    Q_OBJECT

     /* Spanish comment: al agregar el macro Q_OBJECT,  si da error algo
     como "undefined reference to 'vtable..."
     probar reejecutando qmake. Una alternativa podría ser
     eliminando la carpeta donde se crea el archivo ejecutable
     y volviendo a compilar el proyecto.
     ver: https://forum.qt.io/topic/9739/solved-custom-qgraphicspixmapitem/6*/

public:
    MyPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = NULL );

    virtual ~MyPixmapItem(){}

    void setCropRectangle(const QRectF &crop_rectangle);



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;


    QPainterPath shape() const;

private:
    qreal DISTANCE_FOR_RESIZE;
    bool editMode=true; // if is true, the mouse events will be ignored (this optimizes the app)
    bool initCropFlag = false; // this is activate when the user clikc on borders
    QRectF *rectangleSelected;
    QRectF crop_rectangle;
    QRectF north_west_rectangle;
    QRectF north_rectangle;
    QRectF north_east_rectangle;
    QRectF east_rectangle;
    QRectF south_east_rectangle;
    QRectF south_rectangle;
    QRectF south_west_rectangle;
    QRectF west_rectangle;
    QRectF center_rectangle;
    QPointF mousePresPosition;
    int shiftRotationAngle; // this is considered for rotating the pixmaps cursors.
    QHash<QString, QCursor> myCursors;

    void edit_crop_rectangle(const QPointF &p);
    void correct_crop_rectangle();
    QCursor selectCursor(const QPointF &p);
    void setFullRect(const QRectF& rect);
    void calculateEdgesRectangules();
    void loadCursors();

signals:
    void crop_rectangle_change(QRectF);


public slots:
    void rotateCursors(int shiftRotationAngle);

};

#endif // MYPIXMAPITEM_H
