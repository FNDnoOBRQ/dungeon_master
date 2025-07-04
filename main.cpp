using namespace std;
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include "library.h"
#include <fstream>

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

class Creature{
protected:
    string name;
    char sign;
    int health;
    int damage;
    int gold;
public:
    Creature(string n, char s, int h, int d, int g): name(n), sign(s), health(h), damage(d), gold(g){}
    string getName()const{return name;}
    char getSign()const{return sign;}
    int getHealth()const{return health;}
    int getDamage()const{return damage;}
    int getGold()const{return gold;}

    void reduceHealth(int value){
        health -= value;
    }
    bool isDead(){
        return (health <= 0);
    }
    void addGold(int value){
        gold += value;
    }
};


class Player : public Creature{
    int level = 0;
public:
    Player(string name): Creature(name, '@', 40, 5, 0){}
    void Levelup(){
        level++;
        damage += 5;
    }
    int getLevel(){return level;}
    bool hasWon(){return (level >= 20);}
};


class Monster: public Creature{
public:
    enum MONSTERTYPE{
        ORC,
        DRAGON,
        SLIME,
        GOBLIN,
        MAX_TYPES
    };
    struct MonsterData{
        string name;
        char sign;
        int health;
        int damage;
        int gold;
    };
    static MonsterData monsterData[MAX_TYPES];
    Monster(MONSTERTYPE type): Creature(monsterData[type].name, monsterData[type].sign, monsterData[type].health, monsterData[type].damage, monsterData[type].gold){}

    static Monster getRandomMonster(){
        int number = getRandomNumber(0, MAX_TYPES-1);
        return Monster(static_cast<MONSTERTYPE>(number));
    }
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "Dragon", 'D', 100, 10, 100 },
	{ "Orc", 'o', 50, 5, 25 },
	{ "Slime", 's', 5, 1, 1 },
    { "Golbin", 'g', 15, 7, 10}
};

void showStats(Player &p){
    cout <<"Name: "<< p.getName()  <<"\nHp: " << p.getHealth()<<"\nDamage: " << p.getDamage() << "\nGold: " << p.getGold() << "\nLevel: " << p.getLevel() << endl;
    cout << endl;
}



string MyName(){
    cout << "Enter your name: ";
    string n;
    cin >> n;
    return n;
}

int main(){
    time_t currentTime = time(nullptr);
    ofstream Myfile("result.txt");
    srand(time(0));
    rand();

    Player p(MyName());
    cout << "Welcome to the DUNGEON!\n";
    while(!p.isDead() && !p.hasWon()){
        Monster m = Monster::getRandomMonster();
        cout << "Your enemy is " << m.getName() << endl;
        cout << "What would you do? (R)un or (F)ight: ";
        string input;
        cin >> input;
        if(input == "r" || input == "R"){
            if(getRandomNumber(1,2) == 1){
                cout << "You avoid appointment with this creature, but for a short time." << endl;
                continue;
            }
            else{
                cout << m.getName() <<" hit you."<<endl;
                p.reduceHealth(m.getDamage());
                showStats(p);
                continue;
            }
        }
        else if(input == "f" || input == "F"){
            m.reduceHealth(p.getDamage());
            cout << "You hit monster."<<endl;
            if(m.getHealth() > 0){
                cout << "He is still alive!!!" << endl;
                cout << m.getName() <<" hit you."<<endl;
                p.reduceHealth(m.getDamage());
                showStats(p);
            }
            else{
                p.Levelup();
                cout << "\nLevel up!\n\a"<<endl;
            }
        }
    }
    if(p.isDead()){
        cout << "You die. You had " << p.getGold() << " gold and " << p.getLevel() << " level."<<endl;
        cout << "Try to beat the creature next time." << endl;
    }
    else if(p.hasWon()){
        cout << "Well done. You're having:\n" << "Hp: " << p.getHealth() << "\n" << "Gold: " << p.getGold() << "\n" << "Level: " << p.getLevel() << endl;  
        Myfile << "Time: " << ctime(&currentTime) <<"\tComplete!\n";
    }
    return 0;
}