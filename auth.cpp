/*
 *  Custom Minecraft Launcher - ILMM - auth.cpp
 *  by Hopskocz
 */

#include "auth.h"

Authentication::Authentication() {
    accessToken = "1";
    clientId = "1";
}

Authentication::~Authentication() {

}

int Authentication::setAccountInfo( QNetworkAccessManager *mgr ) {
    int errCode = 0;
    QEventLoop eventLoop;

    QObject::connect( mgr, SIGNAL( finished( QNetworkReply* )), &eventLoop, SLOT( quit() ));
    QNetworkRequest request( QUrl( QString( "https://login.minecraft.net/" ))); // /?user="+username+"&password="+password+"&version=15
    //request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    QUrlQuery params;
    params.addQueryItem("username", username );
    params.addQueryItem("password", password );
    params.addQueryItem("version", "15" );
    // etc
    QByteArray data;
    data = params.query(QUrl::FullyEncoded).toUtf8();
    qDebug() << data;

    QNetworkReply *reply = mgr->post( request, data );

    eventLoop.exec();

    QString temp = reply->readAll().constData();
    qDebug() << temp;
    QStringList tempList = temp.split(":");
    qDebug() << tempList.length();
    if( tempList.length() >= 4 ) {
        clientId = tempList[4];
        gameUsername = tempList[2];
    }
    else
    {
        errCode = 1;
    }


    reply->deleteLater();
    return errCode;
}

int Authentication::login( QNetworkAccessManager *mgr ) {
    //if file with clientid and access token dont exist then
    setUuid();

    QEventLoop eventLoop;
    QObject::connect( mgr, SIGNAL( finished( QNetworkReply* )), &eventLoop, SLOT( quit() ));
    QString request = "{\"clientToken\":\""+getUuid()+"\",\"username\":\""+getUsername()+"\", \"password\": \""+getPassword()+"\",\"agent\":{\"name\":\"Minecraft\",\"version\":1}}";
    QByteArray json = request.toUtf8();
    QNetworkRequest req( QUrl( QString( "https://authserver.mojang.com/authenticate" )));
    req.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
    QNetworkReply *reply = mgr->post( req, json );

    eventLoop.exec();

    //qDebug() << reply->readAll().constData();

    QByteArray replyJson = reply->readAll();

    reply->deleteLater();

    qDebug() << replyJson;

    QJsonDocument jsonDoc = QJsonDocument::fromJson( replyJson );
    QJsonObject inside = jsonDoc.object();
    QJsonValue value = inside.value( QString( "clientToken" ));
    if( value.toString() == uuid ) {
        setAccessToken( inside.value( QString( "accessToken" )).toString() );
        QJsonObject item = inside.value( QString( "selectedProfile" )).toObject();
        setClientId( item.value( QString( "id" )).toString() );
        setGameUsername( item.value( QString( "name" )).toString() );
    }
    else
    if( inside.value("errorMessage").toString() == "Invalid credentials. Invalid username or password." ) {
        return 1;
    }
    else
    if( inside.value("errorMessage").toString() == "Invalid credentials. Account migrated, use e-mail as username." ) {
        return 2;
    }
    else

    qDebug() << "Trololo: "+inside.value("errorMessage").toString();






    return 0;
}

// + setting
void Authentication::setUsername( QString inUsername ) {
    username = inUsername;
}

void Authentication::setPassword( QString inPassword ) {
    password = inPassword;
}

void Authentication::setUuid() {
    uuid = QUuid::createUuid().toString().replace("{","").replace("}","");
}

void Authentication::setAccessToken( QString inAccessToken ) {
    accessToken = inAccessToken;
}

void Authentication::setClientId( QString inClientId ) {
    clientId = inClientId;
}

void Authentication::setGameUsername( QString inGameUsername ) {
    gameUsername = inGameUsername;
}
// - setting

// + getting
QString Authentication::getUsername() {
    return username;
}
QString Authentication::getPassword() {
    return password;
}
QString Authentication::getUuid() {
    return uuid;
}
QString Authentication::getClientId() {
    return clientId;
}
QString Authentication::getAccessToken() {
    return accessToken;
}
QString Authentication::getGameUsername() {
    return gameUsername;
}

// - getting
