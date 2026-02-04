#include <dpp/dpp.h>
#include <thread>
#include <fstream>
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
char diceRoll(int low, int up);
void embed(const dpp::slashcommand_t& event, string title, string desc, string imgURL="none", embedField* fields, unsigned char fieldAmount);
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
            bot.global_command_create(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
            bot.global_command_create(dpp::slashcommand("dice", "performs a dice roll (min 1, max 6)", bot.me.id));
            bot.global_command_create(dpp::slashcommand("embed", "testing embeds rn", bot.me.id));
            // bot.global_command_create(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
            // bot.global_command_create(dpp::slashcommand(PING, PING_DESCRIPTION, bot.me.id));
        }
    });

    bot.start(dpp::st_wait);
    return 0;
}
// functions
void executeSlashCommand(string userInput, const dpp::slashcommand_t& event) {
    string result;
    if (userInput == "ping") {
        result = "Bot is ready to operate";
    } else if (userInput == "dice") {
        result = diceRoll(1, 6);
    } else if (userInput == "embed") {

        // obviously not how this will work once implemented properly

        embedField field1, field2, field3;

        field1.title = "title";
        field2.title = "anothertitle";
        field3.title = "anothertitletitle";

        field1.contents = "this is some content for field1";
        field2.contents = "this is more content for field 2";
        field3.contents = "you didnt expect this content to be so out of line from the other two did you?";

        embedField embedFields[] = {field1, field2, field3}; 

        for (embedField field : embedFields)
            field.value = true;

        embed(event, "title", "description", "none", embedFields, 3);
    } else {
        event.reply("Whoops! We haven't designed an output for this command!");
    }
    cout << "executed" << userInput << endl;
}

char diceRoll(int low, int up) {
    srand(time(NULL));
    return (rand() % up + low) + '0';
}

void embed(const dpp::slashcommand_t& event, string title, string desc, string imgURL, embedField* fields, unsigned char fieldAmount) {
    dpp::embed embed = getEmbed(title, desc, imgURL, fields, fieldAmount);
    dpp::message msg(event.command.channel_id, embed);
    event.reply(msg);
}

dpp::embed getEmbed(string title, string desc, string imgURL, embedField* fields, unsigned char fieldAmount) {
    if (imgURL == "none")
        imgURL = "https://dpp.dev/DPP-Logo.png";
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::sti_blue)
        .set_title(title)
        .set_description(desc)
        .set_thumbnail(imgURL)
        .set_image("https://dpp.dev/DPP-Logo.png")
        .set_footer(
            dpp::embed_footer()
            .set_text("Footer text")
        )
        .set_timestamp(time(0));
    
    for (int i = 0; i < fieldAmount; i++) {
        embed.add_field(fields[i].title, fields[i].contents, fields[i].value);
    }

    return embed;
}