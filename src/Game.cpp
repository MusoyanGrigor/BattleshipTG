#include "Game.hpp"

Game::Game(std::string id, std::shared_ptr<Player> p1)
    : m_gameID(std::move(id)),
      m_player1(std::move(p1)),
      m_player2(nullptr),
      m_currentTurn(1),
      m_state(GameState::WAITING_FOR_PLAYER) {
}

// Getters
std::string Game::getGameID() const { return m_gameID; }
std::shared_ptr<Player> Game::getPlayer1() const { return m_player1; }
std::shared_ptr<Player> Game::getPlayer2() const { return m_player2; }
int Game::getCurrentTurn() const { return m_currentTurn; }
GameState Game::getGameState() const { return m_state; }

void Game::setPlayer1(std::shared_ptr<Player> p1) {
    m_player1 = std::move(p1);
    if (m_player1 && m_player2) m_state = GameState::IN_PROGRESS;
}

// Setter for player 2
void Game::setPlayer2(std::shared_ptr<Player> p2) {
    m_player2 = std::move(p2);
    if (m_player1 && m_player2) m_state = GameState::IN_PROGRESS;
}

// Current player
Player *Game::getCurrentPlayer() {
    return (m_currentTurn == 1) ? m_player1.get() : m_player2.get();
}

const Player *Game::getCurrentPlayer() const {
    return (m_currentTurn == 1) ? m_player1.get() : m_player2.get();
}

// Opponent
Player *Game::getOpponent(Player *player) const {
    if (!player) return nullptr;
    return (player == m_player1.get()) ? m_player2.get() : m_player1.get();
}

const Player *Game::getOpponent(const Player *player) const {
    if (!player) return nullptr;
    return (player == m_player1.get()) ? m_player2.get() : m_player1.get();
}

// Make move
bool Game::makeMove(Player *player, const int x, const int y) {
    if (m_state != GameState::IN_PROGRESS) return false;
    if (!player || !getOpponent(player)) return false;
    if (player != getCurrentPlayer()) return false;

    const bool hit = Player::attack(*getOpponent(player), x, y);
    if (!hit) m_currentTurn = 3 - m_currentTurn; // Switch turn
    return hit;
}

// Check win
bool Game::checkWin() const {
    const Player *opponent = getOpponent(getCurrentPlayer());
    if (!opponent) return false;

    const auto &grid = opponent->getBoard().getGrid();
    for (const auto &row: grid)
        for (const auto &cell: row)
            if (cell.hasShip && !cell.isHit) return false;

    return true;
}