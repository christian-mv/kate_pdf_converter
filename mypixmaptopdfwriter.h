/*
 * CLASE:   This classs export transformed pixmaps to pdf files
 *
 * AUTOR: Christian Mesa.
*/

#ifndef MYPIXMAPTOPDFWRITER_H
#define MYPIXMAPTOPDFWRITER_H

#include<QList>
#include<QPagedPaintDevice>
#include "qfileinfoandsettings.h"
class QPixmap;
class QFileInfo;
class QRect;
class QPdfWriter;


class MyPixmapToPdfWriter
{
public:
//    enum KindOfScale{ SCALED, UNSCALED};

    MyPixmapToPdfWriter();
    void setListOfPixmapSources(QList<QFileInfoAndSettings*>  list);
    void setListOfPixmapSources(QStringList &list);
    void setListOfPixmapSources(QString &str);
    void setListOfPixmapSources();
    void setDefaultPageSize();
    void setPageSize(QPagedPaintDevice::PageSize pageSize);
    void setPageOrientation(QPageLayout::Orientation pageOrientation); // esto aplica una orientación para todas las páginas
    void setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode);
    void setKindOfRender(Qt::TransformationMode mode);
    void setExportResolution(int exportResolution);

    bool writeSinglePdf(const QString& filePath);

private:
    QList<QFileInfoAndSettings*> listOfPixmapSources;
    QPagedPaintDevice::PageSize pagesSize = QPagedPaintDevice::A4;
    QPageLayout::Orientation pageOrientation = QPageLayout::Portrait;
    Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio;
    Qt::TransformationMode kindOfRender = Qt::SmoothTransformation;
    int exportResolution = 300;

    bool checkIsThereAreImagesLoaded();


};

#endif // MYPIXMAPTOPDFWRITER_H
