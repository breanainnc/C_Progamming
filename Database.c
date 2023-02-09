#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "UserData.h"

//userInfo struct and varibles for storing table info
struct UserInfo user;
int UserCount = 0;
int TableCount = 0;
char* transferTarget = "0";

//callback used to print info from SQL query result 
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

//callback to get userInfo ID and fullname from SQL query result
static int InputCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        user.ID = atoi(argv[0]);
        strcat(user.fullname, argv[1]);
        user.Balance = atoi(argv[2]);
        return 0;
}

//callback to get user count from SQL query result
static int UsrCountCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        UserCount = atoi(argv[0]);
        UserCount++;
        return 0;
}

//callback used to reveal if table exists
static int TableExistsCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        TableCount = atoi(argv[0]);
        return 0;
}

//callback used to reveal if user exists
static int transferCheckCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        user.ID = atoi(argv[0]);
        user.Balance = atoi(argv[1]);
        return 0;
}
static int updateCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        return 0;
}

//Getter
char* getFullName(){
        return user.fullname;
}
//Getter
int getBalance(){
        return user.Balance;
}

//uses user inputs to create SQL query to used to create a user
int CreateUser(char* username, char* password, char* fullname, char* address, int Balance)
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
        user.Balance = Balance;
        for(int i = 0; i < 50; i++)
        {
                user.address[i] = address[i];
                if(i < 30)
                {
                        user.username[i] = username[i];
                        user.fullname[i] = fullname[i];

                        if(i < 20)
                        {
                                user.password[i] = password[i];
                        }
                }
        }

        char sqlIDCount[150] = "SELECT COUNT(*) FROM USERDB";

        rc = sqlite3_exec(db, sqlIDCount, UsrCountCallback, 0, &err);

        char sql[300] = "INSERT INTO USERDB (ID,USERNAME,PASSWORD,FULLNAME,ADDRESS,BALANCE) VALUES ('";
        sprintf(sql + strlen(sql), "%d", UserCount);
        strcat(sql, "', '");
        strcat(sql, user.username);
        strcat(sql, "', '");
        strcat(sql, user.password);
        strcat(sql, "', '");
        strcat(sql, user.fullname);
        strcat(sql, "', '");
        strcat(sql, user.address);
        strcat(sql, "', ");
        sprintf(sql + strlen(sql), "%d", user.Balance);
        strcat(sql, ");");

        rc = sqlite3_exec(db, sql, callback, 0, &err);
        sqlite3_close(db);
        if(rc != SQLITE_OK ) {
                fprintf(stderr, "SQL error: %s\n", err);
                sqlite3_free(err);
                return 1;

        }
        else{
                printf("User created Successfully\n");
                return 0;
        }
}

//used to check if login credentials are correct
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
                return 1;
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

        char sql[150] = "SELECT ID, FULLNAME, BALANCE FROM USERDB WHERE USERNAME='";
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
                return 1;

        } else {
                if(user.ID == 0)
                {
                        printf("NO RECORD OF USER ACCOUNT\n");
                        return 1;
                }
                else
                {
                        printf("*==* Your Full Name Is: %s *==*\n", user.fullname);
                }
        }

        sqlite3_close(db);
        return 0;

}

//Creates table USERDB
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

//Checks if USERDB exists and runs create function it is not created
int checkTableExists(){
        sqlite3 *db;
        int rc;    char *err = NULL;

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
        sql = "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='USERDB'";

        rc = sqlite3_exec(db, sql, TableExistsCallback, 0, &err);
        sqlite3_close(db);
        if(TableCount == 0)
        {
                printf("FIRST TIME RUNNING PROGRAM . . . .\n\nCREATING DATABASE. . . .\n\n");
                createTable();
        }
        printf(": System Ready\n");
        return 0;
}

//checks if transfer is possible and updates tables if so
int checkLegalTransfer(char* otherUser, int amount)
{
        sqlite3 *db;
        int rc;
        char *err = NULL;

        rc = sqlite3_open("test.db", &db);

        if (rc)
        {
                fprintf(stderr, "Cant open DB\n");
                return(1);
        }
        else
        {
                fprintf(stderr, "Opened DB\n");
        }


        if(amount > getBalance())
        {
                printf("Insufficent FUNDS!\n");
                return 1;
        }

        int id = user.ID;
        int balance = user.Balance;
        user.ID = 0;
        char sql[100] = "SELECT ID, BALANCE FROM USERDB WHERE USERNAME='";
                strcat(sql, otherUser);
        strcat(sql, "';");
        rc = sqlite3_exec(db, sql, transferCheckCallback, 0, &err);
        if(user.ID == 0)
        {
                printf("USER NOT FOUND\n");
                user.ID = id;
                return 1;
        }

        int targetBalance = amount + user.Balance;
        char sqlUpdateTarget[100] = "UPDATE USERDB SET BALANCE=";
        sprintf(sqlUpdateTarget + strlen(sqlUpdateTarget), "%d", targetBalance );
        strcat(sqlUpdateTarget, " WHERE ID=");
        sprintf(sqlUpdateTarget + strlen(sqlUpdateTarget), "%d", user.ID);
        strcat(sqlUpdateTarget, ";");
        rc = sqlite3_exec(db, sqlUpdateTarget, updateCallback, 0, &err);

        user.ID = id;
        user.Balance = balance - amount;

        char sqlUpdate[100] = "UPDATE USERDB SET BALANCE=";
        sprintf(sqlUpdate + strlen(sqlUpdate), "%d", user.Balance );
        strcat(sqlUpdate, " WHERE ID=");
        sprintf(sqlUpdate + strlen(sqlUpdate), "%d", user.ID);
        strcat(sqlUpdate, ";");
        rc = sqlite3_exec(db, sqlUpdate, updateCallback, 0, &err);

        sqlite3_close(db);
        return 0;
}

        
