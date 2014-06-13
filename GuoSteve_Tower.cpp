/* Steve Guo - April 19, 2012 - GuoSteve_Tower.cpp - version 2.2
 *
 * Requires Allegro 4.4.0.1 (http://alleg.sourceforge.net/)!
 *
 * Visually pleasing up to 22 disks!
 *     [Up to 120 disks are supported! ("any higher and the tower is in danger of falling over...")]
 *
 * This code is actually not intended to be distributed: if you've paid money for it you've been rightfully ripped off!
 *
 * ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
 */

#include "GuoSteve_Tower.h"
#include <cmath>    // Include utilized standard library for sinusoidal functions

// Deallocates all nodes
STACK::~STACK(){
    if(!top)    
        return; // Empty case
        
    while(top->previous){
        // More than one node
        top = top->previous;
        delete top->next;
    }
    
    delete top; // Delete last node
    top = NULL;
};

// Pushes argument into a stack (Last-In First-Out wise)
void STACK::Push(int num){
    NODE * newN = new NODE;
    newN->num = num;
    newN->next = NULL;
    
    if(top){
        // More than one node
        top->next = newN;
        newN->previous = top;
        top = newN;
    }
    else{
        // Empty stack case
        top = newN;
        top->previous = NULL;
    }
};

// Pops the top value off the stack and returns it
int STACK::Pop(){
    int temp;
    
    if (top){
        temp = top->num;
        if(top->previous){
            // More than one node
            top = top->previous;
            delete top->next;
            top->next = NULL;
        }
        else{
            // Only one node
            delete top;
            top = NULL;
        }
        return temp;
    }
    else{
        // Empty stack case
        cerr<<"You've somehow bypassed measures and popped an empty stack..."<<endl;
        return (-1);    // Return awkward number in accordance with error
    }
};

// Counts and returns number of nodes
int STACK::Size() const{
    int count = 0;

    for (NODE * current = GetTop(); current; current = current->previous)
        count++;

    return count;
}

// Attributes that are essential for visual functions
TOWER::TOWER(int m, int n){
    towerNum = m;
    gameDisks = n;
}

// Top value of the stack function
int TOWER::TopValue() const{
    if(GetTop())
        return GetTop()->num;    // Top value of stack
    else
        return 999;     // Incredibly high bound for empty stack comparison trick
};

// Displays stack visually
void TOWER::Display(BITMAP *buffer){
    if (!Empty()){  // Visually render if peg/stack is not empty
        NODE * temp =  GetTop();
        
        // Scale factors of a the generic, largest disk
        int topLeftx, bottomRightx, width;
        int height = 16;    // Constant since it does not make sense to scale
        int xCentre = ((towerNum)*HORIZONTAL_RESOLUTION/5 + (towerNum-1)*HORIZONTAL_RESOLUTION/10); // Centre on current tower peg
        int topLefty = 4*VERTICAL_RESOLUTION/5 - (Size()+1)*height;
        int bottomRighty = 4*VERTICAL_RESOLUTION/5 - (Size())*height;
        
        while(temp){
            // Scale the current disk
            width = (temp->num * (((3*HORIZONTAL_RESOLUTION/10)/gameDisks)/2));
            topLeftx = xCentre - width;
            topLefty += height;
            bottomRightx = xCentre + width;
            bottomRighty += height;

            rectfill(buffer, topLeftx, topLefty, bottomRightx, bottomRighty, Rainbow(temp->num));   // Draw the current disk
            if (debugMode){
                textprintf_ex(buffer, font, topLeftx, topLefty, makecol(255,255,255), -1, "left:[%d][%d] right:[%d][%d]", topLeftx, topLefty, bottomRightx, bottomRighty);
                textprintf_ex(buffer, font, topLeftx, topLefty + 8, makecol(255,255,255), -1, "disk: [%d][%d](%d)", towerNum, temp->num, Rainbow(temp->num));
            }
            temp = temp->previous;  // Next disk
        }
    }
};

// An offset rainbow spectrum formula
int TOWER::Rainbow(int disk){
    float increment = (float)disk / gameDisks;  // Scale in regard to total disks

    float r = 128 + 127*sin(increment * 1.5 * M_PI);    // Fluctuate one way from blue
    float g = 128 - 127*cos(increment * 1.5 * M_PI);    // Fluctuates the other way from blue
    float b = 128;                                      // Constant blue

    return makecol((int)r, (int)g, (int)b);             // Return standardized int value of rainbow colour
}
