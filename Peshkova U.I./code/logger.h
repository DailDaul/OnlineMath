#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <mutex>

class Logger {
private:
    
    static const std::string LOG_FILE; // путь к файлу логов

    int logLevel; // уровень логирования (1-ERROR, 2-WARNING, 3-INFO)

    bool fileLoggingEnabled; // запись в файл

    bool consoleOutputEnabled; // вывод в консоль

    int logCount; // кол-во записанных сообщений

    std::ofstream logFileStream; // файловый поток для записи в лог

    std::mutex mutex; // мьютекс для потокобезопасности

    Logger(); // приватный конструктор (паттерн Singleton)

    ~Logger();

    // запрет копирования
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    
    static Logger& getInstance(); // статический метод для получения экземпляра (паттерн Singleton)

    void log(const std::string& message); // для записи сообщения в лог

    void setLogLevel(int level); // для установки уровня логирования

    void enableFileLogging(bool enable); // для включения/выключения записи в файл

    void enableConsoleOutput(bool enable); // для включения/выключения вывода в консоль

    void clearLogs(); // для очистки файла логов

    int getLogCount(); // для получения количества записанных сообщений

    void flush(); // для принудительной записи буфера в файл
};

#endif
