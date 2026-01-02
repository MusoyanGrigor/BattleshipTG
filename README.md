A Telegram-based Battleship game bot with a web interface for board visualization. Built with C++, Crow (HTTP server), and TgBot for Telegram integration.

**Features**
  Play Battleship directly via Telegram.
  Real-time board state updates served via an HTTP server.
  Support for two players per game.
  Board visualization with JSON API endpoints.
  Move handling and game logic implemented in C++.
  Cross-origin support for frontend integration.

**Tech Stack**
  C++ 23 – core game logic and bot.
  TgBot – Telegram bot API integration.
  Crow – lightweight C++ HTTP server.
  Boost – required by TgBot.
  nlohmann/json – JSON serialization.
  Frontend – React for board visualization.

**Usage**
  Run the bot
  ./SeaBattleBot

  Access board via HTTP server
  Default port: 18080
  
  API endpoints:
  GET /boards/<gameID>?viewer=<1|2> – returns board JSON.
  POST /move/<gameID>?viewer=<1|2> – submit a move {x, y}.
  
  Play via Telegram
  Use /newgame, /join, and /move commands with the bot token.
