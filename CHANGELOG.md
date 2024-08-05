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
 