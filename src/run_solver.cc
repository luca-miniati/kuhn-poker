//
// Created by luca miniati on 12/27/24.
//
#include "game/game.h"

int main() {
    int N = 1000000; // number of iterations

    Solver solver;
    solver.Train(N);

    solver.PrintSolution();
}