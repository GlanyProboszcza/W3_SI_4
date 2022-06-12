/*
#include <iostream>
#include <ctime>
#include <vector>
#include "TextTable.h"

//#include <bits/stdc++.h>

int main()
{
    std::cout << "Hello World!\n";
}
*/
#include <iostream>

#define TEXTTABLE_ENCODE_MULTIBYTE_STRINGS
#define TEXTTABLE_USE_EN_US_UTF8

//#include "TextTable.h"
#include <tabulate/table.hpp>



using namespace tabulate;

int main() {

    Table table;

    table.add_row({ "This paragraph contains a veryveryveryveryveryverylong word. The long word will "
                   "break and word wrap to the next line.",
                   "This paragraph \nhas embedded '\\n' \ncharacters and\n will break\n exactly "
                   "where\n you want it\n to\n break." });

    table[0][0].format().width(20);
    table[0][1].format().width(50);

    std::cout << table << std::endl;
}



//a tego też będziemy chyba używać

/*
#include <iostream>
#include <ctime>
#include <vector>
#include "TextTable.h"

//#include <bits/stdc++.h>

int main()
{
    std::cout << "Hello World!\n";
}
*/
