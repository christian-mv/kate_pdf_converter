#include "mypixmaptopdfwriter.h"
#include<QMessageBox>
#include<QPixmap>
#include<QFileInfo>
#include<QPdfWriter>
#include <QRect>
#include<QPainter>
#include<QFileDialog>
#include <QDebug>
#include <QProgressDialog>
#include <QtWidgets>
#include <QThread>


MyPixmapToPdfWriter::MyPixmapToPdfWriter()
{

}

void MyPixmapToPdfWriter::setListOfPixmapSources(QList<QFileInfoAndSettings*> list)
{
    this->listOfPixmapSources = list;
}

void MyPixmapToPdfWriter::setListOfPixmapSources(QStringList &stringlist)
{

    foreach(QString tempString , stringlist)
    {
        this->listOfPixmapSources.append(new QFileInfoAndSettings(tempString));
    }
}

void MyPixmapToPdfWriter::setListOfPixmapSources(QString &str)
{
    this->listOfPixmapSources.append(new QFileInfoAndSettings(str));
}

void MyPixmapToPdfWriter::setListOfPixmapSources()
{
    QStringList stringFiles = QFileDialog::getOpenFileNames(Q_NULLPTR,
                                                            "Select one or more files to open",
                                                            "/home",
                                                            "Image Files (*.png *.jpg *.bmp)");
    if(stringFiles.isEmpty())
        return;
    else
        this->setListOfPixmapSources(stringFiles);

}

// A4 is de page size by default.
void MyPixmapToPdfWriter::setDefaultPageSize()
{
    this->pagesSize = QPagedPaintDevice::A4;
}

void MyPixmapToPdfWriter::setPageSize(QPagedPaintDevice::PageSize pageSize)
{
    this->pagesSize = pageSize;
}

void MyPixmapToPdfWriter::setKindOfRender(Qt::TransformationMode mode)
{
    // aquí solo puede haber las siguientes opciones:
    // Qt::FastTransformation = 0;
    // Qt::SmoothTransformation = 1;
    this->kindOfRender = mode;
}

void MyPixmapToPdfWriter::setExportResolution(int exportResolution)
{
    this->exportResolution = exportResolution;
}



void MyPixmapToPdfWriter::setPageOrientation(QPageLayout::Orientation pageOrientation)
{
    this->pageOrientation = pageOrientation; // todas las páginas tendrán esta orientación
}

void MyPixmapToPdfWriter::setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode)
{
    this->aspectRatioMode = aspectRatioMode;
}



bool MyPixmapToPdfWriter::writeSinglePdf(const QString& filePath)
{

    if(!checkIsThereAreImagesLoaded()){
        return false; // this mean: is not posible render the pdf file.
    }


    QPixmap currentPixmap;
    QFileInfoAndSettings currentFile;
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setResolution(exportResolution); // por defecto sería 1200 pero decidimos dejarlo en 300
    pdfWriter.setPageOrientation(this->pageOrientation); //orientación vertical (esta es por defecto)
    pdfWriter.setPageSize(this->pagesSize);
    QPainter painter(&pdfWriter);

    // barra de progreso.
    QProgressDialog progress;
    progress.setWindowModality(Qt::WindowModal);
//    progress.setWindowTitle(QObject::tr("Pdf converter"));
    progress.setLabelText(QObject::tr("Converting to PDF..."));
    progress.setMinimum(0);
    progress.setMaximum(this->listOfPixmapSources.count()-1);
    progress.show();
    qApp->processEvents();


    // propiedades de la impresora:
//            qDebug()<<"PdmWidth: "<<pdfWriter.width();
//            qDebug()<<"PdmHeight: "<<pdfWriter.height();
//            qDebug()<<"PdmWidthMM: "<<pdfWriter.widthMM();
//            qDebug()<<"PdmHeightMM: "<<pdfWriter.heightMM();
//            qDebug()<<"PdmNumColors: "<<pdfWriter.colorCount();
//            qDebug()<<"PdmDepth: "<<pdfWriter.depth();
//            qDebug()<<"PdmDpiX: "<<pdfWriter.logicalDpiX();
//            qDebug()<<"PdmDpiY: "<<pdfWriter.logicalDpiY();
//            qDebug()<<"PdmPhysicalDpiX: "<<pdfWriter.physicalDpiX();
//            qDebug()<<"PdmPhysicalDpiY: "<<pdfWriter.physicalDpiY();
//            qDebug()<<"PdmDevicePixelRatio: "<<pdfWriter.devicePixelRatio();
//            qDebug()<<"PdmDevicePixelRatioScaled: "<<pdfWriter.devicePixelRatioFScale();


    quint32 iWidth = pdfWriter.width();
    quint32 iHeight = pdfWriter.height();
    QSize s(iWidth, iHeight);
    QPoint centerPdfPoint(iWidth/2, iHeight/2);
    QRect rectPixmap;
    QMatrix *matrixOfRotation;




    for(int i = 0; i < this->listOfPixmapSources.length(); i++)
    {
        matrixOfRotation = new QMatrix();
        painter.restore();

        currentFile = *this->listOfPixmapSources.at(i);
        matrixOfRotation->rotate(currentFile.getRotationAngle());

        //qDebug()<<"ITERATION: "<<i<<" "<<currentFile.baseName();


        currentPixmap = QPixmap(currentFile.filePath())
                .copy( currentFile.getCropRegion().toRect() );                ;

        if(currentPixmap.isNull()){
//            qDebug()<<"ITERATION: "<<i<<" "<<"EROR with Pixmap: "<< currentFile.fileName();
            continue; // saltamos a la siguiente iteracion si no es valido el archivo.
        }


        currentPixmap = currentPixmap.transformed(*matrixOfRotation);
        currentPixmap = currentPixmap.scaled(s, this->aspectRatioMode, this->kindOfRender);


        rectPixmap = currentPixmap.rect();

        rectPixmap.moveCenter(centerPdfPoint);

//        rectPixmap.adjust(iWidth/2, 0, 0, 0);

        painter.drawPixmap(rectPixmap, currentPixmap); // dibujamos

        if(i < listOfPixmapSources.length()-1) // esto evita que quede una hoja en blanco al final
        {
            pdfWriter.newPage();
            progress.setValue(i);
            delete matrixOfRotation;
            qApp->processEvents();
        }

        else{
            progress.setValue(i);
            delete matrixOfRotation;
            qApp->processEvents();
            break;
        }
        if(progress.wasCanceled()){
            return false; //cancelled from progress bar dialog
        }
    }

    painter.end();
    progress.setValue( progress.maximum() );
    qApp->processEvents();
    return true; // all is ok.
}

bool MyPixmapToPdfWriter::checkIsThereAreImagesLoaded()
{
    if(this->listOfPixmapSources.isEmpty())
    {
        QMessageBox::information(Q_NULLPTR, QObject::tr("Operation no possible"),
                                 QObject::tr("You can't write a pdf file without any loaded image, "
                                             "try again loading some images"));
        return false;
    }
    else{
        return true;
    }
}



