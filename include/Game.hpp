#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include <memory>
#include <string>

enum class GameState { WAITING_FOR_PLAYER, IN_PROGRESS, FINISHED };

class Game {
public:
    Game(std::string id, std::shared_ptr<Player> p1);

    [[nodiscard]] std::string getGameID() const;
    [[nodiscard]] std::shared_ptr<Player> getPlayer1() const;
    [[nodiscard]] std::shared_ptr<Player> getPlayer2() const;
    [[nodiscard]] int getCurrentTurn() const;
    [[nodiscard]] GameState getGameState() const;

    void setPlayer1(std::shared_ptr<Player> p1);
    void setPlayer2(std::shared_ptr<Player> p2);

    Player* getCurrentPlayer();
    [[nodiscard]] const Player* getCurrentPlayer() const;

    Player* getOpponent(Player* player) const;
    const Player* getOpponent(const Player* player) const;

    bool makeMove(Player* player, int x, int y);
    [[nodiscard]] bool checkWin() const;

private:
    std::string m_gameID;
    std::shared_ptr<Player> m_player1;
    std::shared_ptr<Player> m_player2;
    int m_currentTurn; // 1 or 2
    GameState m_state;
};

#endif // GAME_HPP