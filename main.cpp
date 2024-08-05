//  Ma+thHer0
//
//  Team : Norin Hossam - 19100108, Zyad Shrin - 19103698
//  NOTE: Please run the code using Xcode(Macbook)


#include <iostream>
#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <algorithm>
#include <cstdio>

using namespace std;

int phyWidth = 700;
int phyHeight = 700;
int logWidth = 100;
int logHeight = 100;
int centerX = logWidth/2;
int centerY = logHeight/2;
int centerY1 = 80;
int centerY2 = 50;
int centerY3 = 20;
int mouseX = centerX, mouseY = centerY;

int RecWid = 15;    //Width of Menu buttons
int Menu = 1, Play = 0, Highscores = 0; //Flags of Menu,Play,Highscores
int GameOver=0; //Flag for game over

int Rx = 50, Ry1= 65, Ry2 = 50, Ry3 = 35;
int sqWid=21;   //Width of Playing Rectangles
int sqDelta=-13;    //Movement of the rectangles (Animation)

int elapsedTime=60; //TIMER for Gameover

int score=0;   //SCORE

int counter1 =0; //For eq1
int counter2 =2; //For eq2
int counter3 =4; //For eq3
string myarray[2000]; //Array has equation, result


std::vector <int> ranks = {40,35,30,18,20,10,9,8,5,4,1};    //Vector of scores
int ScoreX = 15, ScoreY = 70;    //X & Y co-ordinates of score
int ShiftScore; // The rank of your score

bool loopflag=true; //Flag to ensure looping once

int Easy=0, Medium=1, Hard=0; //Difficulty level

string numberString = "";   //String where we save the user's input


void randomizeq()  //Function to randomize equations
{
    array<string, 3> opArray =  //Array of Strings size of 3 of operations
    {
        "+",
        "*",
        "-"
    };
    
    for (int i=0;i<2000;i+=2) //Looping on array
    {
        int Num1 = rand() % 9 + 1;  //Randomize num1 from 1 to 9
        int Num2 = rand() % 9 + 1;  //Randomize num2 from 1 to 9
        int result; //Result of equation
        random_device rd;
        mt19937 generator(rd());
        shuffle(opArray.begin(), opArray.end(), generator); //Shuffle oparray containing operations
        string op  = opArray[0];  //Randomly selected operation
        string equation = to_string(Num1) + " " + op + " " + to_string(Num2); //Random equation
        if (op == "+")
        {
            result = Num1 + Num2;
        }
        else if (op == "-")
        {
            result = Num1 - Num2;
        }
        else
        {
            result = Num1 * Num2;
        }
        myarray[i] = equation; //Place equation in index i
        myarray[i+1]= to_string(result); //Place result in index i+1
    }
}


void printSome(char *str, float x, float y) //Displaying character on screen
{
    glRasterPos2d(x,y); //Where text to be drawn
    for (int i=0; i<strlen(str); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str[i]); //Draw character str[i] with certain font 
    glFlush();
}



void printSome1(char *str, float x, float y)  //Displaying character on screen
{
    glRasterPos2d(x,y);
    for (int i=0; i<strlen(str); i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,str[i]);
    glFlush();
}



void printSome2(const std::string& str, float x, float y) //Displaying string on screen
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2d(x,y);
    for (char c: str)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    glFlush();
}



void timerCallback(int val) //Function for timer starting from 60
{
    if (Play==1) //Starts timer when play button pressed
    {
        elapsedTime -= 1;  //Decrements timer by 1
        
        if (elapsedTime == 0) //If timer=0 then get game over window and reset timer=60
        {
            GameOver=1;
            Play=0;
            Menu=0;
            elapsedTime=60;
        }
    }
    glutTimerFunc(1000, timerCallback, 0); //Call this function every second
}



void init2D(float r, float g, float b)
{
    glClearColor(r,g,b,1); //Initialize the BG color and transparency
    glMatrixMode(GL_PROJECTION); //Mapping for 2D projection, as OPENGL manages imgs as 3D imgs
    gluOrtho2D(0.0,logWidth,0.0,logHeight); //Logical ranges of x coordinates, y coordinates
}



void drawRectangles() //Draw buttons
{

    glColor3f(1.0, 1.0, 1.0); //Play Button
    glBegin(GL_POLYGON);
    glVertex2f(Rx-RecWid/2, Ry1-RecWid/4);
    glVertex2f(Rx+RecWid/2, Ry1-RecWid/4);
    glVertex2f(Rx+RecWid/2, Ry1+RecWid/4);
    glVertex2f(Rx-RecWid/2, Ry1+RecWid/4);
    glEnd();
    
    glBegin(GL_POLYGON); //Highscores Button
    glVertex2f(Rx-RecWid/2, Ry2-RecWid/4);
    glVertex2f(Rx+RecWid/2, Ry2-RecWid/4);
    glVertex2f(Rx+RecWid/2, Ry2+RecWid/4);
    glVertex2f(Rx-RecWid/2, Ry2+RecWid/4);
    glEnd();
     
    glBegin(GL_POLYGON); //Quit Button
    glVertex2f(Rx-RecWid/2, Ry3-RecWid/4);
    glVertex2f(Rx+RecWid/2, Ry3-RecWid/4);
    glVertex2f(Rx+RecWid/2, Ry3+RecWid/4);
    glVertex2f(Rx-RecWid/2, Ry3+RecWid/4);
    glEnd();
    
    if (Easy==1) //Change color of button if selected
    {
        glColor3f(0.0, 1.0, 0.0);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);
    }
    
    glBegin(GL_QUADS); //Easy Button
    glVertex2f(47, 71);
    glVertex2f(43, 71);
    glVertex2f(43, 75);
    glVertex2f(47, 75);
    glEnd();
    
    if (Medium==1) //Change color of button if selected
    {
        glColor3f(0.0, 0.0, 1.0);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);
    }
    
    glBegin(GL_QUADS); //Medium Button
    glVertex2f(52, 71);
    glVertex2f(48, 71);
    glVertex2f(48, 75);
    glVertex2f(52, 75);
    glEnd();
    
    if (Hard==1) //Change color of button if selected
    {
        glColor3f(1.0, 0.0, 0.0);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);
    }
    
    glBegin(GL_QUADS); //Hard Button
    glVertex2f(53, 71);
    glVertex2f(57, 71);
    glVertex2f(57, 75);
    glVertex2f(53, 75);
    glEnd();
    

}


void specialKeyboard(int key, int x, int y) //Keyboard function
{
    
    if (GameOver==1 && key == GLUT_KEY_F1) //If F1 pressed at game over window , game will restart
    {
        GameOver=0;
        Play=1;
        score=0; //Score reset
        loopflag=true; //For the one-time loop
        ShiftScore=0; //Reset rank
    }
    
    else if ( (GameOver==1 || Highscores==1) && key == GLUT_KEY_F2) //If F2 pressed when in game over / highscore window
    {
        GameOver=0;
        Highscores=0;
        Play=0;
        Menu=1;  //Go back to menu
        score=0; //Score reset
        loopflag=true; //For the one-time loop
        ShiftScore=0; //Reset rank
    }
    
    glutPostRedisplay();
}


void keyboardCallback(unsigned char key, int x, int y) //Keyboard functon for user input
{
    if(Play==1) //Only works in play window
    {
        if ( (key >= '0' && key <= '9') || key == '-') //0-9 & - appended in numberString string
        {
            numberString += key;

        }
        
        if (key == 13) //If enter is pressed
        {
            if (numberString == myarray[counter1+1] || numberString == myarray[counter2+1] || numberString == myarray[counter3+1]) //Check for user's input with any of 3 eq's result on screen if correct then increment score
            {
                score++;
            }
            numberString=""; //Reset
        }
        
        glutPostRedisplay();
    }
}




void mouseClick(int btn, int state, int x, int y)
{
     
    if (Menu==1) //Menu window
    {
        if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) //Mouse left button pressed down
        {
            mouseX= x;
            mouseX= 0.5+1.0*mouseX*logWidth/phyWidth;
            mouseY= phyHeight-y;
            mouseY=0.5+1.0*mouseY*logHeight/phyHeight;
            
            if (mouseX>=(Rx-RecWid/2) && mouseX<=(Rx+RecWid/2) && mouseY>=(Ry1-RecWid/4) && mouseY<=(Ry1+RecWid/4)) //Play
            {
                randomizeq(); //Calling randomize function
                Play= 1;
                Menu= 0;
                Highscores= 0;
            }
            if (mouseX>=(Rx-RecWid/2) && mouseX<=(Rx+RecWid/2) && mouseY>=(Ry2-RecWid/4) && mouseY<=(Ry2+RecWid/4)) //Highscore
            {
                Highscores= 1;
                Menu= 0;
                Play= 0;
            }
            if (mouseX>=(Rx-RecWid/2) && mouseX<=(Rx+RecWid/2) && mouseY>=(Ry3-RecWid/4) && mouseY<=(Ry3+RecWid/4)) //Quit
            {
                exit(0);
            }
            
            //Difficulty buttons
            if (mouseX>=43 && mouseX<=47 && mouseY>=71 && mouseY<=75) //Easy
            {
                Easy=1;
                Medium=0;
                Hard=0;
            }
            if (mouseX>=48 && mouseX<=52 && mouseY>=71 && mouseY<=75) //Medium
            {
                Medium=1;
                Easy=0;
                Hard=0;
            }
            if (mouseX>=53 && mouseX<=57 && mouseY>=71 && mouseY<=75) //Hard
            {
                Hard=1;
                Easy=0;
                Medium=0;
            }
            
        }
        glutPostRedisplay();
    }
}


 

void Timer(int value) //Animation speed changes based on difficulty
{
    if (Easy==1)
    {
        glutTimerFunc(200, Timer, value);
    }
    else if(Medium==1)
    {
        glutTimerFunc(100, Timer, value);
    }
    else if (Hard==1)
    {
        glutTimerFunc(50, Timer, value);
    }
    
    glutPostRedisplay();
}



void display(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT); //Clear display window
    
    if (Menu== 1) //Menu window
    {
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        
        glColor3f(0.8, 0.4, 0.5);
        printSome("Ma+h Her0",42,85);
        drawRectangles();
        glColor3f(0.2, 0.7, 0.9);
        printSome1("PLAY",Rx-3,Ry1-1);
        printSome1("HIGHSCORES",Rx-6.5,Ry2-1);
        printSome1("QUIT",Rx-3,Ry3-1);
        printSome1("E", 44.5, 72);
        printSome1("M", 49.5, 72);
        printSome1("H", 54.5, 72);
    }
    
    
    if (Play== 1) //Play window
    {
        glClearColor(0.7, 0.7, 0.7, 1); //Background
        
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);    //White pole in play window
        glVertex2f(49.5, 0);
        glVertex2f(50.5, 0);
        glVertex2f(50.5, 100);
        glVertex2f(49.5, 100);
        glEnd();
        
        
        //3 Rectangles in play window
        glColor3f(0.5, 0.8, 1);
        glBegin(GL_POLYGON);
        glVertex2f(centerX-sqWid/2, centerY1-sqWid/3+sqDelta);
        glVertex2f(centerX+sqWid/2, centerY1-sqWid/3+sqDelta);
        glVertex2f(centerX+sqWid/2, centerY1+sqWid/3+sqDelta);
        glVertex2f(centerX-sqWid/2, centerY1+sqWid/3+sqDelta);
        glEnd();
        
        glBegin(GL_POLYGON);
        glVertex2f(centerX-sqWid/2, centerY2-sqWid/3+sqDelta);
        glVertex2f(centerX+sqWid/2, centerY2-sqWid/3+sqDelta);
        glVertex2f(centerX+sqWid/2, centerY2+sqWid/3+sqDelta);
        glVertex2f(centerX-sqWid/2, centerY2+sqWid/3+sqDelta);
        glEnd();
        
        
        glBegin(GL_POLYGON);
        glVertex2f(centerX-sqWid/2, centerY3-sqWid/3+sqDelta);
        glVertex2f(centerX+sqWid/2, centerY3-sqWid/3+sqDelta);
        glVertex2f(centerX+sqWid/2, centerY3+sqWid/3+sqDelta);
        glVertex2f(centerX-sqWid/2, centerY3+sqWid/3+sqDelta);
        glEnd();
        
        
        sqDelta++; //To change positions of rectangles
        if (sqDelta>=35) //If first rectangle is out of frame (window) then reset the positions of the rectangles
        {
            sqDelta=-13;
            numberString=""; //Reset user input with every animation
            
            //Increment counters by 6 to get the next equations
            counter1+=6;
            counter2+=6;
            counter3+=6;
        }
        
        printSome2(myarray[counter1], centerX-4, centerY1-2+sqDelta); //EQ1 displayed on screen
        printSome2(myarray[counter2], centerX-4, centerY2-2+sqDelta); //EQ2 displayed on screen
        printSome2(myarray[counter3], centerX-4, centerY3-2+sqDelta); //EQ3 displayed on screen
        
        
        //Score
        glColor3f(1.0, 1.0, 1.0);
        printSome("Score: " , 3, 95);
        printSome2(to_string(score),12,95);
        
        //Timer
        printSome("Timer:", 85, 95);
        printSome2(to_string(elapsedTime), 95, 95);
}
   
    
    if (Highscores== 1) //Highscore window
    {
        
        glClearColor(0, 0, 0, 1);
        glColor3f(1, 1, 1);
        printSome("HIGHSCORES",40,85);
        std::sort(ranks.begin(),ranks.end(),std::greater<int>()); //Sort scores in desc order
        
        for(int i=0; i<6; i++) //Display top 6 highscores
        {
            string AddDot= to_string(i+1)+".";
            printSome2(AddDot, ScoreX, ScoreY);
            printSome2(to_string(ranks[i]), ScoreX+4, ScoreY);
            ScoreY-=10;
        }
        ScoreY = 70; //To keep the scores in frame (window)
    }
    
    
    
    if (GameOver==1)    //Gameover window
    {
        
        glClearColor(0.6, 0.5, 0.8, 1.0);
        sqDelta=-13;    //Reset starting position on playing rectangles (restart animation)
        glColor3f(1.0, 1.0, 1.0);
        printSome("TIME'S UP!", 37, 75);
        printSome("Your score is: " , 35, 60);
        printSome2(to_string(score),56,60);
        
        
        std::sort(ranks.begin(),ranks.end(),std::greater<int>());   //Sort scores in desc order
        
        if (loopflag)   //To loop once
        {
            for(int i=0;i<size(ranks);i++)  //Loop on vector of scores
            {
                if (score>=ranks[i])    //To check if the user's score is higher than any of the scores in the vector
                {
                    ShiftScore=i;      //Save in shiftscore the place that ill replace in
                    
                    break; //Leave the loop once found
                }
                else
                {
                    ShiftScore=i+1; //If user's score not higher than any of scores in vector, then place it in the end of the vector
                }
            }
            
            if (ranks[ShiftScore]!=score) //If the score is repeated don't put in the vector
            {
                ranks.push_back(score); //Score pushed in vector if not repeated
            }
            
            std::sort(ranks.begin(),ranks.end(),std::greater<int>()); //Sorting after adding the score in the vector
            loopflag=false; //To get out of the loop (one-time loop)
        }
       
        
        //Rank
        printSome("Your rank is: " , 36, 43);
        printSome2(to_string(ShiftScore+1),56,43);
        printSome("Press 'F1' to RESTART the game or 'F2' to go to the MENU", 7, 25);
        
    }
    glutSwapBuffers();
    glFlush();
}


int main(int argc,char *argv[])
{

    glutInit(&argc,argv); //Initialize GLUT
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //Bit mask to select RGBA mode , bit mask to select double buffered window
    glutInitWindowSize(phyWidth, phyHeight); //Initialize windows width and height in pixels
    glutInitWindowPosition(100,100); //Initialize windows top left position in pixels
    glutCreateWindow("Ma+h Her0"); //Window title
    init2D(0.0,0.0,0.0);
    glutDisplayFunc(display); //Calling display function
    //Calling timer functions
    timerCallback(0);
    Timer(0);
    
    //Calling mouse function
    glutMouseFunc(mouseClick);
    
    //Calling keyboard functions
    glutSpecialFunc(specialKeyboard);
    glutKeyboardFunc(keyboardCallback);
    //randomizeq(); 
    glutMainLoop();
    
}
