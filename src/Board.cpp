#include "Board.hpp"

Board::Board(const std::size_t size) : m_size(size) {
    m_grid.resize(m_size, std::vector<Cell>(m_size));
}

const std::vector<std::vector<Cell>>& Board::getGrid() const {
    return m_grid;
}

bool Board::placeShip(const int x, const int y) {
    if (x < 0 || x >= m_size || y < 0 || y >= m_size) return false;
    if (!m_grid[x][y].hasShip) {
        m_grid[x][y].hasShip = true;
        return true;
    }
    return false;
}

bool Board::receiveAttack(const int x, const int y) {
    if (x < 0 || x >= m_size || y < 0 || y >= m_size) return false;
    m_grid[x][y].isHit = true;
    return m_grid[x][y].hasShip;
}

std::string Board::display() const {
    std::string result;
    for (const auto &row : m_grid) {
        for (const auto &cell : row) {
            if (cell.isHit) result += (cell.hasShip ? "💥" : "🌊");
            else result += (cell.hasShip ? "🚢" : "⬜");
        }
        result += "\n";
    }
    return result;
}