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
	cout << "The user supplied " << filePath << " as the path to the data file." << endl << endl;
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
	
	// becoming a legend
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

	//Calculating the average temp during a year.
int tempTrender::YearlyAverages(){
	
	
	vector <string> vecStrings; // vector that we will add each line to
	string helpString; // used for adding each line to a vector
	int MeanTemp;
	MeanTemp=0;
	
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
	ofstream tempFile("tempFiley.txt");
	cout << "Creating new file with only year and temperature..." << endl;
	
	// adding year and date and temperature  to the new textfile
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
		date0 = vecStrings[i].substr(0,4);
		date1 = vecStrings[i].substr(5, 2);
		date2 = vecStrings[i].substr(8, 2);
		temp = vecStrings[i].substr(20, endofstring);
		tempFile<<date0<<""<<temp<<endl;
	}
	
	tempFile.close(); // closing year temp file
		
	vector<double> YearlyTemps;// vector that will contain the average temp for each year
	int yearsum=0;
	double temper=0.0;
	double yearaverage=0.0;
	int daycounter=0;
	int yearcounter=1961;
	int year=0;
	//opening the file containing years and temperatures.
	ifstream file1("tempFileY.txt");
	cout << file1<<endl;
	//while loop for entire document
	while(file1 >> year >>temper)
			{
				
			yearaverage= yearsum/daycounter;
			YearlyTemps.push_back(yearaverage);
			daycounter=0;
	        yearsum=0;
			cout<<yearcounter<<endl;
			yearcounter++;
		//while loop adding the total temperatures for a year.
			while(file1 >> year >>temper)
				{
				if(year=yearcounter)
				{
					yearsum+=temper;
					daycounter++;
				}
				else break;	
				}
			}
	file1.close();
	yearcounter=1961;
	
	int YearlyTempsize =YearlyTemps.size();
	for (int i=0;i<YearlyTempsize;i++)
	{
		cout<<yearcounter<<"  " <<YearlyTemps[i]<<endl;
		yearcounter++;
	}
	TH1I* histogram = new TH1I("temperature", "Temperature;Temperature[#circC];Entries", 300, -20, 40);
	histogram->SetFillColor(kBlue +1);
	int HistSize = YearlyTemps.size();
	for(int m=0; m < HistSize; m=m+1) { 
		histogram->Fill(YearlyTemps[m]); 	
	}
	
	
	
	
	
return 0;
}
