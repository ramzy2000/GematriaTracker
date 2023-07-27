#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <string>
#include <QPushButton>
#include <QMessageBox>

#include "gematria.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    prepUI();
    setUpDB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Gematria_String_triggered()
{
    // prompt the user to enter a Gematria String
    QString gematriaString = QInputDialog::getText(this, "", "Enter Gematria: ");

    // check if nothing was entered
    if(gematriaString.isEmpty())
        return;

    // lower case the data
    gematriaString = gematriaString.toLower();

    if(!insertIntoDB(gematriaString))
    {
        QMessageBox::warning(this, "Warning", "'" + gematriaString + "' is already in the database");
    }
}

void MainWindow::searchButton_clicked()
{
    // capture whats in the search bar
    QString searchedString = ui->searchLineEdit->text();
    if(searchedString.isEmpty())
        return;
    searchedString = searchedString.toLower();

    // get the gematria number
    quint64 gematria = getGematria(searchedString);

    QSqlQueryModel* query = new QSqlQueryModel(this);
    query->setQuery("SELECT gematriaString FROM GematriaNumbers WHERE gematriaNumber = " + QString::number(gematria) + " AND gematriaString != '" + searchedString + "';");

    // query a list of words in the db that have the same gematria.
    ui->listView->setModel(query);
}

void MainWindow::setUpDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Gematria.db");

    if(db.open())
    {
        // check if any tables exists
        if(db.tables().isEmpty())
        {
            // create one
            qDebug() << "No tables";
            createGematriaTabel();
        }
        // if no tables create new table
    }
    else
    {
        //qDebug() << "Failed: " << db.lastError().text();
    }
}

void MainWindow::createGematriaTabel()
{
    QSqlQueryModel* query = new QSqlQueryModel(this);
    query->setQuery("CREATE TABLE GematriaNumbers (gematriaString TEXT, gematriaNumber INTEGER, PRIMARY KEY(gematriaString));");
}

void MainWindow::prepUI()
{
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchButton_clicked);
}

bool MainWindow::isInDB(QString str)
{
    QSqlQueryModel* query = new QSqlQueryModel(this);
    query->setQuery("SELECT count(gematriaString) FROM GematriaNumbers WHERE gematriaString = '" + str + "';");
    QVariant item = query->data(query->index(0, 0));
    if(item.toInt() > 0)
    {
        return true;
    }
    return true;
}

bool MainWindow::insertIntoDB(QString str)
{
    str = str.toLower();
    // Once captured check if the Gematrai String is alreaady in the db.
    if(isInDB(str))
    {
        return false;
    }
    else
    {
        quint64 gematria =  getGematria(str);

        // store the data in the database
        QSqlQueryModel* query = new QSqlQueryModel(this);
        query->setQuery("INSERT INTO GematriaNumbers (GematriaString, GematriaNumber) VALUES ('"+str+"', "+QString::number(gematria)+")");
        qDebug() << query->lastError().text();
        return true;
    }
}

