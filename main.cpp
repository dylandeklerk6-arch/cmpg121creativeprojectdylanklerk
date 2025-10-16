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

// Forward declarations for functions used in main
void saveGame(const Player& p);
bool loadGame(Player& p);
void printStats(const Player& p);
void levelUp(Player& p);
Enemy randomEnemy();
void battle(Player& p);
void saveStatsToFile(const Player& p);

int main() {
    srand((unsigned)time(0));
    Player player = {"", "", 0, 0, 1, 0, 2, 0};
    cout << "\n========== Elden Ring Text Adventure ==========\n";
    cout << "   1. New Game\n   2. Load Saved Game\n";
    cout << "=============================================\n";
    cout << "Your choice: "; int choice; cin >> choice;
    if (choice == 2 && loadGame(player)) {
        // Loaded game, stats will print at menu loop
    } else {
        cout << "\nEnter your name: "; cin >> player.name;
        cout << "Choose class (Vagabond/Mage/Samurai): "; cin >> player.pclass;

        if (player.pclass == "Mage") {
            player.hp = player.maxhp = 20;
            player.attack = 7;
        } else if (player.pclass == "Samurai") {
            player.hp = player.maxhp = 25;
            player.attack = 9;
        } else if (player.pclass == "Vagabond") {
            player.hp = player.maxhp = 30;
            player.attack = 11;
        } else {
            player.pclass = "Vagabond";
            player.hp = player.maxhp = 30;
            player.attack = 11;
            cout << "Invalid class chosen. Defaulting to Vagabond.\n";
        }
        player.potions = 2; player.level = 1; player.xp = 0;
        cout << "Character created!\n";
    }

    while (player.hp > 0) {
        printStats(player);
        cout << "---------------------\n";
        cout << "Main Menu:\n";
        cout << "  1. Battle\n  2. Use Potion\n  3. Save & Quit\n";
        cout << "---------------------\n";
        cout << "Choice: "; int c; cin >> c;
        if (c == 1) battle(player);
        else if (c == 2 && player.potions > 0) {
            int heal = (player.hp + 12 > player.maxhp) ? (player.maxhp - player.hp) : 12;
            player.hp += heal; player.potions--;
            cout << "\nPotion used. You heal " << heal << " HP. Potions left: " << player.potions << endl;
            cout << "Current HP: " << player.hp << "\n";
        } else if (c == 2) {
            cout << "\nNo potions left!\n";
        } else if (c == 3) {
            saveGame(player);
            cout << "Exiting game...\n";
            break;
        } else {
            cout << "\nInvalid choice! Try again.\n";
        }
    }
    saveStatsToFile(player);
    cout << "\n========== GAME OVER ==========\n";
    cout << "Thank you for playing!\n\n";
    return 0;
}

void saveGame(const Player& p) {
    ofstream out("save.txt");
    out << p.name << " " << p.pclass << " " << p.hp << " " << p.maxhp << " "
        << p.level << " " << p.attack << " " << p.potions << " " << p.xp << "\n";
    out.close();
    cout << "\n---------------------\n";
    cout << "  Game saved!\n";
    cout << "---------------------\n\n";
}

bool loadGame(Player& p) {
    ifstream in("save.txt");
    if (!in) return false;
    in >> p.name >> p.pclass >> p.hp >> p.maxhp >> p.level >> p.attack >> p.potions >> p.xp;
    in.close();
    cout << "\n---------------------\n";
    cout << "  Game loaded!\n";
    cout << "---------------------\n\n";
    return true;
}

void printStats(const Player& p) {
    cout << "\n======= PLAYER STATS =======\n";
    cout << "Name:      " << p.name << endl;
    cout << "Class:     " << p.pclass << endl;
    cout << "HP:        " << p.hp << "/" << p.maxhp << endl;
    cout << "Attack:    " << p.attack << endl;
    cout << "Level:     " << p.level << endl;
    cout << "XP:        " << p.xp << endl;
    cout << "Potions:   " << p.potions << endl;
    cout << "============================\n";
}

void levelUp(Player& p) {
    p.level++; p.maxhp += 5; p.attack += 2; p.hp = p.maxhp;
    cout << "\n***** LEVEL UP! *****\n";
    cout << "You are now level " << p.level << "!\n";
    cout << "Max HP: " << p.maxhp << ", Attack: " << p.attack << "\n";
    cout << "HP refilled to: " << p.hp << "\n\n";
}

Enemy randomEnemy() {
    string names[] = {"Godrick Soldier", "Rotten Stray", "Misbegotten"};
    int hps[] = {18, 14, 22}, atks[] = {5, 4, 6}, xps[] = {10, 8, 12};
    int idx = rand() % 3;
    return {names[idx], hps[idx], atks[idx], xps[idx]};
}

void battle(Player& p) {
    Enemy e = randomEnemy();
    cout << "\n---------------------\n";
    cout << "A wild " << e.name << " appears!\n";
    cout << "Enemy HP: " << e.hp << "  |  Attack: " << e.attack << "\n";
    cout << "---------------------\n";

    while (e.hp > 0 && p.hp > 0) {
        cout << "\nYour HP:  " << p.hp << "\nEnemy HP: " << e.hp << endl;
        cout << "---------------------\n";
        cout << "Choose your action:\n";
        cout << "  1. Attack\n  2. Potion\n";
        cout << "---------------------\n";
        cout << "Your choice: "; int c; cin >> c;

        if (c == 1) {
            e.hp -= p.attack;
            cout << "\nYou hit the enemy for " << p.attack << " damage!" << endl;
        }
        else if (c == 2 && p.potions > 0) {
            int heal = (p.hp + 12 > p.maxhp) ? (p.maxhp - p.hp) : 12;
            p.hp += heal;
            p.potions--;
            cout << "\nYou used a potion and healed " << heal << " HP.\nPotions left: " << p.potions << endl;
        } else if (c == 2) {
            cout << "\nNo potions left!" << endl;
        } else {
            cout << "\nInvalid choice!" << endl;
            continue;
        }

        if (e.hp > 0) {
            p.hp -= e.attack;
            cout << "Enemy strikes for " << e.attack << " damage!" << endl;
        }
    }

    if (p.hp > 0) {
        p.xp += e.xp;
        cout << "\n**** VICTORY! ****\n";
        cout << "You gained " << e.xp << " XP!\n";
        if (p.xp >= p.level * 20) levelUp(p);
    } else {
        cout << "\n!!!! YOU DIED !!!!\n";
    }
}

void saveStatsToFile(const Player& p) {
    ofstream out("player_stats.txt", ios::app);
    if (!out) {
        cout << "Error opening player_stats.txt!\n";
        return;
    }
    out << "Name: " << p.name << ", Class: " << p.pclass
        << ", Level: " << p.level << ", XP: " << p.xp << ", HP: " << p.hp << "\n";
    out.close();
}
