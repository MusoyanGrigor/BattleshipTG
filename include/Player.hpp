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

    void placeShips(std::size_t shipCount);

    static bool attack(Player& opponent, int x, int y); // instance method

private:
    std::string m_id;
    std::string m_name;
    Board m_board;
};

#endif // PLAYER_HPP