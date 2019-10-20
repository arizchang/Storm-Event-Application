#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "defn.h"

//functions used
int getFileSize(string);
void readDetailsFile(hash_table_entry**&, storm_event**&, string, int);
void readFatalitiesFile(hash_table_entry**&, storm_event**&, string, int);
int numberfy(string);
bool testForPrime(int);
int hashTableSize(int);
void insertHashedEvent(hash_table_entry**&, hash_table_entry*&, int);
storm_event* findEvent(annual_storms**&, hash_table_entry**&, int, int, int);

using namespace std;

int main(int argc, char** argv)
{
  int numYears = stoi(argv[1]);
  string* years = new string[numYears];
  string detailsFile = "";
  string fatalitiesFile = "";
  annual_storms** annualStorms = new annual_storms*[numYears];
  int totalNumEvents = 0;

  //filling array of years
  for(int i = 0; i < numYears; i++)
    years[i] = argv[i+2];

  //obtaining total number of storm events for hash table
  for(int i = 0; i < numYears; i++)
    {
      detailsFile = "details-" + years[i] + ".csv";

      //couting number of events in each file
      int detailsLineCount = getFileSize(detailsFile);
      totalNumEvents += detailsLineCount;
    }

  //initializing hash table
  int tableSize = hashTableSize(totalNumEvents);
  hash_table_entry** hashTable = new hash_table_entry*[tableSize];

  //filling in arrays of storm events
  for(int i = 0; i < numYears; i++)
    {
      detailsFile = "details-" + years[i] + ".csv";
      fatalitiesFile = "fatalities-" + years[i] + ".csv";

      //initializing array of storm_event
      int detailsLineCount = getFileSize(detailsFile);
      storm_event** stormEvents = new storm_event*[detailsLineCount];

      //read files
      readDetailsFile(hashTable, stormEvents, detailsFile, tableSize);
      readFatalitiesFile(hashTable, stormEvents, fatalitiesFile, tableSize);

      //making new annual_storms struct
      annual_storms* aYearOfStorms = new annual_storms;
      aYearOfStorms->year = stoi(years[i]);
      aYearOfStorms->events = stormEvents;
      annualStorms[i] = aYearOfStorms;
    }

  //cout << annualStorms[0]->events[hashTable[10120406 % tableSize]->event_index]->state << endl;
  
  //beginning of queries
  int numQueries = 0;
  int querie = 0;
  cout << "How many queries? " << endl;
  cin >> numQueries;

  for(int i = 0; i < numQueries; i++)
    {
      cin >> querie;
      //int eventId = stoi(querie);
      
      storm_event* event = findEvent(annualStorms, hashTable, querie, tableSize, numYears);
      cout << event->state << endl;
    }

  return 0;
}

int getFileSize(string file)
{
  ifstream input(file);

  if(!input.is_open())
    exit(EXIT_FAILURE);

  //determining size of file
  string str;
  int lineCount = 0;
  getline(input, str); //skip first line
  while(getline(input, str))
    lineCount++;

  return lineCount;
}

void readFatalitiesFile(hash_table_entry**& hashTable, storm_event**& stormEvents, string file, int tableSize)
{
  //filling in array of storm_event structs
  ifstream data(file);
  string str;
  int i = 0;
  getline(data, str); //skip first line
  while(getline(data, str))
    {
      istringstream iss(str);
      string token;
      fatality_event* event = new fatality_event; //new storm_event struct
      hash_table_entry* entry = new hash_table_entry; //new hash_table_entry struct

      //filling in a struct of storm_event and hash_table_entry
      getline(iss, token, ',');
      event->fatality_id = stoi(token);

      getline(iss, token, ',');
      event->event_id = stoi(token);

      getline(iss, token, ',');
      event->fatality_type = token[0];

      getline(iss, token, ',');
      event->fatality_date = token;

      getline(iss, token, ',');
      if(token.length() == 0)
	event->fatality_age = -1;
      else
	event->fatality_age = stoi(token);

      getline(iss, token, ',');
      event->fatality_sex = token[0];

      getline(iss, token, ',');
      event->fatality_location = token;

      stormEvents[hashTable[event->event_id % tableSize]->event_index]->f = event;

      //insert event into array of storm events and hash table
      //stormEvents[i] = event;
      //insertHashedEvent(hashTable, entry, tableSize);      


      i++;
    }
}

void readDetailsFile(hash_table_entry**& hashTable, storm_event**& stormEvents, string file, int tableSize)
{
  //linking fatalities to respective storm events
  ifstream data(file);
  string str;
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

      event->f = NULL;
      entry->event_index = i;

      //insert event into array of storm events and hash table
      stormEvents[i] = event;
      insertHashedEvent(hashTable, entry, tableSize);      


      i++;
    }
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

storm_event* findEvent(annual_storms**& annualStorms, hash_table_entry**& hashTable, int eventId, int tableSize, int numYears)
{
  int hash = eventId % tableSize;
  storm_event* event = new storm_event;

  for(int i = 0; i < numYears; i++)
    {
      if(annualStorms[i]->events[hashTable[hash]->event_index]->event_id == eventId)
	return annualStorms[i]->events[hashTable[hash]->event_index];
    }
  return event;
}
