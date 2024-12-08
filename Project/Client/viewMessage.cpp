#include "viewMessage.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "viewMessage.h"


using namespace std;

string constructViewMessage(const string filterFlag, const string name){
    int nameLength = name.length();
    if(name == "NO_FILTER")
        return "1 0";

    return (
        "1 "
        + filterFlag
        + " "
        + to_string(nameLength)
        + " |||| "
        + name
    );

}