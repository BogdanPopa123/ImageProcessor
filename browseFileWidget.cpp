#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_SelectFileButton_clicked()
{
    QString fileContent;

    QString filename= QFileDialog::getOpenFileName(this, "Choose File");


   if(filename.isEmpty())
       return;

   QFile file(filename);

   if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
       return;

   QTextStream in(&file);

   filecontent= in.readAll();

   file.close();

   ui->textEdit->clear();
   ui->textEdit->setPlainText(fileContent);
}
