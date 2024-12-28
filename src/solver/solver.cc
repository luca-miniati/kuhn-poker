//
// Created by luca miniati on 12/27/24.
//

#include <iostream>
#include <map>
#include <iomanip>
#include "solver.h"

std::shared_ptr<Node> Solver::GetNode(const std::string& key) {
    auto it = tree.find(key);
    // if we already made the node, return the ptr
    if (it != tree.end())
        return it->second;

    // otherwise, make the node
    auto node = std::make_shared<Node>(key);
    tree[key] = node;

    // return the ptr
    return node;
}

double Solver::CFR(std::vector<int>& cards, std::string history, double p1, double p2) {
    // load current node
    int player_idx = history.length() % 2;
    std::string info_set = std::to_string(cards[player_idx]) + history;
    std::shared_ptr<Node> node = GetNode(info_set);

    // base case: return payoff for terminal state
    if (node->IsTerminal())
        return node->GetUtility(cards);

    // compute reach probability
    double reach_p = (player_idx == 0) ? p2 : p1;
    // update current player's strategy
    std::vector<double> strategy = node->GetStrategy(reach_p);

    std::vector<double> util(NUM_ACTIONS);
    // utility of this node (the eventual return value of the cfr)
    double node_util = 0;

    // iterate over possible actions
    for (int a = 0; a < NUM_ACTIONS; ++a) {
        char action = ACTIONS[a];
        // player_idx = 0 corresponds to player 1's action
        if (player_idx == 0)
            util[a] = -CFR(cards, history + action, p1 * strategy[a], p2);
        else
            util[a] = -CFR(cards, history + action, p1, p2 * strategy[a]);

        // update total node utility
        node_util += strategy[a] * util[a];
    }

    // update regret for each action
    for (int a = 0; a < NUM_ACTIONS; ++a) {
        double regret = util[a] - node_util;
        node->UpdateRegret(a, reach_p * regret);
    }

    return node_util;
}

double Solver::ComputeTerminalPayoffs(std::vector<int>& cards, std::string history) {
    // load current node
    int player_idx = history.length() % 2;
    std::string info_set = std::to_string(cards[player_idx]) + history;
    std::shared_ptr<Node> node = GetNode(info_set);

    // base case: return payoff for terminal state
    if (node->IsTerminal())
        return node->GetUtility(cards);

    // get avg strategy over training epoch
    std::vector<double> strategy = node->GetAverageStrategy();
    // utility of this node (the eventual return value of the cfr)
    double node_util = 0;

    // iterate over possible actions
    for (int a = 0; a < NUM_ACTIONS; ++a) {
        char action = ACTIONS[a];
        node_util += -ComputeTerminalPayoffs(cards, history + action) * strategy[a];
    }

    return node_util;
}

double Solver::ComputeExpectedValue() {
    double EV = 0;

    std::vector<int> cards = {1, 2, 3};
    // simulate all possible card combos
    for (int i = 0; i < 6; ++i) {
        EV += ComputeTerminalPayoffs(cards, "") / 6;
        next_permutation(cards.begin(), cards.end());
    }

    return EV;
}

void Solver::PrintSolution() {
    // compute EV
    double EV = ComputeExpectedValue();
    std::cout << std::fixed << std::setprecision(4) << "Player 1 EV: " << EV << std::endl;
    std::cout << std::fixed << std::setprecision(4) << "Player 2 EV: " << -EV << std::endl;
    std::cout << std::endl;

    // compute solution for each player
    std::map<int, std::vector<std::pair<std::string, std::vector<double>>>> strategy1, strategy2;
    // card values
    std::map<int, char> cards = {{1, 'J'}, {2, 'Q'}, {3, 'K'}};
    for (const auto& [info_set, node] : this->tree) {
        if (node->IsTerminal())
            continue;
        std::vector<double> strategy = node->GetAverageStrategy();
        int card = info_set[0] - '0';
        std::string h = info_set.substr(1);
        if (info_set.length() % 2)
            strategy1[card].emplace_back(h, strategy);
        else
            strategy2[card].emplace_back(h, strategy);
    }

    // output strategies
    std::cout << "Player 1 Strategy:" << std::endl;
    for (int c = 1; c <= 3; ++c) {
        for (auto [h, strategy] : strategy1[c]) {
            std::cout << std::fixed << std::setprecision(2) <<
            "Card: " << cards[c] <<
            ", History: " << (!h.empty() ? h : "--") <<
            ", Strategy: check " << strategy[0] * 100 <<
            "% | bet " << strategy[1] * 100 << "%" << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << "Player 2 Strategy:" << std::endl;
    for (int c = 1; c <= 3; ++c) {
        for (auto [h, strategy] : strategy2[c]) {
            std::cout << std::fixed << std::setprecision(2) <<
            "Card: " << cards[c] <<
            ", History: " << (!h.empty() ? h : "--") <<
            ", Strategy: check " << strategy[0] * 100 <<
            "% | bet " << strategy[1] * 100 << "%" << std::endl;
        }
    }
    std::cout << std::endl;
}

void Solver::Train(int num_iterations) {
    std::vector cards = {1, 2, 3};

    double util = 0;
    // do 6 permutations to guarantee that we cover all possible states at least once
    for (int i = 0; i < num_iterations + 6; ++i) {
        util += CFR(cards, "", 1, 1);
        std::ranges::next_permutation(cards);
    }
}

std::unordered_map<std::string, std::unique_ptr<Node>> Solver::GetSolution() {
    std::unordered_map<std::string, std::unique_ptr<Node>> solution;
    for (const auto& [info_set, node] : tree)
        solution[info_set] = node->Clone();
    return solution;
}