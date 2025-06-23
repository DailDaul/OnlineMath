#include "logger.h"
#include <iostream>

int main() {
    
    Logger& logger = Logger::getInstance(); // получение экземпляра логгера

    // использование логгера
    logger.log("Application started.");
    logger.setLogLevel(2); // уровень логирования ставится на WARNING
    logger.log("Warning: Disk space is running low.");
    logger.setLogLevel(3);
    logger.log("Info: User logged in.");
    logger.enableFileLogging(false);
    logger.log("This message will not be written to the file.");
    logger.enableFileLogging(true);
    logger.enableConsoleOutput(false);
    logger.log("This message will not be displayed in the console but will be written to the file.");
    logger.enableConsoleOutput(true);
    logger.log("This message will be displayed in the console and written to the file.");


    std::cout << "Number of logged messages: " << logger.getLogCount() << std::endl;
    logger.flush();

    logger.clearLogs(); // очистка файла логов

    logger.log("Logs cleared.");
    logger.flush();

    std::cout << "Number of logged messages after clearing: " << logger.getLogCount() << std::endl;

    // получение еще одного экземпляра логгера (это будет тот же самый экземпляр)
    Logger& anotherLogger = Logger::getInstance();
    anotherLogger.log("Application finished.");

    return 0;
}
