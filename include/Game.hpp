#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include <memory>
#include <string>

enum class GameState { WAITING_FOR_PLAYER, IN_PROGRESS, FINISHED };

class Game {
public:
    std::string gameID;
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
    int currentTurn; // 1 or 2
    GameState state;

    Game(std::string id, std::shared_ptr<Player> p1);

    Player* getCurrentPlayer();
    const Player* getCurrentPlayer() const;

    Player* getOpponent(Player* player) const;
    const Player* getOpponent(const Player* player) const;

    bool makeMove(Player* player, int x, int y);
    bool checkWin() const;
};

#endif // GAME_HPP