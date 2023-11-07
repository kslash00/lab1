#include "acclist.h"
#include <QListWidget>
#include <QListWidgetItem>
#include "QFile"
#include "QJsonArray"
#include "QJsonDocument"
#include "QJsonObject"
#include "addacc.h"
#include "crypt.h"
#include "customwidget.h"
#include "dbmodel.h"
#include "qaesencryption.h"
#include "ui_acclist.h"

AccList::AccList(QString key, QWidget *parent) //передаем ключ в окно
    : QDialog(parent), ui(new Ui::AccList) {
  ui->setupUi(this);
  this->key = key; //инициализация поля key класса AccList
  qInfo() << "key in acclist" << this->key;
  DBModel db(this->key); // передаем поле key конструктору класса DBModel
  auto json = db.json;
  for (int i = 0; i < json.size(); ++i) {
    this->addRow(db.getRow(i)); //добавление строки логин.пароль
  }
}

AccList::~AccList() { delete ui; }

void AccList::addRow(const DBModel::Row row) {  // добавление записей в GUI
  auto item = new QListWidgetItem();
  auto widget = new CustomWidget(key, this);
  widget->setText(row);
  item->setSizeHint(widget->sizeHint());
  ui->listWidget->addItem(item);
  ui->listWidget->setItemWidget(item, widget);
}

void AccList::on_addAccBtn_clicked() {
  addAcc NewWindow; //открывается окно addacc
  NewWindow.setModal(true);
  if (NewWindow.exec() == QDialog::Accepted) {
    auto row = NewWindow.getInputValues();

    DBModel db(this->key); //инициализация поля key в dbmodel
    auto eRow = db.encrypt(row);
    this->addRow(eRow); //добавление добавленной записи в окно addacc

    db.writeRowToJSON(eRow); //добавление зашифрованной записи в json файл
  }
}

void AccList::on_searchButton_clicked() {
  auto searchText = ui->searchInput->text(); //запись введенного текста

  auto listWidget = this->ui->listWidget;

  for (auto i = 0; i < listWidget->count(); i++) { //пробегаемся по строкам
    auto item = listWidget->item(i);
    auto itemWidget =
        dynamic_cast<CustomWidget *>(ui->listWidget->itemWidget(item));
    if (itemWidget->getRow().url.startsWith(searchText)) {
      item->setHidden(false);
    } else {
      item->setHidden(true);
    }
  }
}



