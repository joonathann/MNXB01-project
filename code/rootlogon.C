void rootlogon() {
  gStyle->SetOptStat(0); //Let's make our plots look a bit better. Get rid of the stat box
  gStyle->SetOptTitle(0); //Get rid of the title (good plots use legends instead)
  gStyle->SetTitleSize(0.05, "x"); //Use bigger text on the axes
  gStyle->SetTitleSize(0.05, "y");
  gStyle->SetLabelSize(0.05, "x"); //Use bigger labels too
  gStyle->SetLabelSize(0.05, "y");
  gStyle->SetPadTopMargin(0.05); //Change the margins to fit our new sizes
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);
  
  gROOT->ProcessLine(".L tempTrender.cpp+"); //Load the classes that we want to use automatically - Convenient!
  gROOT->ProcessLine(".L project.cpp+"); //The + means to recompile only if it changed sine last time
  //Now you can type project() to invoke the function defined in project
  //You can also create and invoke functions in a temptrender manually by doing e.g. tempTrender t(pathToFile); t.hotCold();
}
