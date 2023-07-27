#include "queryalldialog.h"
#include "ui_queryalldialog.h"
#include "mainwindow.h"
#include <QSqlQueryModel>

QueryAllDialog::QueryAllDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryAllDialog)
{
    ui->setupUi(this);

    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent);

    if(mainWindow->db.open())
    {
        QSqlQueryModel* tabelModel = new QSqlQueryModel(this);
        tabelModel->setQuery("SELECT * FROM GematriaNumbers;");
        ui->tableView->setModel(tabelModel);
    }
}

QueryAllDialog::~QueryAllDialog()
{
    delete ui;
}
