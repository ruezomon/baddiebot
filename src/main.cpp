#include <dpp/dpp.h>
#include <thread>
#include "../include/json.hpp"

using namespace std;

// globals
const string PING = "ping";
const string PING_DESCRIPTION = "Test connection";

// prototypes
string executeSlashCommand(string userInput);
char diceRoll(int low, int up);

// main
int main(int argc, char** argv) {
    /*
    dpp::cluster bot(BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        string input = event.command.get_command_name();
        event.reply(executeSlashCommand(input));
        if (input == "kill")
            exit(0);
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
            bot.global_command_create(dpp::slashcommand("dice", "performs a dice roll (min 1, max 6)", bot.me.id));
            bot.global_command_create(dpp::slashcommand("kill", "shutdown the bot", bot.me.id));
            // bot.global_command_create(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
            // bot.global_command_create(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
            // bot.global_command_create(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
        }
    });

    bot.start(dpp::st_wait);
    */
   cout << "hello world" << endl;
   return 0;
}
/*
// functions
string executeSlashCommand(string userInput) {
    string result;
    if (userInput == "ping") {
        result = "Bot is ready to operate";
    } else if (userInput == "dice") {
        result = diceRoll(1, 6);
    } else if (userInput == "kill") {
        result = "shutting down...";
    } else {
        result = "Whoops! We haven't designed an output for this command!";
    }
    return result;
}

char diceRoll(int low, int up) {
    srand(time(NULL));
    return (rand() % up + low) + '0';
*/