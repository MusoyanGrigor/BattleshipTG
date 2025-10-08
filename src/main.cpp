#include "TelegramBot.hpp"
#include <cstdlib>
#include <ctime>
#include <string>

int main() {
    // Seed random number generator for ship placement
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Replace with your actual Telegram Bot API token
    const std::string token = "Your bot token";

    TelegramBot bot(token);
    bot.start();  // Start polling and handling commands

    return 0;
}
