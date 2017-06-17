#ifndef MYGENERALSETTINGS_H
#define MYGENERALSETTINGS_H
#include<QPagedPaintDevice>
#include<QDataStream>

class MyGeneralSettings
{
public:
    MyGeneralSettings();
    // setters
    void setStepRotationAngle(int stepRotationAngle);
    void setPageSize(QPagedPaintDevice::PageSize pagesSize);
    void setPageOrientation(QPageLayout::Orientation pageOrientation);
    void setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode);
    void setTransformationMode(Qt::TransformationMode kindOfRender);
    void setExportResolution_DPI(int exportResolution_DPI);

    // getters
    int getStepRotationAngle();
    QPagedPaintDevice::PageSize getPageSize();
    QPageLayout::Orientation getPageOrientation();
    Qt::AspectRatioMode getAspectRatioMode();
    Qt::TransformationMode getTransformationMode();
    int getExportResolution_DPI();

    // I/O support
    friend inline  QDataStream& operator<<(QDataStream& out, const MyGeneralSettings& mySettings);
    friend inline  QDataStream& operator>>(QDataStream& in, MyGeneralSettings& mySettings);


private:
    // members
    int stepRotationAngle;
    QPagedPaintDevice::PageSize pagesSize;
    QPageLayout::Orientation pageOrientation;
    Qt::AspectRatioMode aspectRatioMode;
    Qt::TransformationMode kindOfRender;
    int exportResolution_DPI;

};


inline  QDataStream& operator<<(QDataStream& out, const MyGeneralSettings& mySettings)
{

    int stepRotationAngle_out = mySettings.stepRotationAngle;
    QPagedPaintDevice::PageSize pagesSize_out = mySettings.pagesSize;
    QPageLayout::Orientation pageOrientation_out = mySettings.pageOrientation;
    Qt::AspectRatioMode aspectRatioMode_out = mySettings.aspectRatioMode;
    Qt::TransformationMode kindOfRender_out = mySettings.kindOfRender;
    int exportResolution_DPI_out = mySettings.exportResolution_DPI;



    return out<<qint32(stepRotationAngle_out)
            <<qint32(pagesSize_out)
           <<qint32(pageOrientation_out)
          <<qint32(aspectRatioMode_out)
         <<qint32(kindOfRender_out)
        <<qint32(exportResolution_DPI_out);
}

inline  QDataStream& operator>>(QDataStream& in, MyGeneralSettings& mySettings)
{
    qint32 stepRotationAngle_in;
    qint32 pagesSize_in;
    qint32 pageOrientation_in;
    qint32 aspectRatioMode_in;
    qint32 kindOfRender_in;
    int exportResolution_DPI_in;

    in>>stepRotationAngle_in>>pagesSize_in>>pageOrientation_in>>aspectRatioMode_in>>kindOfRender_in>>exportResolution_DPI_in;

    mySettings.setStepRotationAngle(static_cast<int>(stepRotationAngle_in));
    mySettings.setPageSize(static_cast<QPagedPaintDevice::PageSize>(pagesSize_in));
    mySettings.setPageOrientation(static_cast<QPageLayout::Orientation>(pageOrientation_in));
    mySettings.setAspectRatioMode(static_cast<Qt::AspectRatioMode>(aspectRatioMode_in));
    mySettings.setTransformationMode(static_cast<Qt::TransformationMode>(kindOfRender_in));
    mySettings.setExportResolution_DPI(static_cast<int>(exportResolution_DPI_in));

    return in;
}

#endif // MYGENERALSETTINGS_H
