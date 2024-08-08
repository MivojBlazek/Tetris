# Changelog

All notable changes to this project will be documented in this file.

## 29.07.2024

* Started this project
* Set up first window

## 30.07.2024

* User can spawn blocks, that will just fall down
* User can move blocks to the left and right
* Fixed size of graphics scene
* Only last spawned block can be controlled

## 31.07.2024

* Blocks are falling one by one
* Blocks can detect others and cannot go through them
* User can speed up game via down arrow button on his keyboard
* If any block reaches top of the tower, it will change color to red and pause game
* Pause button (now Debug button) can only stop block in the air and pause game

## 04.08.2024

* Added shapes that contains blocks
* All default Tetris shapes supported
* Shapes cannot exceeded borders or go through others
* Shapes are chosen randomly
* After filling entire row, it is deleted and blocks above fall down by 1 block position

## 05.08.2024

* Removed Debug button as it does nothing interesting
* Blocks can be rotated with up arrow to the right and Z button to the left
* Blocks cannot be rotated if there is not enough space for them
* If block is rotated near obstacle like other block or border of playing field, block is moved to not to be stuck
* Added next field where is displayed next piece that will be falling

## 06.08.2024

* Added hold and is bind on C key on users keyboard
* Hold can be used only once per piece drop and allows user to switch between holding and falling piece

## 07.08.2024

* Added different colors to different shapes
* Added focus on Start button so user can start game just with pressing enter
* Added grid
* Added score and highscore that is saved in highscore.json file
* Score amount that can user get are 1 per soft drop, 100 per removed row, 300 per 2 removed rows at the time and the same for 3 and 4 rows (500 points and 800 points)
* After reaching specific score limit it speed up game a little bit

## 08.08.2024

* Added hard drop feature with space on users keyboard
* Added preview so user can know where does his piece fall
