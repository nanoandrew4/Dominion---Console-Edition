#include <iostream>
#include <cstdlib>
#include "time.h"

/*
    This program determines the amount of time passed, in any interval entered. For example, month will increase 1 per x seconds
    where the interval in which the increment occurs is determined by the user manipulation of the interval variable.
*/

using namespace std;

int monthStore = 0;
int monthCount = 1;

int currTime = 0;
int interval = 1;
int secsUntilIncrease = 0;
int previousInterval = interval;
int startTime = time(0) - interval;
int x = 0;

int main ()
{
    while(true){
        if(x >= 5){interval = 5;} // after the loop runs 5 times, change interval between months from 1 to 5
        if(x >= 15){interval = 3;} // after loop running 7 times, change interval between months from 5 to 3
        if(x >= 24){x = 0; interval = 1;}
        if(previousInterval != interval){secsUntilIncrease = 0; monthStore += monthCount; monthCount = 1; startTime = time(0) -interval;} // will reset the values and add the month to the month tracker before changing interval to prevent catastrophic transitions

        //while(monthCount *interval >= secsUntilIncrease){secsUntilIncrease+=interval;}

        currTime = time(0);
        // += used to ensure handoff from one interval to the next
        monthCount = ((currTime-startTime) / interval); // always has to be 1 for every interval passed

        cout << "monthCount + monthStore is: " << monthStore + monthCount << endl;
        cout << "monthStore is: " << monthStore << endl;
        //cout << "secsUntilIncrease is: " << secsUntilIncrease << endl;
        cout << "currtime is: " << currTime-startTime << endl << endl;
        system("sleep 1"); // pauses for 1, not WIN compatible
        previousInterval = interval; // used to track interval changes

        x++;

        }
}
