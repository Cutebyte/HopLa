/*
 *  Custom Minecraft Launcher - ILMM - filedownloader.h
 *  by Hopskocz
 */

#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

class FileDownloader : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *m_WebCtrl;
    QNetworkReply *m_Reply;
    QFile *m_File;

public:
    explicit FileDownloader(QObject *parent = 0 );
    virtual ~FileDownloader();
    void download( QString, QString, bool );

signals:
    void endLoop();
    void percentage( int );

private slots:
    void onDownloadProgress( qint64, qint64 );
    void onFinished( QNetworkReply* );
    void onReadyRead();
    void onReplyFinished();

};

#endif // FILEDOWNLOADER_H
