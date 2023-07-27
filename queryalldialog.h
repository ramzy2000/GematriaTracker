#ifndef QUERYALLDIALOG_H
#define QUERYALLDIALOG_H

#include <QDialog>

namespace Ui {
class QueryAllDialog;
}

class QueryAllDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryAllDialog(QWidget *parent = nullptr);
    ~QueryAllDialog();

private:
    Ui::QueryAllDialog *ui;
};

#endif // QUERYALLDIALOG_H
