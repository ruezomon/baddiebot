#include <dpp/dpp.h>
#include <thread>
#include <fstream>
#include <vector>
#include "../include/json.hpp"

using namespace std;

// globals
const string PING = "ping";
const string PING_DESCRIPTION = "Test connection";

// structs
struct embedField {
    string title;
    string contents;
    bool value;
};

// prototypes
void executeSlashCommand(string userInput, const dpp::slashcommand_t& event, dpp::command_interaction cmd_data);
bool isNumber(string strNumber);

string diceRoll(int low, int up);
int* getHighLow(vector<dpp::command_data_option> options);

void embed(const dpp::slashcommand_t& event, string title, string desc, string footer, embedField* fields, unsigned char fieldAmount, string thumbnailURL="none", string imgURL="none");
dpp::embed getEmbed(string title, string desc, string footer, embedField* fields, unsigned char fieldAmount, string thumbnailURL, string imgURL);

// main
int main(int argc, char** argv) {

    // initialization
    ifstream file("../config.json");
    nlohmann::json data = nlohmann::json::parse(file);
    const string BOT_TOKEN = data["token"];
    dpp::cluster bot(BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        dpp::command_interaction cmd_data = event.command.get_command_interaction();
        string input = event.command.get_command_name();
        executeSlashCommand(input, event, cmd_data);
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {

            vector<dpp::slashcommand> commands;
            commands.push_back(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
            commands.push_back(dpp::slashcommand("dice", "performs a dice roll", bot.me.id));
            commands.push_back(dpp::slashcommand("embed", "creates an embed of your choice", bot.me.id));

            // commands.push_back(dpp::slashcommand(name, desc, bot.me.id));
            // bot.global_command_create(dpp::slashcommand(name, desc, bot.me.id));

            bot.global_bulk_command_create(commands);

        }
    });

    bot.start(dpp::st_wait);
    return 0;
}
// functions
void executeSlashCommand(string userInput, const dpp::slashcommand_t& event, dpp::command_interaction cmd_data) {
    string result;
    if (userInput == "ping") {
        event.reply("Bot is ready to operate");
    } else if (userInput == "dice") {
        vector<dpp::command_data_option> subcommands = cmd_data.options;
        if (subcommands.size() != 2) event.reply("Incorrect amount of options!");
        else {
            int* highLow = getHighLow(subcommands);
            int low = highLow[0];
            int high = highLow[1];
            if (low <= high) {
                event.reply("Given values could not be handled");
            } else {
                event.reply(diceRoll(low, high));
            }
            delete[] highLow;
        }
    } else if (userInput == "embed") {

        // obviously not how this will work once implemented properly
        embedField field1{.title = "title", .contents = "this is some content for field1", .value = true}, 
        field2{.title = "title", .contents = "this is some content for field2", .value = true}, 
        field3{.title = "title", .contents = "this is some content for field3", .value = true};

        embedField embedFields[] = {field1, field2, field3}; 

    } else {
        event.reply("Whoops! We haven't designed an output for this command!");
    }
    cout << "executed " << userInput << endl;
}

string diceRoll(int low, int up) {
    srand(time(NULL));
    string result; 
    result[0] = (rand() % up + low) + '0';
    return result;
}

void embed(const dpp::slashcommand_t& event, string title, string desc, string footer, embedField* fields, unsigned char fieldAmount, string thumbnailURL, string imgURL) {
    dpp::embed embed = getEmbed(title, desc, footer, fields, fieldAmount, thumbnailURL, imgURL);
    dpp::message msg(event.command.channel_id, embed);
    event.reply(msg);
}

dpp::embed getEmbed(string title, string desc, string footer, embedField* fields, unsigned char fieldAmount, string thumbnailURL, string imgURL) {
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::sti_blue)
        .set_title(title)
        .set_description(desc)
        .set_footer(
            dpp::embed_footer()
            .set_text(footer)
        )
        .set_timestamp(time(0));
    
    if (imgURL != "none") embed.set_image(imgURL);
    if (thumbnailURL != "none") embed.set_thumbnail(thumbnailURL);

    for (int i = 0; i < fieldAmount; i++) {
        embed.add_field(fields[i].title, fields[i].contents, fields[i].value);
    }

    return embed;
}

bool isNumber(string strNumber) {
    bool result = true;
    for (char character : strNumber) {
        if (!('0' <= character <= '9')) {
            result = false;
            break;
        }
    }
    return result;
}

int* getHighLow(vector<dpp::command_data_option> options) {
    int* result = nullptr;
    string param1 = options[0].name;
    string param2 = options[1].name;
    
    if (isNumber(param1) && isNumber(param2)) {
        result = new int[2];
        result[0] = stoi(param1);
        result[1] = stoi(param2);
    }

    return result;
}