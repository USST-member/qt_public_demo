#include "table_mainwindow.h"
#include "ui_table_mainwindow.h"
#include<QStandardItemModel>
#include<QPushButton>
#include<QAbstractItemModel>
#include<QModelIndex>
#include<QVariant>
#include<QInputDialog>
#include<QMessageBox>
#include<QDebug>
#include <QtCore/QCoreApplication>
#include <iostream>
using namespace std;

/*表格中存储的数据类型*/
typedef struct table_data{
    int id;
    QString fir;
    QString sec;
    QString thre;
    QString four;
//    QString five;
//    QString six;
//    QString senv;
}table_data;
/*自定义消息窗口类*/
class MyMessageBox : public QMessageBox {
protected:
void paintEvent ( QPaintEvent * event)
{
    this->setFixedSize(280,200);
}
};

table_MainWindow::table_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::table_MainWindow)
{
    ui->setupUi(this);
    this->udp_init();
    search_widget=new searchwidget;
    this->get_let_time= new QTimer(this);
    this->get_let_time->setInterval(50);
    this->get_let_time->stop();
    //this->setMinimumSize(1000,600);

    //addrow=new QAction(QIcon(""),"新建加工参数",this);

    search_row=new QAction(QIcon(""),"查询加工参数",this);


    menu=menuBar()->addMenu("菜单");
    //menu->setSeparatorsCollapsible(false);
    menu->addAction(search_row);
    menu->addSeparator();



    toolbar=addToolBar("表格工具条");
    toolbar->addAction(search_row);
    toolbar->addSeparator();



    this->table= new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可修改
    table->move(0,45);
    table->resize(500,500);
    this->setWindowTitle("加工参数");
    table->setShowGrid(true);//网格线设置
    table->setGridStyle(Qt::SolidLine);
    table->setSortingEnabled(false);//设置排序
    table->setAlternatingRowColors(true);
    table->setStyleSheet("QHeaderView::section, QTableCornerButton::section {padding: 1px;border: none;border-bottom: 1px solid rgb(75, 120, 154);border-right: 1px solid rgb(75, 120, 154);border-bottom: 1px solid gray;background-color:rgba(0, 0, 0, 1);color:rgba(126,206,244,1);}");
    //table->setStyleSheet("QHeaderView::section, QTableCornerButton::section {padding: 1px;border: none;border-bottom: 1px solid rgb(75, 120, 154);border-right: 1px solid rgb(75, 120, 154);border-bottom: 1px solid gray;background-color:rgba(75, 120, 154, 1);color:rgba(126,206,244,1);}");
    //table->setStyleSheet("QTableView { border: none;""selection-background-color: #8EDE21;""color: black}");
    table->setRowCount(RowCount);    //设置行数
    table->setColumnCount(COLUMNCOUNT); //设置列数
    table->setHorizontalHeaderLabels(QStringList()<<"站点"<<"uid"<<"进入时间"<<"温度（℃）"); //设置行头
    //table->setItem(0,0,new QTableWidgetItem("item1")); //设置表格内容

    /*数据库操作*/
    this->init_sqlite();
    this->convert_sqlite_table();


    connect(this->get_let_time, &QTimer::timeout, this,&table_MainWindow::get_timefun);
    connect(this->udpsocket,&QUdpSocket::readyRead,this,&table_MainWindow::addrowfun);
    connect(this->search_row,&QAction::triggered,this,&table_MainWindow::search_widgetshow);
}
int table_MainWindow::RowCount=0;
bool table_MainWindow::update_flag=false;

/*udp传输的初始化*/
void table_MainWindow::udp_init(void)
{
    this->udp_port=22;
    this->udpsocket=new QUdpSocket(this);
    this->udp_is_stated=false;
    this->address="192.168.13.1";
    udpsocket->bind(QHostAddress(this->address),this->udp_port,QUdpSocket::ShareAddress);
}


/*定时器的触发函数*/
void table_MainWindow::get_timefun(void)
{
    this->search_widget->searchrow[0]=this->search_widget->fir_let->text();
    this->search_widget->searchrow[1]=this->search_widget->sec_let->text();
    this->search_widget->searchrow[2]=this->search_widget->thre_let->text();
    this->search_widget->searchrow[3]=this->search_widget->four_let->text();
//    this->search_widget->searchrow[4]=this->search_widget->five_let->text();
//    this->search_widget->searchrow[5]=this->search_widget->six_let->text();
//    this->search_widget->searchrow[6]=this->search_widget->sev_let->text();
    this->search_widget->empty_count=0;
    for(int i=0;i<COLUMNCOUNT;i++)
    {
        if(this->search_widget->searchrow[i].isEmpty()==true)
            this->search_widget->empty_count++;
    }
    if(this->search_widget->empty_count!=7)
    {
        for (int j = 0; j < RowCount; j++)
        {
            table->setRowHidden(j, true);//隐藏所有行
        }

        for (int k = 0; k < COLUMNCOUNT; k++)
        {
            this->show_searchinfo(this->search_widget->searchrow[k],k);
        }
    }
    else
    {
        for (int i = 0; i < RowCount; i++)
        {
            table->setRowHidden(i,false);//显示所有行
        }
    }
}

/*表格的某项搜索，index表示所在列*/
void table_MainWindow::show_searchinfo(QString str,int index)
{
    if(str.isEmpty()==true)
        return;
    else
    {
        QList <QTableWidgetItem *> item = table->findItems(str, Qt::MatchContains);
        for (int i = 0; i < item.count(); i++)
        {
            if(item.at(i)->column()==index)
            {
                table->setRowHidden(item.at(i)->row(),false);//显示所在行
            }
        }
    }
}

/*数据库初始化*/
bool table_MainWindow::init_sqlite(void)
{
    bool status;
    /*数据库的初始化*/
    if (QSqlDatabase::contains("qt_sql_default_connection"))
        this->db= QSqlDatabase::database("qt_sql_default_connection");
    else
    {
        this->db =QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("Dongkangjia");
        db.setDatabaseName("tabledb.db");
        db.setUserName("dongkangjia");
        db.setPassword("123456..");
    }
    status=db.open();
    QSqlQuery query(this->db);
    if(status==true)
    {
        /*创建数据库表格*/
        status=query.exec("create table parameter_table(id int,fir varchar,sec varchar,thre varchar,four varchar)");
        qDebug()<<status;
        if(status==true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}

/*数据库全部显示*/
void table_MainWindow::convert_sqlite_table(void)
{
    QSqlQuery query(this->db);
    QVector<table_data> table_info;
    query.prepare("SELECT * FROM parameter_table");
    query.exec();	//执行
    while(query.next())
    {
        table_data s;
        s.id=query.value("id").toInt();
        s.fir=query.value("fir").toString();
        s.sec=query.value("sec").toString();
        s.thre=query.value("thre").toString();
//        s.four=query.value("four").toString();
//        s.six=query.value("six").toString();
//        s.five=query.value("five").toString();
//        s.senv=query.value("senv").toString();
        table_info.push_back(s);
    }
    this->RowCount=table_info.size();

    qDebug()<<RowCount;
    for(int i=0;i<RowCount;i++)
    {
            table->setItem(i,0,new QTableWidgetItem(table_info[i].fir));
            table->setItem(i,1,new QTableWidgetItem(table_info[i].sec));
            table->setItem(i,2,new QTableWidgetItem(table_info[i].thre));
            table->setItem(i,3,new QTableWidgetItem(table_info[i].four));
//            table->setItem(i,4,new QTableWidgetItem(table_info[i].five));
//            table->setItem(i,5,new QTableWidgetItem(table_info[i].six));
//            table->setItem(i,6,new QTableWidgetItem(table_info[i].senv));
            qDebug()<<table_info[i].id;
    }
}


/*查询窗口的显示*/
void table_MainWindow::search_widgetshow(void)
{
    this->search_widget->show();
    this->search_widget->fir_let->setText("");
    this->search_widget->sec_let->setText("");
    this->search_widget->thre_let->setText("");
    this->search_widget->four_let->setText("");
//    this->search_widget->five_let->setText("");
//    this->search_widget->six_let->setText("");
//    this->search_widget->sev_let->setText("");
    this->search_widget->search_flag=true;
    this->get_let_time->start();
    return;
}

void table_MainWindow::paintEvent (QPaintEvent *)
{
    int width=table->width();
    int height=table->height();
    this->resize(width,height+45);
    table->setRowCount(RowCount);    //设置行数
    if(update_flag==true)
    {
        update_newrow();
        update_flag=false;
    }
    /*查询窗口关闭时显示表格的所有行*/
    if(search_widget->isVisible()==false)
    {
        this->search_widget->search_flag=false;
        this->get_let_time->stop();
        for (int i = 0; i < RowCount; i++)
        {
            table->setRowHidden(i,false);//显示所在行
        }
    }
    convert_sqlite_table();
}
/*增加一行*/
void table_MainWindow::addrowfun(void)
{
    while (udpsocket->hasPendingDatagrams()) {
               QByteArray datagram;
               datagram.resize(udpsocket->pendingDatagramSize());
               udpsocket->readDatagram(datagram.data(),datagram.size());

               QString msg=datagram;
               QString msg1=msg.section(',',0,0,QString::SectionSkipEmpty);
               QString msg2=msg.section(',',1,1,QString::SectionSkipEmpty);
               QString msg3=msg.section(',',2,2,QString::SectionSkipEmpty);
               QString msg4=msg.section(',',3,3,QString::SectionSkipEmpty);

               newrow[0]=msg1;
               newrow[1]=msg2;
               newrow[2]=msg3;
               newrow[3]=msg4;
       //        newrow[4]=five_let->text();
       //        newrow[5]=six_let->text();
       //        newrow[6]=sev_let->text();
               QSqlQuery query(this->db);
               query.prepare("insert into parameter_table values(?,?,?,?,?)");
               query.bindValue(0,this->RowCount);
               query.bindValue(1,msg1);
               query.bindValue(2,msg2);
               query.bindValue(3,msg3);
               query.bindValue(4,msg4);
               bool sucess=query.exec();
               qDebug()<<"sucess"<<sucess;
               update_flag=true;
               this->RowCount++;
               update();
    }
}

void table_MainWindow::update_newrow(void)
{
    table->setItem(RowCount-1,0,new QTableWidgetItem(newrow[0]));
    table->setItem(RowCount-1,1,new QTableWidgetItem(newrow[1]));
    table->setItem(RowCount-1,2,new QTableWidgetItem(newrow[2]));
    table->setItem(RowCount-1,3,new QTableWidgetItem(newrow[3]));
}


table_MainWindow::~table_MainWindow()
{
    this->db.close();
    delete ui;
}
