/*
 *  Custom Minecraft Launcher - ILMM - downloader.cpp
 *  by Hopskocz
 */

#include "downloader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QEventLoop>
#include "filedownloader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QProcess>

#include <QXmlStreamReader>


Downloader::Downloader( QObject *parent ) {
}

Downloader::~Downloader() {

}



void Downloader::downloadVersion( QString inVersion ) {

    QString version = inVersion;
    QByteArray json;

    FileDownloader *fileDownloader = new FileDownloader();

    emit hide( true );

    // Download minecraft version jar
    emit percentage( 0 );
    connect( fileDownloader, SIGNAL( percentage( int ) ), this, SLOT( filePerc( int ) ) );
    emit title( version+".jar" );

    fileDownloader->download( "http://s3.amazonaws.com/Minecraft.Download/versions/"+version+"/"+version+".jar",
                              QDir::currentPath()+"/versions/"+version+"/"+version+".jar", false );
    emit percentage( 100 );

    // Downloading json file which contains all needed libraries
    emit percentage( 0 );

    connect( fileDownloader, SIGNAL( percentage( int ) ), this, SLOT( filePerc( int ) ) );
    emit title( version+".json" );

    fileDownloader->download( "http://s3.amazonaws.com/Minecraft.Download/versions/"+version+"/"+version+".json",
                              QDir::currentPath()+"/versions/"+version+"/"+version+".json", true );

    emit percentage( 100 );

    QFile file( QDir::currentPath()+"/versions/"+version+"/"+version+".json" );
    if( file.open( QIODevice::ReadOnly | QIODevice::Text )) {
        QTextStream in( &file );
        json = in.readAll().toLocal8Bit();
        qDebug() << json;
        file.close();
    }

    QStringList fileList = parse( &json, version ); //extract informations from json

    QByteArray temparray = "";

    //prepare list
    for( int i = 0; i < fileList.length()-1; i++ ) {
#ifdef Q_OS_WIN
        temparray.append( QDir::currentPath()+"/libraries/"+fileList[i] ).append( "\";\"" );
#endif
#ifdef Q_OS_LINUX
        temparray.append( QDir::currentPath()+"/libraries/"+fileList[i] ).append( "\":\"" );
#endif
    }

    QFile libs( QDir::currentPath()+"/versions/"+version+"/libs.txt" );
    if( libs.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        libs.resize( 0 );
        QTextStream in( &libs );
        in << temparray;
        libs.close();

    }

    //download libraries here in loop
    for( int i = 0; i<fileList.length(); i++ ) {

        //percentage = (int)(((float)i/fileList.length())*100);
        emit percentage( (int)(((float)i/fileList.length())*100) );

        connect( fileDownloader, SIGNAL( percentage( int ) ), this, SLOT( filePerc( int ) ) );
        QStringList temp = fileList[i].split("/");
        emit title( temp[temp.length()-1] );

        //qDebug() << percentage << "%";
        fileDownloader->download( "http://s3.amazonaws.com/Minecraft.Download/libraries/"+fileList[i],
                                      QDir::currentPath()+"/libraries/"+fileList[i], false );

        if( fileList[i].contains("natives") ) {
            QProcess process;
            process.execute( "java UnzipJar \""+QDir::currentPath()+"/natives\" \""+QDir::currentPath()+"/libraries/"+fileList[i]+"\"" );
            //process.waitForFinished();
        }
    }
    emit percentage( 100 );
    qDebug() << "Finished";

    emit hide( false );

    emit finished();

    fileDownloader->deleteLater();
}

void Downloader::filePerc( int perc ) {
    emit filePercentage( perc );
}

QStringList Downloader::parse( QByteArray *inJson , QString inVersion ) {
    QStringList result;

    QJsonDocument jsonDoc = QJsonDocument::fromJson( *inJson );
    QJsonObject inside = jsonDoc.object();
    //qDebug() << inside;
    //qDebug() << inside.value("versions").isArray();

    // Getting mainClass
    QJsonValue mainClass = inside.value("mainClass");
    QFile mainCls( QDir::currentPath()+"/versions/"+inVersion+"/mainClass.txt" );

    if( !mainCls.open( QIODevice::WriteOnly | QIODevice::Text ))
        return QStringList();

    mainCls.resize( 0 );
    QTextStream out( &mainCls );
    out << mainClass.toString();
    mainCls.close();
    // End of getting fabulous main class

    // Getting arguments
    QJsonValue arguments = inside.value("minecraftArguments");
    QFile argFile( QDir::currentPath()+"/versions/"+inVersion+"/arguments.txt" );

    if( !argFile.open( QIODevice::WriteOnly | QIODevice::Text ))
        return QStringList();

    argFile.resize( 0 );
    QTextStream out2( &argFile );
    out2 << arguments.toString();
    argFile.close();
    // End of getting arguments

    QJsonValue val = inside.value("libraries");
    QJsonArray arr = val.toArray();
    //qDebug() << arr[0];
    QJsonObject obj;
    for( int i = 0; i<arr.size(); i++ ) {
        //qDebug() << arr[i];
        obj = arr[i].toObject();
        //qDebug() << obj["id"];
        QString temp = obj["name"].toString();
        if( temp.contains( "nightly" ) ) continue;
        QStringList tempList = temp.split(":");
        tempList[0].replace(".","/");
        temp = tempList.join("/");
        temp.append("/"+tempList[tempList.length()-2]).append("-"+tempList[tempList.length()-1]);
        if( !obj["natives"].isNull() ) {
            qDebug() << "Lo";
            QJsonObject objIns = obj["natives"].toObject();
#ifdef Q_OS_WIN32
            temp.append("-natives-windows.jar");
#endif
#ifdef Q_OS_LINUX
            temp.append("-natives-linux.jar");
#endif
        }
        else
            temp.append(".jar");


        result << temp;
    }
    //qDebug() << result;

    //qDebug() << value;
    return result;
}

void Downloader::downloadAssetsList() {
    // Download assets
    FileDownloader *fileDownloader = new FileDownloader();

    emit hide( true );

    emit percentage(0);

    connect( fileDownloader, SIGNAL( percentage( int ) ), this, SLOT( filePerc( int ) ) );
    emit title( "assets.xml" );

    fileDownloader->download( "http://s3.amazonaws.com/Minecraft.Resources",
                              QDir::currentPath()+"/assets/assets.xml", true );
    emit percentage(0);

    emit hide( false );
}

void Downloader::downloadResourcesList() {
    FileDownloader *fileDownloader = new FileDownloader();

    emit hide( true );

    emit percentage(0);

    connect( fileDownloader, SIGNAL( percentage( int ) ), this, SLOT( filePerc( int ) ) );
    emit title( "resources.xml" );

    fileDownloader->download( "http://s3.amazonaws.com/MinecraftResources",
                              QDir::currentPath()+"/resources/resources.xml", true );
    emit percentage(0);

    emit hide( false );
}



void Downloader::downloadAssets() {

    FileDownloader *fileDownloader = new FileDownloader();

    emit hide( true );

    QByteArray yxemel;
    QFile file( QDir::currentPath()+"/assets/assets.xml" );
    if( file.open( QIODevice::ReadOnly | QIODevice::Text )) {
        QTextStream in( &file );
        yxemel = in.readAll().toLocal8Bit();
        //qDebug() << yxemel;
        file.close();
    }


    QXmlStreamReader xml( yxemel );
    QStringList assetsList;

    while( !xml.atEnd() ) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if( token == QXmlStreamReader::StartDocument ) {
            continue;
        }
        if( token == QXmlStreamReader::StartElement ) {
            if( xml.name() == "Contents" ) {
                //qDebug() << "Lol :D";
                xml.readNext();

                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Contents")) {
                        if(xml.tokenType() == QXmlStreamReader::StartElement) {
                            if(xml.name() == "Key") {
                                xml.readNext();
                                if( xml.text().toString().contains( "." ) )
                                    assetsList << xml.text().toString();
                            }
                        }
                        xml.readNext();
                }
            }
        }
    }
    //qDebug() << assetsList;

    //download libraries here in loop
    for( int i = 0; i<assetsList.length(); i++ ) {

        emit percentage( (int)(((float)i/assetsList.length())*100) );

        connect( fileDownloader, SIGNAL( percentage( int ) ), this, SLOT( filePerc( int ) ) );
        QStringList temp = assetsList[i].split("/");
        emit title( temp[temp.length()-1] );

        fileDownloader->download( "http://s3.amazonaws.com/Minecraft.Resources/"+assetsList[i],
                                  QDir::currentPath()+"/assets/"+assetsList[i], false );

    }
    fileDownloader->deleteLater();

    emit hide( false );
}

void Downloader::downloadResources() {

    FileDownloader *fileDownloader = new FileDownloader();

    emit hide( true );

    QByteArray yxemel;
    QFile file( QDir::currentPath()+"/resources/resources.xml" );
    if( file.open( QIODevice::ReadOnly | QIODevice::Text )) {
        QTextStream in( &file );
        yxemel = in.readAll().toLocal8Bit();
        //qDebug() << yxemel;
        file.close();
    }


    QXmlStreamReader xml( yxemel );
    QStringList assetsList;

    while( !xml.atEnd() ) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if( token == QXmlStreamReader::StartDocument ) {
            continue;
        }
        if( token == QXmlStreamReader::StartElement ) {
            if( xml.name() == "Contents" ) {
                //qDebug() << "Lol :D";
                xml.readNext();

                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Contents")) {
                        if(xml.tokenType() == QXmlStreamReader::StartElement) {
                            if(xml.name() == "Key") {
                                xml.readNext();
                                if( xml.text().toString().contains( "." ) )
                                    assetsList << xml.text().toString();
                            }
                        }
                        xml.readNext();
                }
            }
        }
    }
    qDebug() << assetsList;

    //download libraries here in loop
    for( int i = 0; i<assetsList.length(); i++ ) {

        emit percentage( (int)(((float)i/assetsList.length())*100) );

        connect( fileDownloader, SIGNAL( percentage( int ) ), this, SLOT( filePerc( int ) ) );
        QStringList temp = assetsList[i].split("/");
        emit title( temp[temp.length()-1] );

        fileDownloader->download( "http://s3.amazonaws.com/MinecraftResources/"+assetsList[i],
                                  QDir::currentPath()+"/resources/"+assetsList[i], false );

    }
    fileDownloader->deleteLater();

    emit hide( false );
}

