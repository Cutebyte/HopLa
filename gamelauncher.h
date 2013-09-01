/*
 *  Custom Minecraft Launcher - ILMM - gamelauncher.h
 *  by Hopskocz
 */

#ifndef GAMELAUNCHER_H
#define GAMELAUNCHER_H

#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include "auth.h"

class GameLauncher
{
    bool legit; // a variable to checking if client is legit, or not
    QString username;
    QString password;
    QString mainPath;
    QString separator;
    QString slash;
    QString version;
    QString token;
    QString command;
public:
    GameLauncher();
    void setPremium();
    void setNonPremium();
    void setPassword( QString inPassword );
    void setUsername( QString inUsername );
    void setVersion( QString );
    bool isLegit();
    int commandGen(  );
    QString libPathGen();
    void setMainPath();
    QString getToken( QString input );
    QString getCommand();


};

#endif // GAMELAUNCHER_H
