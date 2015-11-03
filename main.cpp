#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <time.h> // for time()
#include <math.h> // for abs()

/*
    Known Bugs:
    - When changing intervals, you go back or forward in time (years) - HAS NO EFFECT ON GAMEPLAY, except for making you feel weird
    - Months dont change all at once, but only once the screen is refreshed (running the whole loop) - bigger effect on gameplay than one above, but not significant,
    only means more chances of random events and more changes in happiness. Will try to counter balance - Counter balanced, random events and changes in
    happiness only happen once a month - Completely solved, tweaked formula for month calculation

*/

using namespace std;

int mmChoice = 0;
int overviewOpt = 0;

void gameFunc(); // function in charge of the game
void randomEvents(); // function in charge of random events
void manualDisp(); // function in charge of displaying the manual
void buildingControl(); // function in charge of building and demolishing buildings
void creditsAndInfo(); // function in charge of displaying the credits
void pause(int interval); // function used to pause the game for interval seconds
void gameMap(); // function used to display the village map
void editVars(); // function used to edit game variables and start game (cheats)

int population = 0;
int initialPopulation = 0;
int randPopulationIncrese = 0;

int month = 1;
int monthLastPass = 0;
int monthStore = 0;
int year = -306;
string monthName;
int semesterTaxCount = 6;
int taxLevel = 10;

int randomEventChance = 0;
int randomEventEndMonth = 0;
int randEffect = 0;
int destroyBuilding = 0;
bool randEventOccuring = false;

const int VILLAGESIZE1 = 11;
const int VILLAGESIZE2 = 36;
const int VILLAGESIZE3 = 61;
int actualVillageSize = VILLAGESIZE1;
int villageSlotsAmount[VILLAGESIZE3] = {1,0,0,0,0,0,0,0,0,0,0}; // first one will be town center, maybe array could be larger but limit is set as to slot selection
int villagePopulationPerHouse[VILLAGESIZE3]; // used to keep track of how many people live in each house
int villageSlotSelect = 0;
int buildingSelection;
int fountainCount = 0;
int templeCount = 0;
int granaryCount = 0;

bool diseaseEvent = false;
bool plagueEvent = false;
bool huracaneEvent = false;
bool animalAttackEvent = false;
bool rebellionEvent = false;
bool wrathOfTheGods = false;
int buildingsLost = 0;
int villagersLost = 0;
int wheatLost = 0;
int moneyLost = 0;
int happinessLost = 0;

string buildingType[VILLAGESIZE3] = {"Village Forum", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "};
int overviewMenuOptions = 8;
bool cityWall = false;
char cityWallPipe = ' ';
char cityWallDash = ' ';

char upgradeBuilding = 'n';
char confirm = 'n';

bool victory = false;
bool defeat = false;
bool assasined = false;

int startTime = 0;
int currTime = 0;
int pauseTempStop = 0;

int money = 1000;
double happiness = 0;
int wheat = 0;
int randEffectLoss = 0;

int secsPerMonthControl = 3; // initial interval in seconds between months
int secsPerMonthPrevious = 0; // records previous interval to see if it changed

int buildingCost[6] = {1000, 100, 400, 250, 350, 600}; //#1 is Village Forum, #2 is House, #3 is Granary, #4 is Fountain, #5 is Wheat Field, #6 is Temple

/*
Building codes are:
1 - Village Forum
101 - House
201 - Granary
301 - Fountain
401 - Wheat field
501 - Temple
*/

int main()
{
    system("clear");

    cout << "Welcome to Dominion. The expansion of Rome has just begun and you have been \ncommanded to build a village in the name and glory of Rome. You will build it \nfrom the"
    " ground up, and will face adversities, as you try to control peoples \nhappiness while continuing to grow your village. Times might get tough, but if \nyou make it through, "
    "you will be succesful and remembered forever!"  << endl << endl;

    cout << "1. Begin" << endl;
    cout << "2. Manual" << endl;
    cout << "3. Credits & additional info" << endl;
    cout << "4. Edit game variables and play" << endl;
    cout << "0. Exit" << endl;

    cin >> mmChoice;
    while(!cin.good() || mmChoice < 0 || mmChoice > 4){
        if(!cin.good()){
            cin.clear();
            cin.ignore(128, '\n');
        }
        cout << "Input is invalid, please select one of the above options" << endl;
        cin >> mmChoice;
    }

    system("clear");

    if(mmChoice == 1){gameFunc();} // if the main menu choice is 1, call the gameFunc function which is in charge of the game in general
    else if(mmChoice == 2){manualDisp();} // if the main menu choice is 2, call the manualDisp function, which displays the game manual in the console
    else if(mmChoice == 3){creditsAndInfo();} // if the main menu choice is 3, diplay the credits
    else if(mmChoice == 4){editVars();} // if the main menu choice is 4, call the editVars function which will let you modify game variables
    else if(mmChoice == 0){cout << "Thank you for playing!" << endl; exit(0);} // if the main menu option is 0, exit the game

    return 0;
}

void gameFunc()
{
    for(int x = 0; x < VILLAGESIZE3; x++){
        villagePopulationPerHouse[x] = initialPopulation; // set all the values in the villagePopulationPerHouse array to the initialPopulation value (so the population can be modified)
    }

    srand(time(0)); // seed with time

    startTime = time(0) - 5; // set start time to the actual time -5

    while(!victory && !defeat) // controls when the game has ended using the victory and defeat booleans
    {
        if(month > 6 && monthLastPass != month){randomEvents();} // after the 6 month grace period, random event will start occuring

        population = 0; // reset population to 0 for the calculation of population to work
        randPopulationIncrese = rand() % 5 + 1;
        for(int x = 0; x < VILLAGESIZE3; x++){
            population += villagePopulationPerHouse[x]; // adds x to the population, depending on how many villagers live on each slot (houses required for the count to work)
        }

        // sets monthName string to a determined name by using the monthStore and month variables together to determine time passed
        if((monthStore+month) %12 == 1){monthName = "January";}
        else if((monthStore+month) %12 == 2){monthName = "February";}
        else if((monthStore+month) %12 == 3){monthName = "March";}
        else if((monthStore+month) %12 == 4){monthName = "April";}
        else if((monthStore+month) %12 == 5){monthName = "May";}
        else if((monthStore+month) %12 == 6){monthName = "June";}
        else if((monthStore+month) %12 == 7){monthName = "July";}
        else if((monthStore+month) %12 == 8){monthName = "August";}
        else if((monthStore+month) %12 == 9){monthName = "September";}
        else if((monthStore+month) %12 == 10){monthName = "October";}
        else if((monthStore+month) %12 == 11){monthName = "November";}
        else if((monthStore+month) %12 == 0){monthName = "December";}

        system("clear");

        if(plagueEvent || diseaseEvent || huracaneEvent || animalAttackEvent || rebellionEvent || wrathOfTheGods){ // depening on if an event is taking place, will print custom text depening on what event is occuring
            if(plagueEvent){cout << "A plague has spread through your village, you have temporarily lost " << wheatLost << " wheat" << endl;}
            if(diseaseEvent){cout << "A disease has spread through your village, which has killed " << villagersLost << " villagers" << endl;}
            if(huracaneEvent){cout << "A huracane has trampled your village, destroying " << buildingsLost << " building(s)" << endl;}
            if(animalAttackEvent){cout << "A wild animal has entered your village and killed " << villagersLost << " villagers. People are now discontent." << endl;}
            if(rebellionEvent){cout << "Your villagers have revolted. The rebels have murdered " << villagersLost << " villagers and destroyed" << buildingsLost << " buildings. " << wheatLost << " wheat and " << happinessLost << " happiness have been temporarily lost" << endl;pause(2);}
            if(wrathOfTheGods){cout << "The gods have waged their wrath upon your village, destroying " << buildingsLost << " buildings, \nkilling " << villagersLost << " villagers and decreasing your denarii by " << moneyLost << ".\n\n" << wheatLost << " wheat and " << happinessLost << " happiness have temporarily been lost\n\nWatch out, unworthy human, or it will happen again..." << endl; pause(4);}
            plagueEvent = false;
            diseaseEvent = false;
            huracaneEvent = false;
            animalAttackEvent = false;
            rebellionEvent = false;
            wrathOfTheGods = false;

            buildingsLost = 0;
            villagersLost = 0;
            wheatLost = 0;
            moneyLost = 0;
            happinessLost = 0;

            randomEventEndMonth = month+6; // six month cooldown after each event
            randEventOccuring = true; // used to prevent more events from happening at once in combination with randomEventEndMonth
            pause(3);
        }
        if(randomEventEndMonth < month && randomEventEndMonth != 0){ // once six month cooldown over, make random events posible
            randEventOccuring = false;
            randEffectLoss = 0;
            randomEventEndMonth = 0;
        }

        wheat = -randEffectLoss; // wheat set to negative randEffectLoss (which is the loss determined by a random event). If no random event is taking place, it is 0
        for(int x = 0; x < actualVillageSize; x++){
            if(villageSlotsAmount[x] == 401){wheat+=10;} //counts Wheat Fields, to determine wheat production
        }

        system("clear");

        if(year < 0){cout << monthName << " " << abs(year) << "BC" << endl;} // if year is less than 0, print the month, year and BC (Before Christ)
        if(year >= 0){cout << monthName << " " << abs(year) << "AC" << endl;} // if year is more than 0, print month, year and AC (After Christ)

        //print village stats
        cout << endl << "Overview of your village" << endl << endl;
        cout << "The population in the village is: " << population << endl;
        cout << "You have " << money << " denarii" << endl;
        cout << "Your wheat surplus is: " << wheat - population << endl;

        if(population != 0 && wheat != 0){cout << "Population to wheat ratio (try to keep below 1 and above 0): " << double(population) / double(wheat) << endl;}
        else{cout << "Population to wheat ratio (try to keep below 1): 0"  << endl;}

        if(fountainCount != 0)cout << "The level of happiness is " << happiness << endl << endl;
        else{cout << "The level of happiness is 0" << endl << endl;}

        if(cityWall && overviewMenuOptions != 7){overviewMenuOptions--;} // will set the overviewMenuOptions to 7 if the city wall has been built

        // prints options
        cout << "1. Build a building" << endl;
        cout << "2. Demolish a building" << endl;
        cout << "3. View map" << endl;
        cout << "4. View village stats" << endl;
        cout << "5. Reload screen" << endl;
        cout << "6. Change game speed" << endl;
        if(!cityWall){cout << "7. Build city wall, protects from animal attacks" << endl;}
        if(actualVillageSize != VILLAGESIZE3){cout << overviewMenuOptions << ". Upgrade village size" << endl;}
        cout << endl;

        do{
            cin >> overviewOpt;
            if(!cin.good()){
                cin.clear();
                cin.ignore(128, '\n');
            }
            if(!cin.good() || (overviewOpt < 0 || overviewOpt > overviewMenuOptions)){cout << "Input invalid, please enter a valid number" << endl;}
            }while(!cin.good() || (overviewOpt < 0 || overviewOpt > overviewMenuOptions));

        if(overviewOpt == 1 || overviewOpt == 2){buildingControl();} // runs the buildingControl function where all building matters are decided, then continues this function (once the other one has ended)

        else if(overviewOpt == 3){gameMap();} // calls the map to be drawn

        else if(overviewOpt == 4){
            int villageChoice = 0;
            system("clear");
            cout << "Tax revenue per semester is: " << population *taxLevel << endl;
            cout << "Current money in treasury is: " << money << endl << endl;

            do{
                // prints village options
                cout << "1. Change percent of taxation" << endl;
                cout << "2. List villagers per slot" << endl;
                cout << "3. List empty slots" << endl;
                cout << "0. Go back" << endl;
                cin >> villageChoice;
                if(!cin.good()){
                    cin.clear();
                    cin.ignore(128, '\n');
                }

            }while(!cin.good() || villageChoice < 0 || villageChoice > 3); // if input is invalid, error trap will rerun the loop
            if(villageChoice == 1){ // change taxes on citizens (1-100)
                do{ // error trap
                    cout << "Enter new percent of tax: ";
                    cin >> taxLevel;
                    if(!cin.good()){
                        cin.clear();
                        cin.ignore(128, '\n');
                    }
                    if(!cin.good() || taxLevel < 1 || taxLevel > 100){cout << "Input invalid, please choose an integer between 1 and 100" << endl;}
                }while(!cin.good() || taxLevel < 1 || taxLevel > 100);
            }
            else if(villageChoice == 2){ // prints villagers per house (if there is a house on the slot)
                for(int x = 0; x < actualVillageSize; x++){
                    if(villageSlotsAmount[x] == 101){cout << "There are " << villagePopulationPerHouse[x] << " villagers living in the House in slot " << x << endl;}
                }
                pause(3);
            }
            else if(villageChoice == 3){ // print empty slots (with value 0) using the villageSlotsAmount array
                system("clear");
                for(int x = 0; x < actualVillageSize; x++){
                    if(villageSlotsAmount[x] == 0){cout << "Slot " << x << " is empty" << endl;}
                }
                pause(3);
            }
        }

        else if(overviewOpt == 6){ // changes the interval between months, is explained in the sample program provided in Docs folder
            do{
                system("clear");
                cout << "Choose speed" << endl << endl;
                cout << "1. 1 second/month" << endl;
                cout << "3. 3 seconds per month" << endl;
                cout << "5. 5 seconds per month" << endl;
                cout << "7. Pause game (1x10^10 seconds per month)" << endl << endl;
                cin >> secsPerMonthControl;
                if(!cin.good()){
                    cin.clear();
                    cin.ignore(128, '\n');
                }
            }while((secsPerMonthControl != 1 && secsPerMonthControl != 3 && secsPerMonthControl != 5 && secsPerMonthControl != 7) || !cin.good());
            if(secsPerMonthControl == 7){secsPerMonthControl = pow(10, 9);}
        }

        else if(overviewOpt == 7 && !cityWall){ // builds city wall, if funds available
            if(money > 2500){cityWall = true; money -= 2500;}
            else{cout << "You do not have enough money to build the wall, you are missing " << 2500 - money << " denarii" << endl; pause(2);}
        }

        else if(overviewOpt == overviewMenuOptions){ // manages village upgrades,
            if(actualVillageSize == VILLAGESIZE2 && money > 5000){ // upgrade village to lvl 3
                if(money > 5000){
                    for(int x = actualVillageSize; x < VILLAGESIZE3; x++){
                        villageSlotsAmount[x] = 0; // sets value of all new slots to 0
                        buildingType[x] = " "; // empty building type
                    }
                    actualVillageSize = VILLAGESIZE3; // actual village size updated to new size
                    money -= 5000;
                }
                else{cout << "You do not have enough denarii to upgrade. Denarii missing: " << 5000-money << endl; pause(3);}
            }
            if(actualVillageSize == VILLAGESIZE1){ // upgrade village to lvl 2
                if(money > 2000){
                    for(int x = actualVillageSize; x  < VILLAGESIZE2; x++){
                        villageSlotsAmount[x] = 0; // sets value of all new slots to 0
                        buildingType[x] = " "; // empty building type
                    }
                    actualVillageSize = VILLAGESIZE2; // actual village size updated to new size
                    money -= 2000;
                }
                else{cout << "You do not have enough denarii to upgrade. Denarii missing: " << 2000-money << endl; pause(3);}
            }
        }

        while(month + monthStore >= semesterTaxCount){ // collects taxes every 6 months
            semesterTaxCount+=6;
            money += population * taxLevel;
        }

        // for the calculation of months passed, refer to either the sample program provdided in the Docs folder explaining how it works, if you need some explaining
        if(secsPerMonthPrevious != secsPerMonthControl){monthStore += month; month = 1; startTime = time(0) - secsPerMonthControl;}
        currTime = time(0);
        month = ((currTime-startTime) /secsPerMonthControl);
        secsPerMonthPrevious = secsPerMonthControl;
        // calculation of months passed ends
        year = monthStore + month /12 - 306;

        //game logic follows

        if(month != monthLastPass){
            if(wheat == 0){wheat = 1;} // prevent FPE
            happiness -= double(population) / double(wheat); // happiness depends on the wheat surplus or deficit
            for(int x = 0; x < actualVillageSize; x++){ // fountains increase happiness by 5
                if(villageSlotsAmount[x] == 301){
                    happiness += 3;
                }
                if(villageSlotsAmount[x] == 501){ // temples increase happiness by 15
                    happiness += 10;
                }
            }
            happiness -= (population/25 + taxLevel /10 + randEffectLoss); // happiness also decreases based on quantity of population, tax level and the effect of a random event (if any)

            if(money < 100 && population == 0){defeat = true;}
            if(population > 150 && happiness > 100 && actualVillageSize == VILLAGESIZE3){victory = true;} // declares victory once the objectives are met
        }
        monthLastPass = month;
    }
    // if you win or lose the game, program continues here
    system("clear");

    if(defeat){ // prints text if defeat is true
        if(assasined){cout << "You were assasined by one of your fellow villagers.\nYou lost control of the people and failed to resolve the crisis."
        "\nYour reign lasted " << month << " months" << endl;} // prints this if you were assasined during a rebellion
        else{cout << "You lost the game. You played for " << month << " months and finally, your reign has ended" << endl;}
    }
    if(victory){ // prints text if victory is true
        cout << "Your reign was succesful and your objectives were completed. \nCongratulations! You played for " << month << " months." << endl;
    }
}

void editVars() // used for testing, and can be used as cheats, modifies initial variables
{
    int editVarsOption = -1;
    int setValue = 0;

    while(editVarsOption != 0){
        system("clear");
        cout << "Enter the corresponding number to change the value" << endl << endl;
        cout << "1. Edit money" << endl;
        cout << "2. Edit happiness" << endl;
        cout << "3. Edit tax level" << endl;
        cout << "4. Edit population" << endl;
        cout << "0. Start game" << endl << endl;
        do{
            cin >> editVarsOption;
            if(!cin.good()){
                cin.clear();
                cin.ignore(128, '\n');
            }
        if(!cin.good() || editVarsOption < 0 || editVarsOption > 4){cout << "Please enter a valid number" << endl;}
        }while(!cin.good() || editVarsOption < 0 || editVarsOption > 5);

        if(editVarsOption == 1){cout << "Set money to: "; cin >> setValue; money = setValue;}
        else if(editVarsOption == 2){cout << "Set happiness to: "; cin >> setValue; happiness = setValue;}
        else if(editVarsOption == 3){cout << "Set tax level to: "; cin >> setValue; taxLevel = setValue;}
        else if(editVarsOption == 4){cout << "Set population to (will be multiplied by 61): "; cin >> setValue; initialPopulation = setValue;}
        else if(editVarsOption == 0){gameFunc();}
    }
}

void buildingControl()
{
    switch(overviewOpt){
        case 1: // controls the building of buildings
        {
            do{ // error trap
                cout << "Enter the slot number that you wish to build on" << endl << endl;
                cin >> villageSlotSelect;
                if(!cin.good()){
                    cin.clear();
                    cin.ignore(128, '\n');
                }
            }while(!cin.good() || villageSlotSelect < 0 || villageSlotSelect > actualVillageSize);

            if(villageSlotsAmount[villageSlotSelect] == 0){ // if slot is empty

                do{ // print options, and error trap for any error that might occur
                    cout << "This slot in the village is empty, here are your options to build" << endl << endl;
                    cout << "1. House (100)- Provide housing for villagers, and allows village to grow" << endl;
                    cout << "2. Granary (400)- Provides storage for wheat" << endl;
                    cout << "3. Fountain (250)- Provides water for citizens, as well as higene which increases popularity" << endl;
                    cout << "4. Wheat Field (350)- Provides wheat for your villagers to survive and thrive" << endl;
                    cout << "5. Temple (600) - Provides your villagers with a place to pray, and keeps the gods content with your reign, so long as the people have enough of them" << endl;
                    cout << "0. Back to overview" << endl;

                    cin >> buildingSelection;

                    if(!cin.good()){
                        cin.clear();
                        cin.ignore(128, '\n');
                    }
                if(!cin.good() || buildingSelection < 0 || buildingSelection > 5){cout << "Please enter a valid number" << endl; pause(2); system("clear");}
                }while(!cin.good() || buildingSelection < 0 || buildingSelection > 5);

                if(money < buildingCost[buildingSelection]){cout << "Not enough money" << endl; pause(3); return;} // prints if money for building is insufficient, and returns to top of function
                else{money-=buildingCost[buildingSelection];} // removes the cost of the building selected from your money

                // sets the building code in the specified location inside the villageSlotsAmount array for future reference and counting of different variables. If the house is built, sets the selected slot in villagePopulationPerHouse to the random population increase variable for counting of population at top of gameFunc
                if(buildingSelection == 1){villageSlotsAmount[villageSlotSelect] = 101; villagePopulationPerHouse[villageSlotSelect] = randPopulationIncrese;}
                else if(buildingSelection == 2){villageSlotsAmount[villageSlotSelect] = 201;}
                else if(buildingSelection == 3){villageSlotsAmount[villageSlotSelect] = 301;}
                else if(buildingSelection == 4){villageSlotsAmount[villageSlotSelect] = 401;}
                else if(buildingSelection == 5){villageSlotsAmount[villageSlotSelect] = 501;}

                // sets the name of the building in the location specified by the selected slot in the buildingType array
                if(villageSlotsAmount[villageSlotSelect] == 1){buildingType[villageSlotSelect] = "Village Forum";}
                else if(villageSlotsAmount[villageSlotSelect] == 101){buildingType[villageSlotSelect] = "House";}
                else if(villageSlotsAmount[villageSlotSelect] == 201){buildingType[villageSlotSelect] = "Granary";}
                else if(villageSlotsAmount[villageSlotSelect] == 301){buildingType[villageSlotSelect] = "Fountain";}
                else if(villageSlotsAmount[villageSlotSelect] == 401){buildingType[villageSlotSelect] = "Wheat Field";}
                else if(villageSlotsAmount[villageSlotSelect] == 501){buildingType[villageSlotSelect] = "Temple";}
                return;
                }
                if(villageSlotsAmount[villageSlotSelect] != 0){ // prints if a building exists on the slot
                    cout << "This slot already has a building" << endl;
                    pause(2);
                }
            break;
        }
        case 2:
        { // controls building demolition
            do{ // error trap
                cout << "Enter the slot on which the building you want to demolish is on" << endl;
                cin >> villageSlotSelect;
                if(!cin.good()){
                    cin.clear();
                    cin.ignore(128, '\n');
                }
            }while(!cin.good() || villageSlotSelect < 1 || villageSlotSelect > actualVillageSize);
            if(buildingType[villageSlotSelect] == " "){cout << "There is no building on this slot, nothing to demolish" << endl; pause(2);} // prints if slot is empty
            if(buildingType[villageSlotSelect] != " "){ // asks for confirmation before demolishing building
                cout << "Your " << buildingType[villageSlotSelect] << " will be demolished. Confirm? (Y/N)" << endl;
                do{
                    cin >> confirm;
                    if(!cin.good()){
                        cin.clear();
                        cin.ignore(128, '\n');
                        cout << "Please enter Y or N" << endl;
                    }
                }while(!cin.good() && confirm != 'n' && confirm != 'N' && confirm != 'y' && confirm != 'Y');

                if(confirm == 'Y' || confirm == 'y'){villageSlotsAmount[villageSlotSelect] = 0; villagePopulationPerHouse[villageSlotSelect] = 0; buildingType[villageSlotSelect] = " ";} // resets values in location determined by selected slot to default in buildingType, villageSlotsAmount and villagePopulationPerHouse arrays
                if(confirm == 'N' || confirm == 'n'){return;} // if there is no confirmation, returns to top of gameFunc
            }
            break;
        }
    }
}

void randomEvents()
{

    if(!randEventOccuring){
        randomEventChance = rand() % 40 + 1; // random event variable calculated
        //cout << randomEventChance << endl;

        granaryCount = 0;
        fountainCount = 0; // resets values for fountainCount and templeCount since it is incremental
        templeCount = 0;
        for(int x = 0; x < actualVillageSize; x++){ // counts number of granaries
            if(villageSlotsAmount[x] == 201){granaryCount++;}
        }
        for(int x = 0; x < actualVillageSize; x++){ // counts number of fountains
            if(villageSlotsAmount[x] == 301){fountainCount++;}
        }
        for(int x = 0; x < actualVillageSize; x++){ // counts number of temples
            if(villageSlotsAmount[x] == 501){templeCount++;}
        }
        if(granaryCount == 0){granaryCount = 1;}
        if(fountainCount == 0){fountainCount = 1;} // if fountainCount or templeCount are equal to 0, sets to one to prevent FPE and so random events requiring them can occur
        if(templeCount == 0){templeCount = 1;}

        if(randomEventChance == 1 && double(population) / double(fountainCount) > 50 && population > 20){ // disease event, decreases population in village
            diseaseEvent = true;
            for(int x = 0; x < actualVillageSize; x++){
                randEffect = rand() % 2;
                villagersLost += randEffect;
                if(villagePopulationPerHouse[x] >= randEffect){villagePopulationPerHouse[x] -= randEffect;} // prevents values in the array from going negative
                else{villagePopulationPerHouse[x] = 0;}
            }

        } // disease, leads to citizen deaths, should be preventable with fountains (or at least less likely)

        else if(randomEventChance == 2 && wheat / granaryCount > 50){ // plague event, decrease wheat surplus for 6 months
            randEffect = rand() % 10 +1;
            plagueEvent = true;
            randEffectLoss = wheat / randEffect; // random loss of wheat percent, between 1 (biggest) and 100 (smallest)
            wheatLost = randEffectLoss;

        } // plague, temorarily reduces wheat surplus (less effect with granary)

        else if(randomEventChance == 3 && (month %12 == 3 || month %12 == 4 || month %12 == 9 || month %12 == 10)){ // destroys a building (if hits right slot), decreases wheat surplus
            huracaneEvent = true;
            randEffect = rand() % actualVillageSize + 1; // random slot
            if(villageSlotsAmount[randEffect] != 0){buildingsLost = 1;}
            villageSlotsAmount[randEffect] = 0;
            villagePopulationPerHouse[randEffect] = 0;
            buildingType[randEffect] = " ";
            randEffectLoss = wheat / randEffect;
            wheatLost = randEffectLoss;

        } // huracane, potentially destroys homes and reduces surplus of wheat

        else if(randomEventChance == 5 && month % 12 > 3 && month % 12 < 11 && !cityWall && population > 20){ // animal attack kills villagers, less impact than disease
            animalAttackEvent = true;
            for(int x = 0; x < actualVillageSize /2; x++){ // half of village affected, loses random number of villagers per house
                randEffect = rand() % 2;
                villagersLost += randEffect;
                if(villagePopulationPerHouse[x] >= randEffect){villagePopulationPerHouse[x] -= randEffect;} // prevents values in array from going negative
                else{villagePopulationPerHouse[x] = 0;}
            }
            randEffect = rand() % population +1;
            randEffectLoss = randEffect; // decreases happiness by random value
            happinessLost = randEffectLoss;

        } // animal attack, kills a phew citizens, can be prevented with city wall (if implemented), reduces happiness temporarily

        else if(randomEventChance == 8 && happiness < -10){
            rebellionEvent = true;
            for(int x = 0; x < actualVillageSize /3; x++){ // one third of slots affected, if there is a house on the slot, poulation gets reduced by a random quantity
                randEffect = rand() % 3;
                villagersLost += randEffect;
                if(villagePopulationPerHouse[x] >= randEffect){villagePopulationPerHouse[x] -= randEffect;} // prevents values in array going negative
                else{villagePopulationPerHouse[x] = 0;}
            }
            randEffect = rand() % population +1;
            if(randEffect < population / 3 && randEffect > population / 5){defeat = true; assasined = true;} // if this is true, you lose the game by assasination

            randEffectLoss = randEffect; // happiness decreases by a random amount
            happiness -= randEffectLoss;
            happinessLost = randEffectLoss;

            destroyBuilding = rand() % actualVillageSize + 1; // random number of building destroyed by the rebels

            for(int x = 0; x < destroyBuilding; x++){
                randEffect = rand() % actualVillageSize +1;
                if(villageSlotsAmount[randEffect] != 0){buildingsLost++;}
                villageSlotsAmount[randEffect] = 0;
                villagePopulationPerHouse[randEffect] = 0;
                buildingType[randEffect] = " ";
            }
        }
        if(templeCount > 0 && randomEventChance == 13){ // wrath of the gods event, money, happiness, wheat reduced, building destroyed, villagers killed
            if(population / templeCount > 40){ // only if ratio of population to temples is greater than 40
                wrathOfTheGods = true;

                randEffect = rand() % (population / templeCount) + 1;
                randEffectLoss = randEffect; // happiness reduced
                happiness -= randEffectLoss;
                happinessLost = randEffectLoss;

                destroyBuilding = rand() % actualVillageSize + 1; // random number of buildings destroyed
                for(int x = 0; x < destroyBuilding; x++){
                    randEffect = rand() % actualVillageSize +1;
                    if(villageSlotsAmount[randEffect] != 0){buildingsLost++;}
                    villageSlotsAmount[randEffect] = 0;
                    villagePopulationPerHouse[randEffect] = 0;
                    buildingType[randEffect] = " ";
                }

                for(int x = 0; x < actualVillageSize; x++){ // villagers killed
                randEffect = rand() % 4;
                if(villagePopulationPerHouse[x] >= randEffect){villagePopulationPerHouse[x] -= randEffect; villagersLost += randEffect;}
                else{villagersLost += villagePopulationPerHouse[x]; villagePopulationPerHouse[x] = 0;}
                }

                randEffect = rand() % money + 1; // money reduced by random amount
                money -= randEffect;
                moneyLost = randEffect;

            } // wrath of the gods, decreases happiness, and can have many different effects, from previous random events (risk reduced with temples, if implemented)
        }
    }
}

void manualDisp() // displays game manual
{
    cout << "Welcome to the manual. Here I will explain every aspect of the game. When the game starts, you are given 1000 denarii which is the currency, a village with 11 slots"
    " in which the 1st one is the Village Forum, leaving 10 for construction. The first option of the menu is where you choose what buildings to build or upgrade. Select one of "
    "the 10 slots and choose what you want to build. If you have enough money, the building will be built. The second option in the menu is to demolish a building. Again, "
    "choose what slot you wish to demolish and confirm." << endl;

    pause(5);

    main();
}

void creditsAndInfo() // displays game credits
{

    int lineLimit = 17;
    for(int a = 0; a < 17; a++){
        system("clear");
        for(int x = 0; x < lineLimit; x++){
            cout << endl;
        }
        cout << setw(200) << "Credits:" << endl << endl;
        cout << setw(63) << "Coding and Idea development: Andres Sanchez Smith" << setw(0) << endl;
        cout << "For more of my works and future projects, visit: https://github.com/nanoandrew4" << endl;

        lineLimit--; // decreases the amount of endlines each pass

        pause(1); // calls the pause function for 1 sec, so that after every second the loop runs
    }
    pause(5); // calls the pause function for 5 secs
    main(); // back to main menu
}

void pause(int interval)
{
    pauseTempStop = time(0) + interval; // sets the time until the game resumes
    while(time(0) < pauseTempStop); // waits for whatever the interval is until resuming back where the function was called
}

void gameMap() // prints village map depending on size of village
{
    system("clear");

    if(cityWall){cityWallDash = '-'; cityWallPipe = '|';} // displays borders if the city wall was built

    if(actualVillageSize == VILLAGESIZE1){ // drawn initial village
        cout << endl << endl << endl << endl << endl << endl << endl;

        cout << setw(30);
        for(int x = 0; x < 15; x++){
            cout << cityWallDash;
        }
        cout << endl;
        for(int x = 0; x < 11; x+=2){
            if(x < 10)cout << setw(30) << cityWallPipe << setw(5) << buildingType[x].at(0) << setw(5) << buildingType[x+1].at(0) << setw(4) << cityWallPipe <<endl;
            if(x == 10){x--; cout << setw(30) << cityWallPipe << setw(5) << buildingType[x+1].at(0) << setw(9) << cityWallPipe <<endl;}
        }
        cout << setw(30);
        for(int x = 0; x < 15; x++){
            cout << cityWallDash;
        }
    }
    else if(actualVillageSize == VILLAGESIZE2){ // draws level 2 village
        cout << endl << endl << endl << endl << setw(30);
        for(int x = 0; x < 13; x++){cout << cityWallDash;}cout << endl;
        for(int x = 0; x < 8; x+=2){cout << setw(30) << cityWallPipe << setw(3) << buildingType[x].at(0) << setw(6) << buildingType[x+1].at(0) << setw(3) << cityWallPipe << endl;}
        cout << setw(18);
        for(int x = 0; x < 12; x++){cout << cityWallDash;}
        cout << setw(14);
        for(int x = 0; x < 12; x++){cout << cityWallDash;}
        cout << endl;
        for(int x = 8; x < 28; x+=5){
            cout << setw(18) << cityWallPipe << setw(2) << buildingType[x].at(0) << setw(8) << buildingType[x+1].at(0) << setw(8) << buildingType[x+2].at(0) << setw(8) << buildingType[x+3].at(0) << setw(8) << buildingType[x+4].at(0) << setw(2) << cityWallPipe << endl;
        }
        cout << setw(18);
        for(int x = 0; x < 12; x++){cout << cityWallDash;}
        cout << setw(14);
        for(int x = 0; x < 12; x++){cout << cityWallDash;}
        cout << endl;
        for(int x = 28; x < 36; x+=2){cout << setw(30) << cityWallPipe << setw(3) << buildingType[x].at(0) << setw(6) << buildingType[x+1].at(0) << setw(3) << cityWallPipe << endl;}
        cout << setw(30);
        for(int x = 0; x < 13; x++){cout << cityWallDash;}cout << endl;
    }
    else if(actualVillageSize == VILLAGESIZE3){ // draws level 3 village
        cout << endl << endl << endl << endl << setw(25);
        for(int x = 0; x < 31; x++){cout << cityWallDash;}cout << endl;
        for(int x = 0; x < 15; x+=4){cout << setw(25) << cityWallPipe << setw(3) << buildingType[x].at(0) << setw(8) << buildingType[x+1].at(0)<< setw(8) << buildingType[x+2].at(0) << setw(8) << buildingType[x+3].at(0) << setw(3) << cityWallPipe << endl;}
        cout << setw(14);
        for(int x = 0; x < 12; x++){cout << cityWallDash;}
        cout << setw(30);
        for(int x = 0; x < 12; x++){cout << cityWallDash;}
        cout << endl;
        cout << setw(14) << cityWallPipe << setw(6) << buildingType[15].at(0) << setw(10) << buildingType[16].at(0) << setw(10) << buildingType[17].at(0) << setw(10) << buildingType[18].at(0) << setw(10) << buildingType[19].at(0) << setw(6) << cityWallPipe << endl;
        cout << setw(14) << cityWallPipe << setw(6) << buildingType[20].at(0) << setw(8) << buildingType[21].at(0) << setw(8) << buildingType[22].at(0) << setw(8) << buildingType[23].at(0) << setw(8) << buildingType[24].at(0) << setw(8) << buildingType[25].at(0) << setw(6)<< cityWallPipe << endl;
        cout << setw(14) << cityWallPipe << setw(3) << buildingType[26].at(0) << setw(9) << buildingType[27].at(0) << setw(9) << buildingType[28].at(0) << setw(9) << buildingType[29].at(0) << setw(9) << buildingType[30].at(0) << buildingType[31].at(0) << setw(9) << buildingType[32].at(0) << setw(3) << cityWallPipe << endl;
        cout << setw(14) << cityWallPipe << setw(6) << buildingType[33].at(0) << setw(8) << buildingType[34].at(0) << setw(8) << buildingType[35].at(0) << setw(8) << buildingType[36].at(0) << setw(8) << buildingType[37].at(0) << setw(8) << buildingType[39].at(0) << setw(6) << cityWallPipe << endl;
        cout << setw(14) << cityWallPipe << setw(6) << buildingType[40].at(0) << setw(10) << buildingType[41].at(0) << setw(10) << buildingType[42].at(0) << setw(10) << buildingType[43].at(0) << setw(10) << buildingType[44].at(0) << setw(6) << cityWallPipe << endl;
        cout << setw(14);
        for(int x = 0; x < 12; x++){cout << cityWallDash;}
        cout << setw(30);
        for(int x = 0; x < 12; x++){cout << cityWallDash;}
        cout << endl;
        for(int x = 44; x < 60; x+=4){cout << setw(25) << cityWallPipe << setw(3) << buildingType[x+1].at(0) << setw(8) << buildingType[x+2].at(0)<< setw(8) << buildingType[x+3].at(0) << setw(8) << buildingType[x+4].at(0) << setw(3) << cityWallPipe << endl;}
        cout << setw(25);
        for(int x = 0; x < 31; x++){cout << cityWallDash;}cout << endl;
    }
    cout << endl;

    pause(3);
}
