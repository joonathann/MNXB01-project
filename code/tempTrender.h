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
		
		// for tempOnDay
		int month;
		int day;
		double yourTemp;
		
	
	public:
		tempTrender(string filePath); //Construct using the specified file
		~tempTrender(); //Destructor
		
		// Jonathan P - Function that gives a histogram for the temperature for a given day. 
		int tempOnDay(int monthToCalculate, int dayToCalculate, double expTemp);
		
};

#endif









// This was included in the file, just leaving it down here for now...

//void tempOnDay(int monthToCalculate, int dayToCalculate); //Make a histogram of the temperature on this day
//void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
//void hotCold(); //Make a histogram of the hottest and coldest day of the year
//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year
