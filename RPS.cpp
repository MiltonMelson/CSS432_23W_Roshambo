// RPS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

using namespace std;
#include <iostream>

enum RPS {rock, paper, scissors};


int compChoice() {
    // Seeds the time and returns a random number
    srand(time(0));
    return rand() % 3 + 1;
    
    //return 0;
    //return 1;
    //return 2;
}

int convert(const string input) {
    if (input == "Rock" || input == "rock" || input == "r" || input == "0") {
        return RPS::rock;
    }
    else if (input == "Paper" || input == "paper" || input == "p" || input == "1") {
        return RPS::paper;
    }
    else if (input == "Scissors" || input == "scissors" || input == "s" || input == "2") {
        return RPS::scissors;
    }
    cout << "Not a valid input.\n";
    return -1; // If no valid inputs, return -1 = false
}

int userChoice() {
    string input;
    int choice = -1;

    // Loop checking for valid input
    while (choice == -1) {
        cout << "Type in your option (Rock, Paper, Scissors): ";
        cin >> input;
        choice = convert(input); // Converts string to int
    }
    return choice;
}

void showChoice(const RPS choice) {
    switch (choice) {
        case rock: cout << "Rock"; break;
        case paper: cout << "Paper"; break;
        case scissors: cout << "Scissors"; break;
        default: cout << "Invalid choice";
    }
}

int winner(const int user, const int comp) {
    // -1 = tie, 0 = lose, 1 = win
    if (user == RPS::rock && comp == RPS::scissors) {
        return 1;
    }
    else if (user == RPS::scissors && comp == RPS::paper) {
        return 1;
    }
    else if (user == RPS::paper && comp == RPS::rock) {
        return 1;
    }
    else if (user == comp) {
        return -1;
    }
    else {
        return 0;
    }
}

int game(const int user, const int comp) {
    cout << "Your choice: ";
    showChoice(static_cast<RPS>(user));
    cout << endl;

    cout << "Computer's choice: ";
    showChoice(static_cast<RPS>(comp));
    cout << endl;

    showChoice(static_cast<RPS>(user));
    cout << " vs. ";
    showChoice(static_cast<RPS>(comp));
    cout << endl;

    return winner(user, comp);
}

int main() {
    int state;
    int user;
    int uScore = 0;
    int comp;
    int cScore = 0;

    while (uScore < 3 && cScore < 3) {
        user = userChoice();
        comp = compChoice();

        state = game(user, comp);

        if (state == 1) {
            cout << "You win!\n";
            uScore++;
        }
        else if (state == 0) {
            cout << "You lose...\n";
            cScore++;
        }
        else {
            cout << "It's a tie, try again.\n\n";
        }

        if (state != -1) {
            cout << "You: " << uScore << " | Computer: " << cScore << "\n\n";
        }
    }

    return 0;
}
