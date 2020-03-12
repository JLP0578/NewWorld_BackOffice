#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString identifiant,QString super,QString nom,QString login,QString password,QWidget *parent = nullptr);
    ~MainWindow();

    QNetworkAccessManager myNWM;
    QNetworkCookieJar cookieJar;

    void LoadWindow();
    void itemSelectionChanged();

    void Charg_Costumers(QString Type);
    void Charg_Relay();
    void Charg_Productor(QString Type);
    void Charg_Product();
    void Charg_Category();

private slots:
    void on_pushButton_clicked();

    void on_tableWidgetCustomer_itemSelectionChanged();

    void on_AddCustomer_clicked();

private:
    Ui::MainWindow *ui;

    QString identifiantAdmin;
    QString superAdmin;
    QString nomAdmin;
    QString loginAdmin;
    QString passwordAdmin;

    QString URL_Customers = "http://j-lepivert.hd.free.fr/json/viewClient.php";
    QString URL_Productors = "http://j-lepivert.hd.free.fr/json/viewProducteur.php";

    QString tableWidgetCustomer;
    QString tableWidgetRelay;
    QString tableWidgetProductor;
    QString tableWidgetProduct;
    QString tableWidgetCategory;

    QString IdCustomer;
    QString NameCustomer;
    QString SurnameCustomer;
    QString AddressCustomer;
    QString CityCustomer;
    QString PostalCustomer;
    QString PhoneCustomer;
    QString EmailCustomer;
    QString LoginCustomer;
    QString PasswordCustomer;
    QString LongCustomer;
    QString LatCustomer;
    QString CatalogCustomer;
    QString EnableCustomer;

    QString IdProductor;
    QString NameProductor;
    QString SurnameProductor;
    QString AddressProductor;
    QString CityProductor;
    QString PostalProductor;
    QString PhoneProductor;
    QString EmailProductor;
    QString LoginProductor;
    QString PasswordProductor;
    QString LongProductor;
    QString LatProductor;
    QString EnableProductor;

};
#endif // MAINWINDOW_H
