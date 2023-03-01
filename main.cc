//Put your name(s) here: Joseph Dermer, Alexis Leos, Harrison Stokes
//What bullet points did you do: 4, 5
#include "map.h"
#include "Combat.h"
#include "Actors.h"
#include <ncurses.h>
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

void winCondition(bool& win, Map map) {
	win = true;
	for (int row = 0; row < Map::SIZE; ++row) {
		for (int col = 0; col < Map::SIZE; ++col) {
			if (map.get(col, row) == 'M') {
				win = false;
				break;
			}
		}
	}
}



void gameOver(bool win) {
	if (win) cout << "\n\n Congratulations, you WIN!\n\n";
	else cout << "\n\n !!YOU LOST!! \n\n";
}

void characterCreation(vector<Hero>& party) {
	for (int i = 0; i < 4; ++i) {
		cout << "Enter your character's name:\n > ";
		string charName = "N/A";
		cin >> charName;
		if (charName == "q" || charName == "Q") break;

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
	for (int i = 0; i < party.size(); ++i) {
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

void upgrades(vector<Hero>& party, int& money) {
	turn_off_ncurses();
	while (true) {
		cout << "Enter the player to upgrade:\n > ";
		string input = "a";
		cin >> input;
		bool playerCheck = false;
		int player = 0;
		for (int i  = 0; i < party.size(); ++i) {
			if (input == party.at(i).get_name()) {
				playerCheck = true;
				player = i;
			}
		}
		if (!playerCheck) break;
		while (true) {
			cout << "\nUpgrades:\n-------------\n";
			cout << "1 - +2 hp : Cost: $100\n";
			cout << "2 - +2 atk : Cost: $100\n";
			cout << "3 - +2 spd : Cost : $100\n";
			cout << "0 - exit\n";
			cout << "--------------------------\n  > ";
			int upgrade = 0;
			cin >> upgrade;
			if (upgrade == 0) {
				break;
			} else if (upgrade == 1 && money > 99) {
				party.at(player).set_hp(party.at(player).get_hp() + 2);
				money -= 100;
				cout << "New HP: " << party.at(player).get_hp() << endl;
			} else if (upgrade == 2 && money > 99) {
				party.at(player).set_atk(party.at(player).get_atk() + 2);
				money -= 100;
				cout << "New ATK: " << party.at(player).get_atk() << endl;
			} else if (upgrade == 3 && money > 99) {
				party.at(player).set_spd(party.at(player).get_spd() + 2);
				money -= 100;
				cout << "New SPD: " << party.at(player).get_spd() << endl;
			} else if (money < 100)  {
				cout << "Insufficient funds!" << endl;
			} else {
				cout << "Invalid Option!" << endl;
			}
		}
		cout << "Do you want to upgrade another character? <y/n>\n > ";
		char exit = 'q';
		cin >> exit;
		if (exit == 'n' || exit == 'N') break;
	}
}

int main() {
	srand(time(0)); // Prevent rand from generating the same number
					// Current Data Veriables
	vector<Hero> party;
	int money = 0;			// Holds current amount of money
	bool win = false;

	characterCreation(party);


	turn_on_ncurses(); //DON'T DO CIN or COUT WHEN NCURSES MODE IS ON
	Map map;
	int x = Map::SIZE / 2, y = Map::SIZE / 2; //Start in middle of the world
	int old_x = -1, old_y = -1;
	while (true) {
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (ch == 'q' || ch == 'Q') break;
		else if (ch == 's' || ch == 'S') map.save(party, money, x, y);
		else if (ch == 'r' || ch == 'r') map.reload(party, money, x, y);
		else if (ch == 't' || ch == 'T') {
			team(party, money);
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i\n", x, y);
			mvprintw(Map::DISPLAY + 2, 0, "Money: $%i\n", money);
			mvprintw(Map::DISPLAY + 3, 0, "q - quit, s - save, r - load, u - upgrade, t - party info", ' ');
			refresh();
		} else if (ch == 'u' || ch == 'U') {
			upgrades(party, money);
			turn_on_ncurses();
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i\n", x, y);
			mvprintw(Map::DISPLAY + 2, 0, "Money: $%i\n", money);
			mvprintw(Map::DISPLAY + 3, 0, "q - quit, s - save, r - load, u - upgrade, t - party info", ' ');
			refresh();
		} else if (ch == RIGHT) {
			x++;
			if (x >= Map::SIZE) x = Map::SIZE - 1; //Clamp value
		} else if (ch == LEFT) {
			x--;
			if (x < 0) x = 0;
		} else if (ch == UP) {
			y--;
			if (y < 0) y = 0;
		} else if (ch == DOWN) {
			y++;
			if (y >= Map::SIZE) y = Map::SIZE - 1; //Clamp value
		} else if (ch == ERR) { //No keystroke
			; //Do nothing
		}
		//Stop flickering by only redrawing on a change
		if (x != old_x or y != old_y) {
			//clear(); //Put this in if the screen is getting corrupted
			if (map.get(x, y) == Map::TREASURE) {       // Picksup treasure
				money += (rand() % 100) + 1;
				map.set(x, y, '.');
			} else if (map.get(x, y) == Map::WALL) {    // Wall collision
				x = old_x;
				y = old_y;
			} else if (map.get(x, y) == Map::WATER) {   // Water collision
				x = old_x;
				y = old_y;
			} else if (map.get(x, y) == Map::MONSTER) { // Combat init
				Combat C;
				bool won = false;
				vector<Monster> monsters;
				int number = (rand()%3)+1;
				for (int i = 0; i < number; i++) {
					int x = rand() % 6 + 1;
					Monster temp(x);
					monsters.push_back(temp);
				}
				c.start(party, monsters,won,money);                                       // Intiate combat sequence
				if(won) map.set(x, y, '.');
				else {
					map.set(x,y,'M');
					x = old_x;
					y = old_y;
				}
			}	
			map.draw(x, y);
			mvprintw(Map::DISPLAY + 1, 0, "X: %i Y: %i\n", x, y);
			mvprintw(Map::DISPLAY + 2, 0, "Money: $%i\n", money);
			mvprintw(Map::DISPLAY + 3, 0, "q - quit, s - save, r - load, u - upgrade, t - party info", ' ');
			refresh();
			map.set(Map::SIZE / 2, Map::SIZE / 2, '.');
		}
		old_x = x;
		old_y = y;
		usleep(1'000'000 / MAX_FPS);
		winCondition(win, map);
		if (win) break;			// Check whatever the win condition is
	}
	turn_off_ncurses();
	gameOver(money);				// Trigger gameOver sequence and takes in the variable that holds the win condition
}

