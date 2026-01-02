#include "Board.hpp"

Board::Board(const std::size_t size) : m_size(size) {
    m_grid.resize(m_size, std::vector<Cell>(m_size));
}

const std::vector<std::vector<Cell>>& Board::getGrid() const {
    return m_grid;
}

std::size_t Board::getSize() const {
    return m_size;
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

CellState Board::getCellState(const int x, const int y, const bool ownerView) const {
    if (x < 0 || x >= static_cast<int>(m_size) || y < 0 || y >= static_cast<int>(m_size))
        return CellState::Empty; // or throw an exception

    const auto&[hasShip, isHit] = m_grid[y][x];
    if (isHit) return hasShip ? CellState::Hit : CellState::Miss;
    if (hasShip && ownerView) return CellState::Ship;
    return CellState::Empty;
}

std::vector<std::vector<CellState>> Board::getBoardState(const bool ownerView) const {
    std::vector state(m_size, std::vector<CellState>(m_size));
    for (size_t y = 0; y < m_size; ++y)
        for (size_t x = 0; x < m_size; ++x)
            state[y][x] = getCellState(x, y, ownerView);
    return state;
}

std::string Board::displayForOwner() const {
    std::string result;

    for (const auto &row : m_grid) {
        for (const auto &[hasShip, isHit] : row) {
            if (isHit) result += (hasShip ? "💥" : "🌊");
            else result += (hasShip ? "🚢" : "⬜");
            result += " ";
        }
        result += "\n";
    }
    return result;
}

std::string Board::displayForOpponent() const {
    std::string result;

    for (const auto &row : m_grid) {
        for (const auto &[hasShip, isHit] : row) {
            if (isHit) result += (hasShip ? "💥" : "🌊");
            else result += "⬜"; // hide unhit ships
            result += " ";
        }
        result += "\n";
    }
    return result;
}

nlohmann::json Board::toJson(const bool ownerView) const {
    nlohmann::json j;
    j["size"] = m_size;
    j["cells"] = nlohmann::json::array();

    for (size_t y = 0; y < m_size; ++y) {
        nlohmann::json row;
        for (size_t x = 0; x < m_size; ++x) {
            switch (getCellState(x, y, ownerView)) {
                case CellState::Empty: row.push_back("empty"); break;
                case CellState::Ship:  row.push_back("ship");  break;
                case CellState::Hit:   row.push_back("hit");   break;
                case CellState::Miss:  row.push_back("miss");  break;
            }
        }
        j["cells"].push_back(row);
    }
    return j;
}