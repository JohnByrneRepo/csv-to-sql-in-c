#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sqlite3.h"
#include <assert.h>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

int main(void) {
   const char filename[] = "test.csv";
   FILE *file = fopen(filename, "r");
   if (file) {
      size_t i;
      char buffer[BUFSIZ];
      char headings[100][100];
      char values[100][100];
	  int idx = 0;
	  int numberOfColumns = 0;
	  const char *delimiter = ",";
	  const char *empty = " ";
      for (i = 0; fgets(buffer, sizeof buffer, file); ++i) {
    	  if (i == 0) {
			  printf("Headings: ");
			  printf(buffer);
			  printf("\n");
			  char* s;
			  for (s=&buffer[0]; *s != '\0'; s++) {
				  idx++;
				  printf("%c\n", *s);
				  if(!strchr(delimiter, *s)) {
					  headings[numberOfColumns][idx] = *s;
				  } else {
					  printf("comma found\n");
					  printf("heading %i:", numberOfColumns + 1);
					  for (int x = 0; x < idx; x++) {
						  printf("%c", headings[numberOfColumns][x]);
					  }
					  numberOfColumns++;
					  idx=0;
				  }
			  }
    	  } else if (i == 1) {

			  // Get value types from 2nd row
			  // for table column types

			  printf("Values: ");
			  printf(buffer);
			  printf("\n");
			  char* s;
//			  int fieldTypes[numberOfColumns];
			  idx = 0;
			  numberOfColumns = 0;
			  int columnTypes[numberOfColumns];
			  for (s=&buffer[0]; *s != '\0'; s++) {
				  idx++;
				  printf("%c\n", *s);
				  if(!strchr(delimiter, *s)) {
					  values[numberOfColumns][idx] = *s;
				  } else {
					  printf("comma found\n");
					  printf("value %i:", numberOfColumns + 1);
					  for (int x = 0; x < idx; x++) {
						  printf("%c", values[numberOfColumns][x]);
					  }
					  printf("\n");
					  numberOfColumns++;
					  idx=0;
				  }
			  }

			  // Trim the value strings and
			  // populate columnTypes
			  //
			  //


			  // We have all the headings stored in
			  // char arrays, time to create the table

			  // example: CREATE TABLE IF NOT EXISTS TTC (id INTEGER PRIMARY KEY,
			  // l3_sales INTEGER, Branch_Code INTEGER, Store_name TEXT)"

			  char tableCreate[5000] = "CREATE TABLE IF NOT EXISTS DATA (";

			  printf("Headings:\n");
			  for (int z = 0; z < numberOfColumns + 1; z++) {
				  int lspace = 0, rspace = 0;
				  printf("Heading number %i\n" , z + 1);
				  for (int s=0; s<10; s++) {
					  if (strchr(empty, headings[z][s])) {
						  lspace++;
					  } else {
						  break;
					  }
				  }
				  for (int rs=99; rs>0; rs--) {
					  if (strchr(empty, headings[z][rs])) {
						  rspace++;
					  } else {
						  break;
					  }
				  }
				  printf("Space left:%i\n" , lspace);
				  printf("Space right:%i\n" , rspace);
				  int length = (100 - rspace) - lspace + 2;
				  if (z == numberOfColumns) {
					  length--; // for the newline
				  }
				  char trimmed[length];
				  int idx = 0;
				  for (int t = lspace; t < length; t++) {
					  trimmed[idx] = headings[z][t];
					  idx++;
				  }
				  trimmed[idx] = '\0';
				  printf("Trimmed heading: %s\n", trimmed);
				  printf("Trimmed heading length: %i", strlen(trimmed));
				  printf("\n");
				  printf("Checking if numeric..\n");
				  printf("\n\n");
				  int text = 0;

				  // Test against columnTypes, not headings
				  for (int ii = 0; ii < strlen(trimmed); ii++) {
					  if (!isdigit(trimmed[ii])) {
						  text = 1;
						  break;
					  }
				  }
				  if (text == 0) {
					  char type[] = " INTEGER, ";
					  char col[] = "";
					  strcat(trimmed, type);
					  strcat(tableCreate, trimmed);
				  } else {
					  char type[] = " TEXT, ";
					  char col[] = "";
					  strcat(trimmed, type);
					  strcat(tableCreate, trimmed);
				  }
			  }
			  printf("Table creation string: %s\n", tableCreate);
		  } else if (i == 2) {
			  printf("Row ");
			  printf("%i: ", i);
			  printf(buffer);
			  printf("\n");
		  }
      }
      fclose(file);
      std::cin.get();
   }
 }
