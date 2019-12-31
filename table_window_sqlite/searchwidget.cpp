#include "searchwidget.h"
#include<QDebug>

searchwidget::searchwidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(280,400);
    this->setWindowTitle("查询参数");
    fir_lbl=new QLabel(this);
    fir_lbl->setText("加工批号:");
    fir_lbl->resize(100,40);
    sec_lbl=new QLabel(this);
    sec_lbl->setText("加工材料:");
    sec_lbl->resize(100,40);
    sec_lbl->move(0,50);
    thre_lbl=new QLabel(this);
    thre_lbl->setText("加工速度:(mm/s)");
    thre_lbl->resize(130,40);
    thre_lbl->move(0,100);
    four_lbl=new QLabel(this);
    four_lbl->setText("加载力:（g）");
    four_lbl->resize(100,40);
    four_lbl->move(0,150);
    five_lbl=new QLabel(this);
    five_lbl->setText("材料厚度:（mm）");
    five_lbl->resize(120,40);
    five_lbl->move(0,200);
    six_lbl=new QLabel(this);
    six_lbl->setText("刀头角度:（°）");
    six_lbl->resize(100,40);
    six_lbl->move(0,250);
    sev_lbl=new QLabel(this);
    sev_lbl->setText("划片方式:");
    sev_lbl->resize(100,40);
    sev_lbl->move(0,300);

    fir_let=new QLineEdit(this);
    fir_let->resize(100,30);
    fir_let->move(150,5);
    sec_let=new QLineEdit(this);
    sec_let->resize(100,30);
    sec_let->move(150,55);
    thre_let=new QLineEdit(this);
    thre_let->resize(100,30);
    thre_let->move(150,105);
    four_let=new QLineEdit(this);
    four_let->resize(100,30);
    four_let->move(150,155);
    five_let=new QLineEdit(this);
    five_let->resize(100,30);
    five_let->move(150,205);
    six_let=new QLineEdit(this);
    six_let->resize(100,30);
    six_let->move(150,255);
    sev_let=new QLineEdit(this);
    sev_let->resize(100,30);
    sev_let->move(150,305);

    this->set_arry_empty();

}
void searchwidget::set_arry_empty(void)
{
    for(int i=0;i<COLUMNCOUNT;i++)
        this->searchrow[i]="";
}
bool searchwidget::search_flag=false;
int searchwidget::empty_count=0;
