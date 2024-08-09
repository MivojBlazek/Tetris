# Tetris

This is common tetris game in C++ Qt framework.

I am free to any suggestions that can make my coding better and cleaner.

## Running game

### Linux

>   Program was tested on Ubuntu 22.04.4 LTS so there can be some issues with different OS.

After downloading and extracting .zip file from GitHub use Makefile to compile and run the game.

| Commands            | Description |
| ------------------- | ----------- |
| make / make compile | Uses first target in Makefile that should compile source files |
| make run            | Runs compiled program (GUI support is required) |
| make all            | Compiles and runs program after that |
| make clean          | Removes build from ./build/Linux/ |

### Windows

>   Program was tested just only on Windows 10

For Windows there is a option to download Qt from their official website and compile source files and run executable in that program. Or there is already compiled program in `./build/Windows/`. Just go to that directory and find executable file `Tetris.exe` and run it.

## Controls

Game has common controls via keyboard.

At the start of the game you should press Start button but you can do it just pressing `Enter` on keyboard.

| Key           | Description |
| :-----------: | ----------- |
| Left arrow    | Moves piece to the left by 1 column if it is possible |
| Right arrow   | Moves piece to the right by 1 column if it is possible |
| Up arrow / X  | Rotates piece to the right if it is possible |
| Z             | Rotates piece to the left if it is possible |
| Down arrow    | Soft drop - moves piece down by 1 row to speed up game |
| Space         | Hard drop - moves piece down to the bottom to speed up game |
| C             | Save current piece to the hold and takes the one from hold |

## Score

### What is score

Score is used to compete with others. It also speed up game after reaching some limits.

Highest reached score is saved in .json file and you can see it in the game.

### Acquiring score

Score distribution is different for each type.

| Process                    | Description |
| -------------------------- | ----------- |
| Soft drop                  | 1 per every row |
| Hard drop                  | 2 per every row |
| 1 removed row at the time  | 100 |
| 2 removed rows at the time | 300 |
| 3 removed rows at the time | 500 |
| 4 removed rows at the time | 800 |

## Some details

If the game is too slow for you or you want to make the window larger / smaller, you can do it in `config.h`.

>  Remember to compile game after each change in config.h.