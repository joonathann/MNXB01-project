#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <TH1.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TMultiGraph.h>

#include "tempTrender.h"

using namespace std;

tempTrender::tempTrender(string filePath) {
	cout << endl << "The user supplied " << filePath << " as the path to the data file." << endl;
	dataFile=filePath;
}

tempTrender::~tempTrender() {}


int tempTrender::TempFile() {

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
	cout << "Creating new file with only year, month, day and temperature..." << endl;
	
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
}


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
	TH1I* histogram = new TH1I("temperature_mean", "Temperature;Temperature[#circC];Entries", 300, -20, 40);  
	histogram->SetFillColor(kRed +1);
	double vecTempSize = vecTemp.size();
	for(int m=0; m < vecTempSize; m++) {
		histogram->Fill(vecTemp[m]);
	}
	
	TCanvas* canvas = new TCanvas("MeanTempAndTempOnDate", "MeanTempAndTempOnDate");
	histogram->Draw();
	
}


// Martin 
int tempTrender::MinMax(){
	//defining all the variable we need
	int YearCounter=1961;
	int DesiredYear;
	int YearNo;
	int MonthNo;
	int DayNo;
	int MinimumMonth;
	int MinimumDay;
	int MaximumMonth;
	int MaximumDay;
	double Temperature;
	double YearMin;
	double YearMax;
	//user enters specific year they wish to know min and max of
	cout << "Enter the year you wish to know the minimum and maximum temperature of between 1961 and 2015: "<< endl;
	cin >> DesiredYear;
	int InternalYear=DesiredYear-1961;
	ifstream File("tempFile.txt"); // open file
	//define all the vectors we need
	vector <double> Minvec;
	vector <double> Maxvec;
	vector <int> MinDay;
	vector <int> MinMonth;
	vector <int> MinYear;
	vector <int> MaxDay;
	vector <int> MaxMonth;
	vector <int> MaxYear;
	
	//adds all temperatures (min and max) and dates to vectors
	while(File >> YearNo >> MonthNo >> DayNo >> Temperature){
		if(YearNo == YearCounter){
			if(YearMin > Temperature){
				YearMin = Temperature;
				MinimumMonth = MonthNo;
				MinimumDay = DayNo;
			}
			if(YearMax < Temperature){
				YearMax = Temperature;
				MaximumMonth = MonthNo;
				MaximumDay = DayNo;
			}
		}
		else {
			//resets when new year comes, and adds things to vectors
			YearCounter++;
			
			Minvec.push_back(YearMin);
			Maxvec.push_back(YearMax);
			
			MinMonth.push_back(MinimumMonth);
			MinDay.push_back(MinimumDay);
			
			MaxMonth.push_back(MaximumMonth);
			MaxDay.push_back(MaximumDay);
			
			YearMin=0;
			YearMax=0;
		}
	}
	//output for speccific year
	cout << "The minimum temperature during the year "<< DesiredYear << " was: " << Minvec[InternalYear] << " which happnened on " << MinDay[InternalYear] << "/" << MinMonth[InternalYear] << endl
	<<  "The maximum temperature during the year "<< DesiredYear << " was: " << Maxvec[InternalYear] << " which happnened on " << MaxDay[InternalYear] << "/" << MaxMonth[InternalYear] << endl;
	
	const int NoYears=2015;
	double MinList[NoYears];
	double MaxList[NoYears];
	double YearList[NoYears];
	//converts vectors to lists for graphing purposes
	for (int i=0; i<NoYears; i++){
		MinList[i]=Minvec[i];
		MaxList[i]=Maxvec[i];
		YearList[i]=i+1961;
	}
	//graphing the data that was produced
	TGraph* MinGraph = new TGraph(54, YearList, MinList);
	MinGraph -> SetLineColor(4);
	MinGraph -> SetLineWidth(2);
	
	TGraph* MaxGraph = new TGraph(54, YearList, MaxList);
	MaxGraph -> SetLineColor(2);
	MaxGraph -> SetLineWidth(2);

	TMultiGraph *MinMaxGraph = new TMultiGraph();
	MinMaxGraph -> Add(MinGraph);
	MinMaxGraph -> Add(MaxGraph);
	MinMaxGraph -> SetTitle("Minimum and Maximum Temperature per year from 1961 to 2015; Year; Temperature( #circC)");
	TCanvas* canvas = new TCanvas("MinMax", "MinMax");
	MinMaxGraph -> Draw("A");
	
	File.close(); // closing file
	return 0;
}


// Jonathan W - Calculating the average temp during a year.
int tempTrender::YearlyAverages(){
	
	vector <string> vecStrings; // vector that we will add each line to
	string helpString; // used for adding each line to a vector
	int MeanTemp=0;
	
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
		tempFile<<date0<<" "<<temp<<endl;
	}
	
	tempFile.close(); // closing year temp file
	
	
	//opening the file containing years and temperatures.	
	ifstream file1("tempFiley.txt");	
	
	
	vector<double> YearlyTemps;// vector that will contain the average temp for each year
	vector<double>Years;
	double yearsum=0;
	double temper=0.0;
	int yearcounter=1961;
	int yearNo=0;
	int yearlength;
	double YearMean=0.0;
	int daycounter=0;
	
	//while loop for the entire document
	while(file1 >> yearNo >> temper)
	{
		if (yearNo==yearcounter)
		{
//		cout<<yearNo << " "<< temper <<endl;  //This was used to verify the file being read in properly.
		yearsum+=temper;
		daycounter++;	
		}
			
		//This segment first calculates the average temperature and the 
		// resets the sums before moving on to the next year.
		
		else	
		{
		yearlength=daycounter;	
		YearMean=yearsum/yearlength;
		YearlyTemps.push_back(YearMean);			
		daycounter=0;		
		yearsum=0;
		Years.push_back(yearcounter);
		yearcounter++;	
		}
	}
	
	// Printing the total aswell as the average temperature for the last year.
	// mostly used to check stuff
	
	//cout <<"sum of temperatures for a year is "<< yearsum <<"  "<<yearlength*YearMean<< endl;
	//cout<< "amount of points in a year is " << yearlength << endl ;
	//cout<< "year mean " <<YearMean<<endl << endl;
 	
 	
	file1.close();
	
	
	TH1I* hist= new TH1I("temperature_year", "Temperature;Year;Yearly Temperature", 300, 1, 2019);	
	
	
	//creating a variable that holds the amount of years counted.
	
	int YearlyTempsize =YearlyTemps.size();
	//cout <<"stuff? = "<<YearlyTempsize<<endl;
	
	
	// graph stuff
	
	const int n=YearlyTempsize;
	double x[n],y[n];
	
	for(int i=0;i<YearlyTempsize;i++)
	{
		x[i]= Years[i];
		y[i]= YearlyTemps[i];
	}
	
	TGraph* graph = new TGraph(n,x,y);
	graph->SetLineColor(2);
	graph->SetLineWidth(2);
	graph->SetMarkerColor(4);
	graph->SetMarkerStyle(20);
	graph->SetTitle("Average temperature per year from 1961 to 2015");
	graph->GetXaxis()->SetTitle("Year");
	graph->GetYaxis()->SetTitle("Temperature( #circC) ");
	graph->GetXaxis()->CenterTitle(true);
	graph->GetYaxis()->CenterTitle(true);
	TCanvas *c1 = new TCanvas("Other stuff"," Yearly Averages ",200,10,700,500);
	c1->SetGrid();
	graph->Draw();
	
	yearcounter=1961;
	//printing the vector containing the average temperature for each year.
	
	for (int i =0;i<YearlyTempsize;i++)
	{
		cout<<"The average temperature during "<<yearcounter << " was "<<YearlyTemps[i]<<" (C) " <<endl<<endl;
		yearcounter++;
		
	}
	
	return 0;
}
