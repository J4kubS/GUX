#ifndef DRAW_H
#define DRAW_H

#define LINE_FG_COLOR 0
#define LINE_BG_COLOR 1
#define FILL_FG_COLOR 2
#define FILL_BG_COLOR 3

#define COUNT_COLORS 4
#define BLACK 0
#define WHITE 1
#define RED 2
#define BLUE 3

#define DEFAULT_SHAPE 1
#define POINT 0
#define LINE 1
#define RECTANGLE 2
#define FILLED_RECTANGLE 3
#define ELLIPSE 4
#define FILLED_ELLIPSE 5

#define DEFAULT_WIDTH 1
int widths[] = { 0, 3, 8 };

#define DEFAULT_LINE 0
#define SOLID 0
#define DOUBLE_DASHED 1

#define DEFAULT_LINE_FG_COLOR 0
#define DEFAULT_LINE_BG_COLOR 1
#define DEFAULT_FILL_FG_COLOR 0
#define DEFAULT_FILL_BG_COLOR 1

typedef struct drawing {
	Pixel lineFGColor;
	Pixel lineBGColor;
	Pixel fillFGColor;
	Pixel fillBGColor;
	int x1, y1;
	int x2, y2;
	int shape;
	int width;
	int line;
} Drawing;

Drawing *drawings = NULL;
Drawing inputDrawing;
size_t drawingsAllocated = 0;
size_t drawingsCount = 0;

char *colorNames[COUNT_COLORS] = { "Black", "White", "Red", "Blue" };
Pixel colors[COUNT_COLORS];
Colormap colorMap;
Pixel lineFGColor;
Pixel lineBGColor;
Pixel fillFGColor;
Pixel fillBGColor;

int isButtonPressed;
int shape = DEFAULT_SHAPE;
int width;
int line = DEFAULT_LINE;

XtAppContext context;
Widget application;
Widget mainWindow;
Widget quitDialog;
Widget menuBar;
Widget fileMenu;
Widget holder;
Widget tools;

Widget shapeLabel;
Widget shapeRadio;
Widget widthLabel;
Widget widthRadio;
Widget lineLabel;
Widget lineRadio;
Widget lineFGColorLabel;
Widget lineFGColorCombo;
Widget lineBGColorLabel;
Widget lineBGColorCombo;
Widget fillFGColorLabel;
Widget fillFGColorCombo;
Widget fillBGColorLabel;
Widget fillBGColorCombo;

Widget drawArea;

void initialize(int, char **);
void fileCallback(Widget, XtPointer, XtPointer);
void quitCallback(Widget, XtPointer, XtPointer);
void quitDialogCallback(Widget, XtPointer, XtPointer);
void motionHandler(Widget, XtPointer, XEvent *, Boolean *);
void exposeCallback(Widget, XtPointer, XtPointer);
void drawCallback(Widget, XtPointer, XtPointer);
void shapeCallback(Widget, XtPointer, XtPointer);
void widthCallback(Widget, XtPointer, XtPointer);
void lineCallback(Widget, XtPointer, XtPointer);
void colorCallback(Widget, XtPointer, XtPointer);
int main(int argc, char **argv);

#endif