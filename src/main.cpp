#include <dpp/dpp.h>
#include <thread>
#include <fstream>
#include <vector>
#include "../include/json.hpp"

using namespace std;

// globals

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

void embed(const dpp::slashcommand_t& event, string title, string desc, string footer, embedField* fields, unsigned char fieldAmount, string thumbnailURL="none", string imgURL="none");
dpp::embed getEmbed(string title, string desc, string footer, embedField* fields, unsigned char fieldAmount, string thumbnailURL, string imgURL);

bool isBadWord(const dpp::message_create_t* event, vector<string>* badWords);

// main
int main(int argc, char** argv) {

    // initialization
    ifstream file("../config.json");
    nlohmann::json data = nlohmann::json::parse(file);
    const string BOT_TOKEN = data["token"];
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
    
    vector<string> vecBadWords;
    // filling all bad words into the vector
    {
        std::ifstream badWordsFile;
        badWordsFile.open("../badwords.txt");
        string word;
        while (getline(badWordsFile, word)) vecBadWords.push_back(word);
    }

    bot.on_log(dpp::utility::cout_logger());

    bot.on_message_create([&bot, &vecBadWords](const dpp::message_create_t& event) {
        if (isBadWord(&event, &vecBadWords))
            event.reply("Wir würden dich darum bitten, diese Nachricht zu löschen, da sie 1 oder mehrere Wörter beinhält, welche auf diesem Server nicht erlaubt sind.", true);
    });

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        dpp::command_interaction cmd_data = event.command.get_command_interaction();
        string input = event.command.get_command_name();
        executeSlashCommand(input, event, cmd_data);
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {

            vector<dpp::slashcommand> commands;
            commands.push_back(dpp::slashcommand("ping", "Test connection", bot.me.id));
            commands.push_back(
                dpp::slashcommand("dice", "performs a dice roll", bot.me.id)
                .add_option(dpp::command_option(dpp::co_string, "lowest", "Lowest number which can be rolled", true).set_auto_complete(false))
                .add_option(dpp::command_option(dpp::co_string, "highest", "Highest number which can be rolled", true).set_auto_complete(false))
            );
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

        string lowStr = get<string>(event.get_parameter("low"));
        string highStr = get<string>(event.get_parameter("high"));
        
        if (!(isNumber(lowStr) && isNumber(highStr))) {
            event.reply("Given values were not recognized as numbers!");
        } else {
            event.reply(diceRoll(stoi(lowStr), stoi(highStr)));
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

bool isBadWord(const dpp::message_create_t* event, vector<string>* badWords) {
    string message = event->msg.content;
    for (int i = 0; i < message.length(); i++) {
        message[i] = tolower(message[i]);
    }

    for (string i : *badWords) {
        if (message.find(i) != std::string::npos) return true;
    }
    return false;
}