/*
 *  Custom Minecraft Launcher - ILMM - versions.h
 *  by Hopskocz
 */

#ifndef VERSIONS_H
#define VERSIONS_H

#include <QNetworkAccessManager>
#include <QStringList>
#include <QObject>

class Versions : public QObject
{
    Q_OBJECT
signals:
    void percentage( int );
    void filePercentage( int );
    void title( QString );
    void hide( bool );
public:
    explicit Versions( QObject *parent = 0 );
    virtual ~Versions();
    void downloadDB();
    QByteArray loadDB();
    QStringList parseJson( QByteArray* );
private slots:
    void filePerc( int );
};

#endif // VERSIONS_H
