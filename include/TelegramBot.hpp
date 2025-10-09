#ifndef TELEGRAMBOT_HPP
#define TELEGRAMBOT_HPP

#include <tgbot/tgbot.h>
#include <unordered_map>
#include <string>
#include <memory>
#include "Game.hpp"

class TelegramBot {
public:
    std::string token;
    std::unordered_map<std::string, std::shared_ptr<Game>> gamesByID;
    std::unordered_map<std::string, std::string> chatToGame;

    explicit TelegramBot(std::string  _token);
    void start();

private:
    void handleNewGameCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);
    void handleJoinCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);
    void handleMoveCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);

    static std::string generateGameID();
};

#endif // TELEGRAMBOT_HPP