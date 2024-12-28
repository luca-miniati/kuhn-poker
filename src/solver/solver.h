//
// Created by luca miniati on 12/27/24.
//

#ifndef SOLVER_H
#define SOLVER_H
#include <node.h>
#include <unordered_map>


/**
 * Solver implementing chance-sampled CFR to Kuhn Poker.
 */
class Solver {
    // maps information set -> Node
    std::unordered_map<std::string, std::shared_ptr<Node>> tree;

    /**
     * Returns a smart pointer to the Node corresponding to `key`. If such a node does not exist,
     * it is created and returned.
     */
    std::shared_ptr<Node> GetNode(const std::string& key);

    /**
     * Runs an iteration of CFR, returning the average game value of all terminal states.
     */
    double CFR(std::vector<int>& cards, std::string history, double p1, double p2);

    /**
     * Get the average utility of all the terminal nodes. This is equivalent to the expected value
     * of P1.
     */
    double ComputeTerminalPayoffs(std::vector<int>& cards, std::string history);

public:
    /**
     * Trains the CFR algorithm for a set number of iterations.
     */
    void Train(int num_iterations);

    /**
     * Returns a copy of the computed solution.
     */
    std::unordered_map<std::string, std::unique_ptr<Node>> GetSolution();

    /**
     * Returns the expected value of P1.
     */
    double ComputeExpectedValue();

    /**
     * Prints to stdout the result of training. That is, this prints the frequencies at which each
     * player should play.
     */
    void PrintSolution();

};



#endif //SOLVER_H
