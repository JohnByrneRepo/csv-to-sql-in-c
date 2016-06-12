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
	  int idx = 0;
	  int headingNumber = 0;
	  const char *delimiter = ",";
	  const char *empty = " ";
      for (i = 0; fgets(buffer, sizeof buffer, file); ++i) {
    	  if (i == 0) {
			  printf("Headings: ");
			  printf(buffer);
			  char* s;
			  for (s=&buffer[0]; *s != '\0'; s++) {
				  idx++;
				  printf("%c\n", *s);
				  if(!strchr(delimiter, *s)) {
					  if (idx < 2 && strchr(empty, *s)) {
						  idx = 0;
					  } else {
						  headings[headingNumber][idx] = *s;
					  }
				  } else {
					  printf("comma found\n");
					  printf("heading %i:", headingNumber + 1);
					  for (int x = 0; x < idx; x++) {
						  printf("%c", headings[headingNumber][x]);
					  }
					  headingNumber++;
					  idx=0;
				  }
			  }
    	  } else {
    		  if (i == 1) {
    			  printf("Headings:\n");
    			  for (int z = 0; z < headingNumber + 1; z++) {
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
//					  for (int x = 1; x < headingLengths[z]; x++) {
//						  printf("%c", headings[z][x]);
//					  }
					  int length = (100 - rspace) - lspace + 1;
					  char trimmed[length];
					  int idx = 0;
					  for (int t = lspace; t < length; t++) {
						  trimmed[idx] = headings[z][t];
						  idx++;
					  }
					  printf("Trimmed heading: %s\n", trimmed);
					  printf("Trimmed heading: %s\n", trimmed);
					  printf("Trimmed heading: %s\n", trimmed);
					  printf("Trimmed heading length: %i", strlen(trimmed));
					  printf("\n");
					  printf("Checking if numeric..\n");
					  printf("\n\n");
//					  for (i = 0; i < strlen(headings[z][x]); ++i) {
//					      if (!isdigit(s[i]) && !strchr(empty, *s))
//					      {
//					          valid = FALSE;
//					          break;
//					      }
//					  }
    			  }
    		  }
    		  if (i < 2) {
				  printf("Row ");
				  printf("%i: ", i);
				  printf(buffer);
				  printf("\n");
    		  }
    	  }
      }
      fclose(file);
      std::cin.get();
   }
 }
