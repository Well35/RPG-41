//Put your name(s) here: Joseph Dermer, Alexis Leos, Harrison Stokes
//What bullet points did you do: 4, 5
#include "map.h"
#include "Combat.h"
#include "Actors.h"
#include <cstdlib> // For rand()
#include <ctime>  // For srand()
#include <unistd.h>

const int MAX_FPS = 90; //Cap frame rate 
const unsigned int TIMEOUT = 10; //Milliseconds to wait for a getch to finish
const int UP = 65; //Key code for up arrow
const int DOWN = 66;
const int LEFT = 68;
const int RIGHT = 67;


//Turns on full screen text mode
void turn_on_ncurses() {
	initscr();//Start curses mode
	start_color(); //Enable Colors if possible
	init_pair(1,COLOR_WHITE,COLOR_BLACK); //Set up some color pairs
	init_pair(2,COLOR_CYAN,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	init_pair(5,COLOR_RED,COLOR_BLACK);
	init_pair(6,COLOR_MAGENTA,COLOR_BLACK);
	clear();
	noecho();
	cbreak();
	timeout(TIMEOUT); //Set a max delay for key entry
}

//Exit full screen mode - also do this if you ever want to use cout or gtest or something
void turn_off_ncurses() {
	clear();
	endwin(); // End curses mode
	if (system("clear")) {}
}

void gameOver(int money) {
	if(300 <= money) cout << "\n\n Congratulations, you WIN!\n\n";
	else cout << "\n\n !!YOU LOST!! \n\n";
}

void characterCreation(vector<Hero>& party) {
    for(int i = 0; i < 4; ++i) {
    cout << "Enter your character's name:\n > ";
    string charName = "N/A";
    cin >> charName;
    if(charName == "q" || charName == "Q") break;

    cout << "Enter your class:\n";
    cout << "1 - Mechanic\n";
    cout << "2 - Netrunnner\n";
    cout << "3 - Mech\n";
    cout << "4 - Wizard\n"; // TODO
    cout << "--------------\n > ";
    int classType = 0;
    cin >> classType;

    Hero tempHero(charName, classType);
    party.push_back(tempHero);
    }
}

void team(vector<Hero>& party, int& money) {
    turn_off_ncurses();
    for(int i = 0; i < party.size(); ++i) {
        cout << party.at(i).get_name() << endl;
        cout << "--------------------" << endl;
        cout << "Class: " << party.at(i).get_classtype() << endl;
        cout << "HP: " << party.at(i).get_hp() << endl;
        cout << "ATK: " << party.at(i).get_atk() << endl;
        cout << "SPD: " << party.at(i).get_spd() << endl;
        cout << endl;
    }
    cout << "Money: $" << money << endl;
    cout << "\nEnter 'q' or 'Q' to exit\n > ";
    char input = 'a';
    cin >> input;

    turn_on_ncurses();
}

int main() {
	srand(time(0)); // Prevent rand from generating the same number
	// Current Data Veriables
	vector<Hero> party;		// Holds all current party members
	int money = 0;			// Holds current amount of money
	
	characterCreation(party);
	
	Combat c;
	
	
	turn_on_ncurses(); //DON'T DO CIN or COUT WHEN NCURSES MODE IS ON
	Map map;
	int x = Map::SIZE / 2, y = Map::SIZE / 2; //Start in middle of the world
	int old_x = -1, old_y = -1;
	while (true) {
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (ch == 'q' || ch == 'Q') break;
		else if(ch == 's' || ch == 'S') map.save(party,money,x,y);
		else if(ch == 'r' || ch == 'r') map.reload(party,money,x,y);
        	else if(ch == 't' || ch == 'T') {
          		team(party,money);
         		map.draw(x,y);
         		mvprintw(Map::DISPLAY+1,0,"X: %i Y: %i\n",x,y);
            		mvprintw(Map::DISPLAY+2,0,"Money: $%i\n",money);
        	 	refresh();
	        }
		else if (ch == RIGHT) {
			x++;
			if (x >= Map::SIZE) x = Map::SIZE - 1; //Clamp value
		}
		else if (ch == LEFT) {
			x--;
			if (x < 0) x = 0;
		}
		else if (ch == UP) {
			y--;
			if (y < 0) y = 0;
		}
		else if (ch == DOWN) {
			y++;
			if (y >= Map::SIZE) y = Map::SIZE - 1; //Clamp value
		}
		else if (ch == ERR) { //No keystroke
			; //Do nothing
		}
		//Stop flickering by only redrawing on a change
		if (x != old_x or y != old_y) {
            		//clear(); //Put this in if the screen is getting corrupted
           		if(map.get(x,y)==Map::TREASURE) {           // Picksup treasure
         			money += (rand()%100)+1;
         		        map.set(x,y,'.');
        		     } else if(map.get(x,y) == Map::WALL) {      // Wall collision
          		     	x = old_x;
           		        y = old_y;
         		     } else if(map.get(x,y) == Map::WATER) {     // Water collision
         		        x = old_x;
       		         	y = old_y;
       		             } else if(map.get(x,y) == Map::MONSTER) {  // TODO!
        		        ;                                       // Intiate combat sequence
         		       map.set(x,y,'.');
         		     }
			map.draw(x,y);
			mvprintw(Map::DISPLAY+1,0,"X: %i Y: %i\n",x,y);
			mvprintw(Map::DISPLAY+2,0,"Money: $%i\n",money);
			refresh();
			map.set(Map::SIZE/2,Map::SIZE/2,'.');
		}
		old_x = x;
		old_y = y;
		usleep(1'000'000/MAX_FPS);
		if(300 <= money) break;			// Check whatever the win condition is
	}
	turn_off_ncurses();
	gameOver(money);				// Trigger gameOver sequence and takes in the variable that holds the win condition
}
