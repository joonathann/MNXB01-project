#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class tempTrender {
	
	private:
		string dataFile;
		
		int year;
		int month;
		int day;
		double yourTemp;
		
	
	public:
		tempTrender(string filePath); //Construct using the specified file
		~tempTrender(); //Destructor
		
		// Jonathan P - Function that gives a histogram for the temperature for a given day. 
		int tempOnDay(int monthToCalculate, int dayToCalculate, double expTemp);
		
		// Andreas - Function that gives the mean temp and a histogram for the temperature for a given date. 
		void MeanTempAndTempOnDate(int yearToCalculate, int monthToCalculate, int dayToCalculate);
		
};

#endif
