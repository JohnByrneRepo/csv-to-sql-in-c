/*************************************************************
    Baseline code to experiment with SQLite performance.

    Input data is a 28 Mb TAB-delimited text file of the
    complete Toronto Transit System schedule/route info 
    from http://www.toronto.ca/open/datasets/ttc-routes/

**************************************************************/

//Refer http://stackoverflow.com/questions/1711631/improve-insert-per-second-performance-of-sqlite

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sqlite3.h"

#define INPUTDATA "test.csv"
#define DATABASE "test.sqlite"
#define TABLE "CREATE TABLE IF NOT EXISTS TTC (id INTEGER PRIMARY KEY, Route_ID TEXT, Branch_Code TEXT, Version INTEGER, Stop INTEGER, Vehicle_Index INTEGER, Day Integer, Time TEXT)"
#define BUFFER_SIZE 256

int main(int argc, char **argv) {

    sqlite3 * db;
    sqlite3_stmt * stmt;
    char * sErrMsg = 0;
    char * tail = 0;
    int nRetCode;
    int n = 0;

    clock_t cStartClock;

    FILE * pFile;
    char sInputBuf [BUFFER_SIZE] = "\0";

    char * sRT = 0;  /* Route */
    char * sBR = 0;  /* Branch */
    char * sVR = 0;  /* Version */
    char * sST = 0;  /* Stop Number */
    char * sVI = 0;  /* Vehicle */
    char * sDT = 0;  /* Date */
    char * sTM = 0;  /* Time */

    char sSQL [BUFFER_SIZE] = "\0";

    /* Open the Database and create the Schema */
    sqlite3_open(DATABASE, &db);
    sqlite3_exec(db, TABLE, NULL, NULL, &sErrMsg);
    sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
    sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);

    /*********************************************/
    /* Open input file and import into Database*/
    cStartClock = clock();

    sprintf(sSQL, "INSERT INTO TTC VALUES (NULL, @RT, @BR, @VR, @ST, @VI, @DT, @TM)");
    sqlite3_prepare_v2(db,  sSQL, BUFFER_SIZE, &stmt, &tail);

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);

    pFile = fopen (INPUTDATA,"r");

    while (!feof(pFile)) {

        fgets (sInputBuf, BUFFER_SIZE, pFile);

        sRT = strtok (sInputBuf, "\t");     /* Get Route */
        sBR = strtok (NULL, "\t");      /* Get Branch */
        sVR = strtok (NULL, "\t");      /* Get Version */
        sST = strtok (NULL, "\t");      /* Get Stop Number */
        sVI = strtok (NULL, "\t");      /* Get Vehicle */
        sDT = strtok (NULL, "\t");      /* Get Date */
        sTM = strtok (NULL, "\t");      /* Get Time */

        sqlite3_bind_text(stmt, 1, sRT, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, sBR, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, sVR, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, sST, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, sVI, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, sDT, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, sTM, -1, SQLITE_TRANSIENT);

        sqlite3_step(stmt);

        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);

        n++;

    }
    fclose (pFile);

/*
	pFile = fopen (INPUTDATA,"r");
	while (!feof(pFile)) {

		fgets (sInputBuf, BUFFER_SIZE, pFile);

		sqlite3_bind_text(stmt, 1, strtok (sInputBuf, "\t"), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, strtok (NULL, "\t"), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 3, strtok (NULL, "\t"), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 4, strtok (NULL, "\t"), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 5, strtok (NULL, "\t"), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 6, strtok (NULL, "\t"), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 7, strtok (NULL, "\t"), -1, SQLITE_TRANSIENT);

		sqlite3_step(stmt);     // Execute the SQL Statement
		sqlite3_clear_bindings(stmt);   // Clear bindings
		sqlite3_reset(stmt);        // Reset VDBE

		n++;
	}
	fclose (pFile);
*/


    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);

    printf("Imported %d records in %4.2f seconds\n", n, (clock() - cStartClock) / (double)CLOCKS_PER_SEC);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
