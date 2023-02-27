#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 

char* green = "\033[32m";
char* red = "\033[1;31m";
char* white = "\033[0m";
char* yellow = "\033[1;33m";
char* purple = "\033[1;35m";

char username[30];

int checkInput(char*, char*);
int getBalance();
char* getFullName();
int CreateUser(char*, char*, char*, char*, int);
int checkTableExists();
int checkLegalTransfer(char*, int);
int correctGuess(int, int);
int wrongGuess(int);
int checkBet(int);

// Function that returns user choice from various services
int WelcomeMenu()
{
	printf("%s\n*============*\nWelcome to Cahill's Bank!\n*============*%s\n\n", red, white);
	printf("What SERVICE do you require?\n");
	printf("1 --- Log In \n");
	printf("2 --- Sign Up\n");
	printf("3 --- Exit\n");

	int choice;
	scanf("%d", &choice);

	if(choice > 0 && choice < 4){
		return choice;
	}
	else{
		printf("ERROR::CHOICE INCORRECT\n");
		return -1;
	}
}

//Checks string to see if chars are legal
int checkUserInput(char* input){
        for(int i = 0; i < strlen(input); i++){
                if(isalpha(input[i]) == 0 ) { return 1; }
        }
        return 0;
}

//Banking menu after log in
void BankingOptions()
{
	        printf("%s*============*\nWELCOME to your Cahill's ACCOUNT\n*============*\n%s\n", yellow, white);
                printf("What ACTION would you like to perform?\n");
                printf("1 --- See Balance \n");
                printf("2 --- Transfer Funds\n");
                printf("3 --- Bet Funds\n");
                printf("4 --- Exit\n");

                int choice;
                scanf("%d", &choice);

                if(choice > 0 && choice < 5)
                {
                        switch (choice)
                        {
                                case 1:
                                        printf("\n%sBank Balance Selected%s\n", yellow, white);
                                        printf("%s\n::%s BANK BALANCE::\n", purple, getFullName());
                                        printf("%s :: %d\n%s", username , getBalance(), white);
                                        break;
                                case 2:
                                        printf("%sTransfer Option Selected%s\n", yellow, white);
					
					printf("%s:Enter the USERNAME of the individual to transfer to:\n%s",purple,white);
					char otherUser[30];
					scanf("%s",otherUser);
					if(checkUserInput(otherUser) == 1){
						printf("Illegal chars used\n");
						break;
					}

					printf("%s:Enter the AMOUNT you wish to transfer:\n%s",purple,white);
                                        int amount;
                                        scanf("%d", &amount);
					
                                        int success = checkLegalTransfer(otherUser, amount);
					if(success == 0)
					{
						printf("%s\n\n Your transfer of the amount %d to user %s has been successful!\n\n%s", purple, amount, otherUser, white);
					}
					
                                        break;
                                case 3:
                                        printf("%sBet Funds Option Selected%s\n", yellow, white);
					printf("%s\n\nWelcome to the BET area\n\n%s", red, white);
					printf("The player decide on a range of numbers!\n");
					printf("The bigger the range the more valuable the BET return!\n");
					printf("The MAX range is 0 to 100!\n");
					printf("\n\nPlease selected your maximum value ::  (Must be lower than 100)\n");
					
					int range = 0;
					scanf("%d", &range);
					if(range < 1 || range > 99){
						printf("Error in range");
						break;
					}


					printf("\nPlease place a BET:\n");
					
					int bet = 0;
					scanf("%d", &bet);
				        if(checkBet(bet) == 1){
						printf("Not Enough Funds\n\n");
						break;
					}
						
					printf("Please enter a number between 1 and %d : \n", range);

					int guess = 0;
					scanf("%d", &guess);

					int randomNum = (rand() % range);

					printf("The Random Number is .");
					printf(". . . %d\n", randomNum);

					if(randomNum == guess){
						correctGuess(bet, range);
						printf("%sGuessed number: %d\n", green, guess);
					       	printf("Winning number: %d%s\n", randomNum, yellow);
						printf("You have WON%s!%s!%s!%s!\n", green, red, purple, white);
							
					}
					else{
						wrongGuess(bet);
						printf("%sGuessed number: %d\n", red, guess);
                                                printf("Winning number: %d\n", randomNum);
						printf("You Lost...%s\n", white);
					}


                                        break;
                                case 4:
                                        printf("\033[1;36m\n*============*\nGoodbye From Cahill's Bank!\n*============*\n\033[0m");
					exit(0);
                                        break;
                        }

                }
                 else
                {
                        printf("ERROR::CHOICE INCORRECT\n");
                }
		 BankingOptions();

}

//Login function asks for username and password and checks SQLite database
void LogIn()
{
	printf("%sm\n*============*\nLog into Cahill's Bank!\n*============*%s\n\n", yellow , white);
	printf("What is your username?\n");

	scanf("%s", username);
	printf("What is your password?\n");
	char password[20];
       	scanf("%s", password);
	
	int inputCheck = checkInput(username, password);
	if(inputCheck == 1)
	{
		printf("\n%s*============*\nACCESS DENIED\n*============*\n%s\n", red,  white);
	}
	else
	{
		BankingOptions();
	}
}

//Prompts user to enter details to create account
int CreateAccount()
{
	printf("%s\n*============*\nCreate a Cahill's Bank Account!\n*============*\n%s\n", yellow, white);
	printf("\n%s ONLY ALPHABETIC CHARACTERS ALLOWED IN FIELDS \n%s", red , white);
	int ID;
        char username[30];
        char password[20];
        char fullname[30];
        char address[50];
        int Balance;
	
	
	//gets input and checks if it has all legal chars
	printf("Choose a username between 0 -- 30 characters\n");
	scanf("%s", username);
	if(checkUserInput(username) == 1){ 
		printf("%s\nIllegal character used!\n%s", red , white);
		return 1; 
	}	
	printf("Choose a password between 0 -- 20 characters\n");
	scanf("%s", password);
	if(checkUserInput(password) == 1){
                printf("%s\nIllegal character used!\n%s", red , white);
                return 1;
        }
	printf("Enter Your Full Name (0 -- 30)\n");
	scanf("%s", fullname);
	if(checkUserInput(fullname) == 1){
                printf("%s\nIllegal character used!\n%s", red , white);
                return 1;
        }
	printf("Enter Your Address (0 -- 50)\n");
	scanf("%s", address);
	if(checkUserInput(address) == 1){
                printf("%s\nIllegal character used!\n%s", red , white);
                return 1;
        }
	printf("Generating Balance. . . .\n");
	Balance = 1000;
	

	//Runs CreateUser function from Database.c to create an entry in the SQL table
	int success = CreateUser(username, password, fullname, address, Balance);
	
	if(success != 0)
	{
		printf("ERROR in user creation\n");
		return 1;
	}	
	else
	{
		printf("%s\n*============*\n:YOUR ACCOUNT HAS BEEN CREATED:\n*============*\n", yellow );
		printf(":Username: %s\n:Fullname: %s\n:Address: %s\n:Balance %d:\n*============*\n%s\n", username, fullname, address, Balance, white);
		return 0;
	}
}	

int main(void)
{
	//function to check if table exist and function that returns an int that represents menu selection
	checkTableExists();	
	int choice = WelcomeMenu();

	switch (choice) {
		case 1:
			LogIn();
			break;
		case 2: 
			CreateAccount();
			break;
 		case 3:
			printf("\033[1;36m\n*============*\nGoodbye From Cahill's Bank!\n*============*\n\033[0m");
			return 0;
			break;

	}
	return 0;
}
