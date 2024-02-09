//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
    Implementation of the Snake class.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "Snake.h"
#include <QMessageBox>
#include <QPainter>
#include <QTime>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Snake::Snake(QWidget* parent)
    : QWidget(parent) {
    // Set the board style and size
    setStyleSheet("background-color:rgb(18,58,99);");
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Start the game button
    createStartButton();

    // Help button
    createHelpButton();

    // Restart and exit buttons
    createRestartButton();
    createExitButton();

    // Media Player for sound effects
    createSnakePlayer();

    // Set the initial Game State
    inGame = true;
    isPaused = false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Initilization of snake direction
void Snake::initializeSnakeDirection() {
    /*
      Generate a random number
      between 0-3 (0 or 1 or 2 or 3)
      Check getRandomInt() method for
      more details.
    */
    startDirection = getRandomInt(4);

    // Initialize all directions to false
    leftDirection = false;
    rightDirection = false;
    upDirection = false;
    downDirection = false;

    // Initialize Pause state
    isPaused = false;

    // Set initial direction based on startDirection
    switch (startDirection) {
    case 0:
        leftDirection = true;
        break;
    case 1:
        rightDirection = true;
        break;
    case 2:
        upDirection = true;
        break;
    case 3:
        downDirection = true;
        break;
    }

    // Reset the inGame state to true
    if (!inGame) {
        inGame = true;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Initialize Snake starting position based on startDirection
void Snake::initializeSnake() {
    // Starting number of the Snake joints
    dots = 3;
    /*
      The joints of the snake are placed
      based on the startingDirection.
    */
    switch (startDirection) {
    case 0:
        // Direction of the Snake == leftDirection
        for (int i = 0; i < dots; i++) {
            x[i] = 150 + (10 * i);
            y[i] = 50;
        }
        break;
    case 1:
        // Direction of the Snake == rightDirection
        for (int i = 0; i < dots; i++) {
            x[i] = 50 - (10 * i);
            y[i] = 50;
        }
        break;
    case 2:
        // Direction of the Snake == upDirection
        for (int i = 0; i < dots; i++) {
            x[i] = 150;
            y[i] = 150 + (i * 10);
        }
        break;
    case 3:
        // Direction of the Snake == downDirection
        for (int i = 0; i < dots; i++) {
            x[i] = 150;
            y[i] = 50 - (i * 10);
        }
        break;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Load dot, head and apple images.
void Snake::loadImages() {
    dot.load("images/dot.png");
    head.load("images/head.png");
    apple.load("images/apple.png");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Game initilization
void Snake::initGame() {
    // Check if Start button is visible
    if (startButton->isVisible()) {
        startButton->setVisible(false);
    }
    // check if Help button is visible
    if (helpButton->isVisible()) {
        helpButton->setVisible(false);
    }
    /*
      --Load images
      --Initialize the snake
      --Locate the apple
      --Start Timer
      --Set the focus on the game window
    */
    loadImages();
    initializeSnakeDirection();
    initializeSnake();
    locateApple();
    timerId = startTimer(DELAY);
    setFocus();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
    This method generates the random coordinates
    for the apple. It also checks for any overlaps
    between the generated apple and the snake. If
    an ovelap is found, a new set of coordinates is
    generated, until there is no overlap.
    Check also the getRandomInt() method for more details.
*/
void Snake::locateApple() {
    int r, i;
    do {
        // Generate random position for the apple
        r = getRandomInt(RAND_POS);
        apple_x = (r * DOT_SIZE);
        r = getRandomInt(RAND_POS);
        apple_y = (r * DOT_SIZE);
        // Check for overlap with snake joints
        overlap = false;
        for (i = 0; i < dots; i++) {
            if (apple_x == x[i] && apple_y == y[i]) {
                overlap = true;
                break;
            }
        }
    } while (overlap);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
  Check if the apple collides with the head,
  thus increasing the number of joints of the
  snake by one
*/
void Snake::checkApple() {
    /*
      x[0], y[0] --> Snake head.
      apple_x, apple_y --> Apple coords.
    */
    if ((x[0] == apple_x) && (y[0] == apple_y)) {
        dots++;
        // play the power up sound effect
        setPowerUpFile();
        locateApple();
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
    Function to check if the snake has
    collided with a wall
*/
void Snake::checkCollision() {
    /*
        Check if the head x[0], y[0] has collided
        with any part of the body/
    */
    for (int i = dots; i > 0; i--) {

        if ((x[0] == x[i]) && (y[0] == y[i])) {
            inGame = false;
        }
    }
    /*
        Check if the head x[0], y[0] has
        gone out of bounds
    */
    if (y[0] >= WINDOW_HEIGHT) {
        inGame = false;
    }

    if (y[0] < 0) {
        inGame = false;
    }

    if (x[0] >= WINDOW_WIDTH) {
        inGame = false;
    }

    if (x[0] < 0) {
        inGame = false;
    }

    /*
      If the snake has collided
      or has gone out of bound the game is over.
    */
    if (!inGame) {
        //Kill the timer
        killTimer(timerId);
        // Play Game Over sound effect
        setGameOverFile();
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Move the Snake!
void Snake::move() {
    /*  Move the body parts of the Snake
        starting from the tail. Each joint of
        the snake is moves into the position
        of the one in front of it.
    */
    for (int i = dots; i > 0; i--) {
        x[i] = x[(i - 1)];
        y[i] = y[(i - 1)];
    }
    /*
        If the direction of the snake is
        to the left, the x coord is lowered by one.
    */
    if (leftDirection) {
        x[0] -= DOT_SIZE;
    }
    /*
        If the direction of the snake is
        to the right, the x coord is increased by one.
    */
    if (rightDirection) {
        x[0] += DOT_SIZE;
    }
    /*
        If the direction of the snake is
        up, the y coord is lowered by one.
    */
    if (upDirection) {
        y[0] -= DOT_SIZE;
    }
    /*
        If the direction of the snake is
        down, the y coord is increased by one.
    */
    if (downDirection) {
        y[0] += DOT_SIZE;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Draw Stuff
void Snake::doDrawing() {
    QPainter qp(this);
    // If the game is still on , draw stuff
    if (inGame) {
        qp.drawImage(apple_x, apple_y, apple);

        for (int i = 0; i < dots; i++) {
            if (i == 0) {
                qp.drawImage(x[i], y[i], head);
            } else {
                qp.drawImage(x[i], y[i], dot);
            }
        }
        // Check if the game is Paused.
        if (isPaused) {
            // Game is Paused
            gameIsPaused(qp);
            return;
        }
    } else {
        // Game Over
        gameOver(qp);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Stuff to do when the game is over
void Snake::gameOver(QPainter& qp) {
    // Game over text to be displayed
    QString message = "Game over";
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(message);
    int textHeight = fm.height();

    // Score text to be displayed
    QString scoreMessage = "Score: " + QString::number((dots - 3));
    int scoreTextWidth = fm.horizontalAdvance(scoreMessage);

    // Dimensions of the "Restart" and "Exit" buttons
    int buttonWidth = 100;
    int buttonHeight = 30;

    qp.setFont(font);
    qp.translate(QPoint(width() / 2, height() / 2));

    // Display the "Game Over" and "Score" texts
    qp.drawText(-textWidth / 2, -textHeight / 2, message);
    qp.drawText(-scoreTextWidth / 2, -textHeight / 2 - buttonHeight - 20, scoreMessage);

    // Set the position of "Restart" and "Score" buttons
    restartButton->setGeometry(width() / 2 - buttonWidth / 2, height() / 2 + textHeight / 2, buttonWidth, buttonHeight);
    exitButton->setGeometry(width() / 2 - buttonWidth / 2, height() / 2 + textHeight / 2 + buttonHeight + 10, buttonWidth,
                            buttonHeight);
    // Display the two buttons
    restartButton->setVisible(true);
    exitButton->setVisible(true);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Stuff to do when the game is Paused
void Snake::gameIsPaused(QPainter& qp) {
    // Text to be displayed when the game is paused
    QString pause_message_1 = "Game is Paused";
    QString pause_message_2 = "Press P to continue";
    QString pause_message_3 = "Press esc to exit";
    QFont pause_font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(pause_font);
    /*
      Set the text at the desired position inside the window
      (Centered and pause_message_2 right below pause_message_1)
    */
    int pause_textWidth_1 = fm.horizontalAdvance(pause_message_1);
    int pause_textWidth_2 = fm.horizontalAdvance(pause_message_2);
    int pause_textWidth_3 = fm.horizontalAdvance(pause_message_3);
    int pause_textHeight = fm.height();
    int combined_height = pause_textHeight * 2;

    qp.setFont(pause_font);
    qp.translate(QPoint(width() / 2, height() / 2 - combined_height / 2));

    // Display the text
    qp.drawText(-pause_textWidth_1 / 2, -pause_textHeight / 2, pause_message_1);
    qp.drawText(-pause_textWidth_2 / 2, pause_textHeight / 2, pause_message_2);
    qp.drawText((-pause_textWidth_3 / 2)-0.5, (pause_textWidth_3 / 3)-0.5, pause_message_3);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Start button. When pressed, the game begins!
void Snake::createStartButton() {
    // Start the Game
    startButton = new QPushButton("Start", this);
    startButton->setVisible(true);

    // Fixed size button to 150x50 pixels
    startButton->setFixedSize(150, 50);

    // Calculate the position to center the button
    int x = (width() - startButton->width()) / 2;
    int y = (height() - startButton->height()) / 2 - 30;

    // Set the position of the button
    startButton->setGeometry(x, y, startButton->width(), startButton->height());
    connect(startButton, &QPushButton::clicked, this, &Snake::initGame);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Help button. When presses, the directions are displayed
void Snake::createHelpButton() {
    helpButton = new QPushButton("Help", this);
    helpButton->setVisible(true);
    // Fixed size to 150x50 pixels
    helpButton->setFixedSize(150, 50);
    // Set the position relative to "Start" button
    int x = startButton->x();
    int y = startButton->y() + startButton->height() + 10;
    helpButton->move(x, y);

    connect(helpButton, &QPushButton::clicked, this, &Snake::displayHelp);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Restart Button. Visible only after the Game is Over
void Snake::createRestartButton() {
    // Restart Game
    restartButton = new QPushButton("Restart", this);
    restartButton->setVisible(false);
    connect(restartButton, &QPushButton::clicked, this, &Snake::restartGame);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Exit Button. Visible only after the Game is Over
void Snake::createExitButton() {
    // Exit Game
    exitButton = new QPushButton("Exit", this);
    exitButton->setVisible(false);
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Create the Media Player (for sound effects)
void Snake::createSnakePlayer() {
    // Create the media player
    SnakePlayer = new QMediaPlayer;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Function to set the media player in a clean state
void Snake::resetSnakePlayer() {
    // Stop the current playback
    SnakePlayer->stop();
    // Remove any previously set file
    SnakePlayer->setMedia(QMediaContent());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Add game over sound effect
void Snake::setGameOverFile() {
    // Add the file
    SnakePlayer->setMedia(QUrl::fromLocalFile("/home/dot/Documents/GitHub/Snake_Game/Sound_Effects/game_over.mp3"));
    SnakePlayer->play();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Add power up sound effect
void Snake::setPowerUpFile() {
    // Add the file
    SnakePlayer->setMedia(QUrl::fromLocalFile("/home/dot/Documents/GitHub/Snake_Game/Sound_Effects/power_up.mp3"));
    SnakePlayer->play();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Function to restart the Game.
void Snake::restartGame() {
    // Check if start button is visible
    if (startButton->isVisible()) {
        startButton->setVisible(false);
    }
    // check if Help button is visible
    if (helpButton->isVisible()) {
        helpButton->setVisible(false);
    }
    restartButton->setVisible(false);
    exitButton->setVisible(false);

    // Reset the Media Player
    resetSnakePlayer();
    // Initialize game
    initGame();

    // Update Canvas and focus on it
    update();
    setFocus();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Display help message
void Snake::displayHelp() {
    // Create a QMessageBox to display the help text
    QMessageBox::information(this, "Help",
                             "--Use the arrow keys to contorl the snake\n\n--Eat the apples\n\n--Press P to pause/unpause the game");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// This function is called when stuff need to be painted
void Snake::paintEvent(QPaintEvent* e) {
    Q_UNUSED(e);
    // Draw Stuff. Check doDrawing() function.
    doDrawing();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Event handler function
void Snake::timerEvent(QTimerEvent* e) {
    Q_UNUSED(e);
    // Check is the game is still being played
    if (inGame) {
        if (!isPaused) {
            checkApple();
            checkCollision();
            move();
        }
    }
    // Repaint stuff
    repaint();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
  Function for the movement of the snake.
  The movement of the Snake is possible by
  using the arrow keys.
*/
void Snake::keyPressEvent(QKeyEvent* e) {
    int key = e->key();

    // The player can pause the game
    if (key == Qt::Key_P) {
        /*
          Prevent the user from trying to
          pause the game while in the game
          has not started yet
        */
        if (startButton->isVisible()) {
            return;
        }
        // User has pressed the P button
        if (inGame) {
            if (!isPaused) {
                isPaused = true;
                killTimer(timerId);
            } else {
                isPaused = false;
                timerId = startTimer(DELAY);
            }
        }
        update();
    }
    /*
        The user can press the esc button
        to exit, when the game is Paused
    */
    if (key == Qt::Key_Escape){
        if (!inGame){
            return;
        }else{
            if(isPaused){
                qApp->exit();
            }
        }
    }

    if ((key == Qt::Key_Left) && (!rightDirection)) {
        leftDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if ((key == Qt::Key_Right) && (!leftDirection)) {
        rightDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if ((key == Qt::Key_Up) && (!downDirection)) {
        upDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    if ((key == Qt::Key_Down) && (!upDirection)) {
        downDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    QWidget::keyPressEvent(e);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
  Function to generate a random integer
  between 0 and (max_val-1). It is used
  to generate the coords of the apple and
  for the initial direction of the snake.
*/
int Snake::getRandomInt(int max_val) {
    // Get current system time
    QTime time = QTime::currentTime();
    /*
        Generate a random number.
        The RNG is seeded with the current
        millisecond value of the system time.
    */
    qsrand((uint)time.msec());
    /*
        Generate random number between
        0 and (max_val-1)
    */
    int r = qrand() % max_val;
    return r;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
