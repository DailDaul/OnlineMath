#ifndef DATABASE_H
#define DATABASE_H
QT -= графический интерфейс пользователя

      ####### ДОБАВИТЬ!!!
      QT + = sql

              CONFIG + = консоль c ++ 11
    CONFIG -= app_bundle

# Вы можете сделать так, чтобы ваш код не компилировался, если он использует устаревшие API.
# Для этого раскомментируйте следующую строку.
#ОПРЕДЕЛЯЕТ += QT_DISABLE_DEPRECATED_BEFORE=0x060000 # отключает все API, устаревшие до Qt 6.0.0

            ИСТОЧНИКИ += \
       main.cpp

# Правила развертывания по умолчанию.
           qnx: target.path = /tmp/$${TARGET}/bin
                           else: unix:!android: target.path = /opt/$${TARGET}/bin
      !isEmpty(target.path): INSTALLS += target
#endif // DATABASE_H
