#pragma once

#include <iostream>
#include <ctime>
#include <vector>

#include <tabulate/table.hpp>

//creating structure of task
//initializing structure fields with some default values
//if we forget to enter data to these fields, these values will already be there
struct singleTask {
	std::string taskName = "New_task";
	bool isImportant = false;
	std::time_t dueDate = std::time(nullptr);
	char isDone[2] = " "; //this is the character used to define if task is done
	// it can either be "X" or " "
};

std::vector<singleTask> allTasks;

bool checkUrgency(const std::time_t& locDueDate);
void printMatrix(const std::vector<singleTask>& locAllTasks);
std::time_t dateString2Time(const char& locTimeString);

