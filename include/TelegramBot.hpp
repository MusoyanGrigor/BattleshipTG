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

    std::shared_ptr<Game> getGame(const std::string& gameID) const {
        auto it = m_gamesByID.find(gameID);
        if (it != m_gamesByID.end())
            return it->second;
        return nullptr;
    }

    void addGame(const std::string& gameID, std::shared_ptr<Game> game) {
        m_gamesByID[gameID] = std::move(game);
    }

    void sendPhotoToGame(const std::string& gameID,
                     const std::string& filePath,
                     const std::string& caption = "");

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