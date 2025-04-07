# Используем базовый образ Ubuntu
FROM ubuntu:20.04

# Устанавливаем необходимые пакеты
RUN apt-get update && \
    apt-get install -y qtcreator qtbase5-dev qttools5-dev-tools qt5-qmake cmake telnet g++ && \
    apt-get clean

# Создаем рабочую директорию
WORKDIR /usr/src/myapp

# Копируем файлы проекта в контейнер
COPY . .

# Выполняем сборку проекта
RUN qmake echoSserver.pro && make

# Указываем команду, которая будет выполняться при запуске контейнера
CMD ["./echoSserver"]
