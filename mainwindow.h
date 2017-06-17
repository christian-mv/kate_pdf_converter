#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPagedPaintDevice>
#include<QGraphicsScene>
#include "mydialogsettings.h"
#include<QMap>
#include <QRectF>
#include "qfileinfoandsettings.h"
#include "mygeneralsettings.h"

#define VERSION_OF_PDF_CONVERTER "1.0"
#define MY_APP_NAME "Kate pdf converter"


class QFileInfo;
class MyPixmapItem;



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    void wheelEvent(QWheelEvent *event);

    // support for drag and drop
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);


private slots:
    void on_actionAdd_file_triggered(); //1
    void on_listWidget_clicked(const QModelIndex &index); // 2
    void on_actionRemove_file_triggered();    // 3
    void on_actionUpItem_triggered(); // 4
    void on_actionDownItem_triggered();  // 5
    void on_listWidget_currentRowChanged(int currentRow); // 6
    void on_actionExit_Program_triggered(); // 7
    void on_actionAbout_Qt_5_triggered(); // 8
    void on_actionAbout_PDF_converter_triggered(); // 10
    void on_actionExport_to_PDF_triggered(); // 11
    void on_actionCrop_Current_Image_triggered(); // 12
    void on_actionZoom_to_image_triggered(); // 13
    void showSelectedImage(int posIndexSelected); // 14


private:
    enum {MagicNumber = 0x7F51C883};
    Ui::MainWindow *ui;
    QString currentSessionFileName;
    MyDialogSettings *mySettings;
    MyGeneralSettings *myGeneralSettings;
    QList<QFileInfoAndSettings*> list_of_files;
    QGraphicsScene scene;
    MyPixmapItem *pixmapItem;    
    void updateListWidget(int currentRow =0);
    void addContextMenuToMyListWIdget();
    void readSettings();
    void writeSettings();
    QString myRecentDirectory;
    bool repaintImageFlag=true;
    QString getMyRecentDirectory();
    void setMyRecentDirectory(const QString &myRecentDirectory);
    void setCurrentSessionFileName(const QString& fileAbsolutaPath);
    void saveChangesInCurrentSession(); // used when exist a current file
    void createAndSaveCurrentSession(); // used when doesn't exist a current file
    bool writeFileSession(QString& fileAbsolutaPath);
    bool readFileSession(QString& fileName);
    bool isOkToContinue();
    void freeMemoryOf_list_of_files();
    bool modifiedSession;
    void aboutPdfConverter();


    void show_list_of_files();// depuration method

signals:
    void rotation_angle_change(int);

private slots:
    void updateCropRectangle(const QRectF &crop_rectangle);

    void on_actionLeftRotate_triggered();
    void on_actionRightRotate_triggered();
    void on_actionSettings_triggered();
    void on_actionSaveFile_triggered();
    void on_actionSave_As_triggered();
    void on_actionLoad_file_triggered();
    void on_actionNew_Session_triggered();

};


#endif // MAINWINDOW_H
