#include <cstdlib>
#include <string>
using namespace std;

class Actor {
  int hp = 10;
  int atk = 1;
  int spd = 1;
  string name = "N/A";
  
  public:
  Actor();
  Actor(int newHp, int newAtk, int newSpd, string newName) {
    hp = newHp;
    atk = newAtk;
    spd = newSpd;
    name = newName;
  };
  ~Actor(){};
  
  void set_hp(int newHp) { hp = newHp; }
  void set_atk(int newAtk) {atk = newAtk; }
  void set_spd(int newSpd) {spd = newSpd; }
  void set_name(string newName) {name = newName; }
  
  int get_hp() const { return hp; }
  int get_atk() const { return atk; }
  int get_spd() const { return spd; }
  string get_name() const { return name; }
  
};


class Hero : Actor {
  /*
  1 = Mechanic
  2 = Rogue
  3 = Paladin
  4 = Wizard
  */
  int classtype = 0;  
  public:
  Hero(string newName, int classs) {
    set_name(newName);
    classtype = classs;
    
    if(classtype == 1) {
      set_hp(rand() % 75 + 50);
      set_atk(rand() % 12 + 8);
      set_spd(rand() % 14 + 10);
    } 
    else if(classtype == 2) {
      set_hp(rand() % 75 + 50);
      set_atk(rand() % 100 + 75);
      set_spd(rand() % 20 + 16);      
    } 
    else if(classtype == 3) {
      set_hp(rand() % 100 + 75);
      set_atk(rand() % 20 + 16);
      set_spd(rand() % 12 + 8);      
    } 
    else if(classtype == 4) {
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
  
  ~Hero(){};
  
};




class Monster : Actor {
  classtype = 0;
  public:
  Monster(int classs) {
    classtype = class;
    if(classtype == 1) {
      set_name("Mechoblin");
      set_hp(rand() % 25 + 10);
      set_atk(rand() % 12 + 8);
      set_spd(rand() % 14 + 10);
    } 
    else if(classtype == 2) {
      set_name("Bytes");
      set_hp(rand() % 25 + 10);
      set_atk(rand() % 12 + 8);
      set_spd(rand() % 14 + 16);     
    } 
    else if(classtype == 3) {
      set_name("Drone");
      set_hp(rand() % 40 + 20);
      set_atk(rand() % 12 + 8);
      set_spd(rand() % 14 + 10);        
    } 
    else if(classtype == 4) {
      set_name("Boo_lean");
      set_hp(rand() % 40 + 20);
      set_atk(rand() % 12 + 8);
      set_spd(rand() % 14 + 10);      
    } 
    else if(classtype == 5) {
      set_name("Aracnotek");
      set_hp(rand() % 40 + 20);
      set_atk(rand() % 14 + 12);
      set_spd(rand() % 16 + 14);      
    } 
    else if(classtype == 6) {
      set_name("Mechnadragon");
      set_hp(rand() % 150 + 200);
      set_atk(rand() % 16 + 20);
      set_spd(rand() % 14 + 18);      
    } 
    else {
      set_name("Unknown");
      set_hp(1);
      set_atk(1);
      set_spd(1);      
    } 
  };
  
  ~Monster(){};
  
};
