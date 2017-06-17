#include "mygeneralsettings.h"

MyGeneralSettings::MyGeneralSettings()
{
    setStepRotationAngle(90);
    setPageSize(QPagedPaintDevice::A4);
    setPageOrientation(QPageLayout::Portrait);
    setAspectRatioMode(Qt::KeepAspectRatio);
    setTransformationMode(Qt::SmoothTransformation);
    setExportResolution_DPI(300);
}

void MyGeneralSettings::setStepRotationAngle(int stepRotationAngle)
{
    this->stepRotationAngle = stepRotationAngle;
}

void MyGeneralSettings::setPageSize(QPagedPaintDevice::PageSize pagesSize)
{
    this->pagesSize = pagesSize;
}

void MyGeneralSettings::setPageOrientation(QPageLayout::Orientation pageOrientation)
{
    this->pageOrientation = pageOrientation;
}

void MyGeneralSettings::setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode)
{
    this->aspectRatioMode = aspectRatioMode;
}

void MyGeneralSettings::setTransformationMode(Qt::TransformationMode kindOfRender)
{
    this->kindOfRender = kindOfRender;
}

void MyGeneralSettings::setExportResolution_DPI(int exportResolution_DPI)
{
    this->exportResolution_DPI = exportResolution_DPI;
}

int MyGeneralSettings::getStepRotationAngle()
{
    return this->stepRotationAngle;
}



QPagedPaintDevice::PageSize MyGeneralSettings::getPageSize()
{
    return this->pagesSize;
}

QPageLayout::Orientation MyGeneralSettings::getPageOrientation()
{
    return this->pageOrientation;
}

Qt::AspectRatioMode MyGeneralSettings::getAspectRatioMode()
{
    return this->aspectRatioMode;
}


Qt::TransformationMode MyGeneralSettings::getTransformationMode()
{
    return this->kindOfRender;
}

int MyGeneralSettings::getExportResolution_DPI()
{
    return this->exportResolution_DPI;
}



