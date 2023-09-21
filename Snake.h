//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
    Header file for the Snake Game
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef SNAKE_H
#define SNAKE_H

#include <QApplication>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QObject>
#include <QPushButton>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constants
// Size of the Board
static const int WINDOW_WIDTH = 400;
static const int WINDOW_HEIGHT = 400;

// Dot Size and number of all dots(All dots = 400*400/10*10)
static const int DOT_SIZE = 10;
static const int ALL_DOTS = 1600;

// Random position for the apple and speed of the game
static const int RAND_POS = 39;
static const int DELAY = 143;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Snake : public QWidget {
    Q_OBJECT
public:
    // Constructor
    Snake(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    void keyPressEvent(QKeyEvent*);

private:
    /*
      Images of
      --Snake body
      --Snake head
      --Apple
    */
    QImage dot;
    QImage head;
    QImage apple;

    /*
      --Timer
      --Number of snake joints
    */
    int timerId;
    int dots;

    // Coords of the apple
    int apple_x;
    int apple_y;

    // Snake coords
    int x[ALL_DOTS];
    int y[ALL_DOTS];

    /*
        This integer will be used to randomly
        place the snake in the starting direction.
    */
    int startDirection;

    // Direction of the Snake
    bool leftDirection;
    bool rightDirection;
    bool upDirection;
    bool downDirection;

    // Is the game being played?
    bool inGame;

    // Is the Game paused?
    bool isPaused;

    /*
      Check if there is an overlap
      between the generated apple
      and the snake
    */
    bool overlap;

    /*
      --Initialize the snake direction
      --Initialize the body of the snake
      --Load images
      --Initialize the game
      --Locate the apple
      --Check if the snake has eaten the apple
      --Check if the snake has collided with a wall or body
      --Move the Snake (Very important!)
      --Draw the snake and apple
      --Game Over
      --Game is Paused

      --Random integer generator
    */
    void initializeSnakeDirection();
    void initializeSnake();
    void loadImages();
    void initGame();
    void locateApple();
    void checkApple();
    void checkCollision();
    void move();
    void doDrawing();
    void gameOver(QPainter&);
    void gameIsPaused(QPainter&);
    int getRandomInt(int max_val);

    // Start the game button
    QPushButton* startButton;
    void createStartButton();

    // Help button
    QPushButton* helpButton;
    void createHelpButton();

    // Restart the game button
    QPushButton* restartButton;
    void createRestartButton();

    // Exit the game button
    QPushButton* exitButton;
    void createExitButton();

    // Media Player
    QMediaPlayer* SnakePlayer;
    void createSnakePlayer();
    void resetSnakePlayer();

    // Game Over and Power Up sound effects
    void setGameOverFile();
    void setPowerUpFile();

private slots:
    void restartGame();
    void displayHelp();
};

#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
