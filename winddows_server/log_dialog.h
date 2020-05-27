#ifndef LOG_DIALOG_H
#define LOG_DIALOG_H

#include <QDialog>
#include<QLineEdit>
#include<QLabel>
#include<QPushButton>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlDriver>
#include<QtSql/QSqlRecord>
#include<QtSql/QSqlError>
#include<QMessageBox>

/*数据库的结构内容*/
typedef struct logdata{
    QString id;
    QString md5;
}logdata;

namespace Ui {
class log_Dialog;
}

class log_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit log_Dialog(QWidget *parent = nullptr);
    ~log_Dialog();
    bool logfun(void);
    void closefun(void);

public:
    bool md5_isok;
private:
    QLineEdit log_let;
    QLineEdit md5_let;
    QLabel    log_lbl;
    QLabel    md5_lbl;
    QPushButton ok_button;
    QPushButton cancle_button;
    QSqlDatabase db;


private:
    Ui::log_Dialog *ui;
};

#endif // LOG_DIALOG_H
