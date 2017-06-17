#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QList>
#include <QDebug>
#include "mypixmapitem.h"
#include <QSettings>
#include <QCloseEvent>
#include <QMimeData>
#include <QDropEvent>
#include "mypixmaptopdfwriter.h"
#include <QPrinter>
#include<QPrintDialog>
#include <cmath>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

//    this->scene.setBackgroundBrush( QBrush(QColor(255, 255, 255, 255)) );


    // ui->graphicsView es una instancia de la clase MyGraphicsView, la cual
    // hemos creado como una subclase de GraphicsView.
    ui->graphicsView->setRenderHint(QPainter::Antialiasing); // renderizado con suavizado
    ui->graphicsView->setAlignment(Qt::AlignCenter); // posición de items en esquina inferior derecha
    ui->graphicsView->setScene(&scene);
//    ui->listWidget->setIconSize(QSize(20, 20)); // establece el tamaño de los iconos de los items que se agregue al listWidget
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    addContextMenuToMyListWIdget();
    readSettings();

    setAcceptDrops(true);
    setWindowIcon(QIcon(":/icons/App_Icon.png"));    
    setWindowTitle(QString(MY_APP_NAME)+" V"+QString(VERSION_OF_PDF_CONVERTER));
    setCurrentSessionFileName("");
    modifiedSession = false;

    // settings object
    myGeneralSettings = new MyGeneralSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAdd_file_triggered()
{
    bool flag;
    list_of_files.isEmpty()?flag=true:flag=false; // this is for set the curren element

    QStringList stringFiles = QFileDialog::getOpenFileNames(
                this,
                "Select one or more files to open",
                getMyRecentDirectory(),
                "Image Files (*.BMP *.GIF *.JPEG *.JPG *.PNG *.PBM *.PGM *.PPM *.XBM *.XPM "
                            "*.bmp *.gif *.jpeg *.jpg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)");


    if(stringFiles.isEmpty()){return;}



    QApplication::setOverrideCursor(Qt::WaitCursor);
    QProgressDialog progress;
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimum(0);
    progress.setMaximum(stringFiles.length());
    progress.show();

    for(int i=0; i<stringFiles.length(); i++){
        list_of_files.append( new QFileInfoAndSettings(stringFiles.at(i)) );
        progress.setValue(i);
        qApp->processEvents();

    }

//    QFileInfoAndSettings *temp_file;
//    foreach(QString tempString ,stringFiles )
//    {
//        temp_file = new QFileInfoAndSettings(tempString);
//        list_of_files.append(temp_file);        
//    }

    setMyRecentDirectory( list_of_files.last()->absolutePath() );

    if(flag){
        updateListWidget(0);
    }else{
        updateListWidget(ui->listWidget->currentRow());
    }

    progress.setValue( progress.maximum() );
    progress.close();
    QApplication::restoreOverrideCursor();
    modifiedSession = true;
    qApp->processEvents();

}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    int posIndexSelected = index.row();
    showSelectedImage(posIndexSelected);
}

void MainWindow::on_actionRemove_file_triggered()
{
    auto x = ui->listWidget->selectionModel()->selectedIndexes();

    QList<int> rows;
    QList<QFileInfoAndSettings*> elementsToErase;

    for(int i=0; i<x.length(); i++)
    {
        list_of_files.insert(x.at(i).row(), nullptr); /* this is importan for keep the
                                                        same length of list_of_files
                                                        at the end of every iteration*/
        delete list_of_files.takeAt(x.at(i).row()+1);
    }

    // erase the nullptr ocurrences
    list_of_files.removeAll(nullptr);

    int currentRow = ui->listWidget->currentIndex().row();
    updateListWidget(currentRow);

    // si no hay elementos seleccionado, ó no existe elementos, retorna
    if (currentRow == 0)
        this->scene.clear();
    else if(currentRow < 0)
        return;

    modifiedSession = true;
}

void MainWindow::on_actionUpItem_triggered()
{
    // obtenemos indice actual
    int indice_to_chance = ui->listWidget->currentIndex().row();

    // Retornar cuando se está en la primera posición o cuando no hay elementos,
    if(indice_to_chance <= 0)  // también aplica cuando sólo hay un elemente
        return;


//    // actualizamos QList list_of_files

    list_of_files.move(indice_to_chance, indice_to_chance-1);
    //Actualizamos listWidget

    repaintImageFlag = false; // esto se hace para evitar que se repinte (esto optimiza)
    ui->listWidget->insertItem(indice_to_chance-1, ui->listWidget->takeItem(indice_to_chance));
    repaintImageFlag = true;

//    show_list_of_files(); // verificamos integridad de list_of_files

    ui->listWidget->setCurrentRow(indice_to_chance-1);

}


void MainWindow::on_actionDownItem_triggered()
{
    // obtenemos indice actual
    int indice_to_chance = ui->listWidget->currentIndex().row();
    int nItems = ui->listWidget->count(); // cantidad de items.


    // Retornar cuando se está en la primera posición o cuando no hay elementos,
    if(indice_to_chance >= nItems-1)  // también aplica cuando sólo hay un elemente
        return;

    //Actualizamos listWidget
    list_of_files.move(indice_to_chance, indice_to_chance+1);
    //Actualizamos listWidget

    repaintImageFlag = false; // esto se hace para evitar que se repinte (esto optimiza)
    ui->listWidget->insertItem(indice_to_chance+1, ui->listWidget->takeItem(indice_to_chance));
    repaintImageFlag = true;

    //    show_list_of_files(); // verificamos integridad de list_of_files

    ui->listWidget->setCurrentRow(indice_to_chance+1);

}

void MainWindow::showSelectedImage(int posIndexSelected)
{
    if(!repaintImageFlag)
        return;

    setCursor(Qt::WaitCursor);
    qApp->processEvents();
    QPixmap currentPixmap( list_of_files.at(posIndexSelected)->absoluteFilePath() ); // sin optimizar


/*    // optimización con QPixmapCache (http://doc.qt.io/archives/qq/qq12-qpixmapcache.html)
    QString key =list_of_files.at(posIndexSelected)->absoluteFilePath();

    if ( !QPixmapCache::find(key, &currentPixmap) ) {
        currentPixmap.load(key);
        QPixmapCache::insert(key, currentPixmap);
    }
    // fin optimización.
*/


    scene.clear();
    ui->graphicsView->resetTransform(); // reset QGraphicsView scale


    pixmapItem = new MyPixmapItem(currentPixmap);

    pixmapItem->rotateCursors(list_of_files.at(posIndexSelected)->getRotationAngle());
    connect(pixmapItem, SIGNAL( crop_rectangle_change(QRectF) ), this, SLOT( updateCropRectangle(QRectF) ) );
    connect(this, SIGNAL( rotation_angle_change(int) ), pixmapItem, SLOT( rotateCursors(int) ) );
    qApp->processEvents();
    scene.addItem(pixmapItem);
    pixmapItem->setPos(0,0);
    scene.setSceneRect(pixmapItem->boundingRect());
    ui->graphicsView->setScene(&scene);


    ui->graphicsView->setTransform(list_of_files.at(posIndexSelected)->getTransform());

    if(!list_of_files.at(posIndexSelected)->getCropRegion().isNull()){
        pixmapItem->setCropRectangle(list_of_files.at(posIndexSelected)->getCropRegion());
    }

    ui->graphicsView->show();
    setCursor(Qt::ArrowCursor);
    qApp->processEvents();

}


void MainWindow::show_list_of_files()
{
    QString myString;
    foreach (QFileInfo *tempFile, list_of_files) {
        myString += "\n"+tempFile->absoluteFilePath();
    }
        //mostramos cadena
    QMessageBox::information(this,tr("List of files"),myString);
}

void MainWindow::updateCropRectangle(const QRectF &crop_rectangle)
{
//    qDebug()<<crop_rectangle;

    if(list_of_files.isEmpty())
        return;

    list_of_files.at(ui->listWidget->currentIndex().row())->setCropRegion(crop_rectangle);
}



void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
//   // Your code here.
//   if(list_of_files.isEmpty())
//       return;
//   else{
//       if(ui->actionZoom_to_image->isChecked())
//           ui->graphicsView->fitInView(pixmapItem,Qt::KeepAspectRatio);
//   }


}

void MainWindow::closeEvent(QCloseEvent *event)
{
//    int currentRow = ui->listWidget->currentIndex().row();

    if(isOkToContinue())
    {
        this->writeSettings();
        event->accept();
    }else{
        event->ignore();
    }

}

void MainWindow::wheelEvent(QWheelEvent *event)
{
//    qDebug()<<event;

    if(list_of_files.isEmpty())
        return;

    list_of_files.at(ui->listWidget->currentIndex().row())->setTransform( ui->graphicsView->transform() );
    event->accept();
}



void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{

    const QMimeData *mimeData = event->mimeData();

    if (!mimeData->hasUrls())
        event->ignore();
    else
    {
        QList<QUrl> urlList = mimeData->urls();

        QFileInfoAndSettings *temp_file;
        foreach(QUrl tempUrl ,urlList ){
            temp_file = new QFileInfoAndSettings(tempUrl.toLocalFile());
            list_of_files.append(temp_file);
        }

        if(list_of_files.count()<0)
            updateListWidget();
        else
            updateListWidget(list_of_files.count()-1);

    }
}



void MainWindow::setCurrentSessionFileName(const QString &fileAbsolutaPath)
{
    this->currentSessionFileName = fileAbsolutaPath;

    if(fileAbsolutaPath.isEmpty()){

        setWindowTitle(QString(MY_APP_NAME)
                       +" V"+QString(VERSION_OF_PDF_CONVERTER)
                       +" - [Untitle]");
        return;
    }

    setWindowTitle(QString(MY_APP_NAME)
                   +" V"+QString(VERSION_OF_PDF_CONVERTER)
                   +" - ["+QFileInfo(fileAbsolutaPath).baseName()+"]");
}

void MainWindow::saveChangesInCurrentSession()
{
    // write file
    writeFileSession(this->currentSessionFileName);
}



void MainWindow::createAndSaveCurrentSession()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Session"), getMyRecentDirectory(),
                                                    tr(MY_APP_NAME" files (*.kate)"));
    // write file
    if(!writeFileSession(fileName)){
        statusBar()->showMessage(tr("Save cancelled"), 2000);
        return;
    }

    statusBar()->showMessage(tr("Save file"), 2000);
    setCurrentSessionFileName(fileName);
}

bool MainWindow::writeFileSession(QString& fileAbsolutaPath)
{
    if (fileAbsolutaPath.isEmpty() ) {        
        return false;
    }

    // This assures us that fileName has the route ".kate"
    if(QFileInfo(fileAbsolutaPath).completeSuffix().toLower()!="kate"){
        fileAbsolutaPath +=".kate";
    }

    QFile file(fileAbsolutaPath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr(MY_APP_NAME),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);

    out << quint32(MagicNumber);
    out << qint32( ui->listWidget->currentRow() ); //current file in lisWidget
    out << *myGeneralSettings;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QProgressDialog progress;
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimum(0);
    progress.setMaximum(list_of_files.length());
    progress.show();

    QFileInfoAndSettings temp;
    for(int i=0; i<list_of_files.length(); i++){
        temp = *list_of_files.at(i);
        out << temp;
        progress.setValue(i);
        qApp->processEvents();
    }
    progress.setValue( progress.maximum() );
    progress.close();
    QApplication::restoreOverrideCursor();
    qApp->processEvents();

    return true;
}

bool MainWindow::readFileSession(QString &fileName)
{
    QFile file(fileName);
    QString resultsOfReadingSession;
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr(MY_APP_NAME),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    quint32 magic;
    in >> magic;
    if (magic != MagicNumber) {
        QMessageBox::warning(this, tr(MY_APP_NAME),
                             tr("The file is not a " MY_APP_NAME " file"));
        return false;
    }

    int currentRow_in_listWidget;
    in >> currentRow_in_listWidget;
    in >> *myGeneralSettings;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    this->freeMemoryOf_list_of_files();
    QFileInfoAndSettings* temp;

    while (!in.atEnd()) {
        temp = new QFileInfoAndSettings();

        in >> *temp;
        if(!temp->exists()){
            if(resultsOfReadingSession.isEmpty()){resultsOfReadingSession += "\n------ missing files------ \n";}

            resultsOfReadingSession += "\n"+temp->absoluteFilePath();
        }else{
            list_of_files.append(temp);
        }
    }

    this->updateListWidget(currentRow_in_listWidget);
    QApplication::restoreOverrideCursor();
    if(!resultsOfReadingSession.isEmpty()){
        QMessageBox::warning(this, tr(MY_APP_NAME),
                             tr("The following files could not be loaded. Please verify that the files are in the correct path.\n " )+resultsOfReadingSession);
    }
    return true;
}

bool MainWindow::isOkToContinue()
{

    if(!list_of_files.isEmpty() || !currentSessionFileName.isEmpty())
    {
        int r = QMessageBox::warning(this, tr(MY_APP_NAME),
                                 tr("Do you want to save your changes before?"),
                                 QMessageBox::Yes | QMessageBox::No
                                 | QMessageBox::Cancel);

        //    qDebug()<<"r: "<<r;
        switch (r) {
        case QMessageBox::Cancel:
            return false;
            break;
        case QMessageBox::Yes:
            on_actionSaveFile_triggered();
            return true;
            break;
        case QMessageBox::No:
            return true;
            break;
        default:
            return true;
        }
    }else{
        return true;
    }
}

void MainWindow::freeMemoryOf_list_of_files()
{
    if(list_of_files.isEmpty()){
        return;
    }

    while(list_of_files.length()>0){
        // extraemos el elemento QFileInfo del contenedor QList y luego liberamos su memoria
        delete list_of_files.takeLast();
    }
    updateListWidget();
}

void MainWindow::aboutPdfConverter()
{
    QString msg;
    msg = tr("<h1>" MY_APP_NAME " " VERSION_OF_PDF_CONVERTER "</h1>"
             "<p>" MY_APP_NAME " " VERSION_OF_PDF_CONVERTER
             " is a multi-platform application that let you edit and export "
             "images to pdf files."
             "<br><br><b>" MY_APP_NAME " V" VERSION_OF_PDF_CONVERTER "</b>"
             " is licensed under LGPL v3</p>"
             "<div>Contact details:"
             "<ul><li>Author: Christian C. Mesa.</li>"
             "<li>Author's website: <a href = https://christian-mv.github.io/ >https://christian-mv.github.io/</a></li>"
             "<li>email: <a href = mailto:ing.cmesa@gmail.com> ing.cmesa@gmail.com</a></li>"
             );

    QMessageBox::about(this, tr("Pdf converter V. ")+VERSION_OF_PDF_CONVERTER, msg);
}


void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow<0){
        return;
    }
    else{
        showSelectedImage(currentRow);
    }
}

void MainWindow::updateListWidget(int currentRow)
{
//    qDebug()<<"start updateListWidget";
    ui->listWidget->clear();


    if(list_of_files.isEmpty()){
        return;
    }


    // validamos los archivos y los agregamos al listWidget
    foreach(QFileInfoAndSettings *tempFile, list_of_files )
    {
        if (QPixmap(tempFile->absoluteFilePath()).isNull())
            list_of_files.removeAll(tempFile); // si no puede ser QPixmap, lo eliminamos
        else{
            QListWidgetItem *item = new QListWidgetItem(tempFile->fileName(),
                                                        ui->listWidget);            
//            item->setIcon(QIcon(tempFile->absoluteFilePath())); // falta optimizar esto
            ui->listWidget->addItem(item);
        }
    }


    // actualizamos y validamos la posición inicial
    if(currentRow<0){currentRow=0;}

if(currentRow > ui->listWidget->count()-1){
    currentRow = ui->listWidget->count()-1;
}

    ui->listWidget->setCurrentRow(currentRow);
    ui->listWidget->item(currentRow)->setSelected(true);

//    ui->listWidget->setCurrentItem(ui->listWidget->itemAt(list_of_files.count(),0) );
//    this->show_list_of_files();

}

void MainWindow::addContextMenuToMyListWIdget()
{
    QAction *addFileAction, *removeFileAction, *upItemAction, *downItemAction;
    addFileAction = new QAction(ui->actionAdd_file->icon(),
                                tr("Add files"), this);

    removeFileAction = new QAction(ui->actionRemove_file->icon(),
                                   tr("Remove Selected file"), this);

    upItemAction = new QAction(ui->actionUpItem->icon(),
                               tr("Up item"), this);

    downItemAction = new QAction(ui->actionDownItem->icon(),
                                 tr("Down item"), this);

    ui->listWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->listWidget->addAction(addFileAction);
    ui->listWidget->addAction(removeFileAction);
    ui->listWidget->addAction(upItemAction);
    ui->listWidget->addAction(downItemAction);

    connect(addFileAction, SIGNAL(triggered()),
            this, SLOT(on_actionAdd_file_triggered()));

    connect(removeFileAction, SIGNAL(triggered()),
            this, SLOT(on_actionRemove_file_triggered()));

    connect(upItemAction, SIGNAL(triggered()),
            this, SLOT(on_actionUpItem_triggered()));

    connect(downItemAction, SIGNAL(triggered()),
            this, SLOT(on_actionDownItem_triggered()));
}

void MainWindow::on_actionExit_Program_triggered()
{
        close();
}

void MainWindow::on_actionAbout_Qt_5_triggered()
{
    QMessageBox::aboutQt(this, QString(MY_APP_NAME)+" V"+QString(VERSION_OF_PDF_CONVERTER));
}

void MainWindow::on_actionAbout_PDF_converter_triggered()
{
    aboutPdfConverter();

}


void MainWindow::on_actionExport_to_PDF_triggered()
{
    // object MyPixmapToPdfWriter
    MyPixmapToPdfWriter myPdfWriter;
    myPdfWriter.setListOfPixmapSources(this->list_of_files);
    myPdfWriter.setPageSize(myGeneralSettings->getPageSize());
    myPdfWriter.setPageOrientation(myGeneralSettings->getPageOrientation());
    myPdfWriter.setAspectRatioMode(myGeneralSettings->getAspectRatioMode());
    myPdfWriter.setKindOfRender(myGeneralSettings->getTransformationMode());
    myPdfWriter.setExportResolution(myGeneralSettings->getExportResolution_DPI());


    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export to PDF"),
                                                    getMyRecentDirectory()+QDir::separator()+"untitle.pdf",
                                                    tr("pdf file (*.pdf)"));
    if( fileName.isEmpty() )
        return;


    if(QFileInfo(fileName).completeSuffix().toLower()!="pdf"){
        fileName +=".pdf";
    }
//    setEnabled(false);

    qApp->processEvents(); // equivale a QCoreApplication::processEvents();

    if( !myPdfWriter.writeSinglePdf(fileName) ){
        return; // is not posible create the pdf
    }

//    setEnabled(true);
    qApp->processEvents();

    setMyRecentDirectory( QFileInfo(fileName).absolutePath() );

    QMessageBox::information(this, tr(MY_APP_NAME " V" VERSION_OF_PDF_CONVERTER)
                             ,tr("successful operation"));


//    // metodo 2 (cuando hay disponible impresora en el sistema)
//#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
//    QPrinter printer;
//    QPrintDialog dialog(&printer, this);
//    if (dialog.exec() == QDialog::Accepted) {
//        QPainter painter(&printer);
//        ui->graphicsView->render(&painter);
//    }
//#endif

}

void MainWindow::on_actionCrop_Current_Image_triggered()
{
//    qDebug()<<"on_actionCrop_Current_Image_triggered";
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();

    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    this->setMyRecentDirectory( settings.value("recentDirectory").toString() );
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue( "recentDirectory", getMyRecentDirectory() );
}

QString MainWindow::getMyRecentDirectory()
{
    if(myRecentDirectory.isEmpty())
            return QDir::homePath();
    else
        return myRecentDirectory;
}

void MainWindow::setMyRecentDirectory(const QString &myRecentDirectory)
{
    if(QDir(myRecentDirectory).exists()){
        this->myRecentDirectory = myRecentDirectory;
    }
    else{
        this->myRecentDirectory = QDir::homePath();
    }
}



void MainWindow::on_actionZoom_to_image_triggered()
{

    if( ui->listWidget->currentRow()>=0 ){

        qreal ofsset = 15;

        QRectF rectF = QRectF(pixmapItem->boundingRect().x()-ofsset,
                        pixmapItem->boundingRect().y()+ofsset,
                        pixmapItem->boundingRect().width()+ofsset,
                        pixmapItem->boundingRect().height()+ofsset);
            ui->graphicsView->fitInView(rectF,Qt::KeepAspectRatio);
//            ui->graphicsView->resetTransform(); // deshace la operación
    }
}

void MainWindow::on_actionLeftRotate_triggered()
{
    if(list_of_files.isEmpty())
        return;

    list_of_files.at(ui->listWidget->
                     currentIndex().row())->
            setRotationAngle(list_of_files.at(ui->listWidget->
                                              currentIndex().row())->getRotationAngle()-myGeneralSettings->getStepRotationAngle()%360 );

    ui->graphicsView->rotate(-myGeneralSettings->getStepRotationAngle());

//    qDebug()<<"angleOfRotation: "<<list_of_files.at(ui->listWidget->
//                                                    currentIndex().row())->getRotationAngle();
    list_of_files.at(ui->listWidget->currentIndex().row())->setTransform( ui->graphicsView->transform() );
    list_of_files.at(ui->listWidget->currentIndex().row())->setRotationAngle(list_of_files.at(ui->listWidget->
                                                                                              currentIndex().row())->getRotationAngle());
    emit rotation_angle_change(list_of_files.at(ui->listWidget->currentIndex().row())->getRotationAngle());

    // show Angle in status bar
    statusBar()->showMessage(
                QString::number(list_of_files.at(ui->listWidget->currentIndex().row())->getRotationAngle()%360)
                +"°", 500);

    qApp->processEvents();
}

void MainWindow::on_actionRightRotate_triggered()
{
    if(list_of_files.isEmpty())
        return;

    list_of_files.at(ui->listWidget->
                     currentIndex().row())->
            setRotationAngle(list_of_files.at(ui->listWidget->
                                              currentIndex().row())->getRotationAngle()+myGeneralSettings->getStepRotationAngle()%360 );

    ui->graphicsView->rotate(myGeneralSettings->getStepRotationAngle());

//    qDebug()<<"angleOfRotation: "<<list_of_files.at(ui->listWidget->
//                                                    currentIndex().row())->getRotationAngle();

    list_of_files.at(ui->listWidget->currentIndex().row())->setTransform( ui->graphicsView->transform() );
    list_of_files.at(ui->listWidget->currentIndex().row())->setRotationAngle(list_of_files.at(ui->listWidget->
                                                                                              currentIndex().row())->getRotationAngle());
    emit rotation_angle_change(list_of_files.at(ui->listWidget->currentIndex().row())->getRotationAngle());

    // show Angle in status bar
    statusBar()->showMessage(
                QString::number(list_of_files.at(ui->listWidget->currentIndex().row())->getRotationAngle()%360)
                +"°", 500);

    qApp->processEvents();
}

void MainWindow::on_actionSettings_triggered()
{
//    qDebug()<<"Page orientation before: "<<myGeneralSettings->getPageOrientation();
    mySettings = new MyDialogSettings(this, this->myGeneralSettings);
    mySettings->setWindowTitle( tr(" SETTINGS - "
                                MY_APP_NAME " V" VERSION_OF_PDF_CONVERTER ));
    mySettings->show();
    qApp->processEvents();

}

void MainWindow::on_actionSaveFile_triggered()
{
    if(currentSessionFileName.isEmpty()){
        createAndSaveCurrentSession();
    }else{
        saveChangesInCurrentSession();
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    createAndSaveCurrentSession();
}

void MainWindow::on_actionLoad_file_triggered()
{
    if(!isOkToContinue()){
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open a Kate-file session.." ), getMyRecentDirectory(),
                               tr("(*.kate)"));

    if(fileName.isEmpty()){
        return;
    }
    if(!readFileSession(fileName)){
        statusBar()->showMessage(tr("Load cancelled"), 2000);
        return;
    }
    setCurrentSessionFileName(fileName);
    modifiedSession = true;
}

void MainWindow::on_actionNew_Session_triggered()
{

    // by default only save in an existing session.
    if(modifiedSession){
        if(!isOkToContinue()){
            return;
        }
    }

    this->freeMemoryOf_list_of_files();
    this->updateListWidget();
    setCurrentSessionFileName("");
    modifiedSession = false;
    scene.clear();
    ui->graphicsView->resetTransform(); // reset QGraphicsView scale
}



