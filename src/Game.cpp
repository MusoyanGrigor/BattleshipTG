#include "Game.hpp"

Game::Game(std::string id, Player* p1)
    : gameID(std::move(id)), player1(p1), player2(nullptr),
      currentTurn(1), state(GameState::WAITING_FOR_PLAYER) {}

Player* Game::getCurrentPlayer() {
    return currentTurn == 1 ? player1 : player2;
}

const Player* Game::getCurrentPlayer() const {
    return currentTurn == 1 ? player1 : player2;
}

Player* Game::getOpponent(Player* player) const {
    return (player == player1) ? player2 : player1;
}

const Player* Game::getOpponent(const Player* player) const {
    return (player == player1) ? player2 : player1;
}

bool Game::makeMove(Player* player, const int x, const int y) {
    if (state != GameState::IN_PROGRESS) return false;
    if (player != getCurrentPlayer()) return false;

    const bool hit = Player::attack(getOpponent(player)->getBoard(), x, y);

    currentTurn = 3 - currentTurn;

    return hit;
}

bool Game::checkWin() const {
    const Player* opponent = getCurrentPlayer();
    const auto& grid = getOpponent(opponent)->getBoard().getGrid();
    for (const auto &row : grid)
        for (const auto &cell : row)
            if (cell.hasShip && !cell.isHit) return false;
    return true;
}