/*
* Standard XToolkit and OSF/Motif include files.
*/
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

/*
* Public include files for widgets used in this file.
*/
#include <Xm/MainW.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Protocols.h>
#include <Xm/MessageB.h>
#include <Xm/Label.h>
#include <Xm/ComboBox.h>

/*
* Common C library include files.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "draw.h"

void initialize(int argc, char **argv)
{
	char *fallback[] = {
		"*quitDialog.dialogTitle: QUIT",
		"*quitDialog.messageString: Quit application?",
		"*quitDialog.okLabelString: OK",
		"*quitDialog.cancelLabelString: Cancel",
		"*quitDialog.messageAlignment: XmALIGNMENT_CENTER",
		"*.shapeLabel.labelString: Shapes",
		"*.widthLabel.labelString: Widths",
		"*.lineLabel.labelString: Line",
		"*.lineFGColorLabel.labelString: Line foreground",
		"*.lineBGColorLabel.labelString: Line background",
		"*.fillFGColorLabel.labelString: Fill foreground",
		"*.fillBGColorLabel.labelString: Fill background",
		NULL
	};

	application = XtVaAppInitialize(
		&context,
		"draw",
		NULL,
		0,
		&argc,
		argv,
		fallback,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);

	// Colors
	XColor xcolor, spare;
	char *colorName;
	int i;
	colorMap = DefaultColormapOfScreen(XtScreen(application));

	for (i = 0; i < COUNT_COLORS; i++)
	{
		colorName = colorNames[i];

		if (XAllocNamedColor(XtDisplay(application), colorMap, colorName, &xcolor, &spare) == 0)
		{
			fprintf(stderr, "Error: Cannot allocate colormap entry for \"%s\"\n", colorName);
			exit(1);
		}

		colors[i] = xcolor.pixel;
	}

	width = widths[DEFAULT_WIDTH];
	lineFGColor = colors[DEFAULT_LINE_FG_COLOR];
	lineBGColor = colors[DEFAULT_LINE_BG_COLOR];
	fillFGColor = colors[DEFAULT_FILL_FG_COLOR];
	fillBGColor = colors[DEFAULT_FILL_BG_COLOR];

	mainWindow = XtVaCreateManagedWidget(
		"mainWindow",
		xmMainWindowWidgetClass,
		application,
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		XmNwidth, 1200,
		XmNheight, 695,
		NULL);

	// Menu bar
	XmString file  = XmStringCreateSimple("File");
	XmString clear = XmStringCreateSimple("Clear");
	XmString altC  = XmStringCreateSimple("Alt-C");
	XmString quit  = XmStringCreateSimple("Quit");
	XmString altQ  = XmStringCreateSimple("Alt-Q");
	menuBar = XmVaCreateSimpleMenuBar(
		mainWindow,
		"menuBar",
		XmVaCASCADEBUTTON, file, XK_F,
		NULL);

	fileMenu = XmVaCreateSimplePulldownMenu(
		menuBar,
		"fileMenu",
		0,
		fileCallback,
		XmVaPUSHBUTTON, clear, XK_C, "Alt<Key>C", altC,
		XmVaPUSHBUTTON, quit,  XK_Q, "Alt<Key>Q", altQ,
		NULL);

	XmStringFree(file);
	XmStringFree(clear);
	XmStringFree(altC);
	XmStringFree(quit);
	XmStringFree(altQ);

	holder = XtVaCreateManagedWidget(
		"holder",
		xmFrameWidgetClass,
		mainWindow,
		NULL);

	tools = XtVaCreateManagedWidget(
		"tools",
		xmRowColumnWidgetClass,
		holder,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmVERTICAL,
		XmNpacking, XmPACK_TIGHT,
		NULL);

	// Shapes
	XmString point           = XmStringCreateSimple("Point");
	XmString line            = XmStringCreateSimple("Line");
	XmString rectangle       = XmStringCreateSimple("Rectangle");
	XmString filledRectangle = XmStringCreateSimple("Filled rectangle");
	XmString ellipse         = XmStringCreateSimple("Ellipse");
	XmString filledEllipse   = XmStringCreateSimple("Filled ellipse");
	shapeLabel = XtVaCreateManagedWidget("shapeLabel", xmLabelWidgetClass, tools, NULL);
	shapeRadio = XmVaCreateSimpleRadioBox(
		tools,
		"shapeRadio",
		DEFAULT_SHAPE,
		shapeCallback,
		XmVaRADIOBUTTON, point,           NULL, NULL, NULL,
		XmVaRADIOBUTTON, line,            NULL, NULL, NULL,
		XmVaRADIOBUTTON, rectangle,       NULL, NULL, NULL,
		XmVaRADIOBUTTON, filledRectangle, NULL, NULL, NULL,
		XmVaRADIOBUTTON, ellipse,         NULL, NULL, NULL,
		XmVaRADIOBUTTON, filledEllipse,   NULL, NULL, NULL,
		NULL);

	XmStringFree(point);
	XmStringFree(line);
	XmStringFree(rectangle);
	XmStringFree(filledRectangle);
	XmStringFree(ellipse);
	XmStringFree(filledEllipse);

	// Widths
	XmString _0px = XmStringCreateSimple("0 px");
	XmString _3px = XmStringCreateSimple("3 px");
	XmString _8px = XmStringCreateSimple("8 px");
	widthLabel = XtVaCreateManagedWidget("widthLabel", xmLabelWidgetClass, tools, NULL);
	widthRadio = XmVaCreateSimpleRadioBox(
		tools,
		"widthRadio",
		DEFAULT_WIDTH,
		widthCallback,
		XmVaRADIOBUTTON, _0px, NULL, NULL, NULL,
		XmVaRADIOBUTTON, _3px, NULL, NULL, NULL,
		XmVaRADIOBUTTON, _8px, NULL, NULL, NULL,
		NULL);

	XmStringFree(_0px);
	XmStringFree(_3px);
	XmStringFree(_8px);

	XmString solid        = XmStringCreateSimple("Solid");
	XmString doubleDashed = XmStringCreateSimple("Double dashed");
	lineLabel = XtVaCreateManagedWidget("lineLabel", xmLabelWidgetClass, tools, NULL);
	lineRadio = XmVaCreateSimpleRadioBox(
		tools,
		"lineRadio",
		0,
		lineCallback,
		XmVaRADIOBUTTON, solid,        NULL, NULL, NULL,
		XmVaRADIOBUTTON, doubleDashed, NULL, NULL, NULL,
		NULL);

	XmStringFree(solid);
	XmStringFree(doubleDashed);

	// Colors
	XmString black = XmStringCreateSimple("Black");
	XmString white = XmStringCreateSimple("White");
	XmString red   = XmStringCreateSimple("Red");
	XmString blue  = XmStringCreateSimple("Blue");
	XmString colorStrings[] = { black, white, red, blue };

	// Line foreground
	lineFGColorLabel = XtVaCreateManagedWidget("lineFGColorLabel", xmLabelWidgetClass, tools, NULL);
	lineFGColorCombo = XtVaCreateManagedWidget(
		"lineFGColorCombo",
		xmComboBoxWidgetClass,
		tools,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, COUNT_COLORS,
		XmNitems, colorStrings,
		XmNselectedPosition, DEFAULT_LINE_FG_COLOR,
		NULL);

	// Line background
	lineBGColorLabel = XtVaCreateManagedWidget("lineBGColorLabel", xmLabelWidgetClass, tools, NULL);
	lineBGColorCombo = XtVaCreateManagedWidget(
		"lineBGColorCombo",
		xmComboBoxWidgetClass,
		tools,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, COUNT_COLORS,
		XmNitems, colorStrings,
		XmNselectedPosition, DEFAULT_LINE_BG_COLOR,
		NULL);

	// Fill foreground
	fillFGColorLabel = XtVaCreateManagedWidget("fillFGColorLabel", xmLabelWidgetClass, tools, NULL);
	fillFGColorCombo = XtVaCreateManagedWidget(
		"fillFGColorCombo",
		xmComboBoxWidgetClass,
		tools,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, COUNT_COLORS,
		XmNitems, colorStrings,
		XmNselectedPosition, DEFAULT_FILL_FG_COLOR,
		NULL);

	// Fill background
	fillBGColorLabel = XtVaCreateManagedWidget("fillBGColorLabel", xmLabelWidgetClass, tools, NULL);
	fillBGColorCombo = XtVaCreateManagedWidget(
		"fillBGColorCombo",
		xmComboBoxWidgetClass,
		tools,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, COUNT_COLORS,
		XmNitems, colorStrings,
		XmNselectedPosition, DEFAULT_FILL_BG_COLOR,
		NULL);

	// Color callbacks
	XtAddCallback(lineFGColorCombo, XmNselectionCallback, colorCallback, (XtPointer) LINE_FG_COLOR);
	XtAddCallback(lineBGColorCombo, XmNselectionCallback, colorCallback, (XtPointer) LINE_BG_COLOR);
	XtAddCallback(fillFGColorCombo, XmNselectionCallback, colorCallback, (XtPointer) FILL_FG_COLOR);
	XtAddCallback(fillBGColorCombo, XmNselectionCallback, colorCallback, (XtPointer) FILL_BG_COLOR);

	XmStringFree(black);
	XmStringFree(white);
	XmStringFree(red);
	XmStringFree(blue);

	// Draw area
	drawArea = XtVaCreateManagedWidget(
		"drawArea",
		xmDrawingAreaWidgetClass,
		holder,
		XmNbackground, colors[WHITE],
		NULL);

	XtAddEventHandler(drawArea, ButtonMotionMask, False, motionHandler, NULL);
	XtAddCallback(drawArea, XmNexposeCallback, exposeCallback, drawArea);
	XtAddCallback(drawArea, XmNinputCallback, drawCallback, drawArea);

	quitDialog = XmCreateQuestionDialog(application, "quitDialog", NULL, 0);
	XtVaSetValues(quitDialog, XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL, NULL);
	XtUnmanageChild(XmMessageBoxGetChild(quitDialog, XmDIALOG_HELP_BUTTON));
	XtAddCallback(quitDialog, XmNcancelCallback, quitDialogCallback, (XtPointer) 0);
	XtAddCallback(quitDialog, XmNokCallback, quitDialogCallback, (XtPointer) 1);

	XtManageChild(mainWindow);
	XtManageChild(menuBar);
	XtManageChild(shapeRadio);
	XtManageChild(widthRadio);
	XtManageChild(lineRadio);
	XtManageChild(drawArea);
}

void shapeCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	XmToggleButtonCallbackStruct *data = (XmToggleButtonCallbackStruct *) callData;

	if (data->set)
		shape = (intptr_t) clientData;
}

void widthCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	XmToggleButtonCallbackStruct *data = (XmToggleButtonCallbackStruct *) callData;

	if (data->set)
		width = widths[(intptr_t) clientData];
}

void lineCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	XmToggleButtonCallbackStruct *data = (XmToggleButtonCallbackStruct *) callData;

	if (data->set)
	{
		if ((intptr_t) clientData == DOUBLE_DASHED)
			line = LineDoubleDash;
		else
			line = LineSolid;
	}
}

void colorCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	XmComboBoxCallbackStruct *data = (XmComboBoxCallbackStruct *) callData;
	intptr_t type = (intptr_t) clientData;
	int selected = data->item_position;

	switch (type)
	{
		case LINE_FG_COLOR:
			lineFGColor = colors[selected];
			break;
		case LINE_BG_COLOR:
			lineBGColor = colors[selected];
			break;
		case FILL_FG_COLOR:
			fillFGColor = colors[selected];
			break;
		case FILL_BG_COLOR:
			fillBGColor = colors[selected];
			break;
		default:
			break;
	}
}

void fileCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	if ((intptr_t) clientData == 0)
	{
		if (NULL != drawings)
		{
			free(drawings);

			drawingsAllocated = 0;
			drawingsCount = 0;
			drawings = NULL;
		}

		XClearWindow(XtDisplay(drawArea), XtWindow(drawArea));
	}
	else if ((intptr_t) clientData == 1)
		quitCallback(widget, clientData, callData);
}

void quitCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	XtManageChild(quitDialog);
}

void quitDialogCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	if ((intptr_t) clientData == 1)
	{
		if (drawings != NULL)
			free(drawings);

		exit(0);
	}
}

void draw(Widget widget, GC gc, Drawing drawing)
{
	XGCValues values;
	int width = drawing.width;
	int x1 = drawing.x1;
	int y1 = drawing.y1;
	int x2 = drawing.x2;
	int y2 = drawing.y2;
	values.foreground = drawing.lineFGColor;
	values.background = drawing.lineBGColor;
	values.line_width = drawing.width;
	values.line_style = drawing.line;

	XChangeGC(XtDisplay(widget), gc, GCLineStyle | GCLineWidth | GCForeground | GCBackground, &values);

	switch (drawing.shape)
	{
		case POINT:
			if (width > 0)
				XFillArc(XtDisplay(widget), XtWindow(widget), gc, (int) (x2 - width / 2.0), (int) (y2 - width / 2.0), width, width, 0, 360 * 64);
			else
				XDrawPoint(XtDisplay(widget), XtWindow(widget), gc, x2, y2);
			break;
		case LINE:
			XDrawLine(XtDisplay(widget), XtWindow(widget), gc, x1, y1, x2, y2);
			break;
		case FILLED_RECTANGLE:
			XSetForeground(XtDisplay(widget), gc, drawing.fillFGColor);
			XFillRectangle(XtDisplay(widget), XtWindow(widget), gc, x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
			XSetForeground(XtDisplay(widget), gc, drawing.lineFGColor);
			XDrawRectangle(XtDisplay(widget), XtWindow(widget), gc, x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
			break;
		case RECTANGLE:
			XDrawRectangle(XtDisplay(widget), XtWindow(widget), gc, x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
			break;
		case FILLED_ELLIPSE:
			XSetForeground(XtDisplay(widget), gc, drawing.fillFGColor);
			XFillArc(XtDisplay(widget), XtWindow(widget), gc, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
			XSetForeground(XtDisplay(widget), gc, drawing.lineFGColor);
			XDrawArc(XtDisplay(widget), XtWindow(widget), gc, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
			break;
		case ELLIPSE:
			XDrawArc(XtDisplay(widget), XtWindow(widget), gc, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
			break;
		default:
			break;
	}
}

void motionHandler(Widget widget, XtPointer clientData, XEvent *event, Boolean *cont)
{
	static GC gc = 0;
	Pixel fg, bg;

	if (isButtonPressed)
	{
		if (!gc)
		{
			gc = XCreateGC(XtDisplay(widget), XtWindow(widget), 0, NULL);

			XtVaGetValues(widget, XmNforeground, &fg, XmNbackground, &bg, NULL);
			XSetForeground(XtDisplay(widget), gc, fg ^ bg);
			XSetFunction(XtDisplay(widget), gc, GXxor);
			XSetPlaneMask(XtDisplay(widget), gc, ~0);
		}

		if (isButtonPressed > 1)
			draw(widget, gc, inputDrawing);
		else
			isButtonPressed = 2;

		inputDrawing.x2 = event->xmotion.x;
		inputDrawing.y2 = event->xmotion.y;

		draw(widget, gc, inputDrawing);
	}
}

void exposeCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	static GC gc = 0;
	int i;

	if (drawingsCount > 0)
	{
		if (!gc)
			gc = XCreateGC(XtDisplay(widget), XtWindow(widget), 0, NULL);

		for (i = 0; i < drawingsCount; i++)
			draw(widget, gc, drawings[i]);
	}
}

void drawCallback(Widget widget, XtPointer clientData, XtPointer callData)
{
	XmDrawingAreaCallbackStruct *data = (XmDrawingAreaCallbackStruct *) callData;

	switch (data->event->type)
	{
		case ButtonPress:
			if (data->event->xbutton.button == Button1)
			{
				inputDrawing.lineFGColor = lineFGColor ^ colors[DEFAULT_LINE_BG_COLOR];
				inputDrawing.lineBGColor = lineBGColor ^ colors[DEFAULT_LINE_BG_COLOR];
				inputDrawing.fillFGColor = fillFGColor ^ colors[DEFAULT_FILL_FG_COLOR];
				inputDrawing.fillBGColor = fillBGColor ^ colors[DEFAULT_FILL_BG_COLOR];
				inputDrawing.x1 = data->event->xbutton.x;
				inputDrawing.y1 = data->event->xbutton.y;
				inputDrawing.shape = shape;
				inputDrawing.width = width;
				inputDrawing.line = line;
				isButtonPressed = 1;
			}
			break;
		case ButtonRelease:
			if (data->event->xbutton.button == Button1)
			{
				if (drawingsCount >= drawingsAllocated)
				{
					drawingsAllocated = drawingsAllocated ? drawingsAllocated << 1 : 1;
					drawings = (Drawing *) realloc(drawings, drawingsAllocated * sizeof(Drawing));
				}

				drawings[drawingsCount].lineFGColor = inputDrawing.lineFGColor ^ colors[DEFAULT_LINE_BG_COLOR];
				drawings[drawingsCount].lineBGColor = inputDrawing.lineBGColor ^ colors[DEFAULT_LINE_BG_COLOR];
				drawings[drawingsCount].fillFGColor = inputDrawing.fillFGColor ^ colors[DEFAULT_FILL_FG_COLOR];
				drawings[drawingsCount].fillBGColor = inputDrawing.fillBGColor ^ colors[DEFAULT_FILL_BG_COLOR];
				drawings[drawingsCount].x2 = data->event->xbutton.x;
				drawings[drawingsCount].y2 = data->event->xbutton.y;
				drawings[drawingsCount].shape = inputDrawing.shape;
				drawings[drawingsCount].width = inputDrawing.width;
				drawings[drawingsCount].line = inputDrawing.line;
				drawings[drawingsCount].x1 = inputDrawing.x1;
				drawings[drawingsCount].y1 = inputDrawing.y1;
				isButtonPressed = 0;
				drawingsCount++;

				XClearArea(XtDisplay(widget), XtWindow(widget), 0, 0, 0, 0, True);
			}
			break;
		default:
			break;
	}
}



int main(int argc, char **argv)
{
	initialize(argc, argv);

	Atom wmDelete = XInternAtom(XtDisplay(application), "WM_DELETE_WINDOW", False);
	XmAddWMProtocolCallback(application, wmDelete, quitCallback, NULL);
	XmActivateWMProtocol(application, wmDelete);

	XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL);
	XtRealizeWidget(application);
	XtAppMainLoop(context);

	if (drawings != NULL)
		free(drawings);

	return 0;
}