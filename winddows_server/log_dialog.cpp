#include "log_dialog.h"
#include "ui_log_dialog.h"
#include<QDebug>


log_Dialog::log_Dialog(QWidget *parent) :
    QDialog(parent),md5_isok(false),
    ui(new Ui::log_Dialog)
{
    ui->setupUi(this);
    QFont ft2;
    ft2.setPointSize(11);

    this->setWindowTitle("登陆窗口");
    this->setFixedSize(300,170);
    this->log_lbl.setGeometry(30,20,100,40);
    this->log_lbl.setParent(this);
    this->log_lbl.setText("用户名:");
    this->log_lbl.setFont(ft2);

    this->md5_lbl.setGeometry(30,60,100,40);
    this->md5_lbl.setParent(this);
    this->md5_lbl.setText("密码:");
    this->md5_lbl.setFont(ft2);


    this->log_let.setGeometry(110,24,150,30);
    this->log_let.setParent(this);
    this->log_let.setFont(ft2);

    this->md5_let.setGeometry(110,64,150,30);
    this->md5_let.setParent(this);
    this->md5_let.setEchoMode(QLineEdit::Password);
    this->md5_let.setFont(ft2);

    this->ok_button.setGeometry(30,120,80,30);
    this->ok_button.setText("登录");
    this->ok_button.setParent(this);

    this->cancle_button.setGeometry(180,120,80,30);
    this->cancle_button.setText("退出");
    this->cancle_button.setParent(this);

    connect(&this->cancle_button,&QPushButton::clicked,this,&log_Dialog::closefun);
    connect(&this->ok_button,&QPushButton::clicked,this,&log_Dialog::logfun);
}

bool log_Dialog::logfun()
{
    bool status;
    /*数据库的初始化*/
    if (QSqlDatabase::contains("qt_sql_default_connection"))
        this->db= QSqlDatabase::database("qt_sql_default_connection");
    else
    {
        this->db =QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("dongkangjia");
        db.setDatabaseName("log.db");
        db.setUserName("dongkangjia");
        db.setPassword("123456");
    }
    status=db.open();

    QString logname=this->log_let.text();
    QString md5name=this->md5_let.text();
    QSqlQuery query(this->db);
    QVector<logdata> log_info;
    query.prepare("SELECT * FROM logid");
    query.exec();	//执行
    while(query.next())
    {
        logdata l;
        l.id=query.value("id").toString();
        l.md5=query.value("md5").toString();
        log_info.push_back(l);
    }
    int s=0;
    for(int i=0;i<log_info.size();i++)
    {
        bool is_eqlity1;
        is_eqlity1=logname.compare(log_info[i].id);
        if(is_eqlity1==false)
        {
            bool is_eqlity2;
            is_eqlity2=md5name.compare(log_info[i].md5);
            if(is_eqlity2==false)
            {
                this->md5_isok=true;
                this->close();
                qDebug()<<"md5";
            }
            else
            {
                QMessageBox::warning(this,"错误","密码错误");
                return false;
            }
        }
        else
        {
            s++;
            qDebug()<<s;
            if(s==(log_info.size()))
            {
                QMessageBox::warning(this,"错误","用户名不存在");
                return false;
            }
        }
    }
    return true;
}

void log_Dialog::closefun()
{
    this->close();
}

log_Dialog::~log_Dialog()
{
    delete ui;
}
