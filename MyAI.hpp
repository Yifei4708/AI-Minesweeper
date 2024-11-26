// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream> // temporary use
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include <vector>
#include <iomanip>

using namespace std;

struct Tile {
    int x = -1;
    int y = -1;
    int value = -1;
    char statu = 'n';
    int effective = -1;
};

// unordered_map<Tile, unordered_set<Tile>> C;
// unordered_map<Tile, unordered_set<Tile>> V;

class MyAI : public Agent
{
private:
    int rowDimension;
    int colDimension;
    int totalMines;
    int needUn;
    int needF;
    int remainM;
    //int updataUn;

    int** playboard; // Declare playboard as a member variable
    int** clearedboard;
    int** flaggedboard;
    vector<vector<Tile *>> tileboard;
    //int** tileboard;
    //int** assignValueboard;

    int xArray[20];
    int yArray[20];

    int xFlag[20];
    int yFlag[20];

public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );

    Action getAction ( int number ) override;

    static bool compareTiles(Tile* a, Tile* b);
    bool checkUncoveredNeighbor(int b, int a);
    int findEffective(int b, int a);
    void preprocess();
    bool checkVarAssignments(Tile * v, vector<Tile*> vec_V, int val);
    void backtrack(vector<Tile *> variable, int idx);

    void printBoardInfo(     );
    void printTileInfo( int c, int r );

    ~MyAI();

    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================



    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
