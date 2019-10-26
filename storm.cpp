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
void findEvent(annual_storms**&, hash_table_entry**&, int, int, int);
int search(hash_table_entry**&, int, int);
int searchYear(hash_table_entry**&, int, int);
int getLinkedListSize(hash_table_entry**&, int);
int getFatalitySize(fatality_event*);
int findMax(int*, int);
int getCount(int*, int, int);
void maxHeapify(int*&, int, int);
void buildMaxHeap(int*&, int);
void findMaxFatality();

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
  for(int i = 0; i < tableSize; i++)
    hashTable[i] = NULL;

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
  string querie = "";
  int theEventId = 0;
  string word1 = "";
  string word2 = "";
  string word3 = "";
  string word4 = "";
  string word5 = "";

  cout << "How many queries? ";
  cin >> numQueries;
  getline(cin, querie);

  for(int i = 0; i < numQueries; i++)
    {
      //reading and parsing the querie
      getline(cin, querie);
      istringstream parser;
      parser.str(querie);
      parser >> word1;
      parser >> word2;

      //find event querie
      if(word1 == "find" && word2 == "event")
	{
	  parser >> word3;
	  theEventId = stoi(word3);
      
	  findEvent(annualStorms, hashTable, theEventId, tableSize, numYears);
	}

      //find max queries      
      else if(word1 == "find" && word2 == "max")
	{
	  parser >> word3;

	  //find max fatality
	  if(word3 == "fatality")
	    {
	      parser >> word4;
	      parser >> word5;
	      int fatalityNum = stoi(word4);
	      int year = stoi(word5);
	    }

	  //find max
	  else
	    {

	    }
	}
    }

  //printing out summary of hash table
  int* chainLengths = new int[tableSize];
  for(int i = 0; i < tableSize; i++)
    {
      chainLengths[i] = getLinkedListSize(hashTable, i);
    }

  cout << "Chain Length(N)\t\tNumber of Chains with Length N" << endl;
  int max = findMax(chainLengths, tableSize);
  for(int i = 0; i <= max; i++)
    cout << i << "\t\t\t" << getCount(chainLengths, i, tableSize) << endl;

  float loadFactor = (float)totalNumEvents/(float)tableSize;
  cout << "Load Factor: " << loadFactor << endl;
  return 0;
}

//returns file size
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

      int eventIndex = search(hashTable, event->event_id, tableSize);

      fatality_event* head = stormEvents[eventIndex]->f;

      if(head == NULL)
	event->next = NULL;

      else
	event->next = head;
       
      stormEvents[eventIndex]->f = event;

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
      if(token.length() == 0)
	{
	  event->year = -1;
	  entry->year = -1;
	}
      else
	{
	  event->year = stoi(token);
	  entry->year = stoi(token);
	}

      getline(iss, token, ',');
      event->month_name = token;

      getline(iss, token, ',');
      event->event_type = token;

      getline(iss, token, ',');
      event->cz_type = token[0];

      getline(iss, token, ',');
      event->cz_name = token;

      getline(iss, token, ',');
      if(token.length() == 0)
	event->injuries_direct = -1;
      else
	event->injuries_direct = stoi(token);

      getline(iss, token, ',');
      if(token.length() == 0)
	event->injuries_indirect = -1;
      else
	event->injuries_indirect = stoi(token);

      getline(iss, token, ',');
      if(token.length() == 0)
	event->deaths_direct = -1;
      else
	event->deaths_direct = stoi(token);

      getline(iss, token, ',');
      if(token.length() == 0)
	event->deaths_indirect = -1;
      else
	event->deaths_indirect =  stoi(token);

      getline(iss, token, ',');
      if(token.length() == 0)
	event->damage_property = -1;
      else
	event->damage_property = numberfy(token);

      getline(iss, token, ',');
      if(token.length() == 0)
	event->damage_crops = -1;
      else
	event->damage_crops = numberfy(token);

      getline(iss, token, ',');
      event->tor_f_scale = token;

      event->f = NULL;
      entry->event_index = i;
      entry->next = NULL;

      //insert event into array of storm events and hash table
      stormEvents[i] = event;
      insertHashedEvent(hashTable, entry, tableSize);      


      i++;
    }
}

//turns K or M into 1000 or 1000000
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

//tests if number is prime
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

//inserts event into hash table
void insertHashedEvent(hash_table_entry** &hashTable, hash_table_entry* &entry, int tableSize)
{
  int hash = entry->event_id % tableSize;
  hash_table_entry* head = hashTable[hash];

  if(head == NULL)
    entry->next = NULL;

  else
    {
      entry->next = head;
    }
  hashTable[hash] = entry;
}

//prints out event if found
void findEvent(annual_storms**& annualStorms, hash_table_entry**& hashTable, int eventId, int tableSize, int numYears)
{
  storm_event* event = new storm_event;

  int eventIndex = search(hashTable, eventId, tableSize);
  int year = searchYear(hashTable, eventId, tableSize);

  //sets event pointer to correct event
  if(eventIndex != -1)
    {
      for(int i = 0; i < numYears; i++)
	{
	  if(annualStorms[i]->year == year)
	    {
	      if(annualStorms[i]->events[eventIndex]->event_id == eventId)
		{
		  event = annualStorms[i]->events[eventIndex];
		  i = numYears;
		}
	    }
	}

      //prints out attributes
      cout << "Event ID: " << event->event_id << endl;
      cout << "State: " << event->state << endl;
      cout << "Year: " << event->year << endl;
      cout << "Month: " << event->month_name << endl;
      cout << "Event Type: " << event->event_type << endl;
      cout << "CZ Type: " << event->cz_type << endl;
      cout << "CZ Name: " << event->cz_name << endl;
      cout << "Direct Injuries: " << event->injuries_direct << endl;
      cout << "Indirect Injuries: " << event->injuries_indirect << endl;
      cout << "Direct Deaths: " << event->deaths_direct << endl;
      cout << "Indirect Deaths: " << event->deaths_indirect << endl;
      cout << "Property Damage: $" << event->damage_property << endl;
      cout << "Crop Damage: $" << event->damage_crops << endl;
      cout << "Tornado Fujita Scale: " << event->tor_f_scale << endl << endl;

      fatality_event* current = event->f;
      int fatalityCount = getFatalitySize(current);

      if(current != NULL)
	{ 
	  cout << "Fatality Number: " << fatalityCount << endl;
	  while(current != NULL)
	    {
	      cout << "Fatality ID: " << current->fatality_id << endl;
	      cout << "Event ID: " << current->event_id << endl;
	      cout << "Fatality Type: " << current->fatality_type << endl;
	      cout << "Fatality Date: " << current->fatality_date << endl;
	      cout << "Fatality Age: " << current->fatality_age << endl;
	      cout << "Fatality Sex: " << current->fatality_sex << endl;
	      cout << "Fatality Location: " << current->fatality_location << endl << endl;
	      current = current->next;
	    } 
	  
	}
      else
	cout << "No fatalities" << endl << endl;
    }
  else
    cout << "Event ID not found" << endl << endl;

}

//search for hash table entry
int search(hash_table_entry**& hashTable, int eventId, int tableSize)
{
  int foundKey = -1;
  int hash = eventId % tableSize;
  hash_table_entry* entry = hashTable[hash];

  while(entry != NULL && foundKey == -1)
    {
      if(entry->event_id == eventId)
	foundKey = entry->event_index;
      else
	entry = entry->next;
    }
  return foundKey;
}

//returns year of the event
int searchYear(hash_table_entry**& hashTable, int eventId, int tableSize)
{
  int year = -1;
  int hash = eventId % tableSize;
  hash_table_entry* entry = hashTable[hash];

  while(entry != NULL && year == -1)
    {
      if(entry->event_id == eventId)
	year = entry->year;
      else
	entry = entry->next;
    }
  return year;
}

//returns the size of a single chain in hash table
int getLinkedListSize(hash_table_entry**& hashTable, int index)
{
  int count = 0;
  hash_table_entry* current = hashTable[index];
  while(current != NULL)
    {
      count++;
      current = current->next;
    }
  return count;
}

//returns size of fatality linked list
int getFatalitySize(fatality_event* current)
{
  int count = 0;
  while(current != NULL)
    {
      count++;
      current = current->next;
    }
  return count;
}

//returns maximum chain length
int findMax(int* array, int size)
{
  int max = 0;
  for(int i = 0; i < size; i++)
    {
      if(array[i] > max)
	max = array[i];
    }
  return max;
}

//returns the count of each chain length
int getCount(int* array, int target, int size)
{
  int count = 0;
  for(int i = 0; i < size; i++)
    {
      if(array[i] == target)
	count++;
    }
  return count;
}

//maintains the max heap property of the heap
void maxHeapify(int*& array, int i, int n)
{
  int largest = 0;
  int temp = 0;
  int left = 2 * i + 1; //position of left child
  int right  = 2 * i + 2; //position of right child

  if(left <= n && array[left] > array[right])
    largest = left;
  else
    largest = i;

  if(right <= n && array[right] > array[largest])
    largest = right;

  if(largest != i)
    {
      temp = array[i];
      array[i] = array[largest];
      array[largest] = temp;
      maxHeapify(array, largest, n);
    }
}

//constructs a max heap using maxHeapify
void buildMaxHeap(int*& array, int n)
{
  for(int i = n/2 - 1; i >= 0; i--)
    maxHeapify(array, i, n);
}
