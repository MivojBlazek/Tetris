#ifndef CONFIG_H
#define CONFIG_H

/**
 *  CELL_SIZE determines size of cell that fits one block.
 *  For FullHD display I recommend to use 30 because height of the window will be around 850px.
 *  Recommended interval is from 10 to 50.
 *
 *  Default: 30
*/
#define CELL_SIZE 30

/**
 *  JSON_NAME is just name of .json file where is scored highscore.
 *  Of course you can change highscore in that file and then flex to your friends but remember it was made just to save that value for honest people.
 *
 *  Default: "highscore.json"
*/
#define JSON_NAME "highscore.json"

/**
 *  FALLING_SPEED is time in miliseconds that it will take to fall 1 row.
 *  Recommended 1000. If it is slow to you, just play a bit. On defferent score values it accelerate.
 *  The fastest option is set to FALLING_SPEED / 5 so in default 200 miliseconds to fall 1 row.
 *
 *  Default: 1000
*/
#define FALLING_SPEED 1000

#endif // CONFIG_H
