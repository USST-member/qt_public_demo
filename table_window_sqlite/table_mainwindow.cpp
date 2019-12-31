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

/*表格中存储的数据类型*/
typedef struct table_data{
    int id;
    QString fir;
    QString sec;
    QString thre;
    QString four;
    QString five;
    QString six;
    QString senv;
}table_data;
/*自定义消息窗口类*/
class MyMessageBox : public QMessageBox {
protected:
void paintEvent ( QPaintEvent * event)
{
    this->setFixedSize(280,400);
}
};

table_MainWindow::table_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::table_MainWindow)
{
    ui->setupUi(this);
    search_widget=new searchwidget;
    this->get_let_time= new QTimer(this);
    this->get_let_time->setInterval(50);
    this->get_let_time->stop();

    addrow=new QAction(QIcon(""),"新建加工参数",this);
    delete_row=new QAction(QIcon(""),"删除一行参数",this);
    search_row=new QAction(QIcon(""),"查询加工参数",this);
    update_row=new QAction(QIcon(""),"修改一行参数",this);

    menu=menuBar()->addMenu("菜单");
    //menu->setSeparatorsCollapsible(false);
    menu->addAction(addrow);
    menu->addSeparator();
    menu->addAction(search_row);
    menu->addSeparator();
    menu->addAction(delete_row);
    menu->addSeparator();
    menu->addAction(update_row);
    menu->addSeparator();


    toolbar=addToolBar("表格工具条");
    toolbar->addAction(addrow);
    toolbar->addSeparator();
    toolbar->addAction(search_row);
    toolbar->addSeparator();
    toolbar->addAction(delete_row);
    toolbar->addSeparator();
    toolbar->addAction(update_row);
    toolbar->addSeparator();


    this->table= new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可修改
    table->move(0,45);
    table->resize(700,600);
    this->setWindowTitle("加工参数");
    table->setShowGrid(true);//网格线设置
    table->setGridStyle(Qt::SolidLine);
    table->setSortingEnabled(false);//设置排序
    table->setStyleSheet("QHeaderView::section, QTableCornerButton::section {padding: 1px;border: none;border-bottom: 1px solid rgb(75, 120, 154);border-right: 1px solid rgb(75, 120, 154);border-bottom: 1px solid gray;background-color:rgba(75, 120, 154, 1);color:rgba(126,206,244,1);}");
    //table->setStyleSheet("QTableView { border: none;""selection-background-color: #8EDE21;""color: black}");
    table->setRowCount(RowCount);    //设置行数
    table->setColumnCount(COLUMNCOUNT); //设置列数
    table->setHorizontalHeaderLabels(QStringList()<<"加工批号"<<"加工材料"<<"加工速度(mm/s)"<<"加载力（g）"<<"材料厚度（mm）"<<"刀头角度（°）"<<"划片方式"); //设置行头
    //table->setItem(0,0,new QTableWidgetItem("item1")); //设置表格内容

    /*数据库操作*/
    this->init_sqlite();
    this->convert_sqlite_table();


    connect(this->get_let_time, &QTimer::timeout, this,&table_MainWindow::get_timefun);
    connect(this->addrow,&QAction::triggered,this,&table_MainWindow::addrowfun);
    connect(this->delete_row,&QAction::triggered,this,&table_MainWindow::delete_rowfun);
    connect(this->search_row,&QAction::triggered,this,&table_MainWindow::search_widgetshow);
    connect(this->update_row,&QAction::triggered,this,&table_MainWindow::update_row_item);
}


int table_MainWindow::RowCount=0;
bool table_MainWindow::update_flag=false;

/*定时器的触发函数*/
void table_MainWindow::get_timefun(void)
{
    this->search_widget->searchrow[0]=this->search_widget->fir_let->text();
    this->search_widget->searchrow[1]=this->search_widget->sec_let->text();
    this->search_widget->searchrow[2]=this->search_widget->thre_let->text();
    this->search_widget->searchrow[3]=this->search_widget->four_let->text();
    this->search_widget->searchrow[4]=this->search_widget->five_let->text();
    this->search_widget->searchrow[5]=this->search_widget->six_let->text();
    this->search_widget->searchrow[6]=this->search_widget->sev_let->text();
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
        status=query.exec("create table parameter_table(id int,fir varchar,sec varchar,thre varchar,four varchar,five varchar,six varchar,senv varchar)");
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
        s.four=query.value("four").toString();
        s.six=query.value("six").toString();
        s.five=query.value("five").toString();
        s.senv=query.value("senv").toString();
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
            table->setItem(i,4,new QTableWidgetItem(table_info[i].five));
            table->setItem(i,5,new QTableWidgetItem(table_info[i].six));
            table->setItem(i,6,new QTableWidgetItem(table_info[i].senv));
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
    this->search_widget->five_let->setText("");
    this->search_widget->six_let->setText("");
    this->search_widget->sev_let->setText("");
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
    MyMessageBox add_row_msgbox;
    add_row_msgbox.setWindowTitle("添加参数");
    add_row_msgbox.setFixedSize(280,400);
    QPushButton *sureBtn=add_row_msgbox.addButton(QMessageBox::Ok);
    QPushButton *cancelBtn=add_row_msgbox.addButton(QMessageBox::Cancel);

    QLabel *fir_lbl=new QLabel(&add_row_msgbox);
    fir_lbl->setText("加工批号:");
    fir_lbl->resize(100,40);
    QLabel *sec_lbl=new QLabel(&add_row_msgbox);
    sec_lbl->setText("加工材料:");
    sec_lbl->resize(100,40);
    sec_lbl->move(0,50);
    QLabel *thre_lbl=new QLabel(&add_row_msgbox);
    thre_lbl->setText("加工速度:(mm/s)");
    thre_lbl->resize(150,40);
    thre_lbl->move(0,100);
    QLabel *four_lbl=new QLabel(&add_row_msgbox);
    four_lbl->setText("加载力:（g）");
    four_lbl->resize(100,40);
    four_lbl->move(0,150);
    QLabel *five_lbl=new QLabel(&add_row_msgbox);
    five_lbl->setText("材料厚度:（mm）");
    five_lbl->resize(130,40);
    five_lbl->move(0,200);
    QLabel *six_lbl=new QLabel(&add_row_msgbox);
    six_lbl->setText("刀头角度:（°）");
    six_lbl->resize(100,40);
    six_lbl->move(0,250);
    QLabel *sev_lbl=new QLabel(&add_row_msgbox);
    sev_lbl->setText("划片方式:");
    sev_lbl->resize(100,40);
    sev_lbl->move(0,300);

    QLineEdit *fir_let=new QLineEdit(&add_row_msgbox);
    fir_let->resize(100,30);
    fir_let->move(150,5);
    QLineEdit *sec_let=new QLineEdit(&add_row_msgbox);
    sec_let->resize(100,30);
    sec_let->move(150,55);
    QLineEdit *thre_let=new QLineEdit(&add_row_msgbox);
    thre_let->resize(100,30);
    thre_let->move(150,105);
    QLineEdit *four_let=new QLineEdit(&add_row_msgbox);
    four_let->resize(100,30);
    four_let->move(150,155);
    QLineEdit *five_let=new QLineEdit(&add_row_msgbox);
    five_let->resize(100,30);
    five_let->move(150,205);
    QLineEdit *six_let=new QLineEdit(&add_row_msgbox);
    six_let->resize(100,30);
    six_let->move(150,255);
    QLineEdit *sev_let=new QLineEdit(&add_row_msgbox);
    sev_let->resize(100,30);
    sev_let->move(150,305);

    add_row_msgbox.exec();
    /*将获得的数据填入表格*/
    if(add_row_msgbox.clickedButton()==sureBtn)
    {
        newrow[0]=fir_let->text();
        newrow[1]=sec_let->text();
        newrow[2]=thre_let->text();
        newrow[3]=four_let->text();
        newrow[4]=five_let->text();
        newrow[5]=six_let->text();
        newrow[6]=sev_let->text();
        QSqlQuery query(this->db);
        query.prepare("insert into parameter_table values(?,?,?,?,?,?,?,?)");
        query.bindValue(0,this->RowCount);
        query.bindValue(1,fir_let->text());
        query.bindValue(2,sec_let->text());
        query.bindValue(3,thre_let->text());
        query.bindValue(4,four_let->text());
        query.bindValue(5,five_let->text());
        query.bindValue(6,six_let->text());
        query.bindValue(7,sev_let->text());
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
    table->setItem(RowCount-1,4,new QTableWidgetItem(newrow[4]));
    table->setItem(RowCount-1,5,new QTableWidgetItem(newrow[5]));
    table->setItem(RowCount-1,6,new QTableWidgetItem(newrow[6]));

}
void table_MainWindow::delete_rowfun(void)
{
    QSqlQuery query(this->db);
    if(this->RowCount>0)
    {
        bool sure;
        int row=QInputDialog::getInt(this,tr("删除一行"),tr("删除行号:"),0,1,2147483647,1,&sure);
        if(sure)
        {
            if(row>0)
            {
                QString s1=table->item(row-1,0)->text();
                QString s2=table->item(row-1,1)->text();
                QString s3=table->item(row-1,2)->text();
                QString s4=table->item(row-1,3)->text();
                QString s5=table->item(row-1,4)->text();
                QString s6=table->item(row-1,5)->text();
                QString s7=table->item(row-1,6)->text();
                table->removeRow(row-1);
                qDebug()<<row-1;
                query.exec(QString("delete from parameter_table where id=%1").arg(row-1));
                query.exec(QString("update parameter_table set id=id-1 where id>%1").arg(row-1));
                this->RowCount--;
            }
        }
    }
}
/*修改一行参数*/
void table_MainWindow::update_row_item(void)
{
    bool sure;
    int row=QInputDialog::getInt(this,tr("修改一行"),tr("修改行号:"),0,1,2147483647,1,&sure);
    if(sure)
    {
        if(row>0&&row<this->RowCount+1)
        {
            QSqlQuery query(this->db);
            QString s1=table->item(row-1,0)->text();
            QString s2=table->item(row-1,1)->text();
            QString s3=table->item(row-1,2)->text();
            QString s4=table->item(row-1,3)->text();
            QString s5=table->item(row-1,4)->text();
            QString s6=table->item(row-1,5)->text();
            QString s7=table->item(row-1,6)->text();

            MyMessageBox update_row_msgbox;
            update_row_msgbox.setWindowTitle("添加参数");
            update_row_msgbox.setFixedSize(280,400);
            QPushButton *sureBtn=update_row_msgbox.addButton(QMessageBox::Ok);
            QPushButton *cancelBtn=update_row_msgbox.addButton(QMessageBox::Cancel);

            QLabel *fir_lbl=new QLabel(&update_row_msgbox);
            fir_lbl->setText("加工批号:");
            fir_lbl->resize(100,40);
            QLabel *sec_lbl=new QLabel(&update_row_msgbox);
            sec_lbl->setText("加工材料:");
            sec_lbl->resize(100,40);
            sec_lbl->move(0,50);
            QLabel *thre_lbl=new QLabel(&update_row_msgbox);
            thre_lbl->setText("加工速度:(mm/s)");
            thre_lbl->resize(150,40);
            thre_lbl->move(0,100);
            QLabel *four_lbl=new QLabel(&update_row_msgbox);
            four_lbl->setText("加载力:（g）");
            four_lbl->resize(100,40);
            four_lbl->move(0,150);
            QLabel *five_lbl=new QLabel(&update_row_msgbox);
            five_lbl->setText("材料厚度:（mm）");
            five_lbl->resize(130,40);
            five_lbl->move(0,200);
            QLabel *six_lbl=new QLabel(&update_row_msgbox);
            six_lbl->setText("刀头角度:（°）");
            six_lbl->resize(100,40);
            six_lbl->move(0,250);
            QLabel *sev_lbl=new QLabel(&update_row_msgbox);
            sev_lbl->setText("划片方式:");
            sev_lbl->resize(100,40);
            sev_lbl->move(0,300);

            QLineEdit *fir_let=new QLineEdit(&update_row_msgbox);
            fir_let->resize(100,30);
            fir_let->move(150,5);
            QLineEdit *sec_let=new QLineEdit(&update_row_msgbox);
            sec_let->resize(100,30);
            sec_let->move(150,55);
            QLineEdit *thre_let=new QLineEdit(&update_row_msgbox);
            thre_let->resize(100,30);
            thre_let->move(150,105);
            QLineEdit *four_let=new QLineEdit(&update_row_msgbox);
            four_let->resize(100,30);
            four_let->move(150,155);
            QLineEdit *five_let=new QLineEdit(&update_row_msgbox);
            five_let->resize(100,30);
            five_let->move(150,205);
            QLineEdit *six_let=new QLineEdit(&update_row_msgbox);
            six_let->resize(100,30);
            six_let->move(150,255);
            QLineEdit *sev_let=new QLineEdit(&update_row_msgbox);
            sev_let->resize(100,30);
            sev_let->move(150,305);

            fir_let->setText(s1);
            sec_let->setText(s2);
            thre_let->setText(s3);
            four_let->setText(s4);
            five_let->setText(s5);
            six_let->setText(s6);
            sev_let->setText(s7);

            update_row_msgbox.exec();
            /*将获得的数据填入表格*/
            if(update_row_msgbox.clickedButton()==sureBtn)
            {
                qDebug()<<"__________";
                if(fir_let->text().compare(s1))
                {
                    qDebug()<<"++++++++++++++++";
                    query.exec(QString("update parameter_table set fir=%1 where id=%2").arg(fir_let->text()).arg(row-1));
                    update();
                }
                if(sec_let->text().compare(s2))
                {
                    query.exec(QString("update parameter_table set sec=%1 where id=%2").arg(sec_let->text()).arg(row-1));
                    update();
                }
                if(thre_let->text().compare(s3))
                {
                    query.exec(QString("update parameter_table set thre=%1 where id=%2").arg(thre_let->text()).arg(row-1));
                    update();
                }
                if(four_let->text().compare(s4))
                {
                    query.exec(QString("update parameter_table set four=%1 where id=%2").arg(four_let->text()).arg(row-1));
                    update();
                }
                if(five_let->text().compare(s5))
                {
                    query.exec(QString("update parameter_table set five=%1 where id=%2").arg(five_let->text()).arg(row-1));
                    update();
                }
                if(six_let->text().compare(s6))
                {
                    query.exec(QString("update parameter_table set six=%1 where id=%2").arg(six_let->text()).arg(row-1));
                    update();
                }
                if(sev_let->text().compare(s7))
                {
                    query.exec(QString("update parameter_table set senv=%1 where id=%2").arg(sev_let->text()).arg(row-1));
                    update();
                }
            }
        }
    }
}
table_MainWindow::~table_MainWindow()
{
    this->db.close();
    delete ui;
}
