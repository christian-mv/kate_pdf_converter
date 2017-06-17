#include "qfileinfoandsettings.h"
#include <QDebug>

QFileInfoAndSettings::~QFileInfoAndSettings()
{
//    qDebug()<<"Removing "<< this->absoluteFilePath();
}

QFileInfoAndSettings::QFileInfoAndSettings()
{
    rotationAngle = 0;
}

QFileInfoAndSettings::QFileInfoAndSettings(const QString &file)
    :QFileInfo(file)
{
    rotationAngle = 0;
}

const QTransform& QFileInfoAndSettings::getTransform()
{
    return transform;
}

void QFileInfoAndSettings::setTransform(const QTransform& transform)
{
    this->transform = transform;
}

void QFileInfoAndSettings::setCropRegion(const QRectF& cropRegion)
{
    this->cropRegion = cropRegion;
}

void QFileInfoAndSettings::setRotationAngle(int rotationAngle)
{
    this->rotationAngle = rotationAngle;
}


int QFileInfoAndSettings::getRotationAngle()
{
    return this->rotationAngle;
}



const QRectF& QFileInfoAndSettings::getCropRegion()
{
    return cropRegion;
}


