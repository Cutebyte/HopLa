/*
 *  Custom Minecraft Launcher - ILMM - auth.h
 *  by Hopskocz
 */

#ifndef AUTH_H
#define AUTH_H

#include <QString>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QUrl>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QUuid>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class Authentication {
    QString username;
    QString password;
    QString clientId;
    QString uuid;
    QString gameUsername;
    QString accessToken;
public:
    Authentication();
    ~Authentication();
    int setAccountInfo( QNetworkAccessManager* );
    int login( QNetworkAccessManager* );
    void loginWithToken();
    void setUuid();
    void setUsername( QString );
    void setPassword( QString );
    void setAccessToken( QString );
    void setClientId( QString );
    void setGameUsername( QString );
    QString getUsername();
    QString getGameUsername();
    QString getPassword();
    QString getClientId();
    QString getUuid();
    QString getAccessToken();
};

#endif // AUTH_H
