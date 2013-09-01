/*
 *  Custom Minecraft Launcher - ILMM - downloader.h
 *  by Hopskocz
 */

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QString>

class Downloader : public QObject
{
    Q_OBJECT
signals:
    void percentage( int );
    void title( QString );
    void hide( bool );
    void filePercentage( int );
    void finished();

public:
    explicit Downloader( QObject *parent = 0 );
    virtual ~Downloader();
    void downloadVersion( QString );
    void testDownload();
    void download();
    void downloadResources();
    void downloadResourcesList();
    void downloadAssetsList();
    void downloadAssets();
    void getResourcesList();
    QStringList parse( QByteArray*, QString );
private slots:
    void filePerc( int );
};

#endif // DOWNLOADER_H
