add_library(solver_lib
        node/node.cc
        solver/solver.cc
)
target_include_directories(solver_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/node
        ${CMAKE_CURRENT_SOURCE_DIR}/solver
)

add_library(game_lib
        game/game.cc
)
target_include_directories(game_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/game
        ${CMAKE_CURRENT_SOURCE_DIR}/node
        ${CMAKE_CURRENT_SOURCE_DIR}/solver
)

add_executable(RunSolver run_solver.cc)
target_link_libraries(RunSolver solver_lib)

add_executable(RunGame run_game.cc)
target_link_libraries(RunGame solver_lib game_lib)
