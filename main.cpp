#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <iostream>
#include <QSplashScreen>
#include <QTimer>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Christian Camilo Mesa.");
    QCoreApplication::setOrganizationDomain("ing.cmesa@gmail.com");
    QCoreApplication::setApplicationName(MY_APP_NAME " V" VERSION_OF_PDF_CONVERTER);
    QCoreApplication::setApplicationVersion(VERSION_OF_PDF_CONVERTER);

    QCommandLineParser parser; // ver ejemplo "Application Example"
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    MainWindow mainWin;    
    QSplashScreen splash(&mainWin, QPixmap(":/icons/App_Icon.png"));

    splash.show();
    QTimer::singleShot(1500, &splash, SLOT(close()));
    QTimer::singleShot(500, &mainWin, SLOT(show()));

//    mainWin.show(); // alternative to show the main window
    return app.exec();
}
