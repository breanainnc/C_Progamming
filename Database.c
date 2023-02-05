#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

#include "UserData.h"


struct UserInfo user;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int InputCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        user.ID = atoi(argv[0]);
        strcat(user.fullname, argv[1]);
        return 0;
}

char* getFullName(){
        return user.fullname;
}

int getBalance(){
        return user.Balance;
}

int CreateUser()
{
        sqlite3 *db;
        int rc;
        char *err = NULL;

        rc = sqlite3_open("test.db", &db);

        if (rc)
        {
                fprintf(stderr, "Cant open DB\n");
                return(0);
        }
        else
        {
                fprintf(stderr, "Opened DB\n");
        }

        char* sql;
        sql = "INSERT INTO USERDB (ID,USERNAME,PASSWORD,FULLNAME,ADDRESS,BALANCE) "  \
         "VALUES (1, 'BRE', 'secret', 'Br', 'Corn', 10000 ); ";

        rc = sqlite3_exec(db, sql, callback, 0, &err);
        sqlite3_close(db);
        printf("User created Successfully");
}

int checkInput(char* usrname, char* pswrd)
{
        sqlite3 *db;
        int rc;
        char *err = 0;
        const char* data = "Callback function called";
        user.ID = 0;
        user.Balance = 0;

        printf("username = %s\n", usrname);

        rc = sqlite3_open("test.db", &db);

        if (rc)
        {
                fprintf(stderr, "Cant open DB\n");
                return(0);
        }
        else
        {
                fprintf(stderr, "Opened DB\n");
        }

        for(int i = 0; i < 30; i++)
        {
                user.username[i] = usrname[i];
                if(i < 20)
                {
                        user.password[i] = pswrd[i];
                }
        }

        char sql[150] = "SELECT ID, FULLNAME FROM USERDB WHERE USERNAME='";
        char sqlMiddle[] = "' AND PASSWORD='";
        char sqlEnd[] = "';";
        strcat(sql, user.username);
        strcat(sql, sqlMiddle);
        strcat(sql, user.password);
        strcat(sql, sqlEnd);


        rc = sqlite3_exec(db, sql, InputCallback, (void*)data, &err);

        if( rc != SQLITE_OK ) {
                fprintf(stderr, "SQL error: %s\n", err);
                sqlite3_free(err);
                return 0;

        } else {
                //fprintf(stdout, "Operation done successfully\n");
                if(user.ID == 0)
                {
                        printf("NO RECORD OF USER ACCOUNT\n");
                        return 0;
                }
                else
                {
                        printf("*==* Your Full Name Is: %s *==*\n", user.fullname);
                }
          }

        sqlite3_close(db);
        return 1;

}

int createTable()
{
        sqlite3 *db;
        int rc;
        char *err = NULL;

        rc = sqlite3_open("test.db", &db);

        if (rc)
        {
                fprintf(stderr, "Cant open DB\n");
                return(0);
        }
        else
        {
                fprintf(stderr, "Opened DB\n");
        }


        char* sql;
        sql = "CREATE TABLE USERDB("  \
                "ID     INT PRIMARY KEY     NOT NULL," \
                "USERNAME           TEXT    NOT NULL," \
                "PASSWORD           TEXT    NOT NULL," \
                "FULLNAME           TEXT    NOT NULL," \
                "ADDRESS            TEXT    NOT NULL," \
                "BALANCE            REAL );";

        rc = sqlite3_exec(db, sql, callback, 0, &err);
        sqlite3_close(db);
        printf("Table created Successfully");
        return 0;
}


