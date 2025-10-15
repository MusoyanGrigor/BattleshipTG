#ifndef TELEGRAMBOT_HPP
#define TELEGRAMBOT_HPP

#include <tgbot/tgbot.h>
#include <unordered_map>
#include <string>
#include <memory>
#include "Game.hpp"

class TelegramBot {
public:
    explicit TelegramBot(std::string  token);
    void start();

private:
    std::string m_token;
    std::unordered_map<std::string, std::shared_ptr<Game>> m_gamesByID;
    std::unordered_map<std::string, std::string> m_chatToGame;

    void handleNewGameCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);
    void handleJoinCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);
    void handleMoveCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);

    static std::string generateGameID();
};

#endif // TELEGRAMBOT_HPP