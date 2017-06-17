#ifndef QFILEINFOANDSETTINGS_H
#define QFILEINFOANDSETTINGS_H

#include <QFileInfo>
#include <QRectF>
#include <QTransform>
#include <QDataStream>
class QFileInfoAndSettings : public QFileInfo
{
public:
    ~QFileInfoAndSettings();
    QFileInfoAndSettings();
    QFileInfoAndSettings(const QString &file);
    const QTransform& getTransform();
    const QRectF& getCropRegion();
    void setTransform(const QTransform& transform);
    void setCropRegion(const QRectF& cropRegion);
    void setRotationAngle(int rotationAngle);
    int getRotationAngle();

    // I/O support
    friend inline  QDataStream& operator<<(QDataStream& out, const QFileInfoAndSettings& f);
    friend inline  QDataStream& operator>>(QDataStream& in, QFileInfoAndSettings& f);

private:
    QTransform transform; // matriz identidad
    QRectF cropRegion;
    int rotationAngle;


};

inline QDataStream& operator<<(QDataStream &out, const QFileInfoAndSettings &f)
{
    QString path = f.absoluteFilePath();
    QRectF cropRect = f.cropRegion;
    qint32 angle = qint32(f.rotationAngle);
    QTransform trans = f.transform;
    return out<<path<<cropRect<<angle<<trans;
}

inline  QDataStream& operator>>(QDataStream& in, QFileInfoAndSettings& f)
{
    QString path;
    QRectF cropRect;
    qint32 angle;
    QTransform trans;

    in>>path>>cropRect>>angle>>trans;

    f.setFile(path);
    f.setCropRegion(cropRect);
    f.setRotationAngle(static_cast<int>(angle));
    f.setTransform(trans);

    return in;
}

#endif // QFILEINFOANDSETTINGS_H
