#pragma once
#include "Actors.h"
#include "Combat.h"
#include <vector>
#include <string>
#include <unistd.h>
#include <iostream>
#include <random>
#include <ncurses.h>
using namespace std; //Boo hiss

class Map {
	vector<vector<char>> map;
	vector<Map> mapSaves;
	vector<vector<Hero>> heroSaves;
	vector<int> moneySaves;
	vector<int> xCoord;
	vector<int> yCoord;
	default_random_engine gen;
	public:
	
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
        	timeout(10); //Set a max delay for key entry
    	}

    	//Exit full screen mode - also do this if you ever want to use cout or gtest or something
    	void turn_off_ncurses() {
    	    clear();
    	    endwin(); // End curses mode
    	    if (system("clear")) {}
    	}

	// Map setter function
	void set(int col, int row, char newChar) {
		map.at(row).at(col) = newChar;	
	}
	// Map getter function
	char get(int col, int row) {
		return map.at(row).at(col);
	}
	
	// Game save function
        void save(vector<Hero>& party, int& money, int& newX, int& newY) {
                turn_off_ncurses();
                cout << "Enter the number for the save that is to be changed:\n";
                for(int i = mapSaves.size(); 0 < i; --i) {
                        cout << "Save " << i << endl;
                }
                cout << "New Save 0\n";
                cout << "--------------\n > ";
                int input = 0;
                cin >> input;
                if(input == 0) {
                        Map temp;
                        for(int row = 0; row < map.size(); ++row) {
                                for(int col = 0; col < map.at(row).size(); ++col) {
                                        temp.set(col,row, get(col, row));
                                }
                        }
                        mapSaves.push_back(temp);
                        heroSaves.push_back(party);
                        moneySaves.push_back(money);
                        xCoord.push_back(newX);
                        yCoord.push_back(newY);
                        cout << "New save created!" << endl;
                } else if(input-1 <= mapSaves.size()) {
                        for(int row = 0; row < map.size(); ++row) {
                                for(int col = 0; col < map.at(row).size(); ++col) {
                                        mapSaves.at(input-1).set(col,row,get(col,row));
                                }
                        }
                        for(int slot = 0; slot < heroSaves.at(input-1).size(); ++slot) {
                                heroSaves.at(input-1).at(slot) = party.at(slot);
                        }
                        moneySaves.at(input-1) = money;
                        xCoord.at(input-1) = newX;
                        yCoord.at(input-1) = newY;
                        cout << "Save overrided!" << endl;
                } else {
                        cout << "Save does not exist!" << endl;
                }
                usleep(50'000'000/90);
                turn_on_ncurses();
                draw(newX,newY);
                mvprintw(DISPLAY+1,0,"X: %i Y: %i\n",newX,newY);
                mvprintw(DISPLAY+2,0,"Money: $%i\n",money);
		mvprintw(DISPLAY+3,0,"q - quit, s - save, r - load, u - upgrade, t - party info", ' ');
                refresh();		
        }
	
	// Game reload function
        void reload(vector<Hero>& party, int& money, int& oldX, int& oldY) {
                turn_off_ncurses();
                cout << "Enter the number to load the save:\n";
                for(int i = mapSaves.size(); 0 < i; --i) {
                        cout << "Save " << i << endl;
                }
                cout << "-------------------\n > ";
                int input = 0;
                cin >> input;
                if(input-1 <= mapSaves.size()) {
                        for(int row = 0; row < map.size(); ++row) {
                                for(int col = 0; col < map.size(); ++col) {
                                        set(col,row,mapSaves.at(input-1).get(col,row));
                                }
                        }
                        for(int slot = 0; slot < heroSaves.at(input-1).size(); ++slot) {
                                party.at(slot) = heroSaves.at(input-1).at(slot);
                        }
                        money = moneySaves.at(input-1);
                        oldX = xCoord.at(input-1);
                        oldY = yCoord.at(input-1);
                } else {
                        cout << "Save does not exist" << endl;
                }
		usleep(50'000'000/90);
                turn_on_ncurses();
                draw(oldX,oldY);
                mvprintw(DISPLAY+1,0,"X: %i Y: %i\n",oldX,oldY);
                mvprintw(DISPLAY+2,0,"Money: $%i\n",money);
		mvprintw(DISPLAY+3,0,"q - quit, s - save, r - load, u - upgrade, t - party info", ' ');
                refresh();

        }

	
	static const char HERO     = 'H';
	static const char MONSTER  = 'M';
	static const char WALL     = '#';
	static const char WATER    = '~';
	static const char OPEN     = '.';
	static const char TREASURE = '$';
	static const size_t SIZE = 100; //World is a 100x100 map
	static const size_t DISPLAY = 30; //Show a 30x30 area at a time
	//Randomly generate map
	void init_map() {
		uniform_int_distribution<int> d100(1,100);
		map.clear();
		map.resize(SIZE); //100 rows tall
		for (auto &v : map) v.resize(SIZE,'.'); //100 columns wide
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZE; j++) {
				//Line the map with walls
				if (i == 0 or j == 0 or i == SIZE-1 or j == SIZE-1) 
					map.at(i).at(j) = WALL;
				else if (i == SIZE/2 and j == SIZE/2) 
					map.at(i).at(j) = HERO;
				else {
					//5% chance of monster
					if (d100(gen) <= 5) {
						map.at(i).at(j) = MONSTER;
					}
					else if (d100(gen) <= 3) {
						map.at(i).at(j) = TREASURE;
					}
					else if (d100(gen) <= 10) { //10% each spot is wall
						map.at(i).at(j) = WALL;
					}
					else if (d100(gen) <= 3) { //3% each spot is water
						map.at(i).at(j) = WATER;
					}
					else if (d100(gen) <= 40) { //40% chance of water near other water
						if (map.at(i-1).at(j) == WATER or map.at(i+1).at(j) == WATER or map.at(i).at(j-1) == WATER or map.at(i).at(j+1) == WATER)
							map.at(i).at(j) = WATER;
					}
				}
			}
		}
	}
	//Draw the DISPLAY tiles around coordinate (x,y)
	void draw(int x, int y) {
		int start_x = x - DISPLAY/2;
		int end_x = x + DISPLAY/2;
		int start_y = y - DISPLAY/2;
		int end_y = y + DISPLAY/2;

		//Bounds check to handle the edges
		if (start_x < 0) {
			end_x = end_x - start_x;
			start_x = 0;
		}
		if (end_x > SIZE-1) {
			start_x = start_x - (end_x - (SIZE-1));
			end_x = SIZE-1;
		}
		if (start_y < 0) {
			end_y = end_y - start_y;
			start_y = 0;
		}
		if (end_y > SIZE-1) {
			start_y = start_y - (end_y - (SIZE-1));
			end_y = SIZE-1;
		}

		//Now draw the map using NCURSES
		for (size_t i = start_y; i <= end_y; i++) {
			for (size_t j = start_x; j <= end_x; j++) {
				if (i == y && j == x) {
					attron(A_UNDERLINE | A_BOLD);
					mvaddch(i-start_y,j-start_x,'H');
					attroff(A_UNDERLINE | A_BOLD);
				}
				else {
					int color = 1;
					if (map.at(i).at(j) == WALL)
						color = 5;
					else if (map.at(i).at(j) == WATER)
						color = 2;
					else if (map.at(i).at(j) == HERO)
						color = 3;
					else if (map.at(i).at(j) == TREASURE)
						color = 4;
					else if (map.at(i).at(j) == MONSTER)
						color = 6;

					attron(COLOR_PAIR(color));
					mvaddch(i-start_y,j-start_x,map.at(i).at(j));
					attroff(COLOR_PAIR(color));
				}
			}
		}
	}
	Map() {
		init_map();
	}
};
