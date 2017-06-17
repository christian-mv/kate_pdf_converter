#include "mydialogsettings.h"
#include "ui_mydialogsettings.h"
#include <QPagedPaintDevice>
#include <QMessageBox>


#include <QDebug>

MyDialogSettings::MyDialogSettings(QWidget *parent,  MyGeneralSettings *myGeneralSettings) :
    QDialog(parent),
    ui(new Ui::MyDialogSettings)
{


    ui->setupUi(this);   
    ui->button_cancel->setFocus(); // set focus
    loadMenusOptions(); // load items in the interfaz
    this->myGeneralSettings = myGeneralSettings; // acces to outside object: MyGeneralSettings
    loadAllConfigurationSettingsFromMyGeneralSettingsObject(); // load configuration selected in MyGeneralSettings object
//    qDebug()<<"Page orientation after: "<<myGeneralSettings->getPageOrientation();
}



MyDialogSettings::~MyDialogSettings()
{
//    qDebug()<<"Destructor 'MyDialogSettings'";
    delete ui;
}

void MyDialogSettings::loadMenusOptions()
{   

    // --- page size

    for(int i=0; i<=10; i++){
        ui->comboBox_pageSize->addItem( "A"+QString::number(i) );
    }
    for(int i=0; i<=10; i++){
        ui->comboBox_pageSize->addItem( "B"+QString::number(i) );
    }

    ui->comboBox_pageSize->addItem("C5E: 163 x 229 mm");
    ui->comboBox_pageSize->addItem("Comm10E: 105 x 241 mm");
    ui->comboBox_pageSize->addItem("DLE: 110 x 220 mm");
    ui->comboBox_pageSize->addItem("Executive: 190.5 x 254 mm");
    ui->comboBox_pageSize->addItem("Folio: 210 x 330 mm");
    ui->comboBox_pageSize->addItem("Ledger: 431.8 x 279.4 mm");
    ui->comboBox_pageSize->addItem("Legal: 215.9 x 355.6 mm");
    ui->comboBox_pageSize->addItem("Letter: 215.9 x 279.4 mm");
    ui->comboBox_pageSize->addItem("Tabloid: 279.4 x 431.8 mm");


    // --- page orientation
    ui->comboBox_pageOrientation->addItem("Portrait");
    ui->comboBox_pageOrientation->addItem("Landscape");


    // --- Aspect ratio
    ui->comboBox_aspectRatioMode->addItem("Ignore aspect ratio");
    ui->comboBox_aspectRatioMode->addItem("Keep aspect ratio");
    ui->comboBox_aspectRatioMode->addItem("Keep aspect ratio by expanding");


    // Type of rendering (transformation mode)
    ui->comboBox_typeOfRender->addItem("bilinear filtering");
    ui->comboBox_typeOfRender->addItem("no smoothing");    

}

void MyDialogSettings::setStepsRotationAngle()
{
    myGeneralSettings->setStepRotationAngle( ui->spinBox_stepsRotationAngle->value() );
}



void MyDialogSettings::setPageSize()
{
    switch (ui->comboBox_pageSize->currentIndex()) {
    case 0:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A0);
//        qDebug()<<"A0";
        break;
    case 1:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A1);
//        qDebug()<<"A1";
        break;
    case 2:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A2);
//        qDebug()<<"A2";
        break;
    case 3:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A3);
//        qDebug()<<"A3";
        break;
    case 4:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A4);
//        qDebug()<<"A4";
        break;
    case 5:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A5);
//        qDebug()<<"A5";
        break;
    case 6:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A6);
//        qDebug()<<"A6";
        break;
    case 7:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A7);
//        qDebug()<<"A7";
        break;
    case 8:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A8);
//        qDebug()<<"A8";
        break;
    case 9:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A9);
//        qDebug()<<"A9";
        break;
    case 10:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A10);
//        qDebug()<<"A10";
        break;
    case 11:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B0);
//        qDebug()<<"B0";
        break;
    case 12:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B1);
//        qDebug()<<"B1";
        break;
    case 13:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B2);
//        qDebug()<<"B2";
        break;
    case 14:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B3);
//        qDebug()<<"B3";
        break;
    case 15:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B4);
//        qDebug()<<"B4";
        break;
    case 16:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B5);
//        qDebug()<<"B5";
        break;
    case 17:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B6);
//        qDebug()<<"B6";
        break;
    case 18:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B7);
//        qDebug()<<"B7";
        break;
    case 19:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B8);
//        qDebug()<<"B8";
        break;
    case 20:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B9);
//        qDebug()<<"B9";
        break;
    case 21:
        myGeneralSettings->setPageSize(QPagedPaintDevice::B10);
//        qDebug()<<"B10";
        break;
    case 22:
        myGeneralSettings->setPageSize(QPagedPaintDevice::C5E);
//        qDebug()<<"C5E";
        break;
    case 23:
        myGeneralSettings->setPageSize(QPagedPaintDevice::Comm10E);
//        qDebug()<<"Comm10E";
        break;
    case 24:
        myGeneralSettings->setPageSize(QPagedPaintDevice::DLE);
//        qDebug()<<"DLE";
        break;
    case 25:
        myGeneralSettings->setPageSize(QPagedPaintDevice::Executive);
//        qDebug()<<"Executive";
        break;
    case 26:
        myGeneralSettings->setPageSize(QPagedPaintDevice::Folio);
//        qDebug()<<"Folio";
        break;
    case 27:
        myGeneralSettings->setPageSize(QPagedPaintDevice::Ledger);
//        qDebug()<<"Ledger";
        break;
    case 28:
        myGeneralSettings->setPageSize(QPagedPaintDevice::Legal);
//        qDebug()<<"Legal";
        break;
    case 29:
        myGeneralSettings->setPageSize(QPagedPaintDevice::Letter);
//        qDebug()<<"Letter";
        break;
    case 30:
        myGeneralSettings->setPageSize(QPagedPaintDevice::Tabloid);
//        qDebug()<<"Tabloid";
        break;
    default:
        myGeneralSettings->setPageSize(QPagedPaintDevice::A4);
//        qDebug()<<"default";
        break;
    }
}

void MyDialogSettings::setPageOrientation()
{
    switch (ui->comboBox_pageOrientation->currentIndex()) {
    case 0:
        myGeneralSettings->setPageOrientation(QPageLayout::Portrait);
        break;
    case 1:
        myGeneralSettings->setPageOrientation(QPageLayout::Landscape);
        break;
    default:
        break;
    }
}

void MyDialogSettings::setAspectRationMode()
{
    switch (ui->comboBox_aspectRatioMode->currentIndex()) {
    case 0:
        myGeneralSettings->setAspectRatioMode(Qt::IgnoreAspectRatio);
        break;
    case 1:
        myGeneralSettings->setAspectRatioMode(Qt::KeepAspectRatio);
        break;
    case 2:
        myGeneralSettings->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
        break;
    default:
        myGeneralSettings->setAspectRatioMode(Qt::KeepAspectRatio);
        break;
    }
}

void MyDialogSettings::setTypeOfRendering()
{
    switch (ui->comboBox_typeOfRender->currentIndex()) {
    case 0:
        myGeneralSettings->setTransformationMode(Qt::SmoothTransformation);
        break;
    case 1:
        myGeneralSettings->setTransformationMode(Qt::FastTransformation);
        break;
    default:
        myGeneralSettings->setTransformationMode(Qt::SmoothTransformation);
        break;
    }
}

void MyDialogSettings::setExportResolution()
{
    myGeneralSettings->setExportResolution_DPI(ui->spinBox_exportResolution_DPI->value());
}

int MyDialogSettings::getStepsRotationAngle()
{
    return myGeneralSettings->getStepRotationAngle();
}

QPagedPaintDevice::PageSize MyDialogSettings::getPageSize()
{
    return myGeneralSettings->getPageSize();
}

QPageLayout::Orientation MyDialogSettings::getPageOrientation()
{
    return myGeneralSettings->getPageOrientation();
}

Qt::AspectRatioMode MyDialogSettings::getAspectRationMode()
{
    return myGeneralSettings->getAspectRatioMode();
}

Qt::TransformationMode MyDialogSettings::getTypeOfRendering()
{
    return myGeneralSettings->getTransformationMode();
}

int MyDialogSettings::getExportResolution()
{
    return myGeneralSettings->getExportResolution_DPI();
}

QString MyDialogSettings::toString()
{
    QString str("<br>-------- SETTINGS -------<br>");

            str += "<br>Steps Rotation: " + QString::number(getStepsRotationAngle())
            + "<br>Page Size: " + QString::number(getPageSize())
            + "<br>Page Orientation: " + QString::number(getPageOrientation())
            + "<br>Aspect RationMode: " + QString::number(getAspectRationMode())
            + "<br>Type Of Rendering: " + QString::number(getTypeOfRendering())
            + "<br>ExportResolution: " + QString::number(getExportResolution());

            return str;
}

void MyDialogSettings::loadAllConfigurationSettingsFromMyGeneralSettingsObject()
{

    loadStepRotationAngleFromMyGeneralSettingsObject();
    loadPageSizeFromMyGeneralSettingsObject();
    loadPPageOrientationFromMyGeneralSettingsObject();
    loadAspectRationModeFromMyGeneralSettingsObject();
    loadTypeOfRenderingFromMyGeneralSettingsObject();
    loadExportResolutionFromMyGeneralSettingsObject();
}

void MyDialogSettings::loadStepRotationAngleFromMyGeneralSettingsObject()
{
    ui->spinBox_stepsRotationAngle->setValue(myGeneralSettings->getStepRotationAngle());
}


void MyDialogSettings::loadPageSizeFromMyGeneralSettingsObject()
{
    switch ( myGeneralSettings->getPageSize() ) {
    case QPagedPaintDevice::A0:
        ui->comboBox_pageSize->setCurrentIndex(0);
        break;
    case QPagedPaintDevice::A1:
        ui->comboBox_pageSize->setCurrentIndex(1);
        break;
    case QPagedPaintDevice::A2:
        ui->comboBox_pageSize->setCurrentIndex(2);
        break;
    case QPagedPaintDevice::A3:
        ui->comboBox_pageSize->setCurrentIndex(3);
        break;
    case QPagedPaintDevice::A4:
        ui->comboBox_pageSize->setCurrentIndex(4);
        break;
    case QPagedPaintDevice::A5:
        ui->comboBox_pageSize->setCurrentIndex(5);
        break;
    case QPagedPaintDevice::A6:
        ui->comboBox_pageSize->setCurrentIndex(6);
        break;
    case QPagedPaintDevice::A7:
        ui->comboBox_pageSize->setCurrentIndex(7);
        break;
    case QPagedPaintDevice::A8:
        ui->comboBox_pageSize->setCurrentIndex(8);
        break;
    case QPagedPaintDevice::A9:
        ui->comboBox_pageSize->setCurrentIndex(9);
        break;
    case QPagedPaintDevice::A10:
        ui->comboBox_pageSize->setCurrentIndex(10);
        break;
    case QPagedPaintDevice::B0:
        ui->comboBox_pageSize->setCurrentIndex(11);
        break;
    case QPagedPaintDevice::B1:
        ui->comboBox_pageSize->setCurrentIndex(12);
        break;
    case QPagedPaintDevice::B2:
        ui->comboBox_pageSize->setCurrentIndex(13);
        break;
    case QPagedPaintDevice::B3:
        ui->comboBox_pageSize->setCurrentIndex(14);
        break;
    case QPagedPaintDevice::B4:
        ui->comboBox_pageSize->setCurrentIndex(15);
        break;
    case QPagedPaintDevice::B5:
        ui->comboBox_pageSize->setCurrentIndex(16);
        break;
    case QPagedPaintDevice::B6:
        ui->comboBox_pageSize->setCurrentIndex(17);
        break;
    case QPagedPaintDevice::B7:
        ui->comboBox_pageSize->setCurrentIndex(18);
        break;
    case QPagedPaintDevice::B8:
        ui->comboBox_pageSize->setCurrentIndex(19);
        break;
    case QPagedPaintDevice::B9:
        ui->comboBox_pageSize->setCurrentIndex(20);
        break;
    case QPagedPaintDevice::B10:
        ui->comboBox_pageSize->setCurrentIndex(21);
        break;
    case QPagedPaintDevice::C5E:
        ui->comboBox_pageSize->setCurrentIndex(22);
        break;
    case QPagedPaintDevice::Comm10E:
        ui->comboBox_pageSize->setCurrentIndex(23);
        break;
    case QPagedPaintDevice::DLE:
        ui->comboBox_pageSize->setCurrentIndex(24);
        break;
    case QPagedPaintDevice::Executive:
        ui->comboBox_pageSize->setCurrentIndex(25);
        break;
    case QPagedPaintDevice::Folio:
        ui->comboBox_pageSize->setCurrentIndex(26);
        break;
    case QPagedPaintDevice::Ledger:
        ui->comboBox_pageSize->setCurrentIndex(27);
        break;
    case QPagedPaintDevice::Legal:
        ui->comboBox_pageSize->setCurrentIndex(28);
        break;
    case QPagedPaintDevice::Letter:
        ui->comboBox_pageSize->setCurrentIndex(29);
        break;
    case QPagedPaintDevice::Tabloid:
        ui->comboBox_pageSize->setCurrentIndex(30);
        break;
    default:
        ui->comboBox_pageSize->setCurrentIndex(4); // A4
        break;
    }
}

void MyDialogSettings::loadPPageOrientationFromMyGeneralSettingsObject()
{
    switch ( myGeneralSettings->getPageOrientation() ) {
    case QPageLayout::Portrait:
        ui->comboBox_pageOrientation->setCurrentIndex(0);
        break;
    case QPageLayout::Landscape:
        ui->comboBox_pageOrientation->setCurrentIndex(1);
        break;
    default:
        ui->comboBox_pageOrientation->setCurrentIndex(0);
        break;
    }
}

void MyDialogSettings::loadAspectRationModeFromMyGeneralSettingsObject()
{
    switch (myGeneralSettings->getAspectRatioMode()) {
    case Qt::IgnoreAspectRatio:
        ui->comboBox_aspectRatioMode->setCurrentIndex(0);
        break;
    case Qt::KeepAspectRatio:
        ui->comboBox_aspectRatioMode->setCurrentIndex(1);
        break;
    case Qt::KeepAspectRatioByExpanding:
        ui->comboBox_aspectRatioMode->setCurrentIndex(2);
        break;
    default:
        ui->comboBox_aspectRatioMode->setCurrentIndex(1);
        break;
    }
}

void MyDialogSettings::loadTypeOfRenderingFromMyGeneralSettingsObject()
{
    switch (myGeneralSettings->getTransformationMode()) {
    case Qt::SmoothTransformation:
        ui->comboBox_typeOfRender->setCurrentIndex(0);
        break;
    case Qt::FastTransformation:
        ui->comboBox_typeOfRender->setCurrentIndex(1);
        break;
    default:
        ui->comboBox_typeOfRender->setCurrentIndex(0);
        break;
    }
}

void MyDialogSettings::loadExportResolutionFromMyGeneralSettingsObject()
{
    ui->spinBox_exportResolution_DPI->
            setValue(myGeneralSettings->
                     getExportResolution_DPI());
}



void MyDialogSettings::on_button_accept_clicked()
{
    this->setStepsRotationAngle();
    this->setPageSize();
    this->setPageOrientation();
    this->setAspectRationMode();
    this->setTypeOfRendering();
    this->setExportResolution();
//    QMessageBox::about(this, "Message", toString()); // only for depuration purposes

    delete this; // close dialog and clean memory
}

void MyDialogSettings::on_button_cancel_clicked()
{
    delete this;
}

void MyDialogSettings::on_MyDialogSettings_rejected()
{
    delete this;
}

void MyDialogSettings::on_button_defaultValues_clicked()
{
    ui->spinBox_stepsRotationAngle->setValue(90); // 90°
    ui->comboBox_pageSize->setCurrentIndex(4); // A4
    ui->comboBox_pageOrientation->setCurrentIndex(0); // Portrait
    ui->comboBox_aspectRatioMode->setCurrentIndex(1); // keep aspect ratio
    ui->comboBox_typeOfRender->setCurrentIndex(0); // bilinear filtering
    ui->spinBox_exportResolution_DPI->setValue(300); // 300 DPI
}
