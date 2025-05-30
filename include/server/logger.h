#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

namespace logger
{
	enum LogType : unsigned char
	{
		INFO,
		ERR,
	};

	void log(const std::string& info, LogType logType = LogType::INFO)
	{
		auto t = std::time(nullptr);
		auto time = *std::localtime(&t);

		auto formattedTime = std::put_time(&time, "%H:%M:%S");

		std::string str = logType == LogType::INFO ? "[INFO]: " : "[ERROR]: ";

		std::cout << "[" << formattedTime << "] " << str << info << std::endl;
	}
}
