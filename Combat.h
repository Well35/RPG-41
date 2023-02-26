#include <iostream>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <ncurses.h>
#include <unistd.h>
#include "Bridges.h"
#include "CircDLelement.h"
using namespace std;
using namespace bridges;


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

void turn_off_ncurses() {
	clear();
	endwin(); // End curses mode
	if (system("clear")) {}
}

class Actor {
	public:
		int hp = 0;
		int speed = 0;
		string name;
		string type;
		int dmg = 5;
};

class Monster : public Actor {
	public:
};

class Hero : public Actor {
	public:
};

class LL {
	struct Node {
		Actor* data;
		Node* next = nullptr;
		Node* prev = nullptr;
		Node(Actor* new_data = 0, Node *new_next = 0, Node *new_prev = 0) : data(new_data), next(new_next), prev(new_prev) {}
	};
	int size = 0;
	public:
	Node* head = nullptr;
	Node* tail = nullptr;
	Node* currTurn = nullptr;
	LL() {};
	~LL() {
		while (head) {
			tail->next = nullptr;
			Node *temp = head->next;
			delete head;
			head = temp;
		}
	}
	void push_back(Actor* a) {
		tail = new Node(a,nullptr,tail);
		if (size == 0) head = tail;
		else {
			Node* c = head;
			while (c->next != NULL) c = c->next;
			c->next = tail;
			tail->prev = c;
		}
		//cout << 1 << endl;
		//tail->prev->next = tail;
		//cout << "HI" << endl;
		size++;
	}
	void remove(Node* n) {
		Node* temp = n;
		n = n->next;
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		delete temp;
	}
	void print_list() {
		int i = size;
		for (Node *ptr = head; ptr; ptr = ptr->next) {
			if (!i) break;
			cout << ptr->data->speed << endl;
			i--;
		}
	}
};


class Combat {
	private:
		vector<Actor*> hnm;
		vector<Monster> mon;
		vector<Hero> her;
		int monsterCount = 0;
		int heroCount = 0;
		LL list;
	public:
		void populate() {
			for (auto& a : hnm) {
				list.push_back(a);
			}
			list.tail->next = list.head;
			list.head->prev = list.tail;
		}
		static bool com(const Actor* lhs, const Actor* rhs) {
			return lhs->speed > rhs->speed;
		}
		void print() {
			clear();
			move(0, 5);
			printw("Heroes");
			move(0, 25);
			printw("Monsters");
			move(20, 20);
			for (int i = 0; i < her.size(); i++) {
				move(2+i, 5);
				string temp = to_string(i+1) + ". " + (her.at(i).name) + " hp: " + to_string(her.at(i).hp);
				printw(temp.c_str());
			}
			for (int i = 0; i < mon.size(); i++) {
				move(2+i, 25);
				string temp = to_string(i+1) + ". " + (mon.at(i).name) + " hp: " + to_string(mon.at(i).hp);
				printw(temp.c_str());
			}
		}
		void start(vector<Hero>& heroes, vector<Monster>& monsters) {
			Bridges *bridges =  new Bridges(99, "Well35", "1562414356318");
			bridges->setTitle("An Example Circular Doubly Linked List");
				turn_on_ncurses();
				her = heroes;
				mon = monsters;
				string s;
				for (auto& h : heroes)  {
					hnm.push_back(&h);
					heroCount++;
				}
				for (auto& m : monsters) {
					hnm.push_back(&m);
					monsterCount++;
				}
				sort(hnm.begin(), hnm.end(), com);
				populate();



				list.currTurn = list.head;
				while (true) {
					int x = getch();
					print();
					move(20, 20);
					s = "Who should " + list.currTurn->data->name + " attack?";
					printw(s.c_str());
					if (x == 'q') break;
					if (list.currTurn->data->hp <= 0) {
						list.remove(list.currTurn);	
					}
					if (list.currTurn->data->type == "Hero") {
						if (x >= 0+49 and x < monsterCount+49) {
							mon.at(x-49).hp -= list.currTurn->data->dmg;
							if (mon.at(x-49).hp <= 0) {
								mon.erase(mon.begin()+(x-49));
								monsterCount--;
							}
							list.currTurn = list.currTurn->next;
						}
					}
					else {
						int k = rand() % her.size();
						her.at(k).hp -= mon.at(rand() % mon.size()).dmg;
						if (her.at(k).hp  <= 0) {
							her.erase(her.begin()+k);
							heroCount--;
						}
						list.currTurn = list.currTurn->next;
					}
					//printw(to_string(x).c_str());
					//usleep(1000);
					if (!mon.size() or !her.size()) break;
				}
				turn_off_ncurses();
				//list.print_list();
				if (!mon.size()) cout << "YOU WIN!!" << endl;
				if (!her.size()) cout << "YOU LOSE!!" << endl;
			}

		};
