//compile with g++ -std=c++11

#include <bits/stdc++.h>

using namespace std;

#define deb(x) cout << #x << " : " << x << endl


const int inf = 123123123;
enum Side{
    UP, RIGHT, DOWN, LEFT
};

Side getOpposite(Side s){
    return Side((s+2)%4);
}

ostream& operator<<(ostream& out, const Side& s){
    if(s == 0){
        out << "UP";
    }
    else if(s == 1){
        out << "RIGHT";
    }
    else if(s == 2){
        out << "DOWN" ;
    }
    else{
        out << "LEFT";
    }
    return out;
}

template<typename T, typename TT>
ostream& operator<<(ostream& out, const pair<T, TT>& p){
    out << p.first << " " << p.second ;
    return out;
}


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

Move makeMove(int i, int j, Side s){
    return make_pair(make_pair(i, j), s);
}

Move makeMove(int i, int j, int s){
    return makeMove(i, j, Side(s));
}

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

    int full()const{
        int R = getNumRows();
        int C = getNumCols();
        for(int i = 0 ;i < R ; i++){
            for(int j = 0 ; j < C; j ++){
                for(int k = 0 ; k < 4 ; k++){
                    if(getBox(i, j, Side(k)) == 0)
                        return false;
                }
            }
        }
        return true;
    }

    int getNumLinesAround(int i, int j)const{
        int ret = 0 ;
        for(int k = 0 ;k < 4; k ++){
            ret += getBox(i, j, Side(k));
        }
        return ret; 
    }

    vector<Move> getEmpties(int i, int j)const{
        vector<Move> ret;
        for(int k = 0 ; k < 4 ; k ++){
            if(getBox(i, j, Side(k)) == 0)
                ret.push_back(makeMove(i, j, k));
        }
        return ret; 
    }

    int dfs1(int i, int j , int sideDad, Move & last, int vis[][20])const{
        int ret = 0;
        if(vis[i][j] || !isInside(i, j)){
            return ret;
        }
        if(getEmpties(i,j).size() >= 3){
            return 0;
        }
        vis[i][j] = 1;
        ret ++;
        for(auto it : getEmpties(i, j)){
            if(it.second != sideDad){
                last = it;
                ret += dfs1(i + dr[it.second], j + dc[it.second], getOpposite(it.second), last, vis);
            }
        }
        return ret; 
    }

    vector<pair<int, Move> > getChains()const{
        vector<pair<int, Move> > ret;
        int vis[20][20]= {0};
        int R = getNumRows();
        int C = getNumCols();
        for(int i = 0 ;i < R;  i++){
            for(int j = 0 ;j < C ; j++){
                if(!vis[i][j] && getNumLinesAround(i, j) == 2){
                    Move lastMove;
                    int num =1 + dfs1(i, j, -1, lastMove, vis);
                    ret.push_back(make_pair(num, lastMove)); 
                }
            }
        }
        return ret;
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
  
    vector<pair<int, int> > getCShaped() const{
        int R = getNumRows();
        int C = getNumCols();
        vector<pair < int , int > > vec;
        for(int i = 0 ;i < R ; i++){
            for(int j = 0 ;j < C ; j++){
                if(isCShaped(i, j)){
                    vec.push_back(make_pair(i, j));
                }
            }
        }
        return vec;
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

    int dfs2(int i, int j , int sideDad, int vis[][20], int & is)const{
        int ret = 0;
        if(vis[i][j] || !isInside(i, j) || getEmpties(i, j).size() > 2){
            return ret;
        }
        vis[i][j] = 1;
        ret ++;
        int closed = 1;
        for(auto it : getEmpties(i, j)){
            if(it.second != sideDad){
                ret += dfs2(i + dr[it.second], j + dc[it.second], getOpposite(it.second), vis, is);
                closed = 0;
            }
        }
        is = max(closed, is);
        return ret; 
    }

    vector<pair<int, pair<int, Move> > > getWinChains()const{
        vector<pair<int, pair<int, Move> > > ret;
        int vis[20][20]= {0};
        int R = getNumRows();
        int C = getNumCols();
        for(int i = 0 ;i < R;  i++){
            for(int j = 0 ;j < C ; j++){
                if(!vis[i][j] && getNumLinesAround(i, j) == 3){
                    int is = 0;
                    auto empt = getEmpties(i, j)[0];
                    Move lastMove = empt;
                    vis[i][j] = 1;
                    int num =1 + dfs2(i+dr[empt.second], j + dc[empt.second], getOpposite(empt.second) , vis, is);
                    ret.push_back(make_pair(num,make_pair(is, lastMove))); 
                }
            }
        }
        return ret;

    }

    int dfs3(int i, int j, int num){
        if(num == 0){
            return 0;
        }
        auto empts = getEmpties(i, j);
        if(empts.size() == 1){
            setBox(empts.back(), 1);
            return 1 + dfs3(i + dr[empts.back().second], j + dc[empts.back().second], num -1);
        }
        else if(empts.size() == 0){
            return 1;
        }
        return 0;
    }

    pair<int, Move> greed(vector<pair<int, pair<int, Move> > > chains){
        sort(chains.begin(), chains.end());
        int num = 0; 
        pair<int, Move> ret(0, makeMove(0,0,-1));;
        vector<pair<int, pair<int, Move> > > others;
        for(auto it : chains){
            if(it.second.first){ //closed
                auto theMove = it.second.second;
                if(ret.second.second == -1){
                    ret.second = theMove;
                }
                ret.first += dfs3(theMove.first.first, theMove.first.second, it.first);
            }
            else{
                others.push_back(it);
            }
        }
        for(auto it: others){
            if(it == others.back()){
                auto theMove = it.second.second; 
                if(it.first >= 2){
                    if(ret.second.second == -1){
                        ret.second = theMove;
                    }
                    ret.first += dfs3(theMove.first.first, theMove.first.second, it.first - 2);
                }
                else{
                    if(ret.second.second == -1){
                        ret.second = theMove;
                    }
                    ret.first += dfs3(theMove.first.first, theMove.first.second, it.first);
                }
            }
            else{
                auto theMove = it.second.second; 
                if(ret.second.second == -1){
                    ret.second = theMove;
                }
                ret.first += dfs3(theMove.first.first, theMove.first.second, it.first);
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

    vector<Move> allWinnerMoves()const{
        vector<Move> ret;
        int R = getNumRows();
        int C = getNumCols();

        for(int i = 0 ;i < R; i ++){
            for(int j= 0 ;j < C ; j++){
                if(isCShaped(i, j)){
                    for(int k= 0 ; k < 4 ; k++){
                        if(getBox(i, j, Side(k)) == 0){
                            ret.push_back(makeMove(i, j, k));
                            break;
                        }
                    }
                }
            }
        }
        return ret; 
    }

    vector<Move> allSafeMoves()const{
        vector<Move> ret;
        int R = getNumRows(); 
        int C = getNumCols();

        for(int i = 0 ;i <R ; i++){
            for(int j = 0 ;j  <C ; j++){
                for(int k = 0 ; k < 4 ; k++){
                    if(getBox(i, j, Side(k)) == 0 && safeToDraw(i, j, Side(k))){
                        ret.push_back(makeMove(i, j, k));
                    }
                }
            }
        }

        return ret;
    }
    vector<Move> allValidMoves()const{
        vector<Move> ret;
        int R = getNumRows(); 
        int C = getNumCols();

        for(int i = 0 ;i <R ; i++){
            for(int j = 0 ;j  <C ; j++){
                for(int k = 0 ; k < 4 ; k++){
                    if(getBox(i, j, Side(k)) == 0){
                        ret.push_back(makeMove(i, j, k));
                    }
                }
            }
        }

        sort(ret.begin(), ret.end());
        ret.resize(unique(ret.begin(), ret.end()) - ret.begin());
        //random_shuffle(ret.begin(), ret.end());
        return ret;
    }
    vector<Move> allUnsafeMoves()const{
        vector<Move> ret;
        int R = getNumRows(); 
        int C = getNumCols();

        for(int i = 0 ;i <R ; i++){
            for(int j = 0 ;j  <C ; j++){
                for(int k = 0 ; k < 4 ; k++){
                    if((getBox(i, j, Side(k)) == 0) && !safeToDraw(i, j, Side(k))){
                        ret.push_back(makeMove(i, j, k));
                    }
                }
            }
        }
        sort(ret.begin(), ret.end());
        ret.resize(unique(ret.begin(), ret.end()) - ret.begin());
        //random_shuffle(ret.begin(), ret.end());
        return ret;
    }

    Move randomSafeMove()const{
        auto allS = allSafeMoves();
        if(allS.empty()){
            return makeMove(-1, -1, -1);
        }
        return allS[abs(rand())%allS.size()];
    }

    Move randomUnsafeMove()const{
        auto allU = allUnsafeMoves();
        if(allU.empty()){
            return makeMove(-1, -1, -1);
        }
        return allU[abs(rand())%allU.size()];
    }

    Move randomValidMove()const{
        auto allV = allValidMoves();
        if(allV.empty()){
            return makeMove(-1, -1, -1);
        }
        return allV[abs(rand())%allV.size()];
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

// game is 1 2 3 4, starting from left, clockwise
// game is 2 3 4 1, starting form up   , clockwise
// mine is 0 1 2 3 , starting from up  , clockwise

int gameSideToMe(int side){
    return (side -2 + 4)%4;
}

int mySideToGame(int side){
    if(side == 0){
        return 2;
    }
    if(side== 1){
        return 3;
    }
    if(side == 2){
        return 4;
    }
    return 1;
}

int plusMinus(int turn){
    if(turn == 0){
        return 1;
    }
    return -1;
}

int bt(const Board& b, int turn, int remDepth, int choose , Move * move){
    if(remDepth <= 0 || b.full()){
        return 0;
    }
    int ret = -plusMinus(turn)*inf;
    auto allSafe = b.allSafeMoves();
    auto allWin = b.allWinnerMoves();
//    cout << "all safe moves : " << endl;
//    for(auto it : allSafe){
//        cout << it << endl;
//    }
//    cout << endl;
    if(allWin.empty() && !allSafe.empty()){
        int num = min(choose, (int)allSafe.size());
        vector<Move> chosen;
        for(int i = 0 ;i < num ; i++){
            int ind = abs(rand())%allSafe.size();
            swap(allSafe[ind], allSafe[allSafe.size() -1]); 
            chosen.push_back(allSafe.back());
            allSafe.pop_back();
        }
        for(auto it : chosen){
//            if(remDepth == 5){
//                deb(it);
//            }
            Board bb = b;
            bb.setBox(it, 1);
            Move m;
            int val = bt(bb, 1- turn, remDepth -1 , choose, &m);
            if(turn == 0){
                if(val > ret){
                    ret = val;
                    *move= it;
                }
            }
            else{
                if(val < ret){
                    ret = val;
                    *move= it;
                }
            }
        }
//        if(remDepth == 5){
//            deb(*move);
//        }
    }
    else if(!allWin.empty()){
        auto chains = b.getWinChains();//(num, (open, start))
       // if(remDepth == 5){
       //     cout << "win chains : " << remDepth << endl;
       //     for(auto it : chains){
       //         cout << it << endl;
       //     }
       //     cout<< endl;
       // }
        if(chains.size() == 1 && chains[0].second.first == 0 && chains[0].first == 2){
            auto good = chains[0].second.second;
            auto sec = good;
            sec.first.first += dr[sec.second];
            sec.first.second+= dc[sec.second];
            for(auto it : b.getEmpties(sec.first.first, sec.first.second)){
                if(it.second != getOpposite(sec.second)){
                    sec.second = it.second;
                    break;
                }
            }
            Board bb = b;
            bb.setBox(sec, 1);
            Move m;
            int val1 = bt(bb, 1- turn, remDepth-1, choose, &m);
            Board bb2 = b;
            bb2.setBox(good, 1);
            bb2.setBox(sec, 1);
            int val2 = bt(bb2, turn, remDepth-1, choose, &m) + 2*plusMinus(turn);
            ret = val2;
            *move = good;
            if(turn == 0){
                if(val1 > val2){
                    ret = val1;
                    *move = sec;
                }
            }
            else{
                if(val1 < val2){
                    ret = val1;
                    *move = sec;
                }
            }
           // if(remDepth == 5){
           //     cout << "if I leave : " << val1 << endl;
           //     cout << "if I do : " << val2 << endl;
           //     deb(ret);
           //     deb(*move);
           // }
        }
        else{
            Board bb = b;
            auto whatToDo = bb.greed(chains);
            ret = whatToDo.first * plusMinus(turn);
            *move = whatToDo.second;
            Move m;
            ret += bt(bb, turn, remDepth-1, choose, &m);
        }
    }
    else{//empty safe
        vector<pair<int, Move> > chains = b.getChains();
        sort(chains.begin(), chains.end());
//        if(remDepth == 5){
//            cout << "chains : " << endl;
//            for(auto it : chains){
//                cout << it << endl;
//            }
//            cout << endl;
//        }
        for(int i = 0 ;i < min(choose, (int)chains.size());  i++){
            Board bb = b;
            bb.setBox(chains[i].second, 1);
            Move m;
            int val = bt(bb, 1- turn, remDepth -1, choose, &m);
            if(turn == 0){
                if(val > ret){
                    ret = val;
                    *move = chains[i].second;
                }
            }
            else{
                if(val < ret){
                    ret = val;
                    *move = chains[i].second;
                }
            }
        }
    }
    return ret;
}

Move play(const Board& b, int depth){
    Move ret;
    bt(b, 0, depth, 3, &ret);
    //deb(ret);
    return ret;
}

int main(){
    srand(time(0));
    int rows, cols, turn;
    cin >> rows >> cols >> turn;
    int r, c, s;
    Board b(rows, cols);
    while(cin >> r >> c >>s){
        if(r == -1){
            auto m= play(b,7);
//            auto m = b.randomSafeMove(); 
//            if(m.first.first != -1){
            b.setBox(m, 1);     
            cout << m.first.first << " " << m.first.second << " " << mySideToGame(m.second) << endl;
//            }
        }
        else{
            int mys = gameSideToMe(s);
            b.setBox(r, c, Side(mys),1);
        }
        b.out();
    }
    return 0;
}

