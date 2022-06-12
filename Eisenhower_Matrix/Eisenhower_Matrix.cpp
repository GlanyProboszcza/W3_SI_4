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

//function for prompting user to provide details of new task
//  will add these details to allTasks vector (at the end of vector)
void addTask(std::vector<singleTask>& locAllTasks)
{
    //push_back adds new element to allTasks vector
    // new element is added as last one
    // this new element is new structure of type 'singleTask'
    // fields in new structure are automatically initialized to what is defined in header file
    allTasks.push_back(singleTask());
    //vector index of this newly added element is needed
    // it is saved to lastItemIndex variable of type short 
    // output of size() function is casted to short type
    // 1 (one) is substracted from the size, since indexes of vectors are counted from 0
    short lastItemIndex = short(size(allTasks)) - 1;
    //temporary character of length 10+'\0' is initialized
    // this will be used to store user's input before converting it to time_t
    char dateTempString[11] = "2069-01-01";
    //temporary character variable of length 1+'\0' to store user's input (Y or N)
    // this will be used to save boolean variable in task's structure (isImportant to true or false)
    char isTaskImportant[2] = "N";
    std::cout << "Provide task name: ";
    //Directly saving user's input to task name field 
    //                  in singeTask structure in newly added element of allTasks vector
    std::cin.ignore();
    //std::cin >> allTasks[lastItemIndex].taskName; //this method was using spaces as separators of input. Did not work correctly
    std::getline(std::cin, allTasks[lastItemIndex].taskName);
    std::cout << std::endl << "Provide due date, format is: yyyy-mm-dd: ";
    //Saving user's input to temporary string, to be later converted to time_t
    std::cin >> dateTempString;
    //Using dateString2Time function to convert character vector to time_t
    // converted value (in type of time_t) is returned by dateString2Time function
    // returned value is stored in dueDate field 
    //                  of singleTask structure of newly created element of allTasks vector
    allTasks[lastItemIndex].dueDate = dateString2Time(*dateTempString);
    std::cout << "Is this task important? Y or N: ";
    //saving user's input to temporary character variable
    std::cin >> isTaskImportant;
    //using strcmp function to compare user's input to Y and y
    // strcmp will return interger 0 (zero) if strings are exactly the same
    // this is why output of strcmp is compared to 0 (zero)
    // both lower and upper case is handled by || (OR) 
    if (strcmp(isTaskImportant, "Y") == 0 || strcmp(isTaskImportant, "y") == 0)
    {
        //since expression in if evaluated to true, 
        // the isImportant element of structure 'singleTask' in newly added allTasks vector
        //                  is set to value true
        allTasks[lastItemIndex].isImportant = true;
    }
    else
    {
        //otherwise above boolean element of structure for this new task
        // is set as false (not important task)
        allTasks[lastItemIndex].isImportant = false;
    }
    //NOTE ABOUT URGENCY OF TASKS:
    // Since urgency of task is determined based on current date,
    // it is not required to save any information on tasks' urgency.
    // Each taks' urgency will be evaluated during listing of the matrix
    // in the printMatrix function. 
    // Based on the checkUrgency() function's output task will be displayed in first or second column of matrix

}

//function for changing specific task's status
// 
void changeTaskStatus(std::vector<singleTask>& locAllTasks, short taskToChange)
{
    //
    if (strcmp(locAllTasks[taskToChange].isDone, " ") == 0)
    {
        strcpy_s(allTasks[taskToChange].isDone, "X");
    }
    else
    {
        strcpy_s(allTasks[taskToChange].isDone, " ");
    }
}

short mainMenu(void)
{
    char userChoice;
    std::cout << "\nWhat do You want to do? \n1. Display TO DO board \n2. Add new task\n3. Change task state "
        "\n4. Delete task \n5. Archive and save tasks\n6. Save tasks and EXIT\n>> " << std::endl;
    std::cin >> userChoice;
    return userChoice;
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
    std::cout << "\nCurrent tasks list:" << std::endl;
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
    std::cout << "+------------------------------------+\n" << std::endl;
    std::cout << "+- Welcome to the Eisenhower Matrix -+\n" << std::endl;
    std::cout << "+------------------------------------+\n" << std::endl;

    //creating test task entries
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
    //end creating test task entries

    printMatrix(allTasks);
    char userChoice;
    while (true)
    {
        userChoice = mainMenu();
        std::cout << userChoice << std::endl;
        char taskNumber = 0;
        switch (userChoice)
        {
        case '1': printMatrix(allTasks);
            break;
        case '2': addTask(allTasks);
            break;
        case '3':
            taskNumber = 0;
            printMatrix(allTasks);
            std::cout << "Which task number you want to change?: ";
            std::cin >> taskNumber;
            changeTaskStatus(allTasks, taskNumber - 48 - 1);
            std::cout << "Done!" << std::endl;
            printMatrix(allTasks);
            break;
        case '4':
            taskNumber = 0;
            printMatrix(allTasks);
            std::cout << "Which task number you want to remove?: ";
            std::cin >> taskNumber;
            std::cout << "Delete not supported yet!" << std::endl;
            printMatrix(allTasks);
            break;
        case '5':
            return 0;
            break;
        case '6':
            return 0;
            break;
        default: return 0;
        }



    }
    changeTaskStatus(allTasks, 0);
    changeTaskStatus(allTasks, 1);
    printMatrix(allTasks);
    addTask(allTasks);
    /*
    while (true)
    {
        addTask(allTasks);
        printMatrix(allTasks);
    }
    */


}
