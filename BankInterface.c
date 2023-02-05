#include <stdio.h>

char* red = "\033[1;31m";
char* white = "\033[0m";
char* yellow = "\033[1;33m";
char* purple = "\033[1;35m";

int checkInput(char*, char*);
int getBalance();
char* getFullName();

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
void LogIn()
{
        printf("%sm\n*============*\nLog into Cahill's Bank!\n*============*%s\n\n", yellow , white);
        printf("What is your username?\n");
        char username[30];
        scanf("%s", username);
        printf("What is your password?\n");
        char password[20];
        scanf("%s", password);

        int inputCheck = checkInput(username, password);
        if(inputCheck == 0)
        {
                printf("\n%s*============*\nACCESS DENIED\n*============*\n%s\n", red,  white);
        }
        else
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
                  switch (choice)
                        {
                                case 1:
                                        printf("\n%sBank Balance Selected%s\n", yellow, white);
                                        printf("\n::%s BANK BALANCE::\n", getFullName());
                                        printf("%s :: %d\n", username , getBalance());

                                        break;
                                case 2:
                                        printf("%sTransfer Option Selected%s\n", yellow, white);
                                        break;
                                case 3:
                                        printf("%sBet Funds Option Selected%s\n", yellow, white);
                                case 4:
                                        printf("\033[1;36m\n*============*\nGoodbye From Cahill's Bank!\n*============*\n\033[0m");
                                        break;

                        }

                }
                 else
                {
                        printf("ERROR::CHOICE INCORRECT\n");
                }

        }
}

int CreateAccount()
{
        printf("%s\n*============*\nCreate a Cahill's Bank Account!\n*============*\n%s\n", yellow, white);
        int ID;
        char username[30];
        char password[20];
        char fullname[30];
        char address[50];
        int Balance;



        printf("Choose a username between 0 -- 30 characters\n");
        scanf("%s", username);
        printf("Choose a password between 0 -- 20 characters\n");
        scanf("%s", password);
        printf("Enter Your Full Name (0 -- 30)\n");
        scanf("%s", fullname);
        printf("Enter Your Address (0 -- 50)\n");
        scanf("%d", address);
        printf("Generating Balance. . . .\n");

}

int main(void)
{
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
                        break;

        }
        return 0;
}
              
