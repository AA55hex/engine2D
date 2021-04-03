add_executable(game-3.4 src/game.cpp)
target_link_libraries(game-3.4 PRIVATE engine)
target_compile_features(game-3.4 PRIVATE cxx_std_17)
