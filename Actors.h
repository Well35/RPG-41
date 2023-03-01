#pragma once
#include <string>
#include <iostream>
using namespace std;
class Actor {
	int atk = 1;                             
	int spd = 1;
	int hp = 10;                             
	string name = "Player";
	public:                                    
	string get_name() {
		return name;
	}                                    
	int get_hp() {                              
		return hp;
	}

	int get_atk() const {                      
		return atk;
	}

	int get_spd() const {
		return spd;
	}
	void set_hp(int newHp) {
		hp = newHp;
	}                                    
	void set_atk(int newAtk) {
		atk = newAtk;
	}

	void set_spd(int newSpd) {
		spd = newSpd;                        }
	void set_name(string newName) {
		name = newName;
	}
	virtual string get_type() const { return "NPC"; }
};

class Hero : public Actor {
	//string classname = "class";
	int classtype = 0;
	string type;
	//1 = mechanic
	//4 = technomancer
	//2 = roge??
	//3 = paladin??
	public:
	Hero(string name, int classs) {
		set_name(name);
		classtype = classs;                      
		type = "Hero";
		if (classtype == 1) {                        
			//Mechanic (priest)                      
			set_hp(rand() % 75 + 50);                
			set_atk(rand() % 12 + 8);                
			set_spd(rand() % 14 + 10);                                                    
		}                                        
		else if (classtype == 2) {                   
			//Netrunner(Rogue)                       
			set_hp(rand() % 75 + 50);                
			set_atk(rand() % 14 + 12);                                                        
			set_spd(rand() % 20 + 16);                                                    
		}                                        
		else if (classtype == 3) {                        
			//Mech(Paladin)
			set_hp(rand() % 100 + 75);
			set_atk(rand() % 20 + 16);  
			set_spd(rand() % 12 + 8);            
		} 
		else if (classtype == 4) {
			//Technomancer(Wizard)
			set_hp(rand() % 50 + 25);
			set_atk(rand() % 12 + 8);
			set_spd(rand() % 14 + 10);
		}
		else {
			set_hp(1);
			set_atk(1);
			set_spd(1);
		}
	};
	Hero();

	int atkBasic() {
		return get_atk();
	}
	int special() {
		if (classtype == 1) {
			return rand() % 20 + 15;
		}                                      
		else if (classtype == 2) {
			return atkBasic() *4;
		}
		else if (classtype == 3) {
			return 0;
		}
		else if (classtype == 4) {
			return rand()% -20 + -15;
		}
		else {
			return 0;
		}
		return 0;
	}
	int get_classtype() const {return classtype;}
	string get_type() const override {return type;}
};

class Monster : public Actor {
	int classtype = 0;
	string type;
	public:
	Monster(int classs) {
		// set.name(newName);
		type = "Monster";
		classtype = classs;
		if (classtype == 1) {
			set_name("Mechoblin");
			set_hp(rand() % 25 + 10);
			set_atk(rand() % 12 + 8);
			set_spd(rand() % 14 + 10);
		}
		else if (classtype == 2) {
			set_name("Bytes");
			set_hp(rand() % 25 + 10);
			set_atk(rand() % 12 + 8);
			set_spd(rand() % 14 + 16);
		}
		if (classtype == 3) {
			set_name("Drone");
			set_hp(rand() % 40 + 20);
			set_atk(rand() % 14 + 16);
			set_spd(rand() % 14 + 16);
		}
		if (classtype == 4) {
			set_name("Boo_lean");
			set_hp(rand() % 40 + 20);
			set_atk(rand() % 12 + 8);
			set_spd(rand() % 14 + 10);
		}
		if (classtype == 5) {
			set_name("Aracnotek");
			set_hp(rand() % 40 + 20);
			set_atk(rand() % 14 + 12);
			set_spd(rand() % 16 + 14);
		}
		if (classtype == 6) {
			set_name("Mechadragon");
			set_hp(rand() % 150 + 200);
			set_atk(rand() % 16 + 20);
			set_spd(rand() % 14 + 18);
		}
	};
	Monster();

	int atkBasic() {
		return get_atk();
	}
	int special() {
		if (classtype == 6) {
			return rand() % -40 + -25;
		}
	}
	int get_classtype() const {
		return classtype;
	}
	string get_type() const override {return type;}
};
