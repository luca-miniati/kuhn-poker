//
// Created by luca miniati on 12/27/24.
//

#ifndef GAME_H
#define GAME_H
#include <map>
#include <node.h>
#include <solver.h>


class Game {
    std::unordered_map<std::string, std::unique_ptr<Node>> solution;
    std::string p1, p2, player_card_string, bot_card_string;
    int player_card, bot_card, player_stack = 10, bot_stack = 10;
    std::vector<int> cards = {1, 2, 3};
    std::map<int, std::string> num_to_card = {{1, "J"}, {2, "Q"}, {3, "K"}};

    /**
     * Gets input from the user to initalize game settings.
     */
    void Setup();

    /**
     * Display the game state to the user.
     */
    void Display();

    /**
     * Display the result of a showdown to the user.
     */
    void DisplayHandResult(std::string h, int res);

    /**
     * Display weclome message to the user.
     */
    void DisplayWelcomeMessage();

    /**
     * Update game state (indexed by `key`) given the terminal history `h`.
     */
    void HandleTerminalState(std::string h, std::string key);

    /**
     * Get a player action from stdin.
     */
    std::string GetPlayerAction();

    /**
     * Get a bot action from the computed solution.
     */
    std::string GetBotAction(std::string h);

    /**
     * Game loop for a hand of Kuhn Poker againt the bot.
     */
    void PlayHand();

public:
    /**
     * Entry point to the game.
     */
    void Play();

};



#endif //GAME_H
