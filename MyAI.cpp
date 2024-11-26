// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"
#include <vector>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
using namespace std;



vector<Tile *> variables;
unordered_map<Tile *, vector<Tile*> > V;
unordered_map<Tile *, vector<Tile*> > C;

unordered_map<Tile *, int> assignments;
vector<unordered_map<Tile *, int>> solutions;

int csp_unCover = 0;
//struct Tile v;
//struct Tile con;
//unordered_map<Tile *, unordered_set<Tile*> *> V;

// struct Tile {
//     int x, y, value;
// };
// vector<vector<int>> effectiveL;
// unordered_map<Tile, unordered_set<Tile>> C;
// unordered_map<Tile, unordered_set<Tile>> V;

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    cout << "Here MyAI" << endl;
    cout <<  _rowDimension << endl;
    cout << _colDimension << endl;
    rowDimension = _rowDimension;
    colDimension = _colDimension;
    totalMines   = _totalMines;
    needUn = 0;
    needF = 0;
    remainM = _totalMines;
    // updataUn = 0;

    // ==================================BOARD====================================
    playboard = new int*[colDimension];
    clearedboard = new int*[colDimension];
    flaggedboard = new int*[colDimension];
    tileboard = vector<vector<Tile *>>(colDimension);
    
    //assignValueboard = new int*[colDimension];

    for (int i = 0; i < colDimension; ++i) {
        playboard[i] = new int[rowDimension];
        clearedboard[i] = new int[rowDimension];
        flaggedboard[i] = new int[rowDimension];
        tileboard[i] = vector<Tile *>(rowDimension);
        //assignValueboard[i] = new int[rowDimension];
    }

    // Initialize playboard or perform any other necessary initialization
    for (int i = 0; i < colDimension; ++i) {
        for (int j = 0; j < rowDimension; ++j) {
            playboard[i][j] = -1; // Initialize to some default value
            clearedboard[i][j] = 0;
            flaggedboard[i][j] = 0;
            Tile* t = new Tile();
            t->x = i;
            t->y = j;
            tileboard[i][j] = t;
            //assignValueboard[i][j] = -1;
        }
    }
    playboard[_agentX][_agentY] = 0;

    // ==============================Uncover array========================================
    for (int i = 0; i < 20; ++i) {
        xArray[i] = -1; // Initialize each element with its index
        yArray[i] = -1;
        xFlag[i] = -1;
        yFlag[i] = -1;
    }
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

Agent::Action MyAI::getAction( int number )
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    Action_type new_action = Action_type ::LEAVE;
    bool moveMade = false; //uncover if num = bombsarround
    int bombsurround = 0;
    int surrondnum = 0;

    // ===============================Check if tile need to be uncovered=======================================
    if(needUn > 0){
        if(csp_unCover == 1 && xArray[needUn - 1] != -1 && yArray[needUn - 1] != -1 && needUn <= 20){
            
            int x = xArray[needUn - 1];
            int y = yArray[needUn - 1];
            csp_unCover = 0;
            return {UNCOVER, x, y};
        }
        if(xArray[needUn - 1] != -1 && yArray[needUn - 1] != -1 && needUn <= 20){
            int x = xArray[needUn - 1];
            int y = yArray[needUn - 1];
            playboard[x][y] = number;
            xArray[needUn - 1] = -1;
            yArray[needUn - 1] = -1;
            needUn--;
            //printBoardInfo();
            if(needUn > 0){
                x = xArray[needUn -1];
                y = yArray[needUn - 1];
                new_action = UNCOVER;
                //cout << "Uncover agentX: " << x+1 << " Y: " << y+1 << endl;
                return {UNCOVER, x, y};
            }
        }
    }
    if(needF > 0){
        if(xFlag[needF - 1] != -1 && yFlag[needF - 1] != -1 && needF <= 20){
            int x = xFlag[needF - 1];
            int y = yFlag[needF - 1];
            playboard[x][y] = -2;
            flaggedboard[x][y] = 1;
            remainM--;
            xFlag[needF - 1] = -1;
            yFlag[needF - 1] = -1;
            needF--;
            //printBoardInfo();
            if(needF > 0){
                x = xFlag[needF -1];
                y = yFlag[needF - 1];
                //new_action = UNFLAG;
                //cout << "Flag agentX: " << x+1 << " Y: " << y+1 << endl;
                return {FLAG, x, y};
            }
        }
    }
    // //printBoardInfo(); 
    /**/
    // ===============================Check if tile num = bombsarround =======================================

    for (int a = 0; a < rowDimension; a++){
        for(int b = 0; b < colDimension; b++){
            //cout << "start b: " << b+1 << " a: " << a+1 << endl;
            if(clearedboard[b][a] == 0 && playboard[b][a] > -1){
                bombsurround = 0;
            if(playboard[b][a] >= 1 && playboard[b][a] <= totalMines){
                surrondnum = 0;
            }
                for(int c = -1; c < 2; c++){
                    for(int d = -1; d < 2; d++){
                        if(a+d >= 0 && b+c >= 0 && a+d < rowDimension && b+c < colDimension){
                            // cout << "check       b: " << b+1+c << " a: " << a+1+d << endl;
                            // cout << "Flagboard       : " << flaggedboard[b+c][a+d] << endl;
                            if(flaggedboard[b+c][a+d] == 1){
                                bombsurround++;
                            }
                            if(!(c == 0 && d == 0)){
                                if(playboard[b+c][a+d] >= 0 && playboard[b+c][a+d] <= totalMines){
                                    surrondnum++;
                                }
                            }
                        }else{
                            surrondnum++;
                        }
                    }
                }
                //cout << "bombsurround: " << bombsurround << endl;
                if(playboard[b][a] == bombsurround || (playboard[b][a] == -2 && bombsurround == playboard[b][a] + 2)){
                    int update = 0;
                    int i = 0;
                    for(int c = -1; c < 2; c++){
                        for(int d = -1; d < 2; d++){
                            if(a+d >= 0 && b+c >= 0 && a+d < rowDimension && b+c < colDimension){
                                // cout << "num:  " << endl;
                                // cout << a+d+1 <<endl;
                                // cout << b+c+1 <<endl;
                                if(flaggedboard[b+c][a+d] == 0 && playboard[b+c][a+d] == -1){
                                    //mark around tiles as uncover with no bombs
                                    update = 1;
                                    int y = a+d;
                                    int x = b+c;
                                    // cout << "mark around tiles as uncover with no bombs" << endl;
                                    // cout << x+1 <<endl;
                                    // cout << y+1 <<endl;
                                    xArray[i] = x;
                                    yArray[i] = y;
                                    needUn++;
                                    i++;
                                }
                            }
                        }
                    }
                    //cout << "Cleared b: " << b+1 << " a: " << a+1 << endl;
                    clearedboard[b][a] = 1;
                    // if(b == 0 && a == 4){
                    //     cout << "Press ENTER to see loooooooo..." << endl; // delete
                    //     cin.ignore( 999, '\n'); //delete
                    //     cout << "first clearboard!!!!!!" << endl;
                    // }
                    if(update == 1){
                        //cout << "b: " << b+1 << " a: " << a+1 << endl;
                        moveMade = true;
                        b = colDimension;
                        a = rowDimension;
                    }
                }if(a < rowDimension && b < colDimension){
                    // cout << "here" << endl;
                    // cout << "surrond: " << surrondnum << " clearboard: " << clearedboard[b][a] << endl;
                    if(playboard[b][a] == (8 - surrondnum) && clearedboard[b][a] == 0){
                        int update = 0;
                        int i = 0;
                        for(int c = -1; c < 2; c++){
                            for(int d = -1; d < 2; d++){
                                if(a+d >= 0 && b+c >= 0 && a+d < rowDimension && b+c < colDimension){
                                    if(playboard[b+c][a+d] == -1){
                                        if(flaggedboard[b+c][a+d] == 0){
                                            update = 1;
                                            flaggedboard[b+c][a+d] = 1;
                                            int y = a+d;
                                            int x = b+c;
                                            xFlag[i] = x;
                                            yFlag[i] = y;
                                            needF++;
                                            i++;
                                        }
                                        moveMade = true;
                                    }
                                }
                            }
                        }
                        //cout << "Cleared b: " << b+1 << " a: " << a+1 << endl;
                        clearedboard[b][a] = 1;
                        if(update == 1){
                            //cout << "b2: " << b+1 << " a2: " << a+1 << endl;
                            //cout << "needF " << needF << endl;
                            b = colDimension;
                            a = rowDimension;
                        }

                    }
                }
            }
        }
    }
    // for(int i = 0; i < 9; i++){
    //     cout << "xArray: " << xArray[i] <<  " and yArray: " << yArray[i] << endl;;
    // }
    // for(int i = 0; i < 9; i++){
    //     cout << "xFlag: " << xFlag[i] <<  " and yFlag: " << yFlag[i] << endl;;
    // }

    // if(xArray[0] != -1 && yArray[0] != -1){
    if(needUn > 0){
        int x = xArray[needUn - 1];
        int y = yArray[needUn - 1];
        new_action = UNCOVER;
        return {new_action, x, y};
    }
    if(needF > 0){
        int x = xFlag[needF - 1];
        int y = yFlag[needF - 1];
        new_action = FLAG;
        return {new_action, x, y};
    }
    /**/
    // needUn = 0;

    //cout << "===============================CSP algorithm =======================================\n";
     // ===============================CSP algorithm =======================================
    //cout << "CSP method            !!!!!!!!!!!!" << endl;
 
    // inserting values by using [] operator

    // cout << "Bucket countV: " << V.bucket_count() << endl;
    // cout << "Bucket countC: " << C.bucket_count() << endl;
    //printBoardInfo();
    V.clear();
    C.clear();
    variables.clear();
    solutions.clear();
    assignments.clear();

    preprocess();
    //cout << "after preprocess           !!!!!!!!!!!!" << endl;
    // for(auto t: V){
    //     //cout << "over here x: " << t.first->x+1 << " y: " << t.first->y+1 << " value of v: " << t.first->value << endl;
    // }
    for(auto v: V){
        //cout << "go through V x: " << v.first->x+1 << " y: " << v.first->y+1 << endl;
        variables.push_back(v.first);
        //cout << "want to know x: " << v.first->x+1 << " y: " << v.first->y+1 << " value of v: " << v.first->value << endl;
    }
    sort(variables.begin(), variables.end(), compareTiles);
    backtrack(variables, 0);
    //cout << "after backtrack            !!!!!!!!!!!!" << endl;

    //cout << "Size of solutions: " << solutions.size() << endl; 
    unordered_map<Tile *, int> bombCount;
    for(auto solution: solutions){
        //cout << "Size of each assignment: " << solution.size() << endl;
        for(auto t: solution){
            if(t.second == 1){
                bombCount[t.first]++;
            }else if(t.second == 0){
                bombCount[t.first] += 0;
            }
        }
    }

    int solutions_size = solutions.size();
    int i = 0;
    int j = 0;
    if(solutions_size != 0){
        for(auto t: bombCount){
            int x = t.first->x;
            int y = t.first->y;
            if(t.second == solutions_size){
                flaggedboard[x][y] = 1;
                xFlag[j] = x;
                yFlag[j] = y;
                needF++;
                j++;
                //cout << "a bomb at x: " << x+1 << " y: " << y+1 << " value of v: " << t.second << endl;
            }else if(t.second == 0){
                xArray[i] = x;
                yArray[i] = y;
                needUn++;
                i++;
                //cout << "No bomb at x: " << x+1 << " y: " << y+1 << " value of v: " << t.second << endl;
            }
            t.first->statu = 'n';
            t.first->effective = -1;
            t.first->value = -1;
        }
    }

        
    variables.clear();
    //V.clear();
    C.clear();
    solutions.clear();
    assignments.clear();


    
//--------------------------------------------------------------------------------------------
    if(needF > 0){
        int x = xFlag[needF - 1];
        int y = yFlag[needF - 1];
        new_action = FLAG;
        if(needUn > 0){
            csp_unCover = 1;
        }
        return {new_action, x, y};
    }
    if(needUn > 0){
        //cout << "here: 1" << endl;
        int x = xArray[needUn - 1];
        int y = yArray[needUn - 1];
        new_action = UNCOVER;
        //cout << "uncoverx: " << x+1 << " y: " << y+1 << endl;
        return {new_action, x, y};
    }
    csp_unCover = 0;

//--------------------------------------------------------------------------------------------
    //cout << "random           !!!!!!!!!!!!" << endl;
    // cout << "stopssssssss here!!!!!!!!!!!!!!!!!!!!" << endl;

    

    // //vector<Tile *> guessList;
    // cout <<"size of V: " << V.size() << endl;
    // // for(auto v: V){
    // //     cout << "go through V x: " << v.first->x+1 << " y: " << v.first->y+1 << endl;
    // // }

    // srand(time(NULL));  // Seed the random number generator
    Tile *guess;
    int guessIndex;
    int x, y;
    // guessIndex = rand() % (rowDimension * colDimension);
    int find = 0;
    x = rand() % colDimension;
    y = rand() % rowDimension;
    while(playboard[x][y] > 0 || flaggedboard[x][y] == 1){
        x = rand() % colDimension;
        y = rand() % rowDimension;
    }
    // for (int a = 0; a < colDimension && find != 1; a++){
    //     for(int b = 0; b < rowDimension; b++){
    //         if(playboard[b][a] == -1){
    //             x = b;
    //             y = a;
    //             //guess = tileboard[b][a];
    //             //flaggedboard[b][a] = 1;
    //             break;
    //             find = 1;
    //         }
    //     }
    // }
    // x = guess->x;
    // y = guess->y;
    //cout << "Random x: " << x+1 << " y: " << y+1 << endl;

    xArray[0] = x;
    yArray[0] = y;
    needUn++;
    V.clear();
    if(remainM == 0){
        return {LEAVE,-1,-1};
    }
    if(needUn > 0){
        //cout << "here: 1" << endl;
        int x = xArray[needUn - 1];
        int y = yArray[needUn - 1];
        new_action = UNCOVER;
        //cout << "uncoverx: " << x+1 << " y: " << y+1 << endl;
        return {new_action, x, y};
    }
    // needF++;
    //cout << "end            !!!!!!!!!!!!" << endl;
    
    return {LEAVE,-1,-1};
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================

}

bool MyAI::compareTiles(Tile* a, Tile* b) {
    if (a->x != b->x) {
        return a->x > b->x;  // Sort by x in descending order
    }
    return a->y > b->y;      // If x is the same, sort by y in descending order
}

bool MyAI::checkUncoveredNeighbor(int b, int a){
    for(int c = -1; c < 2; c++){
        for(int d = -1; d < 2; d++){
            if(a+d >= 0 && b+c >= 0 && a+d < rowDimension && b+c < colDimension && !(c == 0 && d == 0)){
                if(playboard[b+c][a+d] > -1){
                    //cout << "here checkUncover" << endl;
                    return true;
                }
            }
        }
    }
    return false;
}
int MyAI::findEffective(int b, int a){
    int num = playboard[b][a];
    for(int c = -1; c < 2; c++){
        for(int d = -1; d < 2; d++){
            if(a+d >= 0 && b+c >= 0 && a+d < rowDimension && b+c < colDimension && !(c == 0 && d == 0)){
                if(playboard[b+c][a+d] == -2){
                    num--;
                }
            }
        }
    } 
    return num;
}

void MyAI::preprocess(){
    int num = 0;
    for (int a = 0; a < rowDimension; a++){
         for(int b = 0; b < colDimension; b++){
            if(a >= 0 && b >= 0 && a < rowDimension && b < colDimension){
                if(num > 25){
                    break;
                }
                if(checkUncoveredNeighbor(b,a) && playboard[b][a] == -1){
                    //cout << "================================\n";
                    Tile * v = tileboard[b][a];
                    num++;

                    //cout << "Vtile x: " << v->x+ 1 << " y: " << v->y+1 << endl;
                    //cout << "Vtile x: " << b+ 1 << " y: " << a+1 << endl;
                    vector<Tile *> vec_v;
                    V[v] = vec_v;
                    //cout << "check v x" << v->x+ 1<< " y" << v->y+1 <<endl;   
                    //find c around it
                    for(int c = -1; c < 2; c++){
                        for(int d = -1; d < 2; d++){
                            int ni = b+c;
                            int nj = a+d;
                            //Tile con = {ni, nj, -1};
                            if(a+d >= 0 && b+c >= 0 && a+d < rowDimension && b+c < colDimension && !(c == 0 && d == 0)&& playboard[ni][nj] > -1 && clearedboard[ni][nj] == 0){ //&& playboard[ni][nj] > -1
                                Tile* con = tileboard[b+c][a+d];
                                con->effective = findEffective(b+c, a+d);
                                con->x = ni;
                                con->y = nj;
                                vector<Tile *> vec_c;
                                C[con] = vec_c;
                                // cout << "Ctile x: " << ni+1 << " y: " << nj+1 << endl;
                                // cout << "Vtile v x" << v->x+ 1<< " y" << v->y+1 <<endl;  
                                v->statu = 'v';
                                C[con].push_back(v);
                                //cout << "size of C[con]: " << C[con].size() << endl;
                                V[v].push_back(con); //v: 71
                                //cout << "size of C[con]: " << C[con].size() << endl;
                            }
                        }
                    }
                }
            }
        }
        if(num > 25){
            break;
        }
    }
        
}

bool MyAI::checkVarAssignments(Tile *v, vector<Tile*> vec_V, int val){
    //cout << "VVVVV x:" << v->x+1 << " y: " << v->y+1 << endl;
    v->value = val;
    for (auto con : vec_V){
        //cout << "what is this x:" << c->x+1 << " y: " << c->y+1 << endl;
        int numUnassigned = 0;
        int numMine = 0;
        int b = con->x;
        int a = con->y;
        for(int c = -1; c < 2; c++){
            for(int d = -1; d < 2; d++){
                if(a+d >= 0 && b+c >= 0 && a+d < rowDimension && b+c < colDimension && !(c == 0 && d == 0)){
                    Tile *t = tileboard[b+c][a+d];
                    if(t->statu == 'v'){
                        if(t->value == -1){
                            numUnassigned++;
                        }
                        if(t->value == 1){
                            numMine++;
                        }
                    }
                }
            }
        }
        //cout << " #M: " << numMine << " effective: " <<  con->effective << " #A+#M: " << numUnassigned+numMine <<  " at the point             x:" << con->x+1 << " y: " << con->y+1 << endl;
        //v->value = -1;
        if(! (numMine <= con->effective && con->effective <= numMine+numUnassigned) ){
            v->value = -1;
            return false;
        }
        //cout << "size of V(v): " << vec_C.size() << endl;
        
    }
    v->value = -1;
    return true;
}

void MyAI::backtrack(vector<Tile *> variable, int idx){
    if(idx == variable.size()){
        solutions.push_back(assignments);
        return;
    }

    Tile *t =variable[idx];
    for(int val: {0, 1}){
        //cout << "go through V x:" << t->x+1 << " y: " << t->y+1 << " assign value " << val << endl;
        if(checkVarAssignments(t, V[t], val)){
            t->value = val;
            assignments[t] = val;
            backtrack(variables, idx+1);
            assignments[t] = -1;
            //assignments.erase(t);
            t->value = -1;
        }
        //cout << "false at x:" << t->x+1 << " y: " << t->y+1 << " assign value " << val << endl;
        //cout << endl;
    }
    //t->value = -1;
}


void MyAI::printBoardInfo(     )
{
    cout << "---------------- Game Board Playerboard------------------\n" << endl;
    for ( int r = rowDimension-1; r >= 0; --r )
    {
        printf("%-4d%c",r+1,'|');
        for ( int c = 0; c < colDimension; ++c )
            printTileInfo ( c, r );
        cout << endl << endl;
    }

    cout << "----------------OVER----------------\n ";
    
}

void MyAI::printTileInfo( int c, int r )
{

    string tileString;
    tileString.append(to_string(playboard[c][r]));

    cout << setw(8) << tileString;
}

MyAI::~MyAI()
{
    // Deallocate memory for playboard
    for (int i = 0; i < rowDimension; ++i) {
        delete[] playboard[i];
        delete[] clearedboard[i];
        delete[] flaggedboard[i];
        //delete[] assignValueboard[i];
    }
    delete[] playboard;
    delete[] clearedboard;
    delete[] flaggedboard;
    //delete[] assignValueboard;
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================



// ======================================================================
// YOUR CODE ENDS
// ======================================================================
