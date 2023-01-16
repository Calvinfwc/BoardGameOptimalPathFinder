/*
* This program contains function prototypes and data structures for our pathfinder 
*/
#ifndef pathfinder_proto
#define pathfinder_proto
#include <vector>
// min-weight perfect matching algo 
#include "munkres.h" 
#include "combinations.h"
#include <fstream>
#include <numeric>
#include <thread>
#define BIG_NUM  100.0f
#define DIAGONAL_WEIGHT 1.4142f
#define BOARD_COL 6
#define BOARD_ROW 5
#define BOARD_SIZE 30
#define NUM_RUNE_TYPES 6 
#define NW 1
#define W 2
#define SW 3
#define S 4
#define SE 5
#define E 6
#define NE 7
#define N 8

/*
* infomation about each rune type at current stage ie. on current board 
*/
struct RuneTypeNode {
    int typeName;  //0:water,1:fire,2:earth,3:light,4:dark,5:heart
    std::vector<int> pos; //index of this vec represents index of the rune . pos[i] represents position of the rune of index = i.
    Matrix<float> dis; //distance matrix between labelled runes ie. current pos and final pos of runes 
    mutable float typeErr; //error for this rune type 
    void computeTypeErr () const; //reset typeErr to 0 and update typeErr
    RuneTypeNode(int name) : typeName(name) , typeErr(0.0f)  {}; 
};

/*
* info about all states of the board at current node  
*/
struct TreeNode { 
    std::vector<int> leadv; //vector of lead positions characterizing a path
    std::vector<int> board;
    std::vector<RuneTypeNode> RTN; 
    float pastErr; //error already occured ie. cost of all previous moves 
    float Err; //estimated minimum error from destination aka board error 
    TreeNode() {
        pastErr = 0.0f; 
        Err = 0.0f; 
    }
};

/*
* infomation about each rune type at final stage ie. on final board 
* similar to RuneTypeNode
*/
struct finRuneTypeNode {
    int typeName; //0:water,1:fire,2:earth,3:light,4:dark,5:heart
    std::vector<int> pos; //position of all runes of type typeName on board 
    finRuneTypeNode(int name) {typeName = name;};
}; 


/*
* @use convert board index int to board coordinate (int,int)
*/
std::vector<int> indexToCoor (const int index);

/*
* moves the lead toward dir 
* @param board board to move 
* @param i position to move
* @param dir direction to move 
* @return false only if move is invalid on this board . Invalid if moves outside of board 
*/
bool move(std::vector<int>& board /*update*/, int& i /*update*/, const int dir);

/*
* @param i board index
* @param j board index
* @param diagToHorizontalWeight weight ratio , ie. weight(diag edge) / weight(horizontal/vertical edge)
* @return minimum distance between board index i , j 
*/
float minDis (const int i , const int j , const float diagToHorizontalWeight);

/*
* initialize the root of the BFS pathfinder . 
* The reason we don't use constructor is that here we can initialize multiple data .
* Also only the root needs to be properly initialized . Others just copy construct from previous one .
* @param in initial board 
* @param out final board 
* @param lead leading rune 
* @param root the root in a BFS 
* @param FRTN FRTN[i] denotes finRuneTypeNode for type of name i 
* @assume in , out have same size 
*/
void preProcessor (const std::vector<int> in , const std::vector<int> out , const int lead , TreeNode& root , std::vector<finRuneTypeNode>& FRTN);

//for sorting . Compares i & j . 
bool compWinner (const TreeNode& i , const TreeNode& j);

//for sorting . Compares i & j . 
bool compForcastedTotalErr (const TreeNode& i , const TreeNode& j);

//takes in size of a vector and returns percentage in [0,1] of the vector to keep 
float percenToKeep(const int size);

/*
* pathfinder for 5*6 board with given lead 
* @param in input board 
* @param out output board 
* @param lead pos of lead rune on input board 
* @param pathWeightBound path error must not exceed this 
* @param solutionNode stores solution 
*/
bool pf56_single (const std::vector<int> in , const std::vector<int> out , const int lead, const int pathWeightBound , TreeNode& solutionNode);

/*
* pathfinder for 5*6 board without given lead . Picks 2 of the best lead pos and run pf56_single . 
* @param in input board 
* @param out output board 
* @param pathWeightBound path error must not exceed this 
* @param solutionNode stores solution 
*/
bool pf56_double (const std::vector<int> in , const std::vector<int> out , const int pathWeightBound , TreeNode& solutionNode);
#endif 
