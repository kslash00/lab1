#include "crypt.h"
#include "QCryptographicHash"

QAESEncryption Crypt::encrypter(QAESEncryption::AES_256, QAESEncryption::CBC);

Crypt::Crypt() {}
QByteArray Crypt::getKey(QString plainKey) {
  QByteArray keyByteArray = plainKey.toUtf8();
  QByteArray resKey =
      QCryptographicHash::hash(keyByteArray, QCryptographicHash::Sha256);

  return resKey;
}
QByteArray Crypt::getIV(QString plainIV) {
  QByteArray resIV = QCryptographicHash::hash((plainIV.toLocal8Bit()),
                                              QCryptographicHash::Md5);
  return resIV;
}

QByteArray Crypt::encrypt(QByteArray text, QString key, QString iv) {
  QByteArray encodedText =
      Crypt::encrypter.encode(text, Crypt::getKey(key), Crypt::getIV(iv));

  return encodedText.toHex();
}

QString Crypt::decrypt(QByteArray text, QString key, QString iv) { //функция расшифровки
  auto textFromHex = QByteArray::fromHex(text); //текст
  auto k = Crypt::getKey(key); //ключ
  auto i = Crypt::getIV(iv); //вектор инициализации
  QByteArray decodedByteText = Crypt::encrypter.decode(textFromHex, k, i);

  auto withoutPad = Crypt::encrypter.removePadding(decodedByteText);
  QString decodedText = QString(withoutPad);

  return decodedText;
}
