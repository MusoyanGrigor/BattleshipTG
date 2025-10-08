#include <tgbot/tgbot.h>
#include <iostream>

int main() {
    const std::string TELEGRAM_BOT_TOKEN = "Your Token";
    TgBot::Bot bot(TELEGRAM_BOT_TOKEN);

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Sea Battle Bot is running! Type /newgame to start.");
    });

    try {
        bot.getApi().deleteWebhook();
        TgBot::TgLongPoll longPoll(bot);
        std::cout << "Bot started..." << std::endl;
        while (true) longPoll.start();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
