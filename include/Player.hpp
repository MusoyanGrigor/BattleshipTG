#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "Board.hpp"

class Player {
public:
    Player(std::string id, std::string name);

    Board& getBoard();
    [[nodiscard]] const Board& getBoard() const;

    void placeShips(std::size_t shipCount);

    static bool attack(Player& opponent, int x, int y); // instance method

    std::string getId() const;
    std::string getName() const;

private:
    std::string m_id;
    std::string m_name;
    Board m_board;
};

#endif // PLAYER_HPP