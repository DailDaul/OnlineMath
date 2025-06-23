# образ ubuntu 18.04
FROM ubuntu:18.04

# информация об авторе
LABEL author="peshkova"

# установка gcc
RUN apt-get update && apt-get install -y gcc g++ make

# создание директории для проекта
RUN mkdir /MyProj_peshkova_241-326

# копирование файлов проекта в контейнер
COPY . /MyProj_peshkova_241-326

# установка рабочей директории
WORKDIR /MyProj_peshkova_241-326

# компиляция проекта
RUN gcc main.cpp logger.cpp -o ex_pro -std=c++11 -lstdc++

# установка значения параметра по умолчанию
ENV DEFAULT_PARAM="upi"

# команда запуска программы с параметром
CMD ["./ex_pro", "$DEFAULT_PARAM"]
