#include "dialoglogin.h"
#include "ui_dialoglogin.h"

#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QDesktopServices>

DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    //pleine ecran
//    this->setWindowState(this->windowState()|Qt::WindowFullScreen);
    //espace pour les sessions
    myNWM.setCookieJar(&cookieJar);
}

DialogLogin::~DialogLogin()
{
    delete ui;
}

void DialogLogin::on_pushButtonWebSite_clicked()
{
    QDesktopServices::openUrl(QUrl("http://j-lepivert.hd.free.fr/NewWorld/"));
}

void DialogLogin::on_pushButtonQuit_clicked()
{
    close();
}

void DialogLogin::on_pushButtonAccess_clicked()
{
    //verif du user et du pass
    QUrl serviceUrl(URL);
    QUrl donnees;
    QUrlQuery query;
    //ajout de $_POST['login'] et $_POST['password']
    query.addQueryItem("Choix", "Admin_Select_Login");
    query.addQueryItem("adminLogin", ui->lineEditLogin->text());
    query.addQueryItem("adminPass",ui->lineEditPassWord->text());
    donnees.setQuery(query);
    QByteArray postData(donnees.toString(QUrl::RemoveFragment).remove(0,1).toLatin1());
    //création de la requête http qui va interroger le service
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    //exécution de la requête http
    QNetworkReply *reply1 = myNWM.post(request,postData);
    //attente de la réception complète de la réponse
    while(!reply1->isFinished())
    {
        qApp->processEvents();
    }
    //lecture de la réponse
    QByteArray response_data = reply1->readAll();

    //affichage pour débugger
    qDebug()<<"reponse:"<<response_data;

    //formation du json à partir de la réponse
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);


    //qDebug()<<jsonResponse.object();
    //qDebug()<<jsonResponse.object().count();
    //on recoit un tableau de 6 cases: 3 indexées par des entiers et trois indexées par les noms des champs
    if(jsonResponse.object().count()==5)//0.1.2 et nom prenom identifiant
    {
        qDebug()<<"ok - login";
        //recupération des infos (nom, prenom et identifiant sont trois champs renvoyés par la requête sql
        //utilisée par le webService)
        identifiant = jsonResponse.object()["adminId"].toString();
        super = jsonResponse.object()["adminSuper"].toString();
        nom = jsonResponse.object()["adminName"].toString();
        login = jsonResponse.object()["adminLogin"].toString();
        password = jsonResponse.object()["adminPass"].toString();
        //si c'est bon on ferme la fenêtre de connexion avec un code de retour QDialog::accepted        

        accept();
    }
    else
    {
        //identification incorrecte
        ui->labelError->setStyleSheet("color:red;");
        ui->labelError->setText(tr("This login or/and this password are incorrect"));
    }
    //nettoyage de reply1
    reply1->deleteLater();
}
