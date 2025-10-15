#include "TelegramBot.hpp"
#include <string>

int main() {
    // Replace with your actual Telegram Bot API token
    const std::string token = "Your bot token";

    TelegramBot bot(token);
    bot.start();  // Start polling and handling commands

    return 0;
}
