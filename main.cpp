// 55798640
//Dylan de Klerk

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Player {
    string name, pclass;
    int hp, maxhp, level, attack, potions, xp;
};

struct Enemy {
    string name;
    int hp, attack, xp;
};

void saveGame(const Player& p) {
    ofstream out("save.txt");
    out << p.name << " " << p.pclass << " " << p.hp << " " << p.maxhp << " "
        << p.level << " " << p.attack << " " << p.potions << " " << p.xp << "\n";
    out.close();
    cout << "Game saved!\n";
}

bool loadGame(Player& p) {
    ifstream in("save.txt");
    if (!in) return false;
    in >> p.name >> p.pclass >> p.hp >> p.maxhp >> p.level >> p.attack >> p.potions >> p.xp;
    in.close();
    cout << "Game loaded!\n";
    return true;
}

void printStats(const Player& p) {
    cout << "Name: " << p.name << " | Class: " << p.pclass
         << " | HP: " << p.hp << "/" << p.maxhp << " | ATK: " << p.attack
         << " | LVL: " << p.level << " | XP: " << p.xp
         << " | Potions: " << p.potions << "\n";
}

void levelUp(Player& p) {
    p.level++; p.maxhp += 5; p.attack += 2; p.hp = p.maxhp;
    cout << "LEVEL UP! Now level " << p.level << ", HP = " << p.hp << "\n";
}

Enemy randomEnemy() {
    string names[] = {"Godrick Soldier", "Rotten Stray", "Misbegotten"};
    int hps[] = {18, 14, 22}, atks[] = {5, 4, 6}, xps[] = {10, 8, 12};
    int idx = rand() % 3;
    return {names[idx], hps[idx], atks[idx], xps[idx]};
}

void battle(Player& p) {
    Enemy e = randomEnemy();
    cout << "A wild " << e.name << " appears!\n";
    while (e.hp > 0 && p.hp > 0) {
        cout << "Your HP: " << p.hp << " | Enemy HP: " << e.hp << endl;
        cout << "1. Attack  2. Potion\nChoice: "; int c; cin >> c;
        if (c == 1) { e.hp -= p.attack; cout << "You hit!\n"; }
        else if (c == 2 && p.potions > 0) { p.hp += 12; if (p.hp > p.maxhp) p.hp = p.maxhp; p.potions--; cout << "Potion!\n"; }
        if (e.hp > 0) { p.hp -= e.attack; cout << "Enemy hits!\n"; }
    }
    if (p.hp > 0) { p.xp += e.xp; cout << "Victory! +XP\n"; if (p.xp >= p.level * 20) levelUp(p); }
    else cout << "You died!\n";
}

// NEW FUNCTION: Save player stats at end to player_stats.txt, fulfilling text file requirement
void saveStatsToFile(const Player& p) {
    ofstream out("player_stats.txt", ios::app); // Append mode to keep a log
    if (!out) {
        cout << "Error opening player_stats.txt!\n";
        return;
    }
    out << "Name: " << p.name << ", Class: " << p.pclass
        << ", Level: " << p.level << ", XP: " << p.xp << ", HP: " << p.hp << "\n";
    out.close();
}

int main() {
    srand((unsigned)time(0));
    Player player = {"", "", 0, 0, 1, 0, 2, 0};
    cout << "Elden Ring Text Adventure:\n1. New Game 2. Load\nYour choice: ";
    int choice; cin >> choice;
    if (choice == 2 && loadGame(player)) { printStats(player); }
    else {
        cout << "Enter your name: "; cin >> player.name;
        cout << "Choose class (Vagabond/Mage/Samurai): "; cin >> player.pclass;
        if (player.pclass == "Mage") { player.hp = player.maxhp = 20; player.attack = 7; }
        else if (player.pclass == "Samurai") { player.hp = player.maxhp = 25; player.attack = 9; }
        else { player.hp = player.maxhp = 30; player.attack = 11; player.pclass = "Vagabond"; }
        player.potions = 2; player.level = 1; player.xp = 0;
    }
    while (player.hp > 0) {
        printStats(player);
        cout << "1. Battle 2. Potion 3. Save & Quit\nChoice: ";
        int c; cin >> c;
        if (c == 1) battle(player);
        else if (c == 2 && player.potions > 0) {
            player.hp += 12; if (player.hp > player.maxhp) player.hp = player.maxhp; player.potions--;
            cout << "Potion used. HP: " << player.hp << endl;
        } else if (c == 3) { saveGame(player); break; }
    }
    // Save player stats at the end of the session
    saveStatsToFile(player);
    cout << "Game over!\n";
    return 0;
}
