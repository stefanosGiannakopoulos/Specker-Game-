#include <iostream>
#include<stdexcept>
#include<math.h>
using namespace std;

class Move {
public:
    Move(int sh, int sc, int th, int tc);
    int getSource() const;
    int getSourceCoins() const;
    int getTarget() const;
    int getTargetCoins() const;

    friend ostream& operator<< (ostream& out, const Move& move);

private:
    int numberOfSourceTake;
    int numberOfCoinsTaken;
    int numberOfSourceGive;
    int number0fCoinsGiven;
};

Move::Move(int sh, int sc, int th, int tc) {
    numberOfSourceTake = sh;
    numberOfCoinsTaken = sc;
    numberOfSourceGive = th;
    number0fCoinsGiven = tc;
}
int Move::getSource() const {
    return numberOfSourceTake;
}
int Move::getSourceCoins() const {
    return numberOfCoinsTaken;
}

int Move::getTarget() const {
    return numberOfSourceGive;
}

int Move::getTargetCoins() const {
    return number0fCoinsGiven;
}

ostream& operator<< (ostream& out, const Move& move) {
    if (move.getTargetCoins() != 0) {
        out << "takes" << " " << move.getSourceCoins() << " " << "coins" << " from" << " " << "heap" << " " << move.getSource() << " " << "and" << " " << "puts" << " " << move.getTargetCoins() << " coins" << " to" << " " << "heap" << " " << move.getTarget();
        return out;
    }
    else if (move.getTargetCoins() == 0) {
        out << "takes" << " " << move.getSourceCoins() << " " << "coins" << " from" << " " << "heap" << " " << move.getSource() << " " << "and" << " " << "puts" << " " << "nothing";
        return out;
    }

}

class State {
public:

    State(int h, const int c[], int n);
    ~State();
    void next(const Move& move) throw(logic_error);
    bool winning() const;
    int getHeaps() const;
    int getCoins(int h) const throw(logic_error);
    int getPlayers() const;
    int getPlaying() const;

    friend ostream& operator << (ostream& out, const State& state);

private:
    int numberOfHeaps;
    int* p;
    int numberOfPlayers;
    int player;
};

State::State(int h, const int c[], int n) {
    numberOfHeaps = h;
    numberOfPlayers = n;
    player = 0;
    p = new int[h];
    for (int i = 0; i < h; i++) {
        p[i] = c[i];
    }
}
State::~State() {
    delete[] p;
}
int State::getHeaps() const {
    return numberOfHeaps;
}
int State::getPlayers() const {
    return numberOfPlayers;
}
int State::getPlaying() const {
    return player;
}
ostream& operator << (ostream& out, const State& state) {
    for (int i = 0; i < state.getHeaps(); i++) {
        if (i < (state.getHeaps() - 1)) {
            out << state.p[i] << ',' << ' ';
        }
        if (i == (state.getHeaps() - 1)) {
            out << state.p[i] << " with" << ' ' << state.getPlaying() << '/' << state.getPlayers() << " playing next";
            return out;
        }
    }
}
int State::getCoins(int h) const throw(logic_error) {
    if ((h >= numberOfHeaps) || h < 0) {
        throw logic_error("INVALID NUMBER OF HEAP");
    }
    return p[h];
}
bool State::winning() const {
    int counter = 0;
    for (int i = 0; i < getHeaps(); i++) {
        if (getCoins(i) == 0) {
            counter++;
        }
    }
    return counter == getHeaps();
}
void State::next(const Move& move) throw(logic_error) {

    if (move.getTargetCoins() >= move.getSourceCoins() || move.getSourceCoins() < 1 || move.getSourceCoins() > p[move.getSource()] || move.getTargetCoins() < 0) {
        throw logic_error("COIN PROBLEM");
    }
    if (move.getSource() < 0 || move.getSource() >= numberOfHeaps || move.getTarget() < 0 || move.getTarget() >= numberOfHeaps) {
        throw logic_error("INVALID NUMBER OF HEAP");
    }

    else {
        p[move.getSource()] = p[move.getSource()] - move.getSourceCoins();
        p[move.getTarget()] = p[move.getTarget()] + move.getTargetCoins();
    }
    player++;
    if (player == numberOfPlayers) {
        player = 0;
    }

}

class Player {
public:
    Player(const string& n) {
        player = n;
    }
    virtual ~Player() { player.clear(); }

    virtual const string& getType() const = 0;

    virtual Move play(const State& s) = 0;
    friend ostream& operator << (ostream& out, const Player& player);

protected:
    string player;
};

ostream& operator << (ostream& out, const Player& player) {
    out << player.getType() << ' ' << "player" << ' ' << player.player;
    return out;
}




class GreedyPlayer :public Player {
public:
    GreedyPlayer(const string& n) : Player(n) {
        TypeOfPlayer = "Greedy";
    }

    virtual const string& getType() const override {
        return TypeOfPlayer;
    }
    virtual Move play(const State& s) override {
        int counter = 0;
        int y = 0;

        for (int i = 0; i < s.getHeaps(); i++) {
            if (y < s.getCoins(i)) {
                y = s.getCoins(i);
                counter = i;
            }
        }
        return Move(counter, y, 0, 0);
    }
private:
    string TypeOfPlayer;
};

class SpartanPlayer :public Player {
public:
    SpartanPlayer(const string& n) :Player(n) {
        TypeOfPlayer = "Spartan";
    }

    virtual  const string& getType() const override {
        return TypeOfPlayer;
    }

    virtual Move play(const State& s) override {
        int counter = 0;
        int y = 0;
        for (int i = 0; i < s.getHeaps(); i++) {
            if (y < s.getCoins(i)) {
                y = s.getCoins(i);
                counter = i;
            }
        }
        return Move(counter, 1, 0, 0);
    }
private:
    string TypeOfPlayer;
};


class SneakyPlayer : public Player {
public:
    SneakyPlayer(const string& n) :Player(n) {
        TypeOfPlayer = "Sneaky";
    }

    virtual const string& getType() const override {
        return TypeOfPlayer;
    }

    virtual Move play(const State& s) override {
        int counter = 0;
        long int  y = 99999999999;
        for (int i = 0; i < s.getHeaps(); i++) {
            if (s.getCoins(i) != 0 && y > s.getCoins(i)) {
                y = s.getCoins(i);
                counter = i;
            }
        }
        return Move(counter, s.getCoins(counter), 0, 0);
    }
private:
    string TypeOfPlayer;
};

class RighteousPlayer :public Player {
public:
    RighteousPlayer(const string& n) :Player(n) {
        TypeOfPlayer = "Righteous";
    }

    virtual const string& getType() const override {
        return TypeOfPlayer;
    }

    virtual Move play(const State& s) override {
        int counter = 0;
        int counterMin = 0;
        int x = s.getCoins(0);
        int y = s.getCoins(0);
        for (int i = 0; i < s.getHeaps(); i++) {
            if (x < s.getCoins(i)) {
                x = s.getCoins(i);
                counter = i;
            }
            if (y > s.getCoins(i)) {
                y = s.getCoins(i);
                counterMin = i;
            }
        }
        return Move(counter, ceil((x * 1.0) / 2), counterMin, (ceil((x * 1.0) / 2) - 1));
    }
private:
    string TypeOfPlayer;
};


class Game {
public:
    Game(int heaps, int players) {
        h = heaps;
        Players = players;
        n = new int[heaps];
        playersArray = new Player * [players];
        counterCoins = 0;
        counterPlayers = 0;
    }
    ~Game() {
        delete[] n;
        delete[] playersArray;
    }
    void addHeap(int coins) throw(logic_error) {
        if (counterCoins > h) throw logic_error("INVALID NUMBER OF HEAPS");
        else if (coins < 0) throw logic_error("INVALID NUMBER OF COINS INITIALIZED");
        else {
            n[counterCoins] = coins;
            counterCoins++;
        }
    } 
    void addPlayer(Player* player) throw(logic_error) {
        if (counterPlayers > Players) throw logic_error("INVALID NUMBER OF PLAYERS");
        else {
            playersArray[counterPlayers] = player;
            counterPlayers++;
        }
    }
    void play(ostream& out) throw(logic_error) {
        if (counterCoins < h) throw logic_error("ADD MORE HEAPS");
        else if (counterPlayers < Players)  throw logic_error ("ADD MORE PLAYERS");
        else if (counterCoins != h && counterPlayers != Players) throw logic_error ("PROBLEM WITH ิวล NUMBER OF HEAPS AND THE NUMBER OF PLAYERS");
        else {
            State s(h, n, Players);

            int i = 0;
            while (!s.winning()) {
                out << "State: ";
                for (int i = 0; i < s.getHeaps(); i++) {
                    if (i < (s.getHeaps() - 1)) {
                        out << s.getCoins(i) << ',' << ' ';
                    }
                    if (i == (s.getHeaps() - 1)) {
                        out << s.getCoins(i) << " with" << ' ' << s.getPlaying() << '/' << s.getPlayers() << " playing next" << endl;
                    }
                }

                Move move = playersArray[i]->play(s);

                if (move.getTargetCoins() != 0) {
                    out << *playersArray[i] << " takes" << " " << move.getSourceCoins() << " " << "coins" << " from" << " " << "heap" << " " << move.getSource() << " " << "and" << " " << "puts" << " " << move.getTargetCoins() << " coins" << " to" << " " << "heap" << " " << move.getTarget() << endl;

                }
                else if (move.getTargetCoins() == 0) {
                    out << *playersArray[i] << " takes" << " " << move.getSourceCoins() << " " << "coins" << " from" << " " << "heap" << " " << move.getSource() << " " << "and" << " " << "puts" << " " << "nothing" << endl;

                }
                s.next(playersArray[i]->play(s));
                i++;
                if (i == Players) i = 0;
            }
            out << "State: ";
            for (int i = 0; i < s.getHeaps(); i++) {
                if (i < (s.getHeaps() - 1)) {
                    out << s.getCoins(i) << ',' << ' ';
                }
                if (i == (s.getHeaps() - 1)) {
                    out << s.getCoins(i) << " with" << ' ' << s.getPlaying() << '/' << s.getPlayers() << " playing next" << endl;
                }
            }
            if (i == 0) out << *playersArray[Players-1] << " wins" << endl;
            else {
                out<< *playersArray[--i] << " wins" << endl;
            }
        }
    }
    int getPlayers() const {
        return Players;
    }
    const Player* getPlayer(int p) const throw(logic_error) {
        if (p >Players) throw logic_error("PLAYER HAS NOT BEEN CREATED ");
        return playersArray[p];
    }

private:
    int h;
    int Players;
    int* n;
    Player** playersArray;
    int counterCoins;
    int counterPlayers;
};

int main() {
    Game specker(3, 4);
     specker.addHeap(10);
     specker.addHeap(20);
     specker.addHeap(17);
     specker.addPlayer(new SneakyPlayer("Tom"));
     specker.addPlayer(new SpartanPlayer("Mary"));
     specker.addPlayer(new GreedyPlayer("Alan"));
     specker.addPlayer(new RighteousPlayer("Robin"));
     specker.play(cout);

}

