#include "TelegramBot.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include <sstream>
#include <random>
#include <utility>

TelegramBot::TelegramBot(std::string token) : m_token(std::move(token)) {}

void TelegramBot::start() {
    TgBot::Bot bot(m_token);
    TgBot::TgLongPoll longPoll(bot);

    bot.getEvents().onCommand("start", [&bot](const TgBot::Message::Ptr& message) {
        bot.getApi().sendMessage(message->chat->id,
            "Welcome to Sea Battle! 🛳\nUse /newgame to start a game.");
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

    std::cout << "Bot started..." << '\n';
    while (true) {
        try { longPoll.start(); }
        catch (const std::exception &e) { std::cout << "Polling error: " << e.what() << '\n'; }
    }
}

void TelegramBot::handleNewGameCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message) {
    const std::string chatId = std::to_string(message->chat->id);
    const std::string gameID = generateGameID();
    constexpr std::size_t shipCount = 9;

    auto player1 = std::make_shared<Player>(std::to_string(message->from->id), message->from->firstName);
    player1->placeShips(shipCount);

    const auto game = std::make_shared<Game>(gameID, player1);
    m_gamesByID[gameID] = game;
    m_chatToGame[chatId] = gameID;

    bot.getApi().sendMessage(message->chat->id,
        "New game created! Share this code: " + gameID);
}

void TelegramBot::handleJoinCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message) {
    std::istringstream iss(message->text);
    std::string cmd, gameID;
    iss >> cmd >> gameID;

    if (!m_gamesByID.contains(gameID)) {
        bot.getApi().sendMessage(message->chat->id, "Game not found.");
        return;
    }

    const auto game = m_gamesByID[gameID];
    if (game->getPlayer2() != nullptr) {
        bot.getApi().sendMessage(message->chat->id, "Game already has two players.");
        return;
    }

    auto player2 = std::make_shared<Player>(std::to_string(message->from->id), message->from->firstName);
    player2->placeShips(5);
    game->setPlayer2(player2);
    m_chatToGame[std::to_string(message->chat->id)] = game->getGameID();

    bot.getApi().sendMessage(message->chat->id, "You joined the game! It's Player 1's turn.");
    bot.getApi().sendMessage(std::stoll(game->getPlayer1()->getId()), "Player 2 joined! Make your move with /move x y.");
}

void TelegramBot::handleMoveCommand(const TgBot::Bot& bot, const TgBot::Message::Ptr &message) {
    const std::string chatId = std::to_string(message->chat->id);
    if (!m_chatToGame.contains(chatId)) {
        bot.getApi().sendMessage(message->chat->id, "You are not in a game.");
        return;
    }

    auto game = m_gamesByID[m_chatToGame[chatId]];
    if (game->getGameState() != GameState::IN_PROGRESS) {
        bot.getApi().sendMessage(message->chat->id, "Game not in progress yet.");
        return;
    }

    Player* player = (game->getPlayer1()->getId() == chatId) ? game->getPlayer1().get() : game->getPlayer2().get();
    if (player != game->getCurrentPlayer()) {
        bot.getApi().sendMessage(message->chat->id, "Not your turn!");
        return;
    }

    const std::string text = message->text;
    std::istringstream iss(text);

    std::string command{""};
    int x{0};
    int y{0};

    if (!(iss >> command >> x >> y) || command != "/move") {
        bot.getApi().sendMessage(message->chat->id, "Invalid format. Use /move x y");
        return;
    }

    if (x < 0 || x > 9 || y < 0 || y > 9) {
        bot.getApi().sendMessage(message->chat->id, "Coordinates must be between 0 and 9.");
        return;
    }

    const bool hit = game->makeMove(player, x, y);
    bot.getApi().sendMessage(message->chat->id, hit ? "💥 Hit!" : "🌊 Miss!");

    const Player* opponent = game->getOpponent(player);
    bot.getApi().sendMessage(std::stoll(opponent->getId()),
        player->getName() + " made a move at (" + std::to_string(x) + "," + std::to_string(y) + ")");

    // Send boards after move
    bot.getApi().sendMessage(chatId, "Your board:\n" + player->getBoard().displayForOwner());
    bot.getApi().sendMessage(chatId, "Opponent's board:\n" + opponent->getBoard().displayForOpponent());

    // Check win
    if (game->checkWin()) {
        bot.getApi().sendMessage(std::stoll(player->getId()), "🎉 You won!");
        bot.getApi().sendMessage(std::stoll(opponent->getId()), "😢 You lost!");

        m_chatToGame.erase(game->getPlayer1()->getId());
        m_chatToGame.erase(game->getPlayer2()->getId());
        m_gamesByID.erase(game->getGameID());
    }
}

std::string TelegramBot::generateGameID() {
    static constexpr char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

    std::string code;
    for (int i = 0; i < 6; ++i) code += alphanum[dis(gen)];
    return code;
}