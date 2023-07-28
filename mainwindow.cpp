#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include "gematria.h"
#include <QAction>
#include "queryalldialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    prepUI();
    setUpDB();
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
            qDebug() << "No tables creating GematriaNumbers table";
            createGematriaTabel();
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", db.lastError().text());
    }
}

void MainWindow::prepUI()
{
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchButton_clicked);
    connect(ui->actionNew_Gematria_String, &QAction::triggered, this, &MainWindow::actionNew_Gematria_String_triggered);
    connect(ui->actionQuery_All, &QAction::triggered, this, &MainWindow::actionQuery_All_triggered);
    connect(ui->actionInsert_Gematria_List, &QAction::triggered, this, &MainWindow::actionInsert_Gematria_List_triggered);
    connect(ui->actionSearch_By_Number, &QAction::triggered, this, &MainWindow::actionSearch_By_Number_triggered);
}

void MainWindow::searchDB()
{
    // capture whats in the search bar
    QString searchedString = ui->searchLineEdit->text();
    if(searchedString.isEmpty())
        return;
    searchedString = searchedString.toLower();

    // get the gematria number
    quint64 gematria = getGematria(searchedString);

    QSqlQueryModel* query = new QSqlQueryModel(this);
    query->setQuery("SELECT gematriaString FROM GematriaNumbers WHERE gematriaNumber = " + QString::number(gematria) + ";");

    // query a list of words in the db that have the same gematria.
    ui->listView->setModel(query);
    ui->gematriaNumberLabel->setText(QString::number(gematria));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createGematriaTabel()
{
    QSqlQueryModel* query = new QSqlQueryModel(this);
    query->setQuery("CREATE TABLE GematriaNumbers (gematriaString TEXT, gematriaNumber INTEGER, PRIMARY KEY(gematriaString));");
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
    return false;
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
        query->setQuery("INSERT INTO GematriaNumbers (GematriaString, GematriaNumber) VALUES ('"+str+"', "+QString::number(gematria)+");");
        return true;
    }
}

// slots
void MainWindow::actionNew_Gematria_String_triggered()
{
    // prompt the user to enter a Gematria String
    QString gematriaString = QInputDialog::getText(this, "", "Enter Gematria: ");
    // check if nothing was entered
    if(gematriaString.isEmpty())
        return;
    // lower case the data
    gematriaString = gematriaString.toLower();

    // insert the data into the database.
    if(!insertIntoDB(gematriaString))
    {
        QMessageBox::warning(this, "Warning", "'" + gematriaString + "' is already in the database");
    }
}

void MainWindow::searchButton_clicked()
{
    this->searchDB();
}

void MainWindow::actionQuery_All_triggered()
{
    QueryAllDialog* dialog = new QueryAllDialog(this);
    dialog->exec();
}

void MainWindow::actionInsert_Gematria_List_triggered()
{
    // get the file the user wants to insert.
    QString fileName = QFileDialog::getOpenFileName(this, "Choose File", "/home");

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString line = "";
        QStringList failed;
        while(!in.atEnd())
        {
            line = in.readLine();

            // get the gematria
            //quint64 gematria = getGematria(line);
            if(!insertIntoDB(line))
            {
                failed.append(line);
            }
        }
        file.close();
        if(!failed.isEmpty())
        {
            for(int i = 0; i < failed.length(); i++)
            {
                qDebug() << failed[i] << "Failed already in data base";
            }
        }
    }
}

void MainWindow::actionSearch_By_Number_triggered()
{
    qDebug() << "Hello";
    quint64 gematria = QInputDialog::getInt(this, "", "Enter Gematria: ", 0, 1);
    if(gematria > 0)
    {
        QSqlQueryModel* query = new QSqlQueryModel(this);
        query->setQuery("SELECT gematriaString FROM GematriaNumbers WHERE gematriaNumber = " + QString::number(gematria) + ";");
        ui->listView->setModel(query);
        ui->gematriaNumberLabel->setText(QString::number(gematria));
    }
}


void MainWindow::on_searchLineEdit_returnPressed()
{
    this->searchDB();
}

