#include "Game.hpp"

Game::Game(std::string id, std::shared_ptr<Player> p1)
    : gameID(std::move(id)), player1(std::move(p1)), player2(nullptr),
      currentTurn(1), state(GameState::WAITING_FOR_PLAYER) {}

Player* Game::getCurrentPlayer() {
    return currentTurn == 1 ? player1.get() : player2.get();
}

const Player* Game::getCurrentPlayer() const {
    return currentTurn == 1 ? player1.get() : player2.get();
}

Player* Game::getOpponent(Player* player) const {
    return (player == player1.get()) ? player2.get() : player1.get();
}

const Player* Game::getOpponent(const Player* player) const {
    return (player == player1.get()) ? player2.get() : player1.get();
}

bool Game::makeMove(Player* player, const int x, const int y) {
    if (state != GameState::IN_PROGRESS) return false;
    if (player != getCurrentPlayer()) return false;

    const bool hit = player->attack(*getOpponent(player), x, y);
    currentTurn = 3 - currentTurn;
    return hit;
}

bool Game::checkWin() const {
    const Player* opponent = getOpponent(getCurrentPlayer());
    const auto& grid = opponent->getBoard().getGrid();
    for (const auto &row : grid)
        for (const auto &cell : row)
            if (cell.hasShip && !cell.isHit) return false;
    return true;
}