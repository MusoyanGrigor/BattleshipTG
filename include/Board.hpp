#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include "Cell.hpp"

class Board {
public:
    explicit Board(std::size_t size = 10);

    [[nodiscard]] const std::vector<std::vector<Cell>>& getGrid() const;
    [[nodiscard]] std::size_t getSize() const;

    bool placeShip(int x, int y);
    bool receiveAttack(int x, int y);

    [[nodiscard]] std::string displayForOwner() const;
    [[nodiscard]] std::string displayForOpponent() const;

private:
    std::vector<std::vector<Cell>> m_grid;
    std::size_t m_size;
};

#endif // BOARD_HPP