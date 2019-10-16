#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "defn.h"

using namespace std;

int main(int argc, char** argv)
{
  vector<annual_storms> annualStorms;
  string* years = new string[3];
  string file = "";
  int numYears = stoi(argv[1]);

  //filling vector of years
  for(int i = 0; i < numYears; i++)
    years[i] = argv[i+2];

  if(years[0] == "1950")
    file = "details-1950.csv";
  else if(years[0] == "1951")
    file = "details-1951.csv";
  else if(years[0] == "1952")
    file = "details-1951.csv";
  else
    cout << "Invalid input" << endl;

  //file reading
  ifstream input(file);

  int eventId;
  string state;
  int year;
  string month_name;
  string event_type;
  char cz_type;
  string cz_name;
  int injuries_direct;
  int injuries_indirect;
  int deaths_direct;
  int deaths_indirect;
  int damage_property;
  int damage_crops;
  string tor_f_scale;

  if(!input.is_open())
    exit(EXIT_FAILURE);

  string str;
  getline(input, str); //skip first line
  while(getline(input, str))
    {
      istringstream iss(str);
      string token;
      while(getline(iss, token, ','))
	cout << token << " ";
    }
  cout << endl;

  return 0;
}
