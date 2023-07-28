#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlDatabase db;


private slots:
    void actionNew_Gematria_String_triggered();

    void searchButton_clicked();

    void actionQuery_All_triggered();

    void actionInsert_Gematria_List_triggered();

    void on_searchLineEdit_returnPressed();

    void on_saveButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlQueryModel* tabelModel = nullptr;

    void setUpDB();

    void createGematriaTabel();

    void prepUI();

    void searchDB();

    bool isInDB(QString str);

    bool insertIntoDB(QString str);
};
#endif // MAINWINDOW_H
