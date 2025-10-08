#ifndef SEABATTLEBOT_BOARD_HPP
#define SEABATTLEBOT_BOARD_HPP

#include <vector>
#include <string>
#include "Cell.hpp"

class Board {
public:
    explicit Board(std::size_t size = 10);

    [[nodiscard]] const std::vector<std::vector<Cell>>& getGrid() const;

    bool placeShip(int x, int y);
    bool receiveAttack(int x, int y);
    [[nodiscard]] std::string display() const;

private:
    std::vector<std::vector<Cell>> m_grid;
    std::size_t m_size;
};

#endif // SEABATTLEBOT_BOARD_HPP
