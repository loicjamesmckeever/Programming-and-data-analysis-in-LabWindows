#include <ansi_c.h>
#include <utility.h>
#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>
#include <stdio.h>
#include "Levangie_McKeever_Lab1_1.h"

static int panelHandle;
static int count;
static char c;
//File to array conversion variables
static float FileArray[300];
static char File[300];
static FILE *fp;
static double bl;
static float nf;

//Display UI panel
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Levangie_McKeever_Lab1_1.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

//Quit program
int CVICALLBACK Bye (int panel, int control, int event,
					 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

//File selection, open, and file to array conversion
int CVICALLBACK Select (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_COMMIT:
				 FileSelectPopup ("c:\\Users\\Student\\Desktop", "*.txt*", "*.txt*", "File Selection", VAL_LOAD_BUTTON, 0, 0, 1, 0,File);
				 fp=fopen(File, "r");
				 for(c=getc(fp); c != EOF; c=getc(fp))
					 if (c=='\n')
						 count=count+1;

				 FileToArray (File, FileArray, VAL_FLOAT,2*count , 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
				 
			break;
	}
	return 0;
}

//Generate two arrays and display plot of data on UI panel
int CVICALLBACK Display (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			   DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);
			   float X[count];
			   float Y[count];
			   int j=0;
			   for (int i=0;i<2*count;i=i+2) {
				   X[j]= FileArray[i];
			   	   j=j+1;
			   }
			   j=0;
			   for (int i=1; i<2*count;i=i+2) {
				   Y[j]=FileArray[i];
			   	   j=j+1;
			   }
			   PlotXY (panelHandle, PANEL_GRAPH, X, Y, count, VAL_FLOAT, VAL_FLOAT, VAL_THIN_LINE,
               VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			break;
	}
	return 0;
}

//Input baseline value
int CVICALLBACK baseline_value (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				 GetCtrlVal (panelHandle, PANEL_NUMERIC, &bl);
			break;
	}
	return 0;
}

//Apply baseline to data
int CVICALLBACK subtract_baseline (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				 for (int i=1; i<2*count; i=i+2) {
					 FileArray[i]=FileArray[i]-bl;
				 }
			break;
	}
	return 0;
}

//Normalize the data 
int CVICALLBACK normalize (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				 float bw;
				 bw=FileArray[2]-FileArray[0];
				 for (int i=1; i<2*count; i=i+2){
					  nf=nf+FileArray[i]*bw;
				 }
				 for(int i=1; i<2*count; i=i+2){
					 FileArray[i]=FileArray[i]/nf;
				 }
				 SetCtrlVal (panelHandle, PANEL_NUMERIC_2, nf);
			break;
	}
	return 0;
}

//Display normalization factor
int CVICALLBACK normalization_display (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

//Calculate the centroid of the peak and plot a line at the centroid's position
int CVICALLBACK calculate_centroid (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				float bw;
				float c=0;
				float c_x;
				float c_y;
				float a=0;
				 bw=FileArray[2]-FileArray[0];
				 for (int i=1; i<2*count; i=i+2){
					  a=a+FileArray[i]*bw;
				 }
				 for(int i=1; i<2*count; i=i+2){
					 c=c+FileArray[i]*bw;
					 c_x=FileArray[i-1];
					 c_y=FileArray[i];
					 if (c>a/2){
						 break;
					 }
				 }
	 			 SetCtrlVal (panelHandle, PANEL_NUMERIC_3, c_x);
				 PlotLine (panelHandle, PANEL_GRAPH, c_x, 0, c_x, c_y, VAL_BLUE);
			break;
	}
	return 0;
}

//Save data to a file
int CVICALLBACK save (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			float SaveFile[count][2];  
			int j=0;
			int size;
			   for (int i=0;i<2*count;i=i+2) {
				   SaveFile[j][0]= FileArray[i];
			   	   j=j+1;
			   }
			   j=0;
			   for (int i=1; i<2*count;i=i+2) {
				   SaveFile[j][1]=FileArray[i];
			   	   j=j+1;
			   }
			   size=2*count;
				 ArrayToFile ("Levangie_McKeever", SaveFile, VAL_FLOAT, size, 2, VAL_DATA_MULTIPLEXED, VAL_GROUPS_AS_COLUMNS, VAL_SEP_BY_TAB, 10, VAL_ASCII, VAL_TRUNCATE);
			break;
	}
	return 0;
}
