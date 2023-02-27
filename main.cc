//Put your name(s) here: Joseph Dermer, Alexis Leos, Harrison Stokes
//What bullet points did you do: 4, 5
#include "map.h"
#include "Combat.h"
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

void save_seq(vector<Map>& mapSaves, vector<vector<Hero>>& heroSaves, vector<int>* moneySaves, Map& map, vector<Hero>& party, int& money) {
	turn_off_ncurses();
	cout << "Enter the number to add to save file:\n";
	for(int i = mapSaves.size(); 0 < i; --i) {
		cout << "Save " << i << endl;
	}
	cout << "New Save 0\n";
	cout << "--------------\n > ";
	int input = 0;
	cin >> input;
	if(input == 0) {
		Map temp;
		map.save(temp); 	// Copies current map onto a temporary map
		mapSaves.push_back(temp); // Stores the temporary map in mapSaves
		heroSaves.push_back(party); // Saves current party
		moneySaves.push_back(money); // Saves current money
		cout << "New save created!" << endl;
	} else if(input-1 <= mapSave.size()) {
		map.save(mapSave.at(input-1));	// Copies current map onto to the selected map in mapSave
		for(int i  = 0; i < heroSave.at(input-1).size(); ++i) {
			heroSaves.at(input-1).at(i) = party.at(i);	// Copies over the saved party data with the current party
		}
		moneySaves.at(input-1) = money;	// Copies over the saved money data with current money
		cout << "Save overrided!" << endl;
	} else {
		cout << "Saves does not exist!\n";	// In case the user puts in a bad input
	}
	usleep(50'000'000);		// Stalls the screen clearing so user can read the messages
	turn_on_ncurses();
}

void reload_seq(vector<Map>& mapSaves, vector<vector<Hero>>& heroSaves, vector<int>* moneySaves, Map& map, vector<Hero>& party, int& money) {
	turn_off_ncurses();
	cout << "Enter the number for the save you want to load:\n";
    	if(saves.size() != 0) {
    	    for(int i = saves.size(); i > 0; --i) {
        	    cout << "Save " << i << endl;
        	}
    	}
   	 cout << "--------------\n > ";
   	 int input = 0;
   	 cin >> input;
   	 if(input <= mapSaves.size()){
   	     map.reload(mapSaves.at(input-1));
   	     for(int i = 0; i < heroSaves.at(input-1).size(); i++) {
   	         party.at(i) = heroSaves.at(input-1).at(i);
       	 }
       	 money = moneySave.at(input-1);
	 cout << "Load complete!" << endl;
    	} else {
    	    cout << "Save does not exist";
    	}
    	usleep(50'000'000/MAX_FPS);
    	turn_on_ncurses();
}

void gameOver(int money) {
	if(300 <= money) cout << "\n\n Congratulations, you WIN!\n\n";
	else cout << "\n\n !!YOU LOST!! \n\n";
}

int main() {
	srand(time(0)) // Prevent rand from generating the same numbers
	// Saved Data Variables
	vector<Map> mapSaves;		// Holds all map saves here
	vector<vector<Hero>> heroSaves; // Hold all hero saves here
	vector<int> moneySaves;		// Hold all money saves here
	
	// Current Data Veriables
	vector<Hero> perty;		// Holds all current party members
	int money = 0;			// Holds current amount of money
	
	Combat c;
	
	
	turn_on_ncurses(); //DON'T DO CIN or COUT WHEN NCURSES MODE IS ON
	Map map;
	int x = Map::SIZE / 2, y = Map::SIZE / 2; //Start in middle of the world
	int old_x = -1, old_y = -1;
	while (true) {
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (ch == 'q' || ch == 'Q') break;
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
