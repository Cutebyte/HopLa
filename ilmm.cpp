/*
 *  Custom Minecraft Launcher - ILMM - ilmm.cpp
 *  by Hopskocz
 */

#include "ilmm.h"
#include "ui_ilmm.h"
#include "options.h"
#include <QDebug>
#include <QPainter>


ILMM::ILMM(QWidget *parent, QString version ) :
    QMainWindow(parent),
    ui(new Ui::ILMM)
{
    ui->setupUi( this );
    ui->premPassword->setEchoMode( QLineEdit::Password );
    this->setWindowTitle( "HopLa Minecraft - "+version );
    ui->offlineBox->setVisible( false );
    ui->onlinebtn->setEnabled( false );
    ui->downloadBox->setVisible( false );

    downloader = new Downloader();
    versions = new Versions();
    QByteArray tempJson = versions->loadDB();
    ui->comboBox->addItems( versions->parseJson( &tempJson ));

    connect( downloader, SIGNAL( percentage( int ) ), ui->progressBar, SLOT( setValue( int ) ) );
    connect( downloader, SIGNAL( filePercentage( int ) ), ui->progressBar_2, SLOT( setValue( int ) ) );
    connect( downloader, SIGNAL( hide( bool ) ), ui->downloadBox, SLOT(setVisible( bool ) ) );
    connect( downloader, SIGNAL( title( QString ) ), ui->fileName, SLOT( setText( QString ) ) );

    //loadSettings();
}
/*
void ILMM::paintEvent( QPaintEvent *event ) {
    QPainter painter( this );
    painter.drawImage( QRect( 0, 0, 640, 320 ), QImage( ":/res/images/background.png" ));
}
*/

void ILMM::showOptions() {
    Options opt( this );
    opt.exec();
}

ILMM::~ILMM()
{
    delete ui;
}

void ILMM::switchToOnline() {
    ui->offlinebtn->setEnabled( true );
    ui->onlinebtn->setEnabled( false );
    ui->offlineBox->setVisible( false );
    ui->onlineBox->setVisible( true );
}

void ILMM::switchToOffline() {
    ui->onlinebtn->setEnabled( true );
    ui->offlinebtn->setEnabled( false );
    ui->onlineBox->setVisible( false );
    ui->offlineBox->setVisible( true );
}

void ILMM::downloadVersions() {
    versions->downloadDB();
    ui->comboBox->clear();
    QByteArray json = versions->loadDB();
    ui->comboBox->addItems( versions->parseJson( &json ) );
}

void ILMM::loadVersions() {

}

void ILMM::downloadResources() {
    downloader->downloadAssetsList();
    downloader->downloadAssets();
    downloader->downloadResourcesList();
    downloader->downloadResources();
}

void ILMM::downloadFiles() {

    downloader->downloadVersion( ui->comboBox->currentText() );

    //downloader.testDownload();
}

void ILMM::launchGame() {
    qDebug() << QUuid::createUuid().toString().replace( "{", "" ).replace( "}", "" );

}

void ILMM::legitMode() {
    downloader->downloadVersion( ui->comboBox->currentText() );
    GameLauncher launch;
    ui->premBtn->setText("Czekaj...");
    ui->premBtn->setEnabled(false);
    launch.setPremium();
    launch.setUsername( ui->premUsername->text() );
    launch.setPassword( ui->premPassword->text() );
    launch.setMainPath();
    launch.setVersion( ui->comboBox->currentText() );
    int errCode = launch.commandGen();
    QString command = launch.getCommand();
    //qDebug() << command;
    switch( errCode ) {
        case 1: QMessageBox::about( this, "Błąd", "Podałeś złe dane!" ); break;
        case 2: QMessageBox::about( this, "Błąd", "Konto uległo migracji. Twój login to adres Twojego adresu e-mail, np: xxxxxx@gmail.com"); break;
        case 0: {
                QProcess process;
                process.execute(command);
                process.waitForFinished();
            } break;
        default: QMessageBox::about( this, "Błąd", "Nie wiadomo co się stało, ale teraz sobie nie pograsz :)"); break;
    }
    ui->premBtn->setEnabled(true);
    ui->premBtn->setText("Odpalaj");



}

void ILMM::nonlegitMode() {

    GameLauncher launch;
    ui->nonpremBtn->setText("Czekaj...");
    ui->nonpremBtn->setEnabled(false);
    launch.setVersion( ui->comboBox->currentText() );
    launch.setNonPremium();
    launch.setUsername( ui->nonPremUsername->text() );
    launch.setMainPath();
    downloader->downloadVersion( ui->comboBox->currentText() );
    launch.commandGen();
    QString command = launch.getCommand();
    qDebug() << command;
    QProcess process;
    process.execute(command);
    process.waitForFinished();

    ui->nonpremBtn->setEnabled(true);
    ui->nonpremBtn->setText("Odpalaj");


}
