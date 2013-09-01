/*
 *  Custom Minecraft Launcher - ILMM - filedownloader.cpp
 *  by Hopskocz
 */

#include "filedownloader.h"
#include <QDebug>
#include <QDir>
#include <QEventLoop>

FileDownloader::FileDownloader( QObject *parent ) :
    QObject(parent) {
    m_WebCtrl = new QNetworkAccessManager;
}

FileDownloader::~FileDownloader() {
    m_WebCtrl->deleteLater();
}

void FileDownloader::download( QString url, QString dest, bool update ) {

    m_WebCtrl->clearAccessCache();

    QDir dir = QDir::currentPath();
    QStringList path = dest.split( "/" );
    path[ path.length()-1 ].clear();
    if( !QDir( path.join("/")).exists() ) {
        dir.mkpath( path.join( "/" ) );
    }

    m_File = new QFile;
    m_File->setFileName( dest );
    if( m_File->exists() && !update ) {
        return;
    }
    m_File->open( QIODevice::WriteOnly );
    //
    //QObject::connect( m_WebCtrl, SIGNAL( finished( QNetworkReply* )), &eventLoop, SLOT( quit() ));
    QUrl encUrl = QUrl::fromEncoded( url.toLocal8Bit() );
    QNetworkRequest request( encUrl );
    m_Reply = m_WebCtrl->get( request );





    QEventLoop eventLoop;
    connect( m_Reply, SIGNAL( downloadProgress( qint64, qint64 ) ), this, SLOT( onDownloadProgress( qint64, qint64 ) ) );
    connect( m_WebCtrl, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( onFinished( QNetworkReply* ) ) );
    connect( m_Reply, SIGNAL( readyRead() ), this, SLOT( onReadyRead() ) );
    connect( m_Reply, SIGNAL( finished() ), this, SLOT( onReplyFinished() ) );
    connect( this, SIGNAL( endLoop() ), &eventLoop, SLOT( quit() ) );
    eventLoop.exec();
}

void FileDownloader::onDownloadProgress( qint64 bytesRead, qint64 bytesTotal ) {
    //qDebug(QString::number( bytesRead ).toLatin1() +" - "+ QString::number( bytesTotal ).toLatin1() );
    emit percentage( ((int)(((float)bytesRead/(float)bytesTotal)*100)) );
}

void FileDownloader::onFinished( QNetworkReply *reply ) {
    if( reply->error() != QNetworkReply::NoError ) {
        qDebug() << reply->errorString().toLocal8Bit();
    }
    if( m_File->isOpen() ) {
        m_File->close();
        m_File->deleteLater();
    }
}

void FileDownloader::onReadyRead() {
    m_File->write( m_Reply->readAll() );
}

void FileDownloader::onReplyFinished() {
    if( m_File->isOpen() ) {
        m_File->close();
        m_File->deleteLater();
    }
    emit endLoop();
}
