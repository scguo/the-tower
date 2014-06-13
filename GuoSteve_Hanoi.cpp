/* Steve Guo - April 19, 2012 - GuoSteve_Hanoi.cpp
 *
 * Requires Allegro 4.4.0.1 (http://alleg.sourceforge.net/)!
 *
 * Visually pleasing up to 22 disks!
 *     [Up to 120 disks are supported! ("any higher and the tower is in danger of falling over...")]
 * 
 * This program is intended as freeware: if you've paid money for it you've been ripped off!
 * ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
 */

#include "GuoSteve_Tower.h"     // Include project header
#include <conio.h>              // An outdated library for keyboard routines
#include <ctime>                // A standard library for time functions

const char * const ToH_GAME_VERSION = "2.2"; // Current development version

// Console text functions
void Line(char, int);
void Center(char *);
void Box(char *);
void Move(int, int, int);

// Algorithmic functions
void consoleHanoi(int, int, int, int);
void GUIhanoi(int,  TOWER *, TOWER *, TOWER *, BITMAP *);

// GUI functions
void GUImain(int);
void GUImove(int, TOWER *, TOWER *, TOWER *, BITMAP *);
void GUIdrawField(BITMAP *);

// Global variables
bool debugMode = false; // Toggles under the hood display
bool qExit = false; // Allows one to abort the GUI recursive solver
int numberMoves = 0;    // Counter of number of moves
int recursiveCalls = -1;    // Negative one because first call is technically not recursive

// Main!
int main(){
    string in = "Empty";        // String for safe input handling
    bool showCommands = true;   // Toggles the display of operation commands
    int n;                      // Number of rings
    time_t rawTime;             // Raw time data
    struct tm * timeInfo;       // Parsed time data

    do{    // Main operation loop
        numberMoves = 0;        // Reset number of moves
        recursiveCalls = -1;    // Reset number of recursive calls

		system("cls");    // Clear console screen

        // Introductory text
		Center("Towers of Hanoi Console Menu!");

        // Current local time
        time(&rawTime);
        timeInfo = localtime (&rawTime);
        cout<<"Current Time: "<<asctime(timeInfo)<<endl;
        
        if(showCommands){   // Commands are shown by default
            Line(196,79);
            cout<<"The Tower of Hanoi is a game that can be solved using a recursive algorithm."<<endl;
            cout<<endl<<"In the GUI version, you can click and drag to manually solve."<<endl;
            cout<<endl<<"Solving consists of moving the contents of the red peg (the source)"<<endl<<"to the blue peg (the destination)"<<endl;
            cout<<endl<<"Because larger disks cannot be placed on small disks,"<<endl<<"one must also use the green peg (the intermediate)."<<endl;
            Line('-',79);
    		cout<<"'a' for GUI"<<endl;
    		cout<<"'c' for console"<<endl;
            Line(196,79);
        }
        else
            Line(205,79);

        Box("'h' (Toggle operations menu), 'q' (To quit)"); // Always show instructions

        cout<<endl<<"> ";     // User input prompter
        in = _getche();       // To avoid use of enter key

        // Menu statement
        if (in == "q")
            break;  // Exit menu loop/game
		else if (in == "h")
			showCommands = !showCommands; // Toggle horrific amount of text
        else if (in == "a"){
            do{ // Filter out negative input
                cout<<endl<<"->Number of rings: ";
                cin>>in;
            }while(atoi(in.c_str()) < 0);   // Checking for valid content
            n = atoi(in.c_str());   // Assign number of disks

            // Messages to relay that scale with number of disks
            if(n == 0)
                cout<<endl<<"0. This doesn't make for a fun game..."<<endl;
            if (n > 13)
                cout<<endl<<"1. You may be here for a long while for anything over 13 rings!"<<endl;
            if (n > 22)
                cout<<endl<<"2. Be careful not to tip the pile!"<<endl;
            if (n > 120)
                cout<<endl<<"3. Alas! You've gone and broken the pile of disks..."<<endl;
            debugMode = false; // To revert to default state
        	GUImain(n);
        }
        else if (in == "c"){
            do{ // Filter out negative input
                cout<<endl<<"->Number of rings: ";
                cin>>in;
            }while(atoi(in.c_str()) < 0);
            n = atoi(in.c_str());   // Assign number of disks
            
            // Messages to relay that scale with number of disks
            if (n == 0)
                cout<<endl<<"0. Fun Fact:"<<endl<<"Number of moves = 2^disks - 1 = 0"<<endl;
            else {
                if (n > 13)
                    cout<<endl<<"You may be here for a long while for anything over 13 rings!"<<endl;

                debugMode = true; // To show messages
                consoleHanoi(n, 1, 3, 2);  // From post 1, Through post 2, To post 3
                cout<<"It took "<<numberMoves<<" moves and "<<recursiveCalls<<" recursive calls"<<endl;
                debugMode = false; // To revert to default state
            }
            system("pause");
        }
	}while (in != "q");
	
	// End text
	system("cls");
	Center("Towers of Hanoi exited!");
	cout<<endl<<"Towers of Hanoi presented by Steve Guo (Program version["<<ToH_GAME_VERSION<<"])"<<endl<<endl;
    Center("Press any key to exit");
	in = _getche();

	return EXIT_SUCCESS;   // Successful exit
}
END_OF_MAIN();// This must be called right after the closing bracket of the MAIN function [it's Allegro specific]

// Prints a horizontal line, using the argument character, for a normal console window
void Line(char c, int n){
	for (int i = 0; i < n; i++)
        cout<<c;
	cout<<endl;
}

// Centers argument string in a normal console window
void Center(char * c){
	cout.width(80/2+strlen(c)/2);
    cout<<c<<endl;
}

// Formats argument string in a stylistic box
void Box(char * c){
    //The char typecast-ed numbers are just aesthetic characters
    cout<<(char)218;
    for (int i = 0; i < strlen(c); i++)
        cout<<(char)196;
    cout<<(char)183<<endl<<(char)179<<c<<(char)186<<endl<<(char)212;
    for (int j = 0; j < strlen(c); j++)
        cout<<(char)205;
    cout<<(char)188<<endl;
}

/* The function "consoleHanoi" is the algorithm that solves the "game"
 *
 * "consoleHanoi" recursively calls itself to solve the problem
 */
void consoleHanoi(int n, int s, int d, int i){
    recursiveCalls++;
    if (n == 1) // Base case optimization that effectively cuts the number of recursions in half
       Move(n, s, d);
    else if (n > 1){    // Not at the base case yet
       consoleHanoi(n-1,s,i,d);    //move n-1 rings from s to i using d
       Move(n, s, d);
       consoleHanoi(n-1,i,d,s);    //move the n-1 rings from i to d using s
    }
}

/* A copy of the "consoleHanoi" function that allows for visual representation
 *
 * Again by moving n rings from s to d using i
 */
void GUIhanoi(int n, TOWER * s, TOWER * d, TOWER * i, BITMAP * buffer){
    show_os_cursor(3);  //Show the busy cursor since nothing can be done anyway

    recursiveCalls++;
    if (qExit)
        return; // Abort request received - abort!
    else if (key[KEY_A])
        qExit = true;   // Abort requested
    else if (n == 1) // Base case optimization that effectively cuts the number of recursions in half
        GUImove(n, s, d, i, buffer);
    else if (n > 1){    // Not at the base case yet
        GUIhanoi(n-1,s, i, d, buffer);    //move n-1 rings from s to i using d
        GUImove(n, s, d, i, buffer);      //update visual
        GUIhanoi(n-1, i, d, s, buffer);   //move the n-1 rings from i to d using s
    }
}

// Graphical representation of the Tower of Hanoi
void GUImain(int n){
    allegro_init();         // Start Allegro
    install_keyboard();     // Ready keyboard routines
    install_mouse();        // Ready mouse routines
    
    long FramesElapsed = 0;   // To store frames elapsed since start of GUI
    
    set_color_depth(desktop_color_depth()); // Setting the color depth
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION,0,0); // Set graphics mode to defined resolution

    // For the double buffering method [smoother animation]
    BITMAP *buffer = create_bitmap(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION); 

    
    if (!buffer){   // Testing to see if the bitmap was created
    	// Allegro changed to a text only mode to ensure that the message will be shown
    	set_gfx_mode(GFX_TEXT,0,0,0,0);
    	allegro_message("Alas an error! [Could not create the buffer... \n try again later when your computer is available]");
    	exit(EXIT_FAILURE);    // Stopping the program since the buffer could not be created and, in turn, do not proceed
    }
    
    // Towers of Hanoi essentials
    TOWER source(1, n), intermediate(2, n), destination(3, n);  // The three towers of Hanoi
    for (int i = n; i > 0; i--)
        source.Push(i); // Places previously specified number of disks into source peg
        
    // Drag and Drop functionality variables
    TOWER temp(0, n);   // Temporary tower for drag and drop disk handling
    bool diskMoving = false;    // Stores if a disk is currently being handled
    int pos, x = 0, y = 0, mickeyx = 0, mickeyy = 0;    // Mouse variables to store raw position data, x/y position, and delta x/y
    int clickedFrom = 0, clickedTo = 0; // Variables to store drag and drop information
    int topLeftx, topLefty, bottomRightx, bottomRighty, colour, height, width, xCentre; //Variables for drag and drop disk rendering
    enable_hardware_cursor();   // Allow default Operating System cursors
    show_os_cursor(5);  // Use the text selector cursor for aesthetics
    
    srand(time(NULL));   // Initialize random seed for winning sequence
    
    // The essential visual program loop
    while (!key[KEY_ESC]) // Escape key exits the GUI
    {
        rest(32); // Speed delay; to prevent over processor usage
        
        if (mouse_on_screen()){ // Only loop if mouse in on the window
            FramesElapsed++; // Add to elapsed frames/time
            
            pos = mouse_pos; // Acquire raw mouse data
            x = pos >> 16;  // Bitshift for correction
            y = pos & 0x0000ffff;   // Multiply by a constant for correction
            get_mouse_mickeys(&mickeyx, &mickeyy);

            GUIdrawField(buffer);
            
            if ((mouse_b & 1) && !diskMoving){  // Left mouse click
                if(debugMode)
                    textprintf_ex(buffer, font, 1,1, makecol(255,255,255), -1, "Left Clicked!");
                    
                diskMoving = true;
                
                if (x > HORIZONTAL_RESOLUTION/20 && x < 7*HORIZONTAL_RESOLUTION/20 && !source.Empty()){
                    clickedFrom = 1;
                    temp.Push(source.Pop());
                }
                else if (x > 7*HORIZONTAL_RESOLUTION/20 && x < 13*HORIZONTAL_RESOLUTION/20 && !intermediate.Empty()){
                    clickedFrom = 2;
                    temp.Push(intermediate.Pop());
                }
                else if (x > 13*HORIZONTAL_RESOLUTION/20 && x < 19*HORIZONTAL_RESOLUTION/20 && !destination.Empty()){
                    clickedFrom = 3;
                    temp.Push(destination.Pop());
                }
                else
                    diskMoving = false;
                    
                if (diskMoving){
                    temp.towerNum = clickedFrom;
                    height = 16;
                    xCentre = ((temp.towerNum)*2*HORIZONTAL_RESOLUTION/10 + (temp.towerNum-1)*HORIZONTAL_RESOLUTION/10);
                    topLefty = y - height/2;
                    bottomRighty = y + height/2;
                    width = (temp.TopValue() * (((3*HORIZONTAL_RESOLUTION/10)/temp.gameDisks)/2));
                    topLeftx = x - width;
                    bottomRightx = x + width;
                    colour = (int)(255 - 225*((float)temp.TopValue()/temp.gameDisks));
                }
            }
            else if ((mouse_b & 1) && diskMoving){  // Left mouse click was held
                if(debugMode)
                    textprintf_ex(buffer, font, 1,1, makecol(255,255,255), -1, "Left Clicked!");
                    
                topLeftx += mickeyx;
                topLefty += mickeyy;
                bottomRightx += mickeyx;
                bottomRighty += mickeyy;
                rectfill(buffer, topLeftx, topLefty, bottomRightx, bottomRighty, temp.Rainbow(temp.TopValue()));
                
                if (debugMode){
                    textprintf_ex(buffer, font, topLeftx, topLefty, makecol(255,255,255), -1, "left:[%d][%d] right:[%d][%d]", topLeftx, topLefty, bottomRightx, bottomRighty);
                    textprintf_ex(buffer, font, topLeftx, topLefty + 8, makecol(255,255,255), -1, "disk: [%d][%d](%d)", temp.towerNum, temp.TopValue(), temp.Rainbow(temp.TopValue()));
                }
            }
            else if (!(mouse_b & 1) && diskMoving){ //Left mouse click released
                if(debugMode)
                    textprintf_ex(buffer, font, 128,1, makecol(255,255,255), -1, "Released Left Click!");

                if (x < HORIZONTAL_RESOLUTION/20)
                    clickedTo = 3;  // Fancy wrap around trick
                else if (x > HORIZONTAL_RESOLUTION/20 && x < 7*HORIZONTAL_RESOLUTION/20)
                    clickedTo = 1;
                else if (x > 7*HORIZONTAL_RESOLUTION/20 && x < 13*HORIZONTAL_RESOLUTION/20)
                    clickedTo = 2;
                else if (x > 13*HORIZONTAL_RESOLUTION/20 && x < 19*HORIZONTAL_RESOLUTION/20)
                    clickedTo = 3;
                else
                    clickedTo = 1; // Again, fancy wrap around trick
                    
                if (clickedFrom == 1){
                    if (clickedTo == 2){
                        if (temp.TopValue() < intermediate.TopValue()){
                            intermediate.Push(temp.Pop());
                            numberMoves++;
                        }
                        else
                            source.Push(temp.Pop());
                    }
                    else if (clickedTo == 3){
                        if (temp.TopValue() < destination.TopValue()){
                            destination.Push(temp.Pop());
                            numberMoves++;
                        }
                        else
                            source.Push(temp.Pop());
                    }
                    else
                        source.Push(temp.Pop());
                }
                else if (clickedFrom == 2){
                    if (clickedTo == 1){
                        if (temp.TopValue() < source.TopValue()){
                            source.Push(temp.Pop());
                            numberMoves++;
                        }
                        else
                            intermediate.Push(temp.Pop());
                    }
                    else if (clickedTo == 3){
                        if (temp.TopValue() < destination.TopValue()){
                            destination.Push(temp.Pop());
                            numberMoves++;
                        }
                        else
                            intermediate.Push(temp.Pop());
                    }
                    else
                        intermediate.Push(temp.Pop());
                }
                else if (clickedFrom == 3){
                    if (clickedTo == 1){
                        if (temp.TopValue() < source.TopValue()){
                            source.Push(temp.Pop());
                            numberMoves++;
                        }
                        else
                            destination.Push(temp.Pop());
                    }
                    else if (clickedTo == 2){
                        if (temp.TopValue() < intermediate.TopValue()){
                            intermediate.Push(temp.Pop());
                            numberMoves++;
                        }
                        else
                            destination.Push(temp.Pop());
                    }
                    else
                        destination.Push(temp.Pop());
                }
                diskMoving = false;
            }
            
            if (mouse_b & 2)    // Right click, which does nothing
                if(debugMode)
                    textprintf_ex(buffer, font, 1, 10, makecol(255,255,255), -1, "Right Clicked! [does not do anything though!]");
                    
            // Auto-solve if not starting from arbitrary position
            if (key[KEY_S] && !source.Empty() && destination.Empty() && intermediate.Empty()){
                GUIhanoi(n, &source, &destination, &intermediate, buffer);  // Call recursive solution
                if(qExit)
                    show_os_cursor(4);  // Recursive solution aborted prematurely
                else
                    show_os_cursor(1);  // Recursive solution solved successfully, use a normal cursor now

                qExit = false;  // Reset abort state to negative
            }

            if (key[KEY_D]) // Toggle debug mode
                debugMode = true; 
            else
                debugMode = false;
            
            // Winning conditions met - win sequence!
            if (!diskMoving && source.Empty() && intermediate.Empty() && !destination.Empty()){
                // Just a colourful, pulsating word under the destination peg
                textprintf_ex(buffer, font, 4*HORIZONTAL_RESOLUTION/5 - 32 , 4*VERTICAL_RESOLUTION/5 + 8, makecol(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1), -1, "Winner!!!");
            }

            if (debugMode){
                textprintf_ex(buffer, font, 0,20, makecol(255,255,255), -1, "x: %d y: %d Elapsed Frames: %d ", x, y, FramesElapsed);
                textprintf_ex(buffer, font, 0,40, makecol(255,255,255), -1, "From: %d To: %d", clickedFrom, clickedTo);
            }

            // Display current state of all towers
            source.Display(buffer);
            intermediate.Display(buffer);
            destination.Display(buffer);

        	blit(buffer, screen, 0, 0, 0, 0, HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION);  // Blitting of the buffer

            if (diskMoving || debugMode)
                clear(buffer);  // Clear the buffer if a disk is being dragged or in debug mode
        }   // Ending brace of the mouse on window check
    }   // Ending brace of the GUI loop

    destroy_bitmap(buffer); // Delete buffer bitmap
    allegro_exit();         // Close Allegro
}

// Relays detail of a disk move
void Move(int n, int s, int d){
    ++numberMoves;
    
    if(debugMode)   // Just so this function can transcend console and GUI
        cout<<"Step #"<<numberMoves<<" moved disk #"<<n<<" from post #"<<s<<" to post #"<<d << " (Recursions: " << recursiveCalls << ")"<<endl;
}

// Moves the disk by GUI rules
void GUImove(int n, TOWER * s, TOWER * d, TOWER * i, BITMAP * buffer){
    if (qExit)
        return;
    else {
        rest(1);    // To prevent processor overusage
        
        d->Push(s->Pop());   // Move the n^th disk;
        
        Move(n, s->towerNum, d->towerNum);
        GUIdrawField(buffer);

        // Display current state of all towers
        s->Display(buffer);
        d->Display(buffer);
        i->Display(buffer);

        // Blitting of the buffer
        blit(buffer, screen, 0, 0, 0, 0, HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION);
        clear(buffer); // Clear the buffer
    }
}

// Drawing game field - the order matters for proper display of overlay
void GUIdrawField(BITMAP * buffer){
    // Draw the pegs
    rectfill(buffer,(2*HORIZONTAL_RESOLUTION/10)-4, VERTICAL_RESOLUTION/5, 2*HORIZONTAL_RESOLUTION/10 + 4, 4*VERTICAL_RESOLUTION/5, makecol(255, 0, 0));
    rectfill(buffer,(5*HORIZONTAL_RESOLUTION/10)-4, VERTICAL_RESOLUTION/5, 5*HORIZONTAL_RESOLUTION/10 + 4, 4*VERTICAL_RESOLUTION/5, makecol(0, 255, 0));
    rectfill(buffer,(8*HORIZONTAL_RESOLUTION/10)-4, VERTICAL_RESOLUTION/5, 8*HORIZONTAL_RESOLUTION/10 + 4, 4*VERTICAL_RESOLUTION/5, makecol(0, 0, 255));

    // Draw helpful stack outlines
	line(buffer, 1*HORIZONTAL_RESOLUTION/20, 0, 1*HORIZONTAL_RESOLUTION/20, VERTICAL_RESOLUTION, makecol(255,255,255));
	line(buffer, 7*HORIZONTAL_RESOLUTION/20, 0, 7*HORIZONTAL_RESOLUTION/20, VERTICAL_RESOLUTION, makecol(255,255,255));
	line(buffer, 13*HORIZONTAL_RESOLUTION/20, 0, 13*HORIZONTAL_RESOLUTION/20, VERTICAL_RESOLUTION, makecol(255,255,255));
	line(buffer, 19*HORIZONTAL_RESOLUTION/20, 0, 19*HORIZONTAL_RESOLUTION/20, VERTICAL_RESOLUTION, makecol(255,255,255));
	
	// Draw a base/ground
    rectfill(buffer,0, 4*VERTICAL_RESOLUTION/5, HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION, makecol(32,32, 32));
    
    // Relay actually useful information
    textprintf_ex(buffer, font, 8,VERTICAL_RESOLUTION-80, makecol(255,255,255), -1, "Click and drag \t (all of the disks on the red peg to the blue peg to win!)");
    textprintf_ex(buffer, font, 8,VERTICAL_RESOLUTION-60, makecol(255,255,255), -1, "s for solution ");
    textprintf_ex(buffer, font, 8,VERTICAL_RESOLUTION-50, makecol(255,255,255), -1, "a to abort the solver");
    textprintf_ex(buffer, font, 8,VERTICAL_RESOLUTION-40, makecol(255,255,255), -1, "hold d for debugging text");
    textprintf_ex(buffer, font, 8,VERTICAL_RESOLUTION-30, makecol(255,255,255), -1, "ESC to exit");
    textprintf_ex(buffer, font, HORIZONTAL_RESOLUTION/2 - 64,VERTICAL_RESOLUTION-20, makecol(255,255,255), -1, "# of moves: %d # of recursive calls: %d Debug mode: %s", numberMoves, (recursiveCalls == -1 ? 0 : recursiveCalls), (debugMode ? "ON" : "OFF"));
}
