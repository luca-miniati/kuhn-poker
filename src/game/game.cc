//
// Created by luca miniati on 12/27/24.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include "game.h"

void Game::Setup() {
    std::cout << "*  Enter difficulty level:" << std::endl << "   (1/2/3) ";
    int difficulty; std::cin >> difficulty;
    while (difficulty != 1 && difficulty != 2 && difficulty != 3) {
        std::cout << "   (1/2/3) ";
        std::cin >> difficulty;
    }

    std::cout << std::endl;
    std::cout << "-> Training the algorithm..." << std::endl;
    Solver solver;
    clock_t start_time = clock();
    switch (difficulty) {
        case 1:
            solver.Train(1);
        break;
        case 2:
            solver.Train(100);
        break;
        case 3:
            solver.Train(500000);
        break;
    }
    std::cout << "-> Done! Trained for " << std::fixed << std::setprecision(4) <<
    static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl << std::endl;

    this->solution = solver.GetSolution();
    std::cout << "*  Choose player: (player 1 goes first, player 2 goes second)" << std::endl << "   (1/2) ";
    int p; std::cin >> p;
    while (p != 1 && p != 2) {
        std::cout << "   (1/2) ";
        std::cin >> p;
    }
    this->p1 = (p == 1) ? "Player" : "Bot";
    this->p2 = (p == 1) ? "Bot" : "Player";
    std::cout << std::endl;
}

void Game::Display() {
    std::cout << "\n\n\n\n\n\n\n\n\n";
    std::cout <<
    "+---------------------------------+" << std::endl <<
    "|            +-------+            |" << std::endl <<
    "|            |  Bot  |            |" << std::endl <<
    "|            +-------+            |" << std::endl <<
    "|           +---+                 |" << std::endl <<
    "|   Card:   | ? |     Stack: " << std::setw(2) << std::to_string(bot_stack) << "   |" << std::endl <<
    "|           +---+                 |" << std::endl <<
    "|                                 |" << std::endl <<
    "|           +---+                 |" << std::endl <<
    "|   Card:   | " << this->num_to_card[player_card] << " |     Stack: " << std::setw(2) << std::to_string(player_stack) << "   |" << std::endl <<
    "|           +---+                 |" << std::endl <<
    "|           +----------+          |" << std::endl <<
    "|           |  Player  |          |" << std::endl <<
    "|           +----------+          |" << std::endl <<
    "+---------------------------------+" << std::endl << std::endl;
}

void Game::DisplayHandResult(std::string h, int res) {
    if (h.back() == 'c' && h != "cc") {
        // 0 if p1 folded, 1 if p2 folded
        int folded = (h.length() + 1) % 2;
        std::cout << "!  " << ((folded) ? this->p2 : this->p1) << " folds" << std::endl;
    } else {
        // showdown
        std::cout << "!  Bot shows " << this->num_to_card[this->bot_card];
        std::cout << ", Player shows " << this->num_to_card[this->player_card] << std::endl;
    }

    // display change in stacks
    if (res > 0)
        std::cout << "+  Player wins " << std::to_string(abs(res)) <<
        " chip" << "s "[abs(res) == 1] << std::endl << std::endl;
    else
        std::cout << "-  Player loses " << std::to_string(abs(res)) <<
        " chip" << "s "[abs(res) == 1] << std::endl << std::endl;
}

void Game::DisplayWelcomeMessage() {
    // display ascii art
    std::cout << std::endl;
    std::cout <<
    "                                                        ****___*****" << std::endl <<
    "                                                      **____***____**__***" << std::endl <<
    "                                                    **___***____*******___**" << std::endl <<
    "                                                  *********************************" << std::endl <<
    "                                               *****------------------------------*" << std::endl <<
    "                                            ****__**------------------------------*" << std::endl <<
    "                                          ***_**__*_------------------------------*" << std::endl <<
    "                                       ***_**_***_*_------------------------------*" << std::endl <<
    "                                        **__*_*_****------------------------------*" << std::endl <<
    "                                         **_*_******------------------------------*" << std::endl <<
    "                                                                                   .-'''-.                                        " << std::endl <<
    "                                                                                  '   _    \\                                      " << std::endl <<
    "     .                   .            _..._             _________   _...._      /   /` '.   \\    .          __.....__             " << std::endl <<
    "   .'|                 .'|          .'     '.           \\        |.'      '-.  .   |     \\  '  .'|      .-''         '.           " << std::endl <<
    " .'  |                <  |         .   .-.   .           \\        .'```'.    '.|   '      |  .'  |     /     .-''\"'-.  `..-,.--.  " << std::endl <<
    "<    |                 | |         |  '   '  |            \\      |       \\     \\    \\     / <    |    /     /________\\   |  .-. | " << std::endl <<
    " |   | ____     _    _ | | .'''-.  |  |   |  |             |     |        |    |`.   ` ..' / |   | ___|                  | |  | | " << std::endl <<
    " |   | \\ .'    | '  / || |/.'''. \\ |  |   |  |             |      \\      /    .    '-...-'`  |   | \\ .\\    .-------------| |  | | " << std::endl <<
    " |   |/  .    .' | .' ||  /    | | |  |   |  |             |     |\\`'-.-'   .'               |   |/  . \\    '-.____...---| |  '-  " << std::endl <<
    " |    /\\  \\   /  | /  || |     | | |  |   |  |             |     | '-....-'`                 |    /\\  \\ `.             .'| |      " << std::endl <<
    " |   |  \\  \\ |   `'.  || |     | | |  |   |  |            .'     '.                          |   |  \\  \\  `''-...... -'  | |      " << std::endl <<
    " '    \\  \\  \'   .'|  '| '.    | '.|  |   |  |          '-----------'                        '    \\  \\  \\                |_|      " << std::endl <<
    "'------'  '---`-'  `--''---'   '---'--'   '--'                                              '------'  '---'                       " << std::endl <<
    "                                               *****------------------------------*" << std::endl <<
    "                                                ***_------------------------------*" << std::endl <<
    "                                                 **_------------------------------*" << std::endl <<
    "                                                  **------------------------------*" << std::endl <<
    "                                                  **------------------------------*" << std::endl <<
    "                                                   **__****************************" << std::endl <<
    "                                                   ************" << std::endl <<
    "                                                      ******" << std::endl;

    std::cout << "Welcome to Kuhn Poker!" << std::endl << std::endl;
}

void Game::HandleTerminalState(std::string h, std::string key) {
    // get result of showdown
    int res = static_cast<int>(this->solution[key]->GetUtility(this->cards));
    // normalize res wrt p1
    if (h.length() == 3)
        res = -res;
    // normalize res wrt player
    if (p1 == "Bot")
        res = -res;

    // show res
    DisplayHandResult(h, res);
    player_stack += res;
    bot_stack -= res;

    // wait for player to hit enter
    std::cout << "(Enter) to continue" << std::endl;
    std::cin.ignore();
    char temp = 'x';
    while (temp != '\n')
        std::cin.get(temp);

}

std::string Game::GetPlayerAction() {
    std::cout << "*  Check or bet:" << std::endl << "(c/b) ";
    std::string c; std::cin >> c;
    while (c != "c" && c != "b") {
        std::cout << "   (c/b) ";
        std::cin >> c;
    }
    std::cout << "-> Player plays " << ((c == "c") ? "check" : "bet") << std::endl << std::endl;
    return c;
}

std::string Game::GetBotAction(std::string h) {
    // get the optimal strategy
    std::vector<double> strategy = this->solution[this->bot_card_string + h]->GetAverageStrategy();
    // pick the move
    double r = ((double) rand()) / ((double) RAND_MAX);
    std::cout << "-> Bot plays " << ((r <= strategy[0]) ? "check" : "bet") << std::endl << std::endl;
    return (r <= strategy[0]) ? "c" : "b";
}

void Game::PlayHand() {
    this->Display();
    std::string h;
    std::string turn = p1;
    // while hand is running
    while (!TERMINAL_HISTORIES.count(h)) {
        if (turn == "Player") {
            // get player's move
            h += GetPlayerAction();
        } else {
            h += GetBotAction(h);
        }

        std::string key = "1" + h;
        // check if that move ended the game
        if (this->solution[key]->IsTerminal())
            // if it did, do this
            HandleTerminalState(h, key);

        turn = (turn == "Player") ? "Bot" : "Player";
    }
}

void Game::Play() {
    this->DisplayWelcomeMessage();
    this->Setup();

    // make rng
    auto rd = std::random_device();
    auto rng = std::default_random_engine(rd());

    // game loop
    bool is_game_over = false;
    while (!is_game_over) {
        std::ranges::shuffle(cards, rng);
        bool oop = (this->p1 == "Player");
        this->player_card = cards[oop ? 0 : 1];
        this->player_card_string = std::to_string(cards[oop ? 0 : 1]);
        this->bot_card = cards[oop ? 1 : 0];
        this->bot_card_string = std::to_string(cards[oop ? 1 : 0]);

        this->PlayHand();
        if (player_stack <= 0 || bot_stack <= 0)
            is_game_over = true;
    }

    std::cout << "Game over!" << std::endl;
    std::cout << ((player_stack <= 0) ? "Bot" : "Player") << " won." << std::endl;
}