#pragma once

#include <iostream>
#include <ctime>
#include <vector>

#include <tabulate/table.hpp>


struct singleTask {
	std::string taskName;
	bool isImportant;
	std::time_t dueDate;
};

std::vector<singleTask> allTasks;

bool checkUrgency(const std::time_t& locDueDate);
void printMatrix(const std::vector<singleTask>& locAllTasks);
std::time_t dateString2Time(const char& locTimeString);

