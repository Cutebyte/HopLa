/*
 *  Custom Minecraft Launcher - ILMM - ilmm.h
 *  by Hopskocz
 */

#ifndef ILMM_H
#define ILMM_H

#include <QMainWindow>
#include <gamelauncher.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QMessageBox>
#include <QUuid>
#include <QProcess>

#include "versions.h"
#include "downloader.h"

namespace Ui {
class ILMM;
}

class ILMM : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ILMM(QWidget *parent = 0, QString version = "v0");
    ~ILMM();
    void launchGame();
    //void paintEvent( QPaintEvent *event );
    
private:
    Ui::ILMM *ui;
    Downloader *downloader;
    Versions *versions;

public slots:
    void legitMode();
    void nonlegitMode();
    void switchToOnline();
    void switchToOffline();
    void downloadVersions();
    void downloadResources();
    void loadVersions();
    void downloadFiles();
    void showOptions();
};

#endif // ILMM_H
