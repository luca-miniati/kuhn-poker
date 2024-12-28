//
// Created by luca miniati on 12/27/24.
//

#ifndef NODE_H
#define NODE_H
#include <string>
#include <unordered_set>
#include <vector>

const std::string ACTIONS = "cb";
const int NUM_ACTIONS = ACTIONS.length();
const std::unordered_set<std::string> TERMINAL_HISTORIES = {"cc", "bb", "bc", "cbc", "cbb"};

/**
 * Represents a node in the game tree of Kuhn Poker. Each node represents a decision point, and
 * stores a corresponding strategy. Additionally, each node stores the sum of regrets and sum of
 * strategies over the training epoch.
 */
class Node {
private:
    std::string info_set;
    std::vector<double> regret_sum, strategy, strategy_sum;
public:
    explicit Node(std::string info_set);

    /**
     * Alternate constructor for copying.
     */
    Node(std::string info_set, std::vector<double> regret_sum, std::vector<double> strategy,
         std::vector<double> strategy_sum);

    /**
     * Returns whether the game is over in this state.
     */
    bool IsTerminal();

    /**
     * 	Returns payoff for terminal state.
     */
    double GetUtility(std::vector<int>& cards);

    /**
     * Update strategy using regret matching, using p as the reachc probability of being in this
     * state.
     */
    std::vector<double> GetStrategy(double p);

    /**
     * Update regret value at the action `a`.
     */
    void UpdateRegret(int a, double v);

    /**
     * Return computed strategy at this node.
     */
    std::vector<double> GetAverageStrategy();

    /**
     * Return a smart pointer to a clone of this node.
     */
    std::unique_ptr<Node> Clone();
};




#endif //NODE_H
