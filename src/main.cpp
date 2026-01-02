#include <string>
#include <thread>
#include "crow.h"
#include "TelegramBot.hpp"
#include "Game.hpp"

void addCorsHeaders(crow::response &res);

void startHttpServer(TelegramBot &bot);

int main() {
    const std::string token = "BOT_TOKEN";

    TelegramBot bot(token);

    std::jthread serverThread([&bot]() {
        startHttpServer(bot);
    });

    bot.start();
    return 0;
}

void addCorsHeaders(crow::response &res) {
    res.add_header("Access-Control-Allow-Origin", "*");
    res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type");
    res.add_header("Content-Type", "application/json");
}

void startHttpServer(TelegramBot &bot) {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/<path>")
            .methods("OPTIONS"_method)
            ([](const crow::request &, crow::response &res, std::string) {
                res.code = 200;
                res.add_header("Access-Control-Allow-Origin", "*");
                res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
                res.add_header("Access-Control-Allow-Headers", "Content-Type");
                res.end();
            });


    CROW_ROUTE(app, "/boards/<string>")
    ([&bot](const crow::request &req, const std::string &gameID) {
        const auto game = bot.getGame(gameID);
        if (!game)
            return crow::response(404, "Game not found");

        // Read viewer (1 or 2)
        int viewer = 1;
        if (req.url_params.get("viewer"))
            viewer = std::stoi(req.url_params.get("viewer"));

        Player *me = nullptr;
        Player *opponent = nullptr;

        if (viewer == 1) {
            me = game->getPlayer1().get();
            opponent = game->getPlayer2().get();
        } else if (viewer == 2) {
            me = game->getPlayer2().get();
            opponent = game->getPlayer1().get();
        } else {
            return crow::response(400, "Invalid viewer");
        }

        if (!me || !opponent)
            return crow::response(400, "Both players not ready");

        nlohmann::json result;
        result["myBoard"] = me->getBoard().toJson(true);
        result["opponentBoard"] = opponent->getBoard().toJson(false);

        crow::response res(result.dump());
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    CROW_ROUTE(app, "/move/<string>")
            .methods("POST"_method)
            ([&bot](const crow::request &req, const std::string &gameID) {
                auto game = bot.getGame(gameID);

                if (!game) {
                    crow::response res(404, "Game not found");
                    addCorsHeaders(res);
                    return res;
                }

                auto data = nlohmann::json::parse(req.body);
                int x = data["x"];
                int y = data["y"];

                game->makeMove(game->getPlayer1().get(), x, y);

                crow::response res(
                    game->getPlayer1()->getBoard().toJson(true).dump()
                );
                addCorsHeaders(res);
                return res;
            });

    app.port(18080).multithreaded().run();
}

