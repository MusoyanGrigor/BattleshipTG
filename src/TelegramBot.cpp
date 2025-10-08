#include "TelegramBot.hpp"
#include "Player.hpp"
#include <cstdlib>
#include <sstream>
#include <random>

TelegramBot::TelegramBot(const std::string& _token) : token(_token) {}

void TelegramBot::start() {
    TgBot::Bot bot(token);
    TgBot::TgLongPoll longPoll(bot);

    bot.getEvents().onCommand("start", [&bot](const TgBot::Message::Ptr& message) {
        handleStartCommand(bot, message);
    });

    bot.getEvents().onCommand("newgame", [this, &bot](const TgBot::Message::Ptr& message) {
        handleNewGameCommand(bot, message);
    });

    bot.getEvents().onCommand("join", [this, &bot](const TgBot::Message::Ptr& message) {
        handleJoinCommand(bot, message);
    });

    bot.getEvents().onCommand("move", [this, &bot](const TgBot::Message::Ptr& message) {
        handleMoveCommand(bot, message);
    });

    printf("Bot started...\n");
    while (true) {
        try { longPoll.start(); }
        catch (const std::exception &e) { printf("Polling error: %s\n", e.what()); }
    }
}


void TelegramBot::handleStartCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message) {
    bot.getApi().sendMessage(message->chat->id, "Welcome to Sea Battle! 🛳\nUse /newgame to start a game.");
}

void TelegramBot::handleNewGameCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message) {
    const std::string chatId = std::to_string(message->chat->id);
    const std::string gameID = generateGameID();

    const auto player1 = new Player(std::to_string(message->from->id), message->from->firstName);
    player1->placeShips();

    auto game = new Game(gameID, player1);
    gamesByID[gameID] = game;
    chatToGame[chatId] = gameID;

    const std::string reply = "New game created! Share this code with your friend to join: " + gameID;
    bot.getApi().sendMessage(message->chat->id, reply);
}

void TelegramBot::handleJoinCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message) {
    std::istringstream iss(message->text);
    std::string cmd, gameID;
    iss >> cmd >> gameID;

    if (!gamesByID.contains(gameID)) {
        bot.getApi().sendMessage(message->chat->id, "Game not found.");
        return;
    }

    Game* game = gamesByID[gameID];
    if (game->player2 != nullptr) {
        bot.getApi().sendMessage(message->chat->id, "Game already has two players.");
        return;
    }

    auto player2 = new Player(std::to_string(message->from->id), message->from->firstName);
    player2->placeShips();
    game->player2 = player2;
    game->state = GameState::IN_PROGRESS;
    chatToGame[std::to_string(message->chat->id)] = gameID;

    bot.getApi().sendMessage(message->chat->id, "You joined the game! It's Player 1's turn.");
    bot.getApi().sendMessage(std::stoll(game->player1->getId()), "Player 2 joined! Make your move with /move x y.");
}

void TelegramBot::handleMoveCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message) {
    const std::string chatId = std::to_string(message->chat->id);
    if (!chatToGame.contains(chatId)) {
        bot.getApi().sendMessage(message->chat->id, "You are not in a game.");
        return;
    }

    Game* game = gamesByID[chatToGame[chatId]];

    if (game->state != GameState::IN_PROGRESS) {
        bot.getApi().sendMessage(message->chat->id, "Game not in progress yet.");
        return;
    }

    Player* player = (game->player1->getId() == chatId) ? game->player1 : game->player2;

    if (player != game->getCurrentPlayer()) {
        bot.getApi().sendMessage(message->chat->id, "Not your turn!");
        return;
    }

    int x, y;
    if (sscanf(message->text.c_str(), "/move %d %d", &x, &y) != 2) {
        bot.getApi().sendMessage(message->chat->id, "Invalid format. Use /move x y");
        return;
    }

    const bool hit = game->makeMove(player, x, y);
    const std::string result = hit ? "💥 Hit!" : "🌊 Miss!";
    bot.getApi().sendMessage(message->chat->id, result);

    const Player* opponent = game->getOpponent(player);
    bot.getApi().sendMessage(std::stoll(opponent->getId()), player->getName() + " made a move at (" + std::to_string(x) + "," + std::to_string(y) + ")");

    if (game->checkWin()) {
        bot.getApi().sendMessage(std::stoll(player->getId()), "🎉 You won!");
        bot.getApi().sendMessage(std::stoll(opponent->getId()), "😢 You lost!");
        gamesByID.erase(game->gameID);
        chatToGame.erase(game->player1->getId());
        chatToGame.erase(game->player2->getId());
        delete game->player1;
        delete game->player2;
        delete game;
    }
}

std::string TelegramBot::generateGameID() {
    static constexpr char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

    std::string code;
    for (int i = 0; i < 6; ++i) code += alphanum[dis(gen)];
    return code;
}