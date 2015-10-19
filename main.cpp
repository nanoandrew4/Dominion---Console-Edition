/*
 * Time Spent on project: 5hrs
 * Coding - 2hr
 * Graphical Dev. - 1hr
 * Brainstorming + Planning - 2hrs
 *
 *
 */


#include "mainmenu.h"

#include <QApplication>
#include <QScreen>

int screenWidth = 800;
int screenHeight = 400;

mainMenu *menu;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QScreen *screen = QApplication::screens().at(0);
    screenWidth = screen->availableSize().width();
    screenHeight = screen->availableSize().height();

    menu = new mainMenu();

    return a.exec();
}
