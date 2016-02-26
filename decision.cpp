#include <bits/stdc++.h>

using namespace std;

enum Side{
    UP, RIGHT, DOWN, LEFT
};

Side sideRightRotate(const Side& s){
    return Side((s +1)%4);
}

Side sideLeftRotate(const Side& s){
    return Side((s+3)%4);
}

int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

typedef unsigned int Under;
typedef pair<pair<int,int>, Side> Move;

class LLC{ // Low Level Container
private:
    vector<Under>arr;
    static const int GROUPS = 8;
    static const int MID_START = 16;
public:
    LLC(){
        arr.resize(GROUPS);
    }

    void set(int group, int col, int val){
        if(get(group, col) != val){
            arr[group/2] ^=(Under)(((Under) 1)<<((group&1)*MID_START + col));
        }
    }

    int get(int group, int col)const {
        return (arr[group/2]>>((group&1)*MID_START + col))&((Under)1);
    }

    int operator[](int i)const{
        return arr[i];
    }

    bool operator<(const LLC& b)const{
        int n = arr.size();
        for(int i = 0 ;i < n ; i++){
            if(arr[i] != b[i]){
                return arr[i] < b[i];
            }
        }
        return false;
    }

    bool operator==(const LLC& b)const{
        int n = arr.size();
        for(int i = 0 ;i < n ; i++){
            if(b[i] != arr[i]){
                return false;
            }
        }
        return true;
    }

    bool operator!=(const LLC& b)const{
        return !(*this == b);
    }

};

class Board{
private:
    LLC hori;
    LLC vert;
    int numRows;
    int numCols;
public:
    Board(int r, int c):numRows(r), numCols(c){
    }

    int getNumRows()const{
        return numRows;
    }

    int getNumCols()const{
        return numCols;
    }
    
    const LLC& getHoriz()const{
        return hori; 
    }

    const LLC& getVert()const{
        return vert;
    }

    int getBox(int r, int c, Side s)const{
        if(s == UP){
            return hori.get(r, c);
        }
        else if(s == RIGHT){
            return vert.get(r, c+1);
        }
        else if(s == DOWN){
            return hori.get(r+1, c);
        }
        else if(s == LEFT){
            return vert.get(r, c);
        }
    }

    void setBox(int r, int c, Side s, int val){
        if(s == UP){
            return hori.set(r, c, val);
        }
        else if(s == RIGHT){
            return vert.set(r, c+1, val);
        }
        else if(s == DOWN){
            return hori.set(r+1, c, val);
        }
        else if(s == LEFT){
            return vert.set(r, c, val);
        }
    }

    int getBox(const Move& m)const {
        return getBox(m.first.first, m.first.second, m.second);
    }

    void setBox(const Move& m, int val){
        setBox(m.first.first, m.first.second, m.second, val);
    }
    
    bool operator<(const Board& b)const{
//        if(getNumRows() != b.getNumRows()){
//            return getNumRows() < b.getNumRows();
//        }
//        if(getNumCols() != b.getNumCols()){
//            return getNumCols() < b.getNumCols();
//        }
        if(getHoriz() != b.getHoriz()){
            return getHoriz() < b.getHoriz();
        }
        return getVert() < b.getVert();
    }

    bool operator==(const Board& b)const{
//        return getNumRows() == b.getNumRows() && getNumCols() == b.getNumCols() && 
        return getHoriz() == b.getHoriz() && getVert() == b.getVert();
    }

    void out()const{
        vector<string> toOut; 
        for(int i = 0 ;i <numRows ; i++){
            string theRow = ".";
            for(int j = 0 ;j < numCols ; j++){
                if(getBox(i, j, UP)){
                    theRow += "_"; 
                }
                else{
                    theRow += " ";
                }
                theRow += ".";
            }
            toOut.push_back(theRow);
            theRow = " ";
            if(getBox(i, 0, LEFT)){
                theRow[0] = '|';
            }
            for(int j = 0 ;j < numCols ; j++){
                theRow += "  " ;
                if(getBox(i, j, RIGHT)){
                    theRow[theRow.size() -1] = '|';
                }
            }
            toOut.push_back(theRow);
        }
        string theRow = ".";
        for(int j = 0 ;j < numCols ; j++){
            if(getBox(numRows -1, j, DOWN)){
                theRow += "_"; 
            }
            else{
                theRow += " ";
            }
            theRow += ".";
        }
        toOut.push_back(theRow);
        for(auto it : toOut){
            cout << it << endl;
        }
        cout << endl ;
    }
   

    bool isSafe(int r, int c)const {
        return getBox(r, c, UP) + getBox(r, c, RIGHT) +
                getBox(r, c, DOWN) + getBox(r, c, LEFT) < 2;
    }

    bool isCShaped(int r, int c)const {
        return getBox(r, c, UP) + getBox(r, c, RIGHT) +
                getBox(r, c, DOWN) + getBox(r, c, LEFT) == 3;
    }
    
    bool isLessThanCShaped(int r, int c)const {
        return getBox(r, c, UP) + getBox(r, c, RIGHT) +
                getBox(r, c, DOWN) + getBox(r, c, LEFT) < 3;
    }
    
    bool isCaptured(int r, int c)const {
        return getBox(r, c, UP) + getBox(r, c, RIGHT) +
                getBox(r, c, DOWN) + getBox(r, c, LEFT) == 4;
    }

    bool isInside(int r, int c)const {
        return r >= 0 && r < numRows && c >= 0 && c < numCols;
    }
    
    bool isDone()const{
        for(int i = 0 ;i< numRows; i ++){
            for(int j = 0 ;j < numCols ; j++){
                 for(int s = 0 ;s < 4 ; s++){
                    if(!getBox(i, j, Side(s)) && safeToDraw(i, j, Side(s))){
                        return false;
                    }
                 }
            }
        }
        return true;
    }

    bool isDone(Move& m)const{
        for(int i = 0 ;i< numRows; i ++){
            for(int j = 0 ;j < numCols ; j++){
                 for(int s = 0 ;s < 4 ; s++){
                    if(!getBox(i, j, Side(s)) && safeToDraw(i, j, Side(s))){
                        m.first.first = i;
                        m.first.second = j;
                        m.second = Side(s);
                        return false;
                    }
                 }
            }
        }
        return true;
    }

    bool safeToDraw(int i, int j, Side s)const{
        return isSafe(i, j) &&
            (!isInside(i + dr[s], j + dc[s]) || isSafe(i + dr[s], j + dc[s]));
    }

    Board getRotateRight()const{
        int R = getNumRows();
        int C = getNumCols();
        Board ret(C, R);
        for(int i = 0 ;i < R;  i++){
            for(int j = 0 ;j < C;  j++){
                for(int s =0 ; s < 4; s++){
                    ret.setBox(j, R-1-i,sideRightRotate(Side(s)), getBox(i, j, Side(s)));
                }
            }
        }
        return ret;
    }

    Board getRotateLeft()const{
        int R = getNumRows();
        int C = getNumCols();
        Board ret(C, R);
        for(int i = 0 ;i < R;  i++){
            for(int j = 0 ;j < C;  j++){
                for(int s =0 ; s < 4; s++){
                    ret.setBox(C-1-j, i,sideLeftRotate(Side(s)), getBox(i, j, Side(s)));
                }
            }
        }
        return ret;
    }

    Board getMirrorVertical()const{
        int m = getNumCols();
        int n = getNumRows();
        Board ret= *this;
        for(int j = 0 ;j < m ; j++){
            for(int i =0 ;i < n/2; i++){
                ret.setBox(i, j, UP, getBox(n-1-i, j, DOWN));
                ret.setBox(n-1-i, j, DOWN, getBox(i,j, UP));
                ret.setBox(i, j, LEFT, getBox(n-1-i, j, LEFT));
                ret.setBox(i, j, RIGHT, getBox(n-1-i, j, RIGHT));
                ret.setBox(n-1-i, j, LEFT, getBox(i, j, LEFT));
                ret.setBox(n-1-i, j, RIGHT, getBox(i, j, RIGHT));
            }
        }
        return ret;
    }

    Board getMirrorHorizontal()const{
        int m = getNumCols();
        int n = getNumRows();
        Board ret= *this;
        for(int i = 0 ; i< n ; i++){
            for(int j = 0; j< m/2; j ++){
                 ret.setBox(i, j, LEFT, getBox(i, m-1-j, RIGHT));
                 ret.setBox(i, m-1-j, RIGHT, getBox(i, j, LEFT));

                 ret.setBox(i, m-1-j, UP, getBox(i, j, UP));
                 ret.setBox(i, m-1-j, DOWN, getBox(i, j, DOWN));
                 ret.setBox(i, j, UP, getBox(i, m-1-j, UP));
                 ret.setBox(i, j, DOWN, getBox(i, m-1-j, DOWN));

            }
        }
        return ret;
    }

    Board minify()const{
        Board ret = *this;
        for(int i = 0 ;i < 2 ; i++){
            for(int j=0 ; j < 2;  j++){
                Board now = *this;
                if(i){
                    now = now.getMirrorVertical();
                }
                if(j){
                    now = now.getMirrorHorizontal();
                }
                for(int k = 0 ;k < 4 ;k ++){
                    ret = min(ret, now);
                    now= now.getRotateRight();
                }
            }
        }
        return ret;
    }
};

map<Board, int> mp;

void go(Board b){
    b = b.minify(); 
    if(mp.find(b) != mp.end()){
        return;
    }
    mp[b] = 1;
    if(b.isDone()){
        return;
    }
    int n = b.getNumRows();
    int m = b.getNumCols();
    for(int i = 0 ;i < n ; i++){
        for(int j = 0 ;j < m ; j++){
            for(int k = 0 ; k < 4 ; k++){
                if(!b.getBox(i, j, Side(k))){
                    b.setBox(i, j, Side(k), 1);
                    go(b); 
                    b.setBox(i, j, Side(k), 0);
                }
            }
        }
    }
}

int main(){
    srand(123123156);
    Board b(3, 3);
    go(b); 
    cout <<mp.size() << endl;
    return 0;
}



