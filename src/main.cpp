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
void executeSlashCommand(string userInput, const dpp::slashcommand_t& event);
string diceRoll(int low, int up);
void embed(const dpp::slashcommand_t& event, string title, string desc, string imgURL, embedField* fields, unsigned char fieldAmount);
dpp::embed getEmbed(string title, string desc, string imgURL, embedField* fields, unsigned char fieldAmount);

// main
int main(int argc, char** argv) {

    // initialization
    ifstream file("../config.json");
    nlohmann::json data = nlohmann::json::parse(file);
    const string BOT_TOKEN = data["token"];
    dpp::cluster bot(BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        string input = event.command.get_command_name();
        executeSlashCommand(input, event);
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {

            vector<dpp::slashcommand> commands;
            commands.push_back(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
            commands.push_back(dpp::slashcommand("dice", "performs a dice roll (min 1, max 6)", bot.me.id));
            commands.push_back(dpp::slashcommand("embed", "testing embeds rn", bot.me.id));

            // commands.push_back(dpp::slashcommand(name, desc, bot.me.id));
            // bot.global_command_create(dpp::slashcommand(name, desc, bot.me.id));

            bot.global_bulk_command_create(commands);

        }
    });

    bot.start(dpp::st_wait);
    return 0;
}
// functions
void executeSlashCommand(string userInput, const dpp::slashcommand_t& event) {
    string result;
    if (userInput == "ping") {
        event.reply("Bot is ready to operate");
    } else if (userInput == "dice") {
        event.reply(diceRoll(1, 6));
    } else if (userInput == "embed") {

        // obviously not how this will work once implemented properly

        embedField field1{.title = "title", .contents = "this is some content for field1", .value = true}, 
        field2{.title = "title", .contents = "this is some content for field1", .value = true}, 
        field3{.title = "title", .contents = "this is some content for field1", .value = true};

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

void embed(const dpp::slashcommand_t& event, string title, string desc, string imgURL, embedField* fields, unsigned char fieldAmount) {
    dpp::embed embed = getEmbed(title, desc, imgURL, fields, fieldAmount);
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
            .set_text("Footer text")
        )
        .set_timestamp(time(0));
    
    if (imgURL != "none") embed.set_image(imgURL);
    if (thumbnailURL != "none") embed.set_thumbnail(thumbnailURL);

    for (int i = 0; i < fieldAmount; i++) {
        embed.add_field(fields[i].title, fields[i].contents, fields[i].value);
    }

    return embed;
}