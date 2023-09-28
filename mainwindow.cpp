#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QAction>
#include <QSqlRecord>
#include <QSqlField>
#include <QProcess>
#include "queryalldialog.h"
#include "gematria.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Gematria Tracker");
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
}

void MainWindow::searchDB()
{
    // capture whats in the search bar
    QString searchedString = ui->searchLineEdit->text();
    if(searchedString.isEmpty())
        return;

    // check if the string is a integer
    bool ok = false;
    quint64 gematria = 0;
    quint64 number =  searchedString.toInt(&ok);
    if(ok)
    {
        gematria = number;
    }
    else
    {
        searchedString = searchedString.toLower();
        gematria = getGematria(searchedString);
    }

    tabelModel = new QSqlQueryModel(this);
    tabelModel->setQuery("SELECT gematriaString FROM GematriaNumbers WHERE gematriaNumber = " + QString::number(gematria) + ";");

    // query a list of words in the db that have the same gematria.
    ui->listView->setModel(tabelModel);
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


void MainWindow::on_searchLineEdit_returnPressed()
{
    this->searchDB();
}


void MainWindow::on_saveButton_clicked()
{
    // check if model data exists
    if(tabelModel == nullptr)
        return;

    // check if model data is populated
    if(tabelModel->rowCount() < 1)
        return;

    QString fileName = QFileDialog::getSaveFileName(this, "", "Save File");

    if(fileName.isEmpty())
        return;

    // save the file with the current data in the GUI.
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        out << "Searched: " << ui->searchLineEdit->text() << '\n';
        out << "Gematria: " << ui->gematriaNumberLabel->text() << '\n';
        out << '\n';
        out << '\n';

        for(int i = 0; i < tabelModel->rowCount(); i++)
        {
            QSqlRecord record = tabelModel->record(i);
            QSqlField field = record.field(0);
            QVariant value = field.value();
            out << value.toString() << '\n';
        }

        file.close();
    }
}


void MainWindow::on_clearButton_clicked()
{
    if(!tabelModel)
        return;

    // clear the search bar
    ui->searchLineEdit->clear();

    // clear the model data
    this->tabelModel->clear();

    ui->gematriaNumberLabel->clear();
}


void MainWindow::on_lookupButton_clicked()
{
    // grab the current selected item data from the ui objest listView object

    if(ui->listView->currentIndex().row() < 0)
        return;
    QSqlRecord record = tabelModel->record(ui->listView->currentIndex().row());
    QSqlField field = record.field(0);
    QVariant value = field.value();
    QString wordToSearch = value.toString();
    QString commandPrefix = "https://www.merriam-webster.com/dictionary/";
    QString command = "start msedge " + commandPrefix + wordToSearch;
    qDebug() << command;

    // use the system to open edge
    std::system(command.toLatin1());
}

