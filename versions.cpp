/*
 *  Custom Minecraft Launcher - ILMM - versions.cpp
 *  by Hopskocz
 */

#include "versions.h"
#include "filedownloader.h"
#include <QEventLoop>
#include <QDebug>
#include <QUrl>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDir>

Versions::Versions( QObject *parent ) {

}

Versions::~Versions() {

}

void Versions::downloadDB() {

    emit hide( true );

    FileDownloader *fileDownloader = new FileDownloader();

    connect( fileDownloader, SIGNAL( percentage( int ) ), this, SLOT( filePerc( int ) ) );

    fileDownloader->download( "http://s3.amazonaws.com/Minecraft.Download/versions/versions.json", QDir::currentPath()+"/versions/versions.json", true );
    qDebug() << "Trololo";
    //qDebug() << versions;
/*
    QFile verDB("versions.json");
    if( !verDB.open( QIODevice::WriteOnly | QIODevice::Text ))
        return;
    verDB.resize(0);
    QTextStream out( &verDB );
    out << versions;
    verDB.close();
*/
    fileDownloader->deleteLater();
    emit hide( false );
}

void Versions::filePerc( int perc ) {
    emit filePercentage( perc );
    emit percentage( perc );
}

QByteArray Versions::loadDB() {
    QFile verDB(QDir::currentPath()+"/versions/versions.json");
    if( !verDB.open( QIODevice::ReadOnly | QIODevice::Text ))
        return "";

    QByteArray json;

    QTextStream in( &verDB );
    json = in.readAll().toLocal8Bit();
    verDB.close();
    return json;

}

QStringList Versions::parseJson( QByteArray *json ) {
    QStringList result;

    QJsonDocument jsonDoc = QJsonDocument::fromJson( *json );
    QJsonObject inside = jsonDoc.object();
    //qDebug() << inside;
    //qDebug() << inside.value("versions").isArray();
    QJsonValue val = inside.value("versions");
    QJsonArray arr = val.toArray();
    //qDebug() << arr[0];
    QJsonObject obj;
    for( int i = 0; i<arr.size(); i++ ) {
        //qDebug() << arr[i];
        obj = arr[i].toObject();
        //qDebug() << obj["id"];
        result << obj["id"].toString();
    }
    //qDebug() << result;

    //qDebug() << value;

    return result;
}
