git push -u origin master//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// height and width of Paddle in pixels
#define PADDLE_H 10
#define PADDLE_W 100
#define PADDLE_DIST 30

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// bricks distance in pixels
#define BRICKS_DIST 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
GLabel initmessage(GWindow window);


int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));
    
    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
   GRect brick = NULL;
   initBricks(window);
    

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel score = initScoreboard(window);
    char s[3];
    char l[3];
    
    GLabel message = initmessage(window);
    char *mess;

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // keep playing until game over
    double speed = 2.0 * drand48();
    double velocityx = speed, velocityy = 1.5;
    
    while (lives > 0 && bricks > 0)
    {
        GEvent mouse_move = getNextEvent(MOUSE_EVENT);
        if (mouse_move != NULL)
            {
            if (getEventType(mouse_move) ==  MOUSE_MOVED)
                {
                double x = getX(mouse_move) - PADDLE_W / 2;
                setLocation(paddle, x, HEIGHT - PADDLE_DIST);
                }
            }
            
        GObject object = detectCollision(window, ball);
           
       
        if (object != NULL)
            {
            if (strcmp(getType(object), "GRect") == 0)
                {
                velocityy = -velocityy;
                if (object != paddle)
                    {
                    removeGWindow(window, object);
                    points = points + 1;
                    bricks = bricks - 10;
                    sprintf(s, "%i", points);
                    setLabel(score, s);
                    double x = (getWidth(window) - getWidth(score)) / 2;
                    double y = (getHeight(window) - getHeight(score)) / 2;
                    setLocation(score, x, y);
                    
                    }
                }
            }           
        move(ball, velocityx, velocityy);
        if (getX(ball) + getWidth(ball) >= getWidth(window))
            {
                velocityx = -velocityx;
            }
        else if (getX(ball) <= 0)
            {
                velocityx = -velocityx;
            }
            
        if (getY(ball) + getWidth(ball) >= getHeight(window))
            {
                lives = lives - 1;
                int wait = 1;
                while (wait == 1)
                    {
                    //sprintf(mess, "Leaves left: %i. Press mouse button to continue", lives);
                    setLabel(message, "Press mouse button to continue");
                    double x = 50;
                    double y = getHeight(window) / 1.5;
                    setLocation(message, x, y);
                        
                    GEvent mouse_click = getNextEvent(MOUSE_EVENT);
                    if (mouse_click != NULL)
                        {
                            if (getEventType(mouse_click) ==  MOUSE_CLICKED)
                                {
                                setLocation(ball, WIDTH / 2 - RADIUS / 2, HEIGHT / 2 - RADIUS / 2);
                                mess = "";
                                setLabel(message, mess);
                                wait = 0;
                                }
                        }
                        
                         
                     }
                
            }
        else if (getY(ball) <= 0)
            {
                velocityy = -velocityy;
            }
        pause(10);
        }
        
    //sprintf(mess, "Your score is: %i. Press mouse button to exit", points);
    setLabel(message, "Game over! Click mouse button to exit.");
    double x = 20;
    double y = getHeight(window) / 1.5;
    setLocation(message, x, y);
        
            
        // TODO
    

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int brick_w = (WIDTH - BRICKS_DIST) / COLS - BRICKS_DIST;
    int brick_h = WIDTH / 4 / ROWS - BRICKS_DIST;
    int xx = BRICKS_DIST;
    int yy = BRICKS_DIST;
    
    
    for (int i = 1; i <= ROWS; i++)
        {
        for (int j = 1; j <= COLS; j++)
            {
            GRect brick = newGRect(xx, yy, brick_w, brick_h);
            setColor(brick, "GREEN");
            setFilled(brick, true);
            add(window, brick);
            xx = xx + brick_w + BRICKS_DIST;
            
            }
         yy = yy + brick_h + BRICKS_DIST;
         xx = BRICKS_DIST;
         }
      
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(WIDTH / 2 - RADIUS / 2, HEIGHT / 2 - RADIUS / 2,  RADIUS, RADIUS);
    setColor(ball, "BLUE");
    setFilled(ball, true);
    add(window, ball);
    return ball;    
    
    // TODO
  
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH / 2 - PADDLE_W / 2, HEIGHT - PADDLE_DIST, PADDLE_W, PADDLE_H);
    setColor(paddle, "RED");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel score = newGLabel("");
    setFont(score, "SansSerif-36");
    add(window, score);
    return score;
}

GLabel initmessage(GWindow window)
{
    GLabel message = newGLabel("");
    setFont(message, "SansSerif-14");
    add(window, message);
    return message;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
