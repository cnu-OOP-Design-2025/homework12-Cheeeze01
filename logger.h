
#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <string>

class Logger
{
private:
    static std::unique_ptr<Logger> instance;
    static std::mutex init_mtx;
    static std::mutex write_mtx;
    std::ofstream logFile;

    Logger(const std::string &filename = "Test/output2.txt")
    {
        logFile.open(filename, std::ios::trunc);
        logFile << "[Init] Logger started." << std::endl;
    }

public:
    ~Logger()
    {
        logFile << "[Shutdown] Logger closed." << std::endl;
        logFile.close();
    }

    static Logger *getInstance(const std::string &filename = "Test/output2.txt")
    {
        if (!instance)
        {
            std::lock_guard<std::mutex> lock(init_mtx);
            if (!instance)
            {
                instance = std::unique_ptr<Logger>(new Logger(filename));
            }
        }
        return instance.get();
    }

    void log(const std::string &message)
    {
        std::lock_guard<std::mutex> lock(write_mtx);
        logFile << message << std::endl;
    }

    static void logCreate(const std::string &characterName)
    {
        getInstance()->log("[Create] " + characterName);
    }

    static void logTryEquip(const std::string &description, const std::string &item)
    {
        getInstance()->log("[Trying to Equip] " + description + " + " + item);
    }

    static void logFailEquip()
    {
        getInstance()->log("[Fail to Equip] ");
    }
};

std::unique_ptr<Logger> Logger::instance;
std::mutex Logger::init_mtx;
std::mutex Logger::write_mtx;
