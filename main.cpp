#include "mainwindow.h"
#include "dialoglogin.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DialogLogin dialogLogin;
    //add fullScreen
    //dialogLogin.setWindowState(dialogLogin.windowState()|Qt::WindowFullScreen);

    if(dialogLogin.exec()==QDialog::Accepted)
    {
        MainWindow maMain(dialogLogin.identifiant,dialogLogin.super,dialogLogin.nom,dialogLogin.login,dialogLogin.password);
        maMain.show();
        return a.exec();
    }
    else return(-1);
}
