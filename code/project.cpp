#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "tempTrender.h"


void project() {
	
	//string pathToFile = "/home/courseuser/MNXB01-project/datasets/smhi-opendata_Lund.csv"; 
	string pathToFile;
	cout << endl << "Please enter the full path to the file."; 
	cout << " It should look somehting like this: .../MNXB01-project/datasets/smhi-opendata_Lund.csv" << endl;
	cout << endl << "Full path: ";
	cin >> pathToFile; //Put the path to your data file here
	tempTrender t(pathToFile); //Instantiate your analysis object
	
	cout << endl << endl << "tempOnDay:" << endl << endl;
	t.tempOnDay(12, 24, -2);
	
	cout << endl << endl << "MeanTempAndTempOnDate:" << endl << endl;
	t.MeanTempAndTempOnDate(1977, 7, 7);
	
	cout << endl << endl << "MinMax:" << endl << endl;
	t.MinMax();
	
	cout << endl << endl << "YearlyAverages:" << endl << endl;
	t.YearlyAverages();
	
}
