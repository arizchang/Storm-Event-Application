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
void maxHeapifyFatality(storm_event**&, int, int);
void buildMaxHeapFatality(storm_event**&, int);
void heapSortFatality(storm_event**&, int);
void findMaxFatality(storm_event**&, int, string, int);
void maxHeapifyPropertyDamage(storm_event**&, int, int);
void buildMaxHeapPropertyDamage(storm_event**&, int);
void heapSortPropertyDamage(storm_event**&, int);
void findMaxPropertyDamage(storm_event**&, int, string, int);
void maxHeapifyCropDamage(storm_event**&, int, int);
void buildMaxHeapCropDamage(storm_event**&, int);
void heapSortCropDamage(storm_event**&, int);
void findMaxCropDamage(storm_event**&, int, int, int);

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
      aYearOfStorms->numStorms = detailsLineCount;
      aYearOfStorms->events = stormEvents;
      annualStorms[i] = aYearOfStorms;
    }

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
	  parser >> word3; //event id
	  theEventId = stoi(word3);
      
	  findEvent(annualStorms, hashTable, theEventId, tableSize, numYears);
	}

      //find max queries      
      else if(word1 == "find" && word2 == "max")
	{
	  parser >> word3;
	  int numStorms = 0;

	  //find max fatality
	  if(word3 == "fatality")
	    {
	      parser >> word4; //number
	      parser >> word5; //year
	      int fatalityNum = stoi(word4);

	      //"all" is entered
	      if(word5 == "all")
		{
		  //determining total number of storm events among all years
		  for(int i = 0; i < numYears; i++)
		    numStorms += annualStorms[i]->numStorms;

		  storm_event** newEvents = new storm_event*[numStorms];

		  //copying storm events into new array
		  int count = 0;
		  for(int i = 0; i < numYears; i++)
		    {
		      for(int j = 0; j < annualStorms[i]->numStorms; j++)
			{
			  newEvents[count] = annualStorms[i]->events[j];
			  count++;
			}
		    }

		  string year = "all years";
		  heapSortFatality(newEvents, numStorms);
		  findMaxFatality(newEvents, fatalityNum, year, numStorms);
		  delete newEvents;
		}

	      //a year is entered
	      else
		{
		  int year = stoi(word5);

		  for(int i = 0; i < numYears; i++)
		    {
		      if(annualStorms[i]->year == year)
			{
			  storm_event** newEvents = new storm_event*[annualStorms[i]->numStorms]; //new array of storm events to be max heapified
			  numStorms = annualStorms[i]->numStorms;

			  //copying events over to new array
			  for(int j = 0; j < numStorms; j++)
			    newEvents[j] = annualStorms[i]->events[j];

			  heapSortFatality(newEvents, numStorms);
			  findMaxFatality(newEvents, fatalityNum, word5, numStorms);
			  delete newEvents;

			  i = numYears;
			} 

		      else if(i == numYears - 1)
			cout << "No data for that year" << endl;
		    }
		}

	      //print count of nodes in max heap and height
	      int height = (int)(log2 (numStorms));
	      int rightHeight = height - 1;
	      int leftHeight = height - 1;
	      if(numStorms < (pow(2, height+1) - 1 - pow(2, height)/2))
		rightHeight = height - 2;

	      cout << "Total number of nodes in max-heap: " << numStorms << endl;
	      cout << "Height of tree: " << height << endl;
	      cout << "Height of left subtree: " << leftHeight << endl;
	      cout << "Height of right subtree: " << rightHeight << endl << endl;
	    }

	  //find max
	  else
	    {
	      parser >> word4;
	      parser >> word5;

	      int topNum = stoi(word3);
	      string damageType = word5;

	      //top property damage amounts
	      if(damageType == "damage_property")
		{
		  //all years
		  if(word4 == "all")
		    {
		      //determining total number of storm events among all years
		      for(int i = 0; i < numYears; i++)
			numStorms += annualStorms[i]->numStorms;

		      storm_event** newEvents = new storm_event*[numStorms];

		      //copying storm events into new array
		      int count = 0;
		      for(int i = 0; i < numYears; i++)
			{
			  for(int j = 0; j < annualStorms[i]->numStorms; j++)
			    {
			      newEvents[count] = annualStorms[i]->events[j];
			      count++;
			    }
			}

		      string year = "all years";
		      heapSortPropertyDamage(newEvents, numStorms);
		      findMaxPropertyDamage(newEvents, topNum, year, numStorms);
		      delete newEvents;
		    }
		  
		  //a specific year is chosen
		  else
		    {
		      int year = stoi(word4);
		      for(int i = 0; i < numYears; i++)
			{
			  if(annualStorms[i]->year == year)
			    {
			      storm_event** newEvents = new storm_event*[annualStorms[i]->numStorms]; //new array of storm events to be max heapified
			      numStorms = annualStorms[i]->numStorms;
		      
			      //copying events over to new array
			      for(int j = 0; j < numStorms; j++)
				newEvents[j] = annualStorms[i]->events[j];


			      heapSortPropertyDamage(newEvents, numStorms);
			      findMaxPropertyDamage(newEvents, topNum, word4, numStorms);
			      delete newEvents;

			      i = numYears;
			    } 

			  else if(i == numYears - 1)
			    cout << "No data for that year" << endl;
			}
		    }
		}
	      
	      //top crop damage amounts
	      else if(damageType == "damage_crops")
		{
		  int year = stoi(word4);
		  for(int i = 0; i < numYears; i++)
		    {
		      if(annualStorms[i]->year == year)
			{
			  storm_event** newEvents = new storm_event*[annualStorms[i]->numStorms]; //new array of storm events to be max heapified
		      
			  //copying events over to new array
			  for(int j = 0; j < annualStorms[i]->numStorms; j++)
			    newEvents[j] = annualStorms[i]->events[j];

			  heapSortCropDamage(newEvents, annualStorms[i]->numStorms);
			  findMaxCropDamage(newEvents, topNum, year, annualStorms[i]->numStorms);
			  delete newEvents;

			  i = numYears;
			} 

		      else if(i == numYears - 1)
			cout << "No data for that year" << endl;
		    }
		}
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
void maxHeapifyFatality(storm_event**& array, int i, int n)
{
  int largest = i;
  storm_event* temp = NULL;
  int left = 2 * i + 1; //position of left child
  int right  = 2 * i + 2; //position of right child

  //determines if parent has larger children
  if(left < n && getFatalitySize(array[left]->f) > getFatalitySize(array[largest]->f))
    largest = left;
  else
    largest = i;

  if(right < n && getFatalitySize(array[right]->f) > getFatalitySize(array[largest]->f))
    largest = right;

  //swaps parent with child
  if(largest != i)
    {
      temp = array[i];
      array[i] = array[largest];
      array[largest] = temp;
      maxHeapifyFatality(array, largest, n);
    }
}

//constructs a max heap using maxHeapify
void buildMaxHeapFatality(storm_event**& array, int n)
{
  for(int i = n/2 - 1; i >= 0; i--)
    maxHeapifyFatality(array, i, n);
}

//prints out the n most fatal storm events in that year
void findMaxFatality(storm_event**& array, int num, string year, int size)
{
  cout << num << " of the most fatal storms in " << year << ": " << endl << endl;

  for(int i = size-1; i >= size-num; i--)
    {
      cout << "Event ID: " << array[i]->event_id << endl;
      cout << "Number of fatalities: " << getFatalitySize(array[i]->f) << endl << endl;
    }
}

//heapsort
void heapSortFatality(storm_event**& array, int n)
{
  storm_event* temp = NULL;

  buildMaxHeapFatality(array, n); //builds max heap

  //continuously takes current max element and moves it to proper place
  for(int i = n-1; i >= 0; i--)
    {
      temp = array[0];
      array[0] = array[i];
      array[i] = temp;
      maxHeapifyFatality(array, 0, i);
    }
}

//maintains the max heap property of the heap
void maxHeapifyPropertyDamage(storm_event**& array, int i, int n)
{
  int largest = i;
  storm_event* temp = NULL;
  int left = 2 * i + 1; //position of left child
  int right  = 2 * i + 2; //position of right child

  //determines if parent has larger children
  if(left < n && array[left]->damage_property > array[largest]->damage_property)
    largest = left;
  else
    largest = i;

  if(right < n && array[right]->damage_property > array[largest]->damage_property)
    largest = right;

  //swaps parent with child
  if(largest != i)
    {
      temp = array[i];
      array[i] = array[largest];
      array[largest] = temp;
      maxHeapifyPropertyDamage(array, largest, n);
    }
}

//constructs a max heap using maxHeapify
void buildMaxHeapPropertyDamage(storm_event**& array, int n)
{
  for(int i = n/2 - 1; i >= 0; i--)
    maxHeapifyPropertyDamage(array, i, n);
}

//prints out n most damaging storm events to property
void findMaxPropertyDamage(storm_event**& array, int num, string year, int size)
{
  cout << num << " of the most costly damages to property in " << year << ": " << endl << endl;

  for(int i = size-1; i >= size-num; i--)
    {
      cout << "Event ID: " << array[i]->event_id << endl;
      cout << "Event Type: " << array[i]->event_type << endl;
      cout << "Damage to Property: $" << array[i]->damage_property << endl << endl;
    }
}

//heapsort
void heapSortPropertyDamage(storm_event**& array, int n)
{
  storm_event* temp = NULL;

  buildMaxHeapPropertyDamage(array, n); //builds max heap

  //contiuously moves the current max element in the proper place
  for(int i = n-1; i >= 0; i--)
    {
      temp = array[0];
      array[0] = array[i];
      array[i] = temp;
      maxHeapifyPropertyDamage(array, 0, i);
    }
}

//maintains the max heap property of the heap
void maxHeapifyCropDamage(storm_event**& array, int i, int n)
{
  int largest = i;
  storm_event* temp = NULL;
  int left = 2 * i + 1; //position of left child
  int right  = 2 * i + 2; //position of right child

  //determines if parent has larger children
  if(left < n && array[left]->damage_crops > array[largest]->damage_crops)
    largest = left;
  else
    largest = i;

  if(right < n && array[right]->damage_crops > array[largest]->damage_crops)
    largest = right;

  //swaps parent with child
  if(largest != i)
    {
      temp = array[i];
      array[i] = array[largest];
      array[largest] = temp;
      maxHeapifyCropDamage(array, largest, n);
    }
}

//constructs a max heap using maxHeapify
void buildMaxHeapCropDamage(storm_event**& array, int n)
{
  for(int i = n/2 - 1; i >= 0; i--)
    maxHeapifyCropDamage(array, i, n);
}

//prints out n most damaging storm events to crops
void findMaxCropDamage(storm_event**& array, int num, int year, int size)
{
  cout << num << " of the most costly damages to property in " << year << ": " << endl << endl;

  for(int i = size-1; i >= size-num; i--)
    {
      cout << "Event ID: " << array[i]->event_id << endl;
      cout << "Event Type: " << array[i]->event_type << endl;
      cout << "Damage to Crops: $" << array[i]->damage_crops << endl << endl;
    }
}

//heapsort
void heapSortCropDamage(storm_event**& array, int n)
{
  storm_event* temp = NULL;

  buildMaxHeapCropDamage(array, n); //builds max heap

  //contiuously moves the current max element in the proper place
  for(int i = n-1; i >= 0; i--)
    {
      temp = array[0];
      array[0] = array[i];
      array[i] = temp;
      maxHeapifyCropDamage(array, 0, i);
    }
}
