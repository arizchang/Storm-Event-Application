#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "defn.h"

int numberfy(string);

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

  if(!input.is_open())
    exit(EXIT_FAILURE);

  string str;
  int lineCount = 0;
  getline(input, str); //skip first line
  while(getline(input, str))
    {
      istringstream iss(str);
      string token;
      while(getline(iss, token, ','))
	{
	  //cout << token << " ";
	}
      lineCount++;
      //cout << endl;
    }
  //cout << "Line count: " << lineCount << endl;

  ifstream data(file);

  storm_event** stormEvents = new storm_event*[lineCount];
  int i = 0;
  getline(data, str); //skip first line
  while(getline(data, str))
    {
      istringstream iss(str);
      string token;
      storm_event* event = new storm_event;

      getline(iss, token, ',');
      event->event_id = stoi(token);
      getline(iss, token, ',');
      event->state = token;
      getline(iss, token, ',');
      event->year = stoi(token);
      getline(iss, token, ',');
      event->month_name = token;
      getline(iss, token, ',');
      event->event_type = token;
      getline(iss, token, ',');
      event->cz_type = token[0];
      getline(iss, token, ',');
      event->cz_name = token;
      getline(iss, token, ',');
      event->injuries_direct = stoi(token);
      getline(iss, token, ',');
      event->injuries_indirect = stoi(token);
      getline(iss, token, ',');
      event->deaths_direct = stoi(token);
      getline(iss, token, ',');
      event->deaths_indirect =  stoi(token);
      getline(iss, token, ',');
      event->damage_property = numberfy(token);
      getline(iss, token, ',');
      event->damage_crops = numberfy(token);
      getline(iss, token, ',');
      event->tor_f_scale = token;

      stormEvents[i] = event;
      i++;
    }

  for(int i = 0; i < lineCount; i++)
    {
      cout << stormEvents[i]->event_id << " " << stormEvents[i]->state << " " << stormEvents[i]->damage_property << endl;
    }

  return 0;
}

int numberfy(string str)
{
  float num = 0;
  if(str[str.length()-1] == 'K')
    {
      str.pop_back();
      num = stof(str);
      num = num*1000;
    }
  else if(str[str.length()-1] == 'M')
    {
      str.pop_back();
      num = stof(str);
      num = num*1000000;
    }
  return (int)num;
}
