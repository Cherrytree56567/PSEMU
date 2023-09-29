/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <string>
#include <iostream>

class Logging
{
public:
	bool warnstatus = true;
	bool errstatus = true;
	bool infostatus = true;
	void err(int message);
	void warn(std::string message);
	void log(std::string message);
};

