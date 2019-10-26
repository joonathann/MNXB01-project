#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <TH1.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>

#include "tempTrender.h"

using namespace std;

tempTrender::tempTrender(string filePath) {
	cout << "The user supplied " << filePath << " as the path to the data file." << endl;
	dataFile=filePath;
}

tempTrender::~tempTrender() {}

// Jonathan - Function that gives a histogram for the temperature for a given day.
int tempTrender::tempOnDay(int monthToCalculate, int dayToCalculate, double expTemp) {
	
	// member variables
	month=monthToCalculate;
	day=dayToCalculate;
	yourTemp=expTemp;
	
	string helpString; // used for adding each line to a vector
	vector <string> vecStrings; // vector that we will add each line to
	
	ifstream myFile(dataFile.c_str()); // open file
	
	// if and else statements for adding each line to vector
	if(myFile.is_open()) {
		cout << "Reading datafile..." << endl;
		while(getline(myFile, helpString)) { 
			vecStrings.push_back(helpString); 
		}
	}
	else {
		cout << "Error: not reading the file!" << endl;
		return 1;
	}
	
	myFile.close(); // closing the file
	
	// removing the first elements in the vector since they contain text and not data
	vecStrings.erase(vecStrings.begin(), vecStrings.begin() + 12);
	
	// removing text in the first lines of data
	for(int n=0; n < 12; n++) {
		vecStrings[n] = vecStrings[n].substr(0, 25);
	}
	
	// creating new file
	ofstream tempFile("tempFile.txt");
	cout << "Creating new file with only month, day and temperature..." << endl;
	
	// adding only date and temperature to the new textfile
	int vs = vecStrings.size();
	int stringLength = 0;
	int endofstring = 0;
	string date1;
	string date2;
	string temp;
	for(int i=0; i < vs; i++) {
		stringLength = vecStrings[i].length();
		endofstring = stringLength - 22;
		date1 = vecStrings[i].substr(5, 2);
		date2 = vecStrings[i].substr(8, 2);
		temp = vecStrings[i].substr(20, endofstring);
		tempFile << date1 << " " << date2 << " " << temp << endl;
	}
	
	tempFile.close(); // closing file
	
	// open the file "tempfile.txt"
	ifstream file("tempFile.txt");
	cout << "Reading new file and creating histogram..." << endl << endl;
	
	// while-loop that finds correct data for month and day
	int monthNo = 0;
	int dayNo = 0;
	double temperature = 0.;
	vector <double> vecTemp;
	
	while(file >> monthNo >> dayNo >> temperature) {
		if(monthNo == month) {
			if(dayNo == day) {
				vecTemp.push_back(temperature);
			}
		}
	}
	
	file.close();
	
	// creating the histogram
	TH1I* histogram = new TH1I("temperature", "Temperature;Temperature[#circC];Entries", 300, -20, 40);
	histogram->SetFillColor(kRed +1);
	int vecTempSize = vecTemp.size();
	for(int m=0; m < vecTempSize; m=m+1) { 
		histogram->Fill(vecTemp[m]); 
	}
	
	double meanTemp = histogram->GetMean(); // mean value for the temperature of the day
	double stdevTemp = histogram->GetRMS(); // standard deviation of the temperature
	double integralOfHist = histogram->Integral();
	double binx = histogram->FindBin(yourTemp);
	double integralOfTemp = histogram->Integral(binx-0.5, binx+0.5);
	double probTemp = integralOfTemp/integralOfHist;
	
	TCanvas* canvas = new TCanvas("tempOnDay", "tempOnDay");
	histogram->Draw();
	
	// making a legend
	TLegend* leg = new TLegend(0.71, 0.86, 0.95, 0.95);
	stringstream dString;
	stringstream mString;
	dString << day;
	mString << month;
	string dayString = dString.str();
	string monthString = mString.str();
	string label = "Temperature on " + dayString + "/" + monthString;
	leg->AddEntry(histogram, label.c_str(), "F");
	leg->Draw();
	
	cout << "Mean value for the temperature at " << day << "/" << month << " in Lund from 1961 to 2015: " << meanTemp << endl;
	cout << "Standard deviation of the temperature at " << day << "/" << month << " in Lund from 1961 to 2015: " << stdevTemp << endl;
	cout << "Probability for your expected temperature (" << yourTemp << " degree Celsius): " << probTemp*100 << "%" << endl;
	
	return 0;
}


// Andreas - Function that gives the mean temp and a histogram for the temperature for a given date.
void tempTrender::MeanTempAndTempOnDate(int yearToCalculate, int monthToCalculate, int dayToCalculate) {
	
	// member variables
	year=yearToCalculate;
	month=monthToCalculate;
	day=dayToCalculate;
	
	string helpString; // used for adding each line to a vector
	vector <string> vecStrings; // vector that we will add each line to
	
	ifstream myFile(dataFile.c_str()); // open file
	
	// if and else statements for adding each line to vector
	if(myFile.is_open()) {
		while(getline(myFile, helpString)) { 
			vecStrings.push_back(helpString); 
		}
	}
	else {
		cout << "Error: not reading the file!" << endl;
	}
	
	myFile.close(); // closing the file
	
	// removing the first elements in the vector since they contain text and not data
	vecStrings.erase(vecStrings.begin(), vecStrings.begin() + 12);
	
	// creating new file
	ofstream tempFile("tempFile.txt");
	
	// removing text in the first lines of data
	for(int n=0; n < 12; n++) {
		vecStrings[n] = vecStrings[n].substr(0, 25);
	}
	
	// adding only date and temperature to the new textfile
	int vs = vecStrings.size();
	int stringLength = 0;
	int endofstring = 0;
	string date0;
	string date1;
	string date2;
	string temp;
	for(int i=0; i < vs; i++) {
		stringLength = vecStrings[i].length();
		endofstring = stringLength - 22;
		date0 = vecStrings[i].substr(0, 4);
		date1 = vecStrings[i].substr(5, 2);
		date2 = vecStrings[i].substr(8, 2);
		temp = vecStrings[i].substr(20, endofstring);
		tempFile << date0 << " " << date1 << " " << date2 << " " << temp << endl;
	}
	
	tempFile.close(); // closing file
	
	// open the file "tempfile.txt"
	ifstream file("tempFile.txt");
	
	// while-loop that finds correct data for month and day
	int yearNo = 0;
	int monthNo = 0;
	int dayNo = 0;
	double temperature = 0.;
	vector <double> vecTemp;
	
	while(file >> yearNo >> monthNo >> dayNo >> temperature) {
		if(yearNo == year) {
			if(monthNo == month) {
				if(dayNo == day) {
					cout  << temperature << endl; 
					vecTemp.push_back(temperature);
				}
			}
		}
	}
	
	
	int L = 0;
	L = vecTemp.size();
	
	cout << "Length : " << L << endl;
	
	
	double mean;
	double tot = 0;
	
	
	for(int i=0; i < L; i++) {
		
		tot = tot + vecTemp[i];	
	}
	
	
	cout << "Total : " << tot << endl;
	
	mean = tot/L;
	
	cout << "Mean : " << mean << endl;
	
	
	file.close();
	
	// creating the histogram
	TH1I* histogram = new TH1I("temperature", "Temperature;Temperature[#circC];Entries", 300, -20, 40);  
	histogram->SetFillColor(kRed +1);
	double vecTempSize = vecTemp.size();
	for(int m=0; m < vecTempSize; m++) {
		histogram->Fill(vecTemp[m]);
	}
	TCanvas* canvas = new TCanvas();
	histogram->Draw();
	
}

