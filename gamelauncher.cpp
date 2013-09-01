/*
 *  Custom Minecraft Launcher - ILMM - gamelauncher.cpp
 *  by Hopskocz
 */

#include "gamelauncher.h"
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QDirIterator>
#include <QUrlQuery>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QUuid>
#include <QFile>
#include <QProcess>
#include "filedownloader.h"


GameLauncher::GameLauncher() {
    command = "";
}

void GameLauncher::setPremium() {
    legit = true;
}

void GameLauncher::setNonPremium() {
    legit = false;
}

void GameLauncher::setPassword( QString inPassword ) {
    password = inPassword;
}

void GameLauncher::setUsername( QString inUsername ) {
    username = inUsername;
}

QString GameLauncher::getCommand() {
    return command;
}

bool GameLauncher::isLegit() {
    return legit;
}

int GameLauncher::commandGen(  ) {

    Authentication auth;
    auth.setUsername( username );
    auth.setPassword( password );

    QByteArray mainClass;
    QString arguments;

    QFile mainClassFile(QDir::currentPath()+"/versions/"+version+"/mainClass.txt" );
    if( mainClassFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        QTextStream in( &mainClassFile );
        mainClass = in.readAll().toLocal8Bit();
        mainClassFile.close();
    }
    else return -666;


    QFile argFile( QDir::currentPath()+"/versions/"+version+"/arguments.txt" );
    if( argFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        QTextStream in( &argFile );
        arguments = in.readAll().toLocal8Bit();
    }
    else return -666;


    qDebug() << arguments;

    if( isLegit() ) {
        QString result = "";
        QString gameUsername = "";

        QEventLoop eventLoop;

        QNetworkAccessManager mgr;
        int errCode;
        errCode = auth.login( &mgr );

        arguments.replace( "${auth_player_name}", auth.getGameUsername() );
        arguments.replace( "${auth_session}", "token:"+auth.getAccessToken().toLocal8Bit()+":"+auth.getClientId().toLocal8Bit() );
        arguments.replace( "${version_name}", version );
        arguments.replace( "${game_directory}", "\""+mainPath+"\"" );
        arguments.replace( "${game_assets}", "\""+mainPath+slash+"assets\"" );

        command.append("java ").append(" -Djava.library.path=\""+mainPath+slash+"natives\"")
               .append( libPathGen() ).append(" "+mainClass+" ").append( arguments );


            return errCode;


    }
    else {



        arguments.replace( "${auth_player_name}", auth.getUsername() );
        arguments.replace( "${auth_session}", "token:"+auth.getAccessToken().toLocal8Bit()+":"+auth.getClientId().toLocal8Bit() );
        arguments.replace( "${version_name}", version );
        arguments.replace( "${game_directory}", "\""+mainPath+"\"" );
        arguments.replace( "${game_assets}", "\""+mainPath+slash+"assets\"" );

        command.append("java ").append(" -Djava.library.path=\""+mainPath+slash+"natives\"")
               .append( libPathGen() ).append(" "+mainClass+" ").append( arguments );


    }
    return 0;
}

QString GameLauncher::libPathGen() {
    QString libPath = "";


    libPath.append(" -cp \"");

    QFile libs( QDir::currentPath()+"/versions/"+version+"/libs.txt" );
    if( libs.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        libs.resize( 0 );
        QTextStream out( &libs );
        libPath.append( out.readAll() );
        libs.close();
    }

    libPath.append( mainPath+slash+"versions"+slash+version+slash+version+".jar\"");

    return libPath;
}

void GameLauncher::setMainPath() {
    mainPath = QDir::currentPath();
#ifdef Q_OS_WIN
    //mainPath = QDir::homePath().replace("/","\\").append("\\Application Data\\.minecraft");
    separator = ";";
    slash = "\\";
#endif
#ifdef Q_OS_LINUX
    //mainPath = QDir::homePath().append("/.minecraft");
    separator = ":";
    slash = "/";
#endif
}

QString GameLauncher::getToken( QString input ) {
    input = input.replace("\"","");
    input = input.replace("{","");
    input = input.replace("}","");
    qDebug() << input;
    QStringList temp = input.split(",");
    qDebug() << temp[0];
    input = temp[0];
    temp = input.split(":");
    qDebug() << temp;
    input = temp[1];
    return input;
}

void GameLauncher::setVersion( QString inVersion ) {
    version = inVersion;
}
