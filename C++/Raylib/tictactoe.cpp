#include "raylib.h"

struct Box {
    float x, y;
    float size = 120;
    char marked = ' ';

    bool isFilled() { return marked != ' ';}
};

bool checkWin(Box boxes[]) {
    return ((boxes[0].isFilled() && boxes[0].marked == boxes[1].marked && boxes[0].marked == boxes[2].marked) ||
            (boxes[3].isFilled() && boxes[3].marked == boxes[4].marked && boxes[3].marked == boxes[5].marked) ||
            (boxes[6].isFilled() && boxes[6].marked == boxes[7].marked && boxes[6].marked == boxes[8].marked) ||
            // Horizontal
            (boxes[0].isFilled() && boxes[0].marked == boxes[3].marked && boxes[0].marked == boxes[6].marked) ||
            (boxes[1].isFilled() && boxes[1].marked == boxes[4].marked && boxes[1].marked == boxes[7].marked) ||
            (boxes[2].isFilled() && boxes[2].marked == boxes[5].marked && boxes[2].marked == boxes[8].marked) ||
            // Vertical
            (boxes[0].isFilled() && boxes[0].marked == boxes[4].marked && boxes[0].marked == boxes[8].marked) ||
            (boxes[2].isFilled() && boxes[2].marked == boxes[4].marked && boxes[2].marked == boxes[6].marked));
            // Diagonal
}

bool checkDraw(Box boxes[]) {
    for (int i = 0; i < 9; i++) {
        if (!boxes[i].isFilled()) return false;
    }
    return true;
}

void resetBoard(Box boxes[]) {
    for (int i = 0; i < 9; i++) boxes[i].marked = ' ';
}

int main() {
    InitWindow(420,500,"Tic Tac Toes");
    bool gameOver = false;
    bool gameDraw = false;
    char currentPlayer = 'X';
    Box boxes[9];
    int index = 0;
    int margin = 20;
    for (int y = 90; y <= 400; y += 120+margin) {
        for (int x = margin/2; x <= 400; x += 120+margin) {
            boxes[index].x = x;
            boxes[index].y = y;
            index++;
        }
    }

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (!gameOver || gameDraw)) {
            for (int i = 0; i < 9; i++) {
                if (CheckCollisionPointRec(mousePos, {boxes[i].x,boxes[i].y,boxes[i].size,boxes[i].size}) && !boxes[i].isFilled()) {
                    boxes[i].marked = currentPlayer;
                    if (checkWin(boxes)) gameOver = true;
                    else if (checkDraw(boxes)) gameDraw = true;
                    else currentPlayer = (currentPlayer == 'X' ? 'O' : 'X');
                    break;
                }
            }
        }
        if ((gameOver || gameDraw) && IsKeyPressed(KEY_R)) {
            resetBoard(boxes);
            gameOver = false;
            gameDraw = false;
            currentPlayer = 'X';
        }

        // DRAW
        // --------------------------------------------------------
        BeginDrawing();
            float textSize = 40;
            for (int i = 0; i < 9; i++) {

                // DRAWS TEXT
                // ------------------------------------------------
                if (gameOver) {
                    DrawText(TextFormat("%c Wins !", currentPlayer),margin,80-textSize-(textSize/2),textSize,WHITE);
                    DrawText("Press R to restart",margin,80-(textSize/2),textSize/2,SKYBLUE);
                } else if (gameDraw) {
                    DrawText("Draw !",margin,80-textSize-(textSize/2),textSize,WHITE);
                    DrawText("Press R to restart",margin,80-(textSize/2),textSize/2,SKYBLUE);
                } else {
                    DrawText("Tic Tac Toe",margin,80-textSize-(textSize/2),textSize,WHITE);
                    DrawText("Click on remaining boxes",margin,80-(textSize/2),textSize/2,SKYBLUE);
                }
                // DRAWS BOXES
                // ------------------------------------------------
                DrawRectangleRounded({boxes[i].x, boxes[i].y, boxes[i].size, boxes[i].size},0.1,2,
                    // MAKES THE EMPTY BOX HIGHLIGHTED
                    (CheckCollisionPointRec(mousePos, {boxes[i].x,boxes[i].y,boxes[i].size,boxes[i].size}) && !boxes[i].isFilled() && (!gameOver || gameDraw)) ? SKYBLUE : DARKBLUE);
                // DRAWS TEXT INSIDE BOX
                if (boxes[i].isFilled()){DrawText(TextFormat("%c", boxes[i].marked),(boxes[i].x+(margin)),(boxes[i].y+(margin/2)),boxes[i].size, WHITE);}
            }
            ClearBackground(BLUE);
        EndDrawing();
    }
}