/*
CS349 Assignment 1
Ye Wang
20559425

  note: Frog Game
*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <sys/time.h>
#include <math.h>
#include <vector>
#include <string>
#include <cstdlib>
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>


// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}


using namespace std;

const int Border  = 4;
int counter = 1; // level counter
// Topline squares
int x1 = 25;
int x2 = x1 + (850 / 3);
int x3 = x2 + (850 / 3);
int x4 = x1 - (850 / 3);
const int topx = 25 - (850 / 3);
// Middleline rects
int x5 = 10;
int x6 = x5 + (850 / 4);
int x7 = x6 + (850 / 4);
int x8 = x7 + (850 / 4);
int x9 = x8 + (850 / 4);
const int midx = 860 + (850 / 4);
// Bottomline rects
int x10 = 50;
int x11 = x10 + (850 / 2);
int x12 = x10 - (850 / 2);
const int botx = 50 - (850 / 2);

Colormap colormap;
XColor red, brown, blue, yellow, green;
char g[] = "#00FF00";
char r[] = "#FFOOOO";
char b[] = "#OOOOFF";
char y[] = "#FFFF00";
char br[] = "#A52A2A";

/*
 * Information to draw on the window
 */
struct XInfo {
	Display *display;
	int screen;
	Window window;
	GC gc;
};

XInfo xinfo;

/*
 * An abstract class representing displayable things.
 */
class Displayable {
 public:
	virtual void paint(XInfo &xinfo) = 0;
};

// draw text
class Text : public Displayable {
 public:
	virtual void paint(XInfo &sxinfo) {
		string str;
		ostringstream convert;
		convert << level;
		str = convert.str();
		string s = "Level: ";
		s.append(str);
		XSetForeground(xinfo.display, xinfo.gc, BlackPixel(xinfo.display, xinfo.screen));
		XDrawImageString(xinfo.display, xinfo.window, xinfo.gc, this->x, this->y, s.c_str(), s.length());
	}

	// constructor
	Text(int x, int y, int level)
	  : x(x), y(y), level(level)
	  {}
 private:
	int x;
	int y;
	int level;
};

// draw text
class TextW : public Displayable {
 public:
	virtual void paint(XInfo &sxinfo) {
		string str;
		ostringstream convert;
		convert << level;
		str = convert.str();
		string s = "Level: ";
		s.append(str);
		string s2 = " completed! press n";
		s.append(s2);
		XSetForeground(xinfo.display, xinfo.gc, red.pixel);
		XDrawImageString(xinfo.display, xinfo.window, xinfo.gc, this->x, this->y, s.c_str(), s.length());
	}

	// constructor
	TextW(int x, int y, int level)
	  : x(x), y(y), level(level)
	  {}
 private:
	int x;
	int y;
	int level;
};

// draw rectangle
class Rec : public Displayable {
 public:
	virtual void paint(XInfo& xinfo) {
		if(recType == "type1") { // draw square
		    XSetForeground(xinfo.display, xinfo.gc, yellow.pixel);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc, x - (50 / 2), y - (50 / 2), 50, 50);
		} else if(recType == "type2") { // draw thin rectangle
		    XSetForeground(xinfo.display, xinfo.gc, blue.pixel);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc, x - (20 / 2), y - (50 / 2), 20, 50);
		} else if(recType == "type0"){
		    XSetForeground(xinfo.display, xinfo.gc, green.pixel);
            XFillRectangle(xinfo.display, xinfo.window, xinfo.gc, x - (50 / 2), y - (50 / 2), 50, 50);
		} else {
		    XSetForeground(xinfo.display, xinfo.gc, brown.pixel);
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc, x - (100 / 2), y - (50 / 2), 100, 50);
		}
	}
// constructor
	Rec(int x, int y, string recType)
	  : x(x), y(y), recType(recType) {}
	private: int x; int y; string recType;};

list<Displayable*> dList; //list of Displayables

// Function to repaint a display list
void repaint(list<Displayable*> dList, XInfo& xinfo) {
	list<Displayable*>::const_iterator begin = dList.begin();
	list<Displayable*>::const_iterator end = dList.end();
	XClearWindow(xinfo.display, xinfo.window);
	while(begin != end) {
		Displayable* d = *begin;
		d->paint(xinfo);
		begin++;
	}
	//dList.clear();
	XFlush(xinfo.display);
}

// helper funtion to draw the top-line blocks
void DrawTopLine() {
	dList.push_back(new Rec(x1,75,"type1"));
	dList.push_back(new Rec(x2,75,"type1"));
	dList.push_back(new Rec(x3,75,"type1"));
	dList.push_back(new Rec(x4,75,"type1"));
	x1 += counter;
	x2 += counter;
	x3 += counter;
	x4 += counter;
	if(x3 > 875) x3 = topx;
	if(x2 > 875) x2 = topx;
	if(x1 > 875) x1 = topx;
	if(x4 > 875) x4 = topx;
	
}

// helper function to draw the mid-line blocks
void DrawMiddleLine() {
	dList.push_back(new Rec(x5,125,"type2"));
        dList.push_back(new Rec(x6,125,"type2"));
        dList.push_back(new Rec(x7,125,"type2"));
        dList.push_back(new Rec(x8,125,"type2"));
	dList.push_back(new Rec(x9,125,"type2"));
        x5 -= counter;
        x6 -= counter;
        x7 -= counter;
        x8 -= counter;
	x9 -= counter;
        if(x5 < -10) x5 = midx;
        if(x6 < -10) x6 = midx;
        if(x7 < -10) x7 = midx;
        if(x8 < -10) x8 = midx;
	if(x9 < -10) x9 = midx;
}

// helper function to draw the bot-line blocks
void DrawBottomLine() {
	dList.push_back(new Rec(x10,175,"type3"));
        dList.push_back(new Rec(x11,175,"type3"));
        dList.push_back(new Rec(x12,175,"type3"));
        x10 += counter;
        x11 += counter;
        x12 += counter;
        if(x10 > 900) x10 = botx;
        if(x11 > 900) x11 = botx;
        if(x12 > 900) x12 = botx;
}

// frames per second to run animation loop
int FPS = 30;

// helper function to set X foreground colour
enum Colour {BLACK, WHITE};
void setForeground(Colour c) {
	if (c == BLACK) {
		XSetForeground(xinfo.display, xinfo.gc, BlackPixel(xinfo.display, xinfo.screen));
	} else {
		XSetForeground(xinfo.display, xinfo.gc, WhitePixel(xinfo.display, xinfo.screen));
	}
}

// initialize a window
void initX(int argc, char *argv[], XInfo &xinfo, Window root, 
	int x, int y, int width, int height, string name) {
	XSizeHints hints;
	unsigned long white, black;
	white = XWhitePixel( xinfo.display, xinfo.screen );
	black = XBlackPixel( xinfo.display, xinfo.screen );
	hints.x = x;
	hints.y = y;
	hints.width = width;
	hints.height = height;
	hints.flags = PPosition | PSize;
	xinfo.window = XCreateSimpleWindow(
	                   xinfo.display,				// display where window appears
	                   root, 						// window's parent in window tree
	                   hints.x, hints.y,			// upper left corner location
	                   hints.width, hints.height,	// size of the window
	                   Border,						// width of window's border
	                   black,						// window border colour
	                   white );					// window background colour
	XSetStandardProperties(
	    xinfo.display,		// display containing the window
	    xinfo.window,		// window whose properties are set
	    name.c_str(),	// window's title
	    "",				// icon's title
	    None,				// pixmap for the icon
	    argv, argc,			// applications command line args
	    &hints );			// size hints for the window
	/*
	 * Create Graphics Context
	 */
	xinfo.gc = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc, BlackPixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc, WhitePixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc, FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc,
	                   1, LineSolid, CapButt, JoinRound);
	XSelectInput(xinfo.display, xinfo.window,
	             ButtonPressMask | KeyPressMask | ExposureMask);
	   // load a larger font
    XFontStruct * font;
    font = XLoadQueryFont (xinfo.display, "12x24");
    XSetFont (xinfo.display, xinfo.gc, font->fid);
	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xinfo.display, xinfo.window );
}

// collision test
int collisionTest(int xPos,int yPos){
    int list1[4] = {x1,x2,x3,x4};
    int list2[5] = {x5,x6,x7,x8,x9};
    int list3[3] = {x10,x11,x12};
    Rec *a = new Rec(1,1,"type1");
    if(yPos == 75){
    	for(int i = 0;i < 4;i++){
    		if(abs(xPos - list1[i]) < 50){
    		    return 1;
    		}
    	}
    }
    if(yPos == 125){
        for(int i = 0;i < 5;i++){
            if(abs(xPos - list2[i]) < 35){
                return 1;
            }
        }
    }
    if(yPos == 175){
    	for(int i = 0;i < 3;i++){
            if(abs(xPos - list3[i]) < 75){
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
	int frogx,frogy;

	xinfo.display = XOpenDisplay("");

	/* allocate the set of colors we will want to use for the drawing. */
    colormap = DefaultColormap(xinfo.display, 0);
    XParseColor(xinfo.display, colormap, g, &green);
    XParseColor(xinfo.display, colormap, r, &red);
    XParseColor(xinfo.display, colormap, b, &blue);
    XParseColor(xinfo.display, colormap, y, &yellow);
    XParseColor(xinfo.display, colormap, br, &brown);
    XAllocColor(xinfo.display, colormap, &green);
    XAllocColor(xinfo.display, colormap, &red);
    XAllocColor(xinfo.display, colormap, &blue);
    XAllocColor(xinfo.display, colormap, &yellow);
    XAllocColor(xinfo.display, colormap, &brown);
	if (xinfo.display == NULL) exit(-1);
	if(argc > 1){
		FPS = atoi(argv[1]);
	}
	int screennum = DefaultScreen(xinfo.display);
	//create window
	initX(argc,argv,xinfo,DefaultRootWindow(xinfo.display),10,10,850,250,"Frog");

	XFlush(xinfo.display);
	
	XEvent event; // save the event here

	// time of last xinfo.window paint
	unsigned long lastRepaint = 0;
	
	XWindowAttributes w;
	XGetWindowAttributes(xinfo.display, xinfo.window, &w);

	frogx = w.width / 2;
	frogy = w.height - (50 / 2);


	// event loop
	while(true){
		if(XPending(xinfo.display) > 0) {
			XNextEvent(xinfo.display, &event);
			switch(event.type){
			case KeyPress: //any keypress
				KeySym key;
				char text[10];
				int i = XLookupString((XKeyEvent*)&event, text, 10, &key,0);
				if(i == 1 && text[0] == 'q') {
					XCloseDisplay(xinfo.display);
					exit(0);
				}
				if(i == 1 && text[0] == 'n') {
					if(frogy == 25) {
						counter++;
						frogx = w.width / 2;
						frogy = w.height - (50 / 2);
						dList.push_back(new Text(700,30,counter));
					}
				}
				switch(key) {
					case XK_Up:
						if(frogy > 25)
						frogy -= 50;
						break;
					case XK_Down:
						if(frogy < 225 && frogy != 25)
						frogy += 50;
						break;
					case XK_Left:
						if(frogx > 25)
						frogx -= 50;
						break;
					case XK_Right:
						if(frogx < 825)
						frogx += 50;
						break;
				}
				break;
			}
		}
		
		unsigned long end = now();
		if(end - lastRepaint > 1000000 / FPS) {
		// clear background
		XClearWindow(xinfo.display, xinfo.window);
		
		//draw the Frog
		setForeground(BLACK);
		dList.push_back(new Rec(frogx, frogy, "type0"));
		dList.push_back(new Text(700,30,counter));
		DrawTopLine();
		DrawMiddleLine();
		DrawBottomLine();
		if(frogy == 25){
            dList.push_back(new TextW(10,30,counter));
        }
		repaint(dList, xinfo);
		if(collisionTest(frogx,frogy) == 1){
		    frogx = w.width / 2;
            frogy = w.height - (50 / 2);
            counter = 1;
		}

		dList.clear();
		XFlush(xinfo.display);
		lastRepaint = now();
		}
		if (XPending(xinfo.display) == 0) {
			usleep(1000000 / FPS - (end - lastRepaint));
		}
	}
	XCloseDisplay(xinfo.display);
	
}
