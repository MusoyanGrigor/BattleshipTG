#include "Board.hpp"

Board::Board(const std::size_t size) : m_size(size) {
    m_grid.resize(m_size, std::vector<Cell>(m_size));
}

const std::vector<std::vector<Cell>>& Board::getGrid() const {
    return m_grid;
}

bool Board::placeShip(const int x, const int y) {
    if (x < 0 || x >= m_size || y < 0 || y >= m_size) return false;
    if (!m_grid[y][x].hasShip) { // swap indices for row/col
        m_grid[y][x].hasShip = true;
        return true;
    }
    return false;
}

bool Board::receiveAttack(const int x, const int y) {
    if (x < 0 || x >= m_size || y < 0 || y >= m_size) return false;
    m_grid[y][x].isHit = true; // swap indices for row/col
    return m_grid[y][x].hasShip;
}

std::string Board::displayForOwner() const {
    std::string result = "  ";
    for (int i = 0; i < m_size; ++i) result += std::to_string(i) + " ";
    result += "\n";

    int rowNum = 0;
    for (const auto &row : m_grid) {
        result += std::to_string(rowNum++) + " ";
        for (const auto &cell : row) {
            if (cell.isHit) result += (cell.hasShip ? "💥" : "🌊");
            else result += (cell.hasShip ? "🚢" : "⬜");
            result += " ";
        }
        result += "\n";
    }
    return result;
}

std::string Board::displayForOpponent() const {
    std::string result = "  ";
    for (int i = 0; i < m_size; ++i) result += std::to_string(i) + " ";
    result += "\n";

    int rowNum = 0;
    for (const auto &row : m_grid) {
        result += std::to_string(rowNum++) + " ";
        for (const auto &cell : row) {
            if (cell.isHit) result += (cell.hasShip ? "💥" : "🌊");
            else result += "⬜"; // hide unhit ships
            result += " ";
        }
        result += "\n";
    }
    return result;
}