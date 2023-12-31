#include "customwidget.h"
#include "dbmodel.h"
#include "ui_customwidget.h"

//SIGNAL - когда какое-то событие произошло
//SLOT - функция, которая обрабатывает SIGNAL

CustomWidget::CustomWidget(QString pass, QWidget *parent)
    : QWidget(parent), ui(new Ui::CustomWidget) {
  ui->setupUi(this);
  this->pass = pass;
  connect(this, SIGNAL(sendRemoveItem(const QString,const QString)), parent,
          SLOT(removeItem(const QString,const QString)));
}

CustomWidget::~CustomWidget() { delete ui; }

void CustomWidget::setText(const DBModel::Row row) { // заполнение данными
  ui->labelURL->setText(row.url);
  ui->labelLogin->setText(row.login);
  ui->labelPass->setText(row.pass);
}

void CustomWidget::on_toolButton_clicked() { // расшифрование данных
  //    emit ShowItem(ui->labelLogin->text());
  DBModel db(this->pass);
  DBModel::Row row = {ui->labelURL->text(), ui->labelLogin->text(),
                      ui->labelPass->text()};

  if (!CustomWidget::show) { //режим отображения текста, изначально зашифр
    ui->labelLogin->setEchoMode(QLineEdit::Normal);
    ui->labelPass->setEchoMode(QLineEdit::Normal);
    DBModel::Row dRow = db.decrypt(row);

    ui->labelLogin->setText(dRow.login); //изменение текста
    ui->labelPass->setText(dRow.pass);

    CustomWidget::show = true;
  } else {
    ui->labelLogin->setEchoMode(QLineEdit::Password);
    ui->labelPass->setEchoMode(QLineEdit::Password);

    DBModel::Row eRow = db.encrypt(row);
    ui->labelLogin->setText(eRow.login); //изменение текста

    ui->labelPass->setText(eRow.pass);

    CustomWidget::show = false;
  }
}

DBModel::Row CustomWidget::getRow() { // инициализируем объект Row, записываем в него url.login.pass
  DBModel::Row row;
  row.login = ui->labelLogin->text();
  row.url = ui->labelURL->text();
  row.pass = ui->labelPass->text();
  return row;
}

void CustomWidget::on_deleteButton_clicked() {
  emit sendRemoveItem(ui->labelURL->text(),ui->labelLogin->text()); //функция emit для испускания SIGNAL
}
