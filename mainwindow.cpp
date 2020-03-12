#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::MainWindow(QString identifiant,QString super,QString nom,QString login,QString password, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowState(this->windowState()|Qt::WindowFullScreen);

    identifiantAdmin = identifiant;
    superAdmin = super;
    nomAdmin = nom;
    loginAdmin = login;
    passwordAdmin = password;

    LoadWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadWindow()
{
    ui->labelNom->setText(nomAdmin);
    Charg_Costumers("all");
    Charg_Relay();
    Charg_Productor("all");
    Charg_Product();
    Charg_Category();
}

void MainWindow::itemSelectionChanged()
{
        QString TableGererProduit = ui->tableWidgetCustomer->model()->data(ui->tableWidgetCustomer->model()->index(ui->tableWidgetCustomer->currentIndex().row(),0)).toString();
        this->tableWidgetCustomer=TableGererProduit;
        Charg_Costumers(this->tableWidgetCustomer);

        QString TableGererCategorie = ui->tableWidgetRelay->model()->data(ui->tableWidgetRelay->model()->index(ui->tableWidgetRelay->currentIndex().row(),0)).toString();
        this->tableWidgetRelay=TableGererCategorie;

        QString TableValiderProduit = ui->tableWidgetProductor->model()->data(ui->tableWidgetProductor->model()->index(ui->tableWidgetProductor->currentIndex().row(),0)).toString();
        this->tableWidgetProductor=TableValiderProduit;

        QString TableValiderUser = ui->tableWidgetProduct->model()->data(ui->tableWidgetProduct->model()->index(ui->tableWidgetProduct->currentIndex().row(),0)).toString();
        this->tableWidgetProduct=TableValiderUser;

        QString TableValiderUser_2 = ui->tableWidgetCategory->model()->data(ui->tableWidgetCategory->model()->index(ui->tableWidgetCategory->currentIndex().row(),0)).toString();
        this->tableWidgetCategory=TableValiderUser_2;
}

void MainWindow::Charg_Costumers(QString Type)
{
    QUrl serviceUrl(URL_Customers);
    QUrl donnees;
    QUrlQuery query;
    query.addQueryItem("v_Client", Type);
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
//    qDebug()<<"reponse:"<<response_data;
//    ui->label_2->setText(response_data);
    //formation du json à partir de la réponse
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    //transfo en JSonArray
//    qDebug()<<"reponse:"<<response_data;

    //obtention du nb de commandes à préparer
    int nbLDC=jsonResponse.array().count();

    if(Type == "all"){
        //qDebug()<<"nombre de ligne de commandes reçues:"<<nbLDC;
        //dimensionnement du tableau dans l'interface graphique

        ui->tableWidgetCustomer->setRowCount(nbLDC);

        ui->tableWidgetCustomer->setColumnCount(14);
        QStringList tableHeader;
        tableHeader<< tr("Id") << tr("Name") << tr("Surname") << tr("Address") << tr("City") << tr("Postal") << tr("Phone") << tr("Email") << tr("Login") << tr("Password") << tr("Long") << tr("Lat") << tr("Catalog") << tr("Enabled");
        ui->tableWidgetCustomer->setHorizontalHeaderLabels(tableHeader);

        for(int noLdc=0; noLdc<nbLDC; noLdc++)
        {
            ui->tableWidgetCustomer->setItem(noLdc,0,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientId"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,1,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientNom"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,2,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientPrenom"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,3,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientRue"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,4,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientVille"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,5,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientCP"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,6,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientTel"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,7,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientMail"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,8,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientLogin"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,9,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientMdp"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,10,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientLongitude"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,11,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientLatitude"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,12,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientCatalogue"].toString()));
            ui->tableWidgetCustomer->setItem(noLdc,13,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["clientActif"].toString()));
        }
        ui->tableWidgetCustomer->resizeColumnsToContents();
        qDebug()<<"ok - LoadCustomers";

    }
    else{
        for(int noLdc=0; noLdc<nbLDC; noLdc++)
        {

            this->IdCustomer = jsonResponse.array()[nbLDC].toObject()["clientId"].toString();
            this->NameCustomer = jsonResponse.array()[nbLDC].toObject()["clientNom"].toString();
            this->SurnameCustomer = jsonResponse.array()[nbLDC].toObject()["clientPrenom"].toString();
            this->AddressCustomer = jsonResponse.array()[nbLDC].toObject()["clientRue"].toString();
            this->CityCustomer = jsonResponse.array()[nbLDC].toObject()["clientVille"].toString();
            this->PostalCustomer = jsonResponse.array()[nbLDC].toObject()["clientCP"].toString();
            this->PhoneCustomer = jsonResponse.array()[nbLDC].toObject()["clientTel"].toString();
            this->EmailCustomer = jsonResponse.array()[nbLDC].toObject()["clientMail"].toString();
            this->LoginCustomer = jsonResponse.array()[nbLDC].toObject()["clientLogin"].toString();
            this->PasswordCustomer = jsonResponse.array()[nbLDC].toObject()["clientMdp"].toString();
            this->LongCustomer = jsonResponse.array()[nbLDC].toObject()["clientLongitude"].toString();
            this->LatCustomer = jsonResponse.array()[nbLDC].toObject()["clientLatitude"].toString();
            this->CatalogCustomer = jsonResponse.array()[nbLDC].toObject()["clientCatalogue"].toString();
            this->EnableCustomer = jsonResponse.array()[nbLDC].toObject()["clientActif"].toString();
         }

            ui->lineEditNameCostumer->setText(this->NameCustomer);
            ui->lineEditSurnameCustomer->setText(this->SurnameCustomer);
            ui->lineEditAddressCustomer->setText(this->AddressCustomer);
            ui->lineEditCityCustomer->setText(this->CityCustomer);
            ui->lineEditPostalCustomer->setText(this->PostalCustomer);
            ui->lineEditPhoneCustomer->setText(this->PhoneCustomer);
            ui->lineEditMailCustomer->setText(this->EmailCustomer);
            ui->lineEditLoginCustomer->setText(this->LoginCustomer);
            ui->lineEditPasswordCustomer->setText(this->PasswordCustomer);
            ui->lineEditLonCustomer->setText(this->LongCustomer);
            ui->lineEditLatCustomer->setText(this->LatCustomer);

            bool cat;
            bool ena;
            if(CatalogCustomer == "1") cat = true;
            if(CatalogCustomer == "0") cat = false;
            if(EnableCustomer == "1") ena = true;
            if(EnableCustomer == "0") ena = false;
            ui->checkBoxCatalogCustomer->setChecked(cat);
            ui->checkBoxEnabledCustomer->setChecked(ena);
    }
}

void MainWindow::Charg_Relay()
{

}

void MainWindow::Charg_Productor(QString Type)
{
    //
    //
    //  MODIF JSON POUR AJOUTER TABLE EMTREPRISE POUR PLUS D'INFO
    //
    //
    QUrl serviceUrl(URL_Productors);
    QUrl donnees;
    QUrlQuery query;
    query.addQueryItem("v_Producteur", Type);
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
//    qDebug()<<"reponse:"<<response_data;
//    ui->label_2->setText(response_data);
    //formation du json à partir de la réponse
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    //transfo en JSonArray
//    qDebug()<<"reponse:"<<response_data;

    //obtention du nb de commandes à préparer

    int nbLDC=jsonResponse.array().count();
    //qDebug()<<"nombre de ligne de commandes reçues:"<<nbLDC;
    //dimensionnement du tableau dans l'interface graphique

    ui->tableWidgetProductor->setRowCount(nbLDC);

    ui->tableWidgetProductor->setColumnCount(13);
    QStringList tableHeader;
    tableHeader<< tr("Id") << tr("Name") << tr("Surname") << tr("Address") << tr("City") << tr("Postal") << tr("Phone") << tr("Email") << tr("Login") << tr("Password") << tr("Long") << tr("Lat") << tr("Enabled");
    ui->tableWidgetProductor->setHorizontalHeaderLabels(tableHeader);

    for(int noLdc=0; noLdc<nbLDC; noLdc++)
    {
        this->IdProductor = jsonResponse.array()[noLdc].toObject()["prodId"].toString();
        this->NameProductor = jsonResponse.array()[noLdc].toObject()["prodNom"].toString();
        this->SurnameProductor = jsonResponse.array()[noLdc].toObject()["prodPrenom"].toString();
        this->AddressProductor = jsonResponse.array()[noLdc].toObject()["prodRue"].toString();
        this->CityProductor = jsonResponse.array()[noLdc].toObject()["prodVille"].toString();
        this->PostalProductor = jsonResponse.array()[noLdc].toObject()["prodCP"].toString();
        this->PhoneProductor = jsonResponse.array()[noLdc].toObject()["prodTel"].toString();
        this->EmailProductor = jsonResponse.array()[noLdc].toObject()["prodMail"].toString();
        this->LoginProductor = jsonResponse.array()[noLdc].toObject()["prodLogin"].toString();
        this->PasswordProductor = jsonResponse.array()[noLdc].toObject()["prodMdp"].toString();
        this->LongProductor = jsonResponse.array()[noLdc].toObject()["prodLongitude"].toString();
        this->LatProductor = jsonResponse.array()[noLdc].toObject()["prodLatitude"].toString();
        this->EnableProductor = jsonResponse.array()[noLdc].toObject()["prodActif"].toString();

        ui->tableWidgetProductor->setItem(noLdc,0,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodId"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,1,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodNom"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,2,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodPrenom"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,3,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodRue"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,4,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodVille"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,5,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodCP"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,6,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodTel"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,7,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodMail"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,8,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodtLogin"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,9,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodMdp"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,10,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodLongitude"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,11,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodLatitude"].toString()));
        ui->tableWidgetProductor->setItem(noLdc,12,new QTableWidgetItem(jsonResponse.array()[noLdc].toObject()["prodActif"].toString()));
    }
    ui->tableWidgetProductor->resizeColumnsToContents();
    qDebug()<<"ok - LoadProductor";
}

void MainWindow::Charg_Product()
{

}

void MainWindow::Charg_Category()
{

}



void MainWindow::on_pushButton_clicked()
{
    close();
}


void MainWindow::on_tableWidgetCustomer_itemSelectionChanged()
{
    QString TableGererProduit = ui->tableWidgetCustomer->model()->data(ui->tableWidgetCustomer->model()->index(ui->tableWidgetCustomer->currentIndex().row(),0)).toString();
    this->tableWidgetCustomer=TableGererProduit;
    Charg_Costumers(this->tableWidgetCustomer);
}

void MainWindow::on_AddCustomer_clicked()
{
    this->NameCustomer = ui->lineEditNameCostumer->text();
    this->SurnameCustomer = ui->lineEditSurnameCustomer->text();
    this->AddressCustomer = ui->lineEditAddressCustomer->text();
    this->CityCustomer = ui->lineEditCityCustomer->text();
    this->PostalCustomer = ui->lineEditPostalCustomer->text();
    this->PhoneCustomer = ui->lineEditPhoneCustomer->text();
    this->EmailCustomer = ui->lineEditMailCustomer->text();
    this->LoginCustomer = ui->lineEditLoginCustomer->text();
    this->PasswordCustomer = ui->lineEditPasswordCustomer->text();
    this->LongCustomer = ui->lineEditLonCustomer->text();
    this->LatCustomer = ui->lineEditLatCustomer->text();

    QString cat;
    QString ena;
    if(ui->checkBoxCatalogCustomer->isChecked() == true) cat = "1";
    if(ui->checkBoxCatalogCustomer->isChecked() == false) cat = "0";
    if(ui->checkBoxCatalogCustomer->isChecked() == true) ena = "1";
    if(ui->checkBoxCatalogCustomer->isChecked() == false) ena = "0";

    this->CatalogCustomer = cat;
    this->EnableCustomer = ena;

    /*
    QUrl serviceUrl(URL_Customers);
    QUrl donnees;
    QUrlQuery query;
    query.addQueryItem("a_Client", "unique");

    query.addQueryItem("clientNom", NameCustomer);
    query.addQueryItem("clientPrenom", SurnameCustomer);
    query.addQueryItem("clientRue", AddressCustomer);
    query.addQueryItem("clientVille", CityCustomer);
    query.addQueryItem("clientCP", PostalCustomer);
    query.addQueryItem("clientTel", PhoneCustomer);
    query.addQueryItem("clientMail", EmailCustomer);
    query.addQueryItem("clientLogin", LoginCustomer);
    query.addQueryItem("clientMdp", PasswordCustomer);
    query.addQueryItem("clientLongitude", LongCustomer);
    query.addQueryItem("clientLatitude", LatCustomer);
    query.addQueryItem("clientCatalogue", CatalogCustomer);
    query.addQueryItem("clientActif", EnableCustomer);

    donnees.setQuery(query);
    QByteArray postData(donnees.toString(QUrl::RemoveFragment).remove(0,1).toLatin1());
    //création de la requête http qui va interroger le service
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    //exécution de la requête http
    QNetworkReply *reply1 = myNWM.post(request,postData);
    while(!reply1->isFinished())
    {
        qApp->processEvents();
    }
    */

}
