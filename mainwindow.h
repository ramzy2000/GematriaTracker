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



private slots:
    void on_actionNew_Gematria_String_triggered();

    void searchButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQueryModel* tabelModel = nullptr;

    void setUpDB();

    void createGematriaTabel();

    void prepUI();

    bool isInDB(QString str);

    bool insertIntoDB(QString str);
};
#endif // MAINWINDOW_H
