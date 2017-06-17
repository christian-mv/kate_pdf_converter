#ifndef MYDIALOGSETTINGS_H
#define MYDIALOGSETTINGS_H

#include <QDialog>
#include "mygeneralsettings.h"


namespace Ui {
class MyDialogSettings;
}

class MyDialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialogSettings(QWidget *parent, MyGeneralSettings *myGeneralSettings);

    ~MyDialogSettings();
    void loadMenusOptions();

    // setters
    void setStepsRotationAngle();
    void setPageSize();
    void setPageOrientation();
    void setAspectRationMode();
    void setTypeOfRendering();
    void setExportResolution();

    // getters
    int getStepsRotationAngle();
    QPagedPaintDevice::PageSize getPageSize();
    QPageLayout::Orientation getPageOrientation();
    Qt::AspectRatioMode getAspectRationMode();
    Qt::TransformationMode getTypeOfRendering();
    int getExportResolution();

    // show Class
    QString toString();

    // load configuration settings from MyGeneralSettings object
    void loadAllConfigurationSettingsFromMyGeneralSettingsObject();

    void loadStepRotationAngleFromMyGeneralSettingsObject();
    void loadPPageOrientationFromMyGeneralSettingsObject();
    void loadPageSizeFromMyGeneralSettingsObject();
    void loadAspectRationModeFromMyGeneralSettingsObject();
    void loadTypeOfRenderingFromMyGeneralSettingsObject();
    void loadExportResolutionFromMyGeneralSettingsObject();

private slots:
    void on_button_accept_clicked();
    void on_button_cancel_clicked();
    void on_MyDialogSettings_rejected();
    void on_button_defaultValues_clicked();

private:
    Ui::MyDialogSettings *ui;
    MyGeneralSettings *myGeneralSettings;
};

#endif // MYDIALOGSETTINGS_H
