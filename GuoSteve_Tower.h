/* Steve Guo - April 19, 2012 - GuoSteve_Tower.h - version 2.2
 *
 * Requires Allegro 4.4.0.1 (http://alleg.sourceforge.net/)!
 *
 * Visually pleasing up to 22 disks!
 *     [Up to 120 disks are supported! ("any higher and the tower is in danger of falling over...")]
 *
 * This header is actually not intended to be distributed: if you've paid money for it you've been rightfully ripped off!
 *
 * ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
 */
 
#ifndef GUOSTEVE_TOWER_H_INCLUDED     // Check to ensure no duplication of prior instances
#define GUOSTEVE_TOWER_H_INCLUDED     // Stating first instance

#include <allegro.h>    // Allegro 4.4.0.1 (http://alleg.sourceforge.net/).
#include <iostream>


#define HORIZONTAL_RESOLUTION 800   // Maximum horizontal resolution, scales accordingly
#define VERTICAL_RESOLUTION 600    // Maximum vertical resolution, scales accordingly

extern bool debugMode;

using namespace std;    // Define own namespace

// Double-linked list structure
struct NODE{
    NODE * next;        // Pointer to next node
    NODE * previous;    // Point to previous node
    int num;            // Data
};

//Base stack class
class STACK{
    private:
        NODE * top; //Safe encapsulation
        
    protected:
        NODE * GetTop() const {return top;} // Top pointer accessor for sub-classes without modifying data
    
    public:
        STACK() {top = NULL;}   //Initialization of top to NULL
        ~STACK();   //Deallocates if out-of scope
        
        void Push(int);     // Generic stack push function
        int Pop();          // Generic stack pop function
        
        bool Empty() const {return top == NULL;}    // Checks if empty without modifying data
        int Size() const;                           // Returns stack size


};

// Tower of Hanoi class (that inherits the Stack class)
class TOWER : public STACK{
    public:
        TOWER(int, int);    // Constructor for the stack that represents one peg
        
        int towerNum;       // Stores which tower the stack is
        int gameDisks;      // Stores total disks in play
        
        int TopValue() const;   // Returns top value of stack
        void Display(BITMAP *); // Visually displays the stack
        int Rainbow(int);       // Returns an arbitrary rainbow colour
};
#endif  // GUOSTEVE_TOWER_H_INCLUDED
