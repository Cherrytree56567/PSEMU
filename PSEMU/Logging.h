#pragma once
#include <string>
#include <iostream>

class Logging
{
public:
	void err(std::string message);
	void warn(std::string message);
	void info(std::string message);
};

