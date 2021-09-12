#include <iostream>
#include<bits/stdc++.h>
#include<vector>
#define Size 4
using namespace std;
int **makeAMove(int **board, int x, int y,int player,int isCheck);
vector<pair <int ,int >* >* possibleMoves(int ** board, int player){
     vector < pair <int ,int > *> *output = new vector< pair <int , int > *>();
        for(int i=0;i<Size;i++){
            for(int j=0;j<Size;j++){
                 if(board[i][j]==0){//if cell is empty
                      if(makeAMove(board,i,j,player,true)){// if valid move is possible at i and j  cell
                            pair<int ,int >*p = new pair<int , int >(i,j);
                            output->push_back(p);
                      }
               }
           }
    }
return output;
}
int evaluatedBoard(int **board, bool *completed){
      if(possibleMoves(board,1)->size()!=0 ||possibleMoves(board,2)->size()!=0){
            *completed= false;//if at lest one player able to take valid moves
             return 0;
      }
     *completed = true;//if no one able to take valid moves
      int count[3]={};
      // score evaluation
      for(int i=0;i<Size;i++){
            for(int j=0;j<Size;j++){
               count[board[i][j]]++;
            }
      }
return count[1]- count[2];// if score +ve maximizer win
}
int ** makeAMove(int **board,int x,int y,int player, int ischeck){
     int **newBoard;
     if(ischeck){
         newBoard = board;
     }
     else{
         newBoard = new int *[Size];
         for(int i=0;i<Size;i++){
                 newBoard[i]= new int [Size];
                 for(int j=0;j<Size;j++){
                       newBoard[i][j]= board[i][j];
                 }
        }
    }
    //for all possible direction
    int xDir[]= {-1,-1,0,1,1,1,0,-1};
    int yDir[]={ 0,1,1,1,0,-1,-1,-1};
    if(x<0 || x>=Size||  y<0|| y>=Size|| board[x][y]!=0){//if index out of bound
                   return NULL;
    }
    bool movePossible = false;
    for(int i=0;i<8;i++){
            int xStep = xDir[i];
            int yStep = yDir[i];
            int currentX = x +xStep;
            int currentY = y+ yStep;
            int count=0;
            while(currentX>=0 && currentX<Size && currentY>=0 && currentY < Size){
                         if(newBoard[currentX][currentY]==0)//if empty cell found
                                       break;
                         else if(newBoard[currentX][currentY]!=player){//if opposite player cell found
                                  currentX+= xStep;
                                  currentY+= yStep;
                                  count++;
                         }
                         else{
                                if(count>0){
                                      movePossible= true;//if at least one valid moves possible
                                      if(ischeck)//ischeck== true if we just have to check  at lest one moves is possible or not and we don't have change the board config.
                                            return newBoard;
                                      int convertX = currentX - xStep;
                                      int convertY = currentY - yStep;
                                      while(convertX!=x|| convertY!=y){// making changes between the two cell (B....B|| W....W) in all possible direction
                                                newBoard[convertX][convertY]= player;
                                                convertX = convertX - xStep;
                                                convertY = convertY - yStep;
                                      }
                                    newBoard[x][y]= player;
                               }
                               break;//no valid moves possible
                    }
       }
   }
   if(movePossible)
         return newBoard;
   else
         return NULL;
}
void printBoard(int **board){
    for(int i=0;i<Size;i++){
           for(int j=0;j<Size;j++){
                  cout<<board[i][j]<<" ";
           }
    cout<<endl;
    }
}
int findNextStep( int**board,bool maximizerTurn,int *x,int *y){
      //check if done with play, if yes return score of evaluated function
      bool completed;
      int score = evaluatedBoard(board, &completed);
      printBoard(board) ;
      cout<<endl;
      if(completed)//if neither player1  nor player2 are able to take valid moves or all the cells of the board get full
             return score;

     //find All possible moves
     int player = maximizerTurn?1:2;//1-> black(maximizer), 2-> white(minimizer)
     vector<pair<int , int > * > *options = possibleMoves(board, player);//getting all possible valid moves
     if(options->size()==0){
            *x=-1;
            *y=-1;
            int dummyX, dummyY;
            return findNextStep(board,!maximizerTurn, &dummyX,&dummyY);//checking other player is able to take valid moves or not
    }
    int best;
    if(maximizerTurn)
            best = INT_MIN;
    else
            best = INT_MAX;
    //for each possible  move
    for(int i=0;i<(int)options->size();i++){
                   //make the move
                   int currentMoveX = options->at(i)->first;
                   int currentMoveY = options->at(i)->second;
                   int **newBoard = makeAMove(board,currentMoveX,currentMoveY,player,false);
                   //make recursive call
                   int dummyX,dummyY;
                   //passing new configuration board to the other player after a player has taken a valid moves
                   int score = findNextStep(newBoard,!maximizerTurn,&dummyX,&dummyY);
                   //update best score based on the player turn (minimizer or maximizer)
                   if(maximizerTurn)
                           best = max(best,score);
                   else
                           best = min(best, score);
                   if(best == score){//updating  with the optimal move
                           *x= currentMoveX;
                           *y = currentMoveY;
                   }
                  //undo the move we will have to maintain a copy of the board for this
                  for(int i=0;i<Size;i++){
                            delete[]newBoard[i];
                  }
                  delete[] newBoard;
  }
  //update x and y return best score
  return best;
}
int main()
{
   int **grid = new int *[Size];
   for(int i=0;i<Size;i++){
       grid[i]= new int[Size];
       for(int j=0;j<Size;j++){
          grid[i][j]=0;
       }
   }
   //1-> black
   //2-> white
   grid[1][1]=1;
   grid[1][2]=2;
   grid[2][2]=1;
   grid[1][0]=2;
   //grid[2][1]=2;
    int xi,yj;
    int score = findNextStep(grid,true,&xi,&yj);
    if(score > 0)
         cout<<"Black player Win!"<<endl;
    else if(score < 0)
          cout<<"White player Win!"<<endl;
     else
        cout<<"Game draw!"<<endl;
   cout<<"BestScore:"<<abs(score)<<endl;
   cout<<"Optimal Move:"<<xi<<","<<yj<<endl;//best move
   return 0;
}
