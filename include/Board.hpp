#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#include "Cell.hpp"

enum class CellState {
    Empty, // no ship, not hit
    Ship,  // ship present, not hit
    Hit,   // ship hit
    Miss   // empty cell hit
};

class Board {
public:
    explicit Board(std::size_t size = 10);

    [[nodiscard]] const std::vector<std::vector<Cell>>& getGrid() const;
    [[nodiscard]] std::size_t getSize() const;

    bool placeShip(int x, int y);
    bool receiveAttack(int x, int y);

    [[nodiscard]] std::string displayForOwner() const;
    [[nodiscard]] std::string displayForOpponent() const;

    [[nodiscard]] CellState getCellState(int x, int y, bool ownerView) const;

    [[nodiscard]] std::vector<std::vector<CellState>> getBoardState(bool ownerView) const;

    [[nodiscard]] nlohmann::json toJson(bool ownerView) const;

private:
    std::vector<std::vector<Cell>> m_grid;
    std::size_t m_size;
};

#endif // BOARD_HPP