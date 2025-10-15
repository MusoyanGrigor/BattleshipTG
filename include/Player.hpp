#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "Board.hpp"

class Player {
public:
    Player(std::string id, std::string name);

    Board& getBoard();
    [[nodiscard]] const Board& getBoard() const;
    [[nodiscard]] std::string getId() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::size_t getShipCount() const;
    void setShipCount(std::size_t shipCount);

    void placeShips(std::size_t shipCount);

    static bool attack(Player& opponent, int x, int y); // instance method

private:
    std::string m_id;
    std::string m_name;
    std::size_t m_shipCount = 5;
    Board m_board;
};

#endif // PLAYER_HPP