//=============================================================================
// Authors : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group : 2
// License : LGPL open source license
//
// Brief : Edge case where many bifurcations are present in the maze.
//
//=============================================================================

#include <array>
#include <iostream>
#include <utility>   // std::pair
#include <cstdlib>   // std::exit

constexpr int SIZE = 12;
using Maze = std::array<std::array<char, SIZE>, SIZE>;

enum class Direction
{
    Down,
    Right,
    Up,
    Left
};

void printMaze(const Maze& maze)                    //This function prints the maze on the terminal so that the user can see all the steps in the solving of the maze
{
    for (const auto& row : maze)
    {
        for (char c : row)
            std::cout << c << ' ';
        std::cout << '\n';
    }
    std::cout << "------------------------\n";      //This function finds the starting "X" position.
}

std::pair<int, int> findStart(const Maze& maze)
{
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (maze[r][c] == 'x')
                return {r, c};

    return {-1, -1}; // not found
}

bool isInside(int r, int c)                         //This function checks whether we are still inside the maze
{
    return r >= 0 && r < SIZE && c >= 0 && c < SIZE;
}

// Exit rule: any boundary cell that is not the starting cell.
bool isExit(int r, int c, int startR, int startC)
{
    bool onBoundary = (r == 0 || r == SIZE - 1 || c == 0 || c == SIZE - 1);
    return onBoundary && !(r == startR && c == startC);
}

void move(Direction dir, int& r, int& c)            //Moves the current position based on the direction.
{
    switch (dir)
    {
        case Direction::Down:  ++r; break;
        case Direction::Right: ++c; break;
        case Direction::Up:    --r; break;
        case Direction::Left:  --c; break;
    }
}

// Recursive traversal: moves only onto '.' squares, marks visited with 'x',
// prints after each move, and exits when an exit is reached.
void traverseMaze(Maze& maze, int r, int c, int startR, int startC)
{
    if (isExit(r, c, startR, startC))
    {
        printMaze(maze);
        std::cout << "Maze is solved!\n";
        std::exit(0);
    }

    Direction directions[4] = {
        Direction::Down,
        Direction::Right,
        Direction::Up,
        Direction::Left
    };

    for (Direction dir : directions)
    {
        int nr = r;
        int nc = c;

        move(dir, nr, nc);

        if (isInside(nr, nc) && maze[nr][nc] == '.')
        {
            maze[nr][nc] = 'x';   // mark visited
            printMaze(maze);      // display after each move
            traverseMaze(maze, nr, nc, startR, startC);
        }
    }
}

int main()
{
    Maze maze = {{
        {{'#','#','#','#','#','#','#','#','#','#','#','#'}},
        {{'#','.','.','.','#','.','.','.','.','.','.','#'}},
        {{'.','.','#','.','#','.','#','#','.','#','.','#'}},
        {{'#','.','#','.','#','.','.','.','.','#','.','#'}},
        {{'#','.','.','.','.','#','#','#','.','#','.','x'}},
        {{'#','.','#','#','.','.','.','#','.','#','.','#'}},
        {{'#','.','.','#','.','#','.','#','.','#','.','#'}},
        {{'#','.','.','#','.','#','.','#','.','#','.','#'}},
        {{'#','.','.','.','.','.','.','.','.','#','.','#'}},
        {{'#','#','#','#','#','#','.','#','#','#','.','#'}},
        {{'#','.','.','.','.','.','.','#','.','.','.','#'}},
        {{'#','#','#','#','#','#','#','#','#','#','#','#'}}
    }};

    auto [startR, startC] = findStart(maze);
    if (startR == -1)
    {
        std::cout << "Error: start position 'x' not found.\n";
        return 1;
    }

    printMaze(maze);
    traverseMaze(maze, startR, startC, startR, startC);

    // If no exit exists, recursion will eventually return here.
    std::cout << "No exit found.\n";
    return 0;
}