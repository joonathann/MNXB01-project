#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "tempTrender.h"


void project() {
	
	string pathToFile = "/home/courseuser/MNXB01-project/datasets/smhi-opendata_Lund.csv"; //Put the path to your data file here
	tempTrender t(pathToFile); //Instantiate your analysis object
	
	t.tempOnDay(10, 10);
	
}











// This was included in the file, just leaving it down here for now...

//t.tempOnDay(8, 23); //Call some functions that you've implemented
//t.tempOnDay(235);
//t.tempPerDay();
//t.hotCold();
//t.tempPerYear(2050);
