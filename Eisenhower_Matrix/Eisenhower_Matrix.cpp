#include "Eisenhower_Matrix.hpp"

//https://stackoverflow.com/a/48512134/14918235 used as is without too much understanding
// just removed the time (to only handle date and moved to year-month-day instead of US date format
std::time_t dateString2Time(const char& locTimeString)
{

    struct tm tm = { 0 }; // Important, initialize all members
    int n = 0;
    sscanf_s(&locTimeString, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    // If scan did not completely succeed or extra junk

    tm.tm_isdst = -1; // Assume local daylight setting per date/time
    tm.tm_mon--;      // Months since January
    // Assume 2 digit year if in the range 2000-2099, else assume year as given
    if (tm.tm_year >= 0 && tm.tm_year < 100) {
        tm.tm_year += 2000;
    }
    tm.tm_year -= 1900; // Years since 1900
    time_t t = mktime(&tm);
    return t;
    //return std::time_t();
}

//simple function which returns true or false
//takes one parameter, which is a reference (&) to variable of type time_t. 
//      since it has a const prefix, it will not be able to modify the referene/variable - there is no reason for it to edit it
bool checkUrgency(const std::time_t& locDueDate)
{
    int timeDiff = 0;
    //difftime is cpp standard function. 
    //it return time in seconds between two dates of type time_t
    //we are saving memory by converting the result to int (we don't need floating point precission)
    timeDiff = int(std::difftime(locDueDate, std::time(nullptr)));
    //check if timeDiff is less than 3 days 259200 in seconds
    if (timeDiff <= 259200)
    {
        //if we are less than 3 days from deadline, return true <-- the task is URGENT!
        return true;
    }
    //if we did not return true above, the execution will reach this line..
    //.. and return false, since the task doesn't seem to be urgent
    return false;
}


void printMatrix(const std::vector<singleTask>& locAllTasks)
{
    //https://theblogrelay.com/wp-content/uploads/2021/03/Eisenhower-Matrix.png
    //define 4 string variables and initiate them with empty strings
    //each one corresponds to one of four cells of Eisenhower matrix
    std::string DoNow = "", Schedule = "", Delegate = "", Delete = "";
    //define new tabulate object
    //  later this object is used and new rows, columns etc are added to it
    tabulate::Table table;
    //iterating over all tasks vector (each element of this vector is a structure
    for (int i = 0; i < size(locAllTasks); ++i)
    {
        //Check if this (number - i) task was marked as important by user
        //If yes, this task will need to go to row 1 (either 'Do now' or 'Schedule' column)
        if (locAllTasks[i].isImportant)
        {
            //Checking if the task if ugent or not
            //if urgent & important >> 'Do now'
            if (checkUrgency(locAllTasks[i].dueDate))
            {
                //here we add something to the string variable for first eisenhower matrix cell
                // if the DoNow variable already had previous task we will add to it:
                //  > add number of the task (i), but we increment this value, since people count from 1, not 0
                //  > convert this intiger value to string, since "+" operator will expect such type to concatenate strings
                //  > next " [" string is combined (concatenated) to previous strings
                //  > here we concatenate either " " (space" or "X" marking this task as done or not done
                //  > then we add "] " and current task name is added
                //  > in the end new line character is added
                //THIS REPEATS FOR EACH MATRIX' CELL
                DoNow = DoNow + std::to_string(i + 1) + " [" + locAllTasks[i].isDone + "] " + locAllTasks[i].taskName + "\n";
                //Do now
            }
            else //if !urgent & important >> 'Schedule'
            {
                Schedule = Schedule + std::to_string(i + 1) + " [" + locAllTasks[i].isDone + "] " + locAllTasks[i].taskName + "\n";
                //Schedule
            }
        }//If not important this will be either 'Delegate' or 'Delete' columnt of second row
        //if !important & urgent >> 'Delegate'
        else if (checkUrgency(locAllTasks[i].dueDate))
        {
            Delegate = Delegate + std::to_string(i + 1) + " [" + locAllTasks[i].isDone + "] " + locAllTasks[i].taskName + "\n";
            //Delegate

        }
        else //if !important & !urgent >> 'Delete'
        {
            Delete = Delete + std::to_string(i + 1) + " [" + locAllTasks[i].isDone + "] " + locAllTasks[i].taskName + "\n";
            //Delete
        }
    }


    //First headers row
    table.add_row(
        {
            "Do NOW!",
            "Schedule"
        }
    );
    //First content rows
    table.add_row(
        {
            DoNow,
            Schedule
        }
    );
    //Second row headers row
    table.add_row(
        {
            "Delegate",
            "Delete"
        }
    );
    //Second content rows
    table.add_row(
        {
            Delegate,
            Delete
        }
    );

    table.column(0).format().width(50);
    table.column(1).format().width(50);

    std::cout << table << std::endl;
}






int main() {
    allTasks.push_back(singleTask());
    allTasks.push_back(singleTask());
    allTasks[0].isImportant = true;
    allTasks[1].isImportant = false;
    strcpy_s(allTasks[0].isDone, "X");
    strcpy_s(allTasks[1].isDone, " ");
    allTasks[0].taskName = "dupa1";
    allTasks[1].taskName = "dupa2";
    allTasks[0].dueDate = dateString2Time(*"2022-06-20"); //not urgent >> more than 3 days ahead
    allTasks[1].dueDate = dateString2Time(*"2022-06-13"); //urgent >> less than 3 days ahead
    printMatrix(allTasks);

}
