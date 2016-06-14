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
#define BUFFER_SIZE 256
sqlite3 * db;
char * sErrMsg = 0;
char * tail = 0;
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
//				  printf("%c\n", *s);
				  if(!strchr(delimiter, *s)) {
					  values[numberOfColumns][idx] = *s;
					  printf("%c\n", *s);
				  } else {
					  printf("comma found\n");
					  printf("value %i:", numberOfColumns + 1);
					  for (int x = 0; x < idx; x++) {
//						  printf("%c", values[numberOfColumns][x]);
					  }
					  printf("\n");
					  numberOfColumns++;
					  idx=0;
				  }
			  }
//			  for (int x = 0; x < idx; x++) {
//				  printf("%c", values[numberOfColumns + 1][x]);
//			  }
//			  printf("\n");

			  // Trim the value strings and
			  // populate columnTypes

			  for (int z=0; z<numberOfColumns + 1; z++) {
				  int lspace = 0, rspace = 0;

				  for (int s=0; s<10; s++) {
					  if (strchr(empty, values[z][s])) {
						  lspace++;
					  } else {
						  break;
					  }
				  }
				  for (int rs=99; rs>0; rs--) {
					  if (strchr(empty, values[z][rs])) {
						  rspace++;
					  } else {
						  break;
					  }
				  }
				  printf("Space left:%i\n" , lspace);
				  printf("Space right:%i\n" , rspace);
				  int length = (100 - rspace) - lspace + 1;
				  if (z == numberOfColumns) {
					  length-=2; // for the newline
				  }
				  char trimmed[length];
				  int idx = 0;
				  for (int t = lspace; t < length; t++) {
					  trimmed[idx] = values[z][t];
					  idx++;
				  }
				  trimmed[idx] = '\0';
				  printf("Trimmed val: %s\n", trimmed);
				  printf("Trimmed val length: %i", strlen(trimmed));
				  printf("\n");
				  printf("Checking if numeric..\n");


				  // check if numeric
				  int text = 0;
				  for (int ii = 0; ii < length - 1; ii++) {
					  if (!isdigit(trimmed[ii])) {
						  text = 1;
						  break;
					  }
				  }
				  printf("Text? %i ", text);
				  columnTypes[z] = text;
				  printf("\n");
				  printf("\n");
			  }

			  // We have all the headings stored in
			  // char arrays, time to create the table

			  // example: CREATE TABLE IF NOT EXISTS TTC (id INTEGER PRIMARY KEY,
			  // l3_sales INTEGER, Branch_Code INTEGER, Store_name TEXT)"

			  char tableCreate[5000] = "CREATE TABLE DATA (";
//			  char tableCreate[5000] = "CREATE TABLE IF NOT EXISTS DATA (";

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

				  // Test against columnTypes, not headings
				  if (z == numberOfColumns) {
					  if (columnTypes[z] == 0) {
						  char type[] = " INTEGER)";
						  strcat(trimmed, type);
						  strcat(tableCreate, trimmed);
					  } else {
						  char type[] = " TEXT)";
						  strcat(trimmed, type);
						  strcat(tableCreate, trimmed);
					  }
				  } else {
					  if (columnTypes[z] == 0) {
						  char type[] = " INTEGER, ";
						  strcat(trimmed, type);
						  strcat(tableCreate, trimmed);
					  } else {
						  char type[] = " TEXT, ";
						  strcat(trimmed, type);
						  strcat(tableCreate, trimmed);
					  }
				  }
			  }
			  printf("Table creation string: %s\n", tableCreate);

		      char sSQL [BUFFER_SIZE] = "\0";

			  char DATABASE[] = "test.sqlite";
//			  char TABLE[] = "CREATE TABLE data (id INTEGER PRIMARY KEY, colname TEXT)";
			  char TABLE[] = "CREATE TABLE data (id INTEGER PRIMARY KEY, colname TEXT)";
//			  char TABLE[] = "CREATE TABLE IF NOT EXISTS data (id INTEGER PRIMARY KEY, colname TEXT)";
			  sqlite3_open(DATABASE, &db);

//			  sqlite3_exec(db, TABLE, NULL, NULL, &sErrMsg);
			  sqlite3_exec(db, tableCreate, NULL, NULL, &sErrMsg);
			  sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
			  sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);

//			  cStartClock = clock();
			  sqlite3_stmt *stmt;                                                                         /* 1 */

			  sprintf(sSQL, "INSERT INTO data VALUES (NULL, @AB, @CD)");
			  sqlite3_prepare_v2(db,  sSQL, 256, &stmt, NULL); //&tail);

			  sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);

//			  char * sAB = 0;
//			  char * sCD = 0;

			  char * sAB = 0;
			  char * sCD = 0;

//			  for (int iii=0; iii<numberOfColumns; iii++) {
				  // Insert field loop
				  sqlite3_bind_text(stmt, 1, sAB, -1, SQLITE_TRANSIENT);
				  sqlite3_bind_text(stmt, 2, sCD, -1, SQLITE_TRANSIENT);
//			  }
			  sqlite3_step(stmt);

			  sqlite3_clear_bindings(stmt);
			  sqlite3_reset(stmt);

			  // End insert row
		      sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);

		//      printf("Imported %d records in %4.2f seconds\n", n, (clock() - cStartClock) / (double)CLOCKS_PER_SEC);

		      sqlite3_finalize(stmt);


		      sqlite3_close(db);


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
