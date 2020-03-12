#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDialog>

namespace Ui {
class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogin(QWidget *parent = nullptr);
    ~DialogLogin();

    QNetworkAccessManager myNWM;
    QNetworkCookieJar cookieJar;

    void serviceRequestFinish(QNetworkReply *rep);

    QString password;
    QString login;
    QString nom;
    QString super;
    QString identifiant;

private slots:
    void on_pushButtonWebSite_clicked();

    void on_pushButtonQuit_clicked();

    void on_pushButtonAccess_clicked();

private:
    Ui::DialogLogin *ui;
    QString URL = "http://j-lepivert.hd.free.fr/json/viewAdmin.php" ;
};

#endif // DIALOGLOGIN_H
