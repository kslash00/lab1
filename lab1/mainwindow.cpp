#include "mainwindow.h"
#include "QCryptographicHash"
#include "QFile"
#include "QMessageBox"
#include "QString"
#include "acclist.h"
#include "crypt.h"
#include "dbmodel.h"
#include "qaesencryption.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
  QString pinCode = ui->Pin->text();

  QFile file(DBModel::kFileName); //создается объект класса Qfile, в скобках передаем путь к файлу

  if (!file.exists()) {  //проверка существования файла (если сущ - возвращает true и выполняем код, иначе не выполняем
    auto encodedPin = //зашифровывает пин код, результат записывается в encodedpin
        Crypt::encrypt((pinCode + "\n[]").toUtf8(), pinCode, pinCode);

    file.open(QIODevice::ReadWrite | QIODevice::Text); //открывается файл для чтения и записи
    file.write(encodedPin); //записываем в файл зашифрованный пин код
    file.close();

    QMessageBox::warning(this, "Pin", "A new PIN code has been created");

    return MainWindow::on_pushButton_clicked();
  }
  file.open(QIODevice::ReadWrite | QIODevice::Text); //снова открываем файл для чтения и записи
  QByteArray readedText = file.readAll(); // в переменную readedText записывается все содержимое файла
  file.close();

  QString decodedText = Crypt::decrypt(readedText, pinCode, pinCode); //расшифровка текста
  QString decodedKey = decodedText.split("\n")[0]; //запись ключа в decodedKey из первой строки json файла

  qInfo() << "Key" << decodedKey;

  if (decodedKey == pinCode) { //проверка соответствия введенного пин кода и пин кода из файла
    AccList NewWindow(pinCode); //если верно - открывается следующее окно acclist
    NewWindow.setModal(true);
    NewWindow.exec();
  } else {
    QMessageBox::warning(this, "Authotization", "Invalid PIN code"); //если неверно - вывод соответствующего сообщения
  }
}
