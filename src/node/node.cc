//
// Created by luca miniati on 12/27/24.
//

#include "node.h"

Node::Node(std::string info_set) : info_set(info_set) {
    // zero out all arrays
    this->regret_sum.resize(NUM_ACTIONS);
    this->strategy.resize(NUM_ACTIONS);
    // init uniform strategy
    for (int a = 0; a < NUM_ACTIONS; ++a) this->strategy[a] = 1.0 / NUM_ACTIONS;
    this->strategy_sum.resize(NUM_ACTIONS);
}

Node::Node(std::string info_set, std::vector<double> regret_sum, std::vector<double> strategy,
     std::vector<double> strategy_sum)
    : info_set(info_set), regret_sum(regret_sum), strategy(strategy), strategy_sum(strategy_sum) {}

bool Node::IsTerminal() {
    // extract history from info set
    std::string h = this->info_set.substr(1);
    return TERMINAL_HISTORIES.contains(h);
}

double Node::GetUtility(std::vector<int>& cards) {
    if (!this->IsTerminal())
        throw std::runtime_error("called get_payoff on non-terminal node.");

    // extract history from info set
    std::string h = this->info_set.substr(1);
    int t = h.length();

    // we bet and they folded
    if (h.substr(t - 2) == "bc")
        return 1;

    int c1 = cards[t % 2];
    int c2 = cards[1 - (t % 2)];

    // action went check check
    if (h == "cc")
        return (c1 > c2) ? 1 : -1;

    // action went bet call
    return (c1 > c2) ? 2 : -2;
}

std::vector<double> Node::GetStrategy(double p) {
    double norm = 0;
    for (int a = 0; a < NUM_ACTIONS; a++) {
        this->strategy[a] = fmax(this->regret_sum[a], 0.0);
        norm += this->strategy[a];
    }
    for (int a = 0; a < NUM_ACTIONS; a++) {
        if (norm > 0)
            this->strategy[a] /= norm;
        else
            this->strategy[a] = 1.0 / NUM_ACTIONS;
        this->strategy_sum[a] += p * this->strategy[a];
    }
    return this->strategy;
}

void Node::UpdateRegret(int a, double v) {
    this->regret_sum[a] += v;
}

std::vector<double> Node::GetAverageStrategy() {
    std::vector<double> average_strategy(NUM_ACTIONS);
    double norm = 0;
    for (int a = 0; a < NUM_ACTIONS; ++a)
        norm += this->strategy_sum[a];
    for (int a = 0; a < NUM_ACTIONS; ++a) {
        if (norm > 0)
            average_strategy[a] = this->strategy_sum[a] / norm;
        else
            average_strategy[a] = 1.0 / NUM_ACTIONS;
    }
    return average_strategy;
}

std::unique_ptr<Node> Node::Clone() {
  return std::make_unique<Node>(this->info_set, this->regret_sum, this->strategy,
                                this->strategy_sum);
}