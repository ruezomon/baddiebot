# baddiebot
Discord Bot specifically designed for the Discord Server "Middle-age craft 3"

Just for your information ->
this repository does NOT include the Bot Token!!
It is safely stored in a .json :3
This is why my code is open source, but you still cant use it yourself.

IF you want to use this Code for your own bot the config.json should look like this...

```json JSON Example
{
    "token": "YOUR_BOT_TOKEN"
}
```

...and be placed in the root of your directory. 

The CMakeLists.txt file is actually given and you should be able to compile the code easily via cmake. 

You might need to install some dependencies (cmake will tell you explicitly, you might want to actually READ the error messages, instead of just "whoopsy daisy"-ing and asking someone else), 
including D++ (C++ Discord API).
How to download D++ (official website):
https://dpp.dev/installing.html

Have fun! :D