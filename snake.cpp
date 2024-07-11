#include <conio.h> 
#include <iostream> 
#include <windows.h> 
using namespace std; 

const int width = 80; 
const int height = 20; 

int x, y; 
int fruitCordX, fruitCordY; 
int playerScore; 
int snakeTailX[100], snakeTailY[100]; 
int snakeTailLen; 
enum snakesDirection { STOP = 0, LEFT, RIGHT, UP, DOWN }; 
snakesDirection sDir; 
bool isGameOver; 
bool isPaused = false;
int highestScore = 0;

// Function to initialize game variables 
void GameInit() 
{ 
    isGameOver = false; 
    sDir = STOP; 
    x = width / 2; 
    y = height / 2; 
    fruitCordX = rand() % width; 
    fruitCordY = rand() % height; 
    playerScore = 0; 
    snakeTailLen = 0;
} 

// Function to generate new fruit coordinates ensuring it doesn't spawn inside the snake's body
void GenerateFruit() 
{
    bool fruitOnSnake;
    do {
        fruitCordX = rand() % width; 
        fruitCordY = rand() % height; 
        fruitOnSnake = false;
        for (int i = 0; i < snakeTailLen; i++) {
            if (snakeTailX[i] == fruitCordX && snakeTailY[i] == fruitCordY) {
                fruitOnSnake = true;
                break;
            }
        }
    } while (fruitOnSnake);
}

// Function for creating the game board & rendering 
void GameRender(string playerName) 
{ 
    system("cls"); // Clear the console 

    // Creating top walls with '-' 
    for (int i = 0; i < width + 2; i++) 
        cout << "-"; 
    cout << endl; 

    for (int i = 0; i < height; i++) { 
        for (int j = 0; j <= width; j++) { 
            // Creating side walls with '|' 
            if (j == 0 || j == width) 
                cout << "|"; 
            // Creating snake's head with 'O' 
            if (i == y && j == x) 
                cout << "O"; 
            // Creating the snake's food with '#' 
            else if (i == fruitCordY && j == fruitCordX) 
                cout << "#"; 
            // Creating snake's tail with 'o' 
            else { 
                bool prTail = false; 
                for (int k = 0; k < snakeTailLen; k++) { 
                    if (snakeTailX[k] == j && snakeTailY[k] == i) { 
                        cout << "o"; 
                        prTail = true; 
                    } 
                } 
                if (!prTail) 
                    cout << " "; 
            } 
        } 
        cout << endl; 
    } 

    // Creating bottom walls with '-' 
    for (int i = 0; i < width + 2; i++) 
        cout << "-"; 
    cout << endl; 

    // Display player's score 
    cout << playerName << "'s Score: " << playerScore << endl; 
    cout << "Highest Score: " << highestScore << endl;
    if (isPaused) 
        cout << "Game Paused. Press 'p' to resume." << endl;
} 

// Function for updating the game state 
void UpdateGame() 
{ 
    int prevX = snakeTailX[0]; 
    int prevY = snakeTailY[0]; 
    int prev2X, prev2Y; 
    snakeTailX[0] = x; 
    snakeTailY[0] = y; 

    for (int i = 1; i < snakeTailLen; i++) { 
        prev2X = snakeTailX[i]; 
        prev2Y = snakeTailY[i]; 
        snakeTailX[i] = prevX; 
        snakeTailY[i] = prevY; 
        prevX = prev2X; 
        prevY = prev2Y; 
    } 

    switch (sDir) { 
    case LEFT: 
        x--; 
        break; 
    case RIGHT: 
        x++; 
        break; 
    case UP: 
        y--; 
        break; 
    case DOWN: 
        y++; 
        break; 
    } 

    // Checks for snake's collision with the wall 
    if (x >= width || x < 0 || y >= height || y < 0) 
        isGameOver = true; 

    // Checks for collision with the tail 
    for (int i = 0; i < snakeTailLen; i++) { 
        if (snakeTailX[i] == x && snakeTailY[i] == y) 
            isGameOver = true; 
    } 

    // Checks for snake's collision with the food 
    if (x == fruitCordX && y == fruitCordY) { 
        playerScore += 10; 
        GenerateFruit(); 
        snakeTailLen++; 
        if (playerScore > highestScore)
            highestScore = playerScore;
    } 
} 

// Function to set the game difficulty level 
int SetDifficulty() 
{ 
    int dfc, choice; 
    cout << "\nSET DIFFICULTY\n1: Easy\n2: Medium\n3: Hard\nNOTE: if not chosen or pressed any other key, the difficulty will be automatically set to medium\nChoose difficulty level: "; 
    cin >> choice; 
    switch (choice) { 
    case '1': 
        dfc = 150; 
        break; 
    case '2': 
        dfc = 100; 
        break; 
    case '3': 
        dfc = 50; 
        break; 
    default: 
        dfc = 100; 
    } 
    return dfc; 
} 

// Function to handle user input 
void UserInput() 
{ 
    // Checks if a key is pressed 
    if (_kbhit()) { 
        // Getting the pressed key 
        switch (_getch()) { 
        case 'a': 
            if (sDir != RIGHT) sDir = LEFT; 
            break; 
        case 'd': 
            if (sDir != LEFT) sDir = RIGHT; 
            break; 
        case 'w': 
            if (sDir != DOWN) sDir = UP; 
            break; 
        case 's': 
            if (sDir != UP) sDir = DOWN; 
            break; 
        case 'x': 
            isGameOver = true; 
            break; 
        case 'p': 
            isPaused = !isPaused; 
            break; 
        } 
    } 
} 

// Main function / game loop 
int main() 
{ 
    string playerName; 
    cout << "Enter your name: "; 
    cin >> playerName; 
    int dfc = SetDifficulty(); 

    GameInit(); 
    while (!isGameOver) { 
        if (!isPaused) {
            GameRender(playerName); 
            UserInput(); 
            UpdateGame(); 
            Sleep(dfc); 
        } else {
            UserInput();
        }
    } 

    cout << "Game Over! Final Score: " << playerScore << endl;
    return 0; 
}
