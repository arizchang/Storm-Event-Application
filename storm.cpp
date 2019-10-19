#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "defn.h"

//functions used
int numberfy(string);
bool testForPrime(int);
int hashTableSize(int);
void insertHashedEvent(hash_table_entry**, hash_table_entry*, int);

using namespace std;

int main(int argc, char** argv)
{
  string* years = new string[3];
  string file = "";
  int numYears = stoi(argv[1]);
  annual_storms** annualStorms = new annual_storms*[numYears];

  //filling vector of years
  for(int i = 0; i < numYears; i++)
    years[i] = argv[i+2];

  if(years[0] == "1950")
    file = "details-1950.csv";
  else if(years[0] == "1951")
    file = "details-1951.csv";
  else if(years[0] == "1952")
    file = "details-1952.csv";
  else
    cout << "Invalid input" << endl;

  //file reading
  ifstream input(file);

  if(!input.is_open())
    exit(EXIT_FAILURE);

  //determining size of file
  string str;
  int lineCount = 0;
  getline(input, str); //skip first line
  while(getline(input, str))
    lineCount++;
   
  //initializing hash table
  int tableSize = 0;
  cout << hashTableSize(lineCount) << endl;
  tableSize = hashTableSize(lineCount);
  hash_table_entry** hashTable = new hash_table_entry*[hashTableSize(lineCount)];

  //filling in array of storm_event structs
  ifstream data(file);
  storm_event** stormEvents = new storm_event*[lineCount];
  int i = 0;
  getline(data, str); //skip first line
  while(getline(data, str))
    {
      istringstream iss(str);
      string token;
      storm_event* event = new storm_event; //new storm_event struct
      hash_table_entry* entry = new hash_table_entry; //new hash_table_entry struct

      //filling in a struct of storm_event and hash_table_entry
      getline(iss, token, ',');
      event->event_id = stoi(token);
      entry->event_id = stoi(token);

      getline(iss, token, ',');
      event->state = token;

      getline(iss, token, ',');
      event->year = stoi(token);
      entry->year = stoi(token);

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

      entry->event_index = i;

      stormEvents[i] = event;
      hashTable[entry->event_id % tableSize] = entry;


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

bool testForPrime(int val)
{
  int limit, factor = 2;

  limit = (long)( sqrtf( (float) val ) + 0.5f );
  while( (factor <= limit) && (val % factor) )
    factor++;

  return( factor > limit );
}

//determines hash table size
int hashTableSize(int num)
{
  num = 2*num;
  while(!testForPrime(num))
    num++;
  return num;
}

void insertHashedEvent(hash_table_entry** &hashTable, hash_table_entry* &entry, int tableSize)
{
  int hash = entry->event_id % tableSize;

  if(hashTable[hash] == NULL)
    hashTable[hash] = entry;

  else
    {
      entry->next = hashTable[hash];
      hashTable[hash] = entry;
    }
}
