# Board Game Optimal Path Finder
A Path Finder for a board game called "tower of savior" (or a similar game "pazudora")

**I. Backgraound** : 
In "tower of savior" player will be given a 5*6 board ("in" board) . The board has 30 runes on it , there're 6 types of runes . Player can pick any initial rune (lead) and move it around (in any of the 8 directions) until time's up . 

After time's up , the board has been modified ("out" board) , a rune dissolves if it has at least 3 horizontal (or vertical) adjacent runes of the same type .  **(see section V for a video)**

**II. Problem** : 
A player is given an "in" board may have a desired "out" board . However the player has limited time to move (the "lead" rune) . Since there're many possible paths (sequence of moves that can convert the "in" to the "out" board) , the player may want to find the "shortest path" .

**III. Path Finder** : Attempts to find "shorter path" between an "in" and "out" board fast. Program input is the "in" and "out" board , a bound on maximum path weight allowed and a TreeNode struct for storing solution . 

Here I explain a bit more in detail :

1. I defined path weight for horizontal and vertical move to be 1 , diagonal move to be 1.4142 . 

2. The solution is a sequence of board positions where the lead has visited . This sequence is the optimal path given by this program .

3. I defined the board as an array with indices 0,...,30 . Counting from left to right , top to bottom . e.g. second row has indices 6,...,11

**IV. Techniques used** : Breadth-first search , minimum-weight perfect matching , greedy algorithm and heuristics .

**V. Example :**
Execution of example.cpp (video) : https://youtube.com/shorts/XDCyspMy5o8?feature=share

**VI. Credit to :** 
1. "munkres.h"  : https://github.com/saebyn/munkres-cpp
