#include "Player.hpp"
#include <random>
#include <utility>

Player::Player(std::string id, std::string name)
    : m_id(std::move(id)), m_name(std::move(name)), m_board(10) {}

Board& Player::getBoard() { return m_board; }
const Board& Player::getBoard() const { return m_board; }
std::string Player::getId() const { return m_id; }
std::string Player::getName() const { return m_name; }

void Player::placeShips(const std::size_t shipCount = 5) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution dist(0, 9);

    int placed = 0;
    while (placed < shipCount) {
        const int x = dist(gen);
        const int y = dist(gen);
        if (m_board.placeShip(x, y)) placed++;
    }
}

bool Player::attack(Player& opponent, const int x, const int y) {
    return opponent.getBoard().receiveAttack(x, y);
}