# Wellcome to Brainduck

# Introduction
Brainduck is a Brainfuck-like programming language with some extra steps to make it even more difficult to do stuff!

# How it works
In Brainduck there are cells. Each cell has it's own value which is eather an int or a string.
You can move to other cells using this commands:
 - `<` move to the left 
 - `>` move to the right
 - `^` move up
 - `v` move down
<br />
If you want to move to a cell with the index < 0, nothing will happen.

# Special commands/features

Each cell has its own state:
 - The int state, where the cell contains an int
 - The string state, where the cell contains a string value

If you use the `+` or `-` operators on an int-cell, the value will be changed mathematicly (+1/-1) <br />
But if you use the `+` operators on a string-cell, the cell will look for an other cell nearby and add them together. <br />
When it looks for a good cell, it first checks the cell under the current one and if it's not empty it will be choosen, otherwise the cell will look behind itself and check again. If the cell cant find a good cell, nothing will happen. <br />
The `-` operator just popes the last char from the string. <br />
You can switch the state using the `#` command. <br />

With the `.` command, you can print the value of the current cell depending on its state. <br />

If you want to get User-input, you can use the `?` command. (If the current cell is in an int-state, the inputet char will be loaded as ASCII-value) <br />

Using the `;` command, you can copy the value of the last cell behind the current in it. <br />

Because I'm lazy, the `/` command will print a newline on the screen. <br />

The `=` command will exit the programm. <br />

The `~` command will copy a to-int value from the string state into the int state of the current cell.  <br />
If it fails during the to-int process, nothing will happen. <br />

There are also loops in this language. <br />
Instead of the `[]` like in brainfuck, it uses the `{}` braces. <br />
Why, you may ask? I dont't know! <br />
But they work just like they do in brainfuck, it loops until the current cell is in the int-state and == 0. If it is in the string state, the loop won't stop. <br />

But, this language also uses the `[]` braces. <br />
If at the `[` the cell is in the int-state and == 0 or it's in the string-state, the commands in the braces will be runed. <br />
Else they will be skipped. **This is not stackable like the loops!** <br />

The last command is the system command (`!`) this is maybe a little bit useless but this command executes the string-state value in the shell. <br />

Spaces and new lines will be ignored, but will be counted as a character! <br />

And, because we have to justify the "duck" in the name, every error message will begin with an angry duck! <br />

# How to use it
Clone the git-repo and compile the main.cpp file like this:
```
g++ main.cpp ArgParser/ArgParser/ArgParser.cpp -std=c++17 -o Brainduck.<extention>
```

If you are on linux, you can use the .x86_64 extention,
on windows use `.exe`.

Parsing a file can be done like this:
```
Brainduck file.bd
```

# Logs
Normaly, all commands and cell states will be loged in the Deduck.log file.
You can disable this using the `--no-logs` flag.

# Creator
SirWolf and like 30 minutes of boredom.

# Some Examples
## truth-programm
Prints 0 if the input is 0 and will spam 1 if the input is 1.
```
#?~#{.}./
```
## cat programm
Prints your input and waits for the next.
```
#{?./}
```
## adds two numbers
Adds your first input to the second.
```
#?~#>#?~#<{->+<}>./
```
