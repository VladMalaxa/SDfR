#include <iostream>
#include <array>
#include <utility>

const int size = 12; // Size should not vary.
using Maze = std::array<std::array<char, size>, size>; //Maze is a class to help with the declaration of Labirynthus

enum class Dir {//directions to move in
    down,
    up,
    right,
    left
};

void moveIN(int& i, int& j, Dir direction)
{
    switch(direction)
    {
        case Dir::down:  i++; break;
        case Dir::up:    i--; break;
        case Dir::right: j++; break;
        case Dir::left:  j--; break;
    }
}

Maze createMaze(){
    Maze Labyrinth = {{
        {{'#','#','#','#','#','#','#','#','#','#','#','#'}},
        {{'#','.','.','.','#','.','.','.','.','.','.','#'}},
        {{'.','.','#','.','#','.','#','#','#','#','.','#'}},
        {{'#','#','#','.','#','.','.','.','.','#','.','#'}},
        {{'#','.','.','.','.','#','#','#','.','#','.','x'}},
        {{'#','#','#','#','.','#','.','#','.','#','.','#'}},
        {{'#','.','.','#','.','#','.','#','.','#','.','#'}},
        {{'#','.','.','#','.','#','.','#','.','#','.','#'}},
        {{'#','.','.','.','.','.','.','.','.','#','.','#'}},
        {{'#','#','#','#','#','#','.','#','#','#','.','#'}},
        {{'#','.','.','.','.','.','.','#','.','.','.','#'}},
        {{'#','#','#','#','#','#','#','#','#','#','#','#'}}
    }};
    return Labyrinth;
}

void printMaze(const Maze Labyrinth){//initialize variable same as the createMaze function
    for (int i = 0; i < size; i++) {//Loop through the matrix to print each line.
        for (int j = 0; j < size; j++) {
            std::cout << Labyrinth[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

std::pair<int,int> startPosition(const Maze Labyrinth) //needs to return a pair of coordinates
{
    for (int i = 0; i < size; i++) {//Loop through the matrix to print each line.
        for (int j = 0; j < size; j++) {
            if (Labyrinth[i][j] == 'x')
            {
                std::cout <<"Starting position is: "<< i << " and "<< j;
                std::cout << '\n';
                std::cout << '\n';
                return {i, j};
            }
    }
    }
    return{15,15};//not found, outside bounds
}

bool insideMaze(int i, int j){//check if we are out of bounds
    return (i<size && j< size && i>=0 && j>=0);
}



void traverseMaze( Maze& Labyrinth, int i, int j, Dir direction ,int counter){// 
    moveIN(i,j,direction);
    if (insideMaze(i,j)!=true){
        std::cout<<"Exit found";
        std::cout<<'\n'<<"Number of iterations: "<<counter;
        std::exit(0);
    }
    if (Labyrinth[i][j] == '#' || Labyrinth[i][j] == 'x') {
        return;
    }
    Labyrinth[i][j] = 'x';
    printMaze(Labyrinth);
    std::cout << '\n';
    counter++;
    traverseMaze(Labyrinth, i, j,Dir::up,counter) ;
    traverseMaze(Labyrinth, i, j,Dir::down,counter);
    traverseMaze(Labyrinth, i, j,Dir::left,counter);
    traverseMaze(Labyrinth, i, j,Dir::right,counter);
    

}


int main(){
    Maze Labyrinth = createMaze();
    printMaze(Labyrinth);
    auto [start_vert,start_horiz] = startPosition(Labyrinth);//auto detection used to match with pair
    int counter = 0;
    traverseMaze(Labyrinth, start_vert+1,start_horiz,Dir::left,counter);
}