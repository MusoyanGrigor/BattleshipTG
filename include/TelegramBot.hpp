#pragma once
#include <tgbot/tgbot.h>
#include <unordered_map>
#include <string>
#include "Game.hpp"

class TelegramBot {
public:
    std::string token;
    std::unordered_map<std::string, Game*> gamesByID;
    std::unordered_map<std::string, std::string> chatToGame;

    explicit TelegramBot(const std::string& _token);

    void start();

private:
    static void handleStartCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);
    void handleNewGameCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);
    void handleJoinCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);
    void handleMoveCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message);

    static std::string generateGameID();
};