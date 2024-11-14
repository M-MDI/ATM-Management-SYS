#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
    char confirmPassword[50];
};


// authentication functions
void login(char *a, char *pass);
//get the information of the new user (the username must be unique)
void registerMenu();


// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void initMenu(struct User *u);


//this function dosn't work, it must print the informations of 
//all the account that appear under the user (use the username to check) 

// this function ask the user for the account he want to change in it
// after checking he is the owner of the account
// you must ask him for what he want to change (number or country)
//create a new file with all the informations in the record file
// except the information of the that account, must be writing updated
// after that remove the old one and rename the new file to the old name (record.txt)
void updateAccount(struct User u);

// in this function you must ask the user for the account number he want 
// to check information, if he is the owner of it give him all the information about it, 
// inter = amount * interet_percentage/12.              plus
//the interet he will get every month if he get it, if he don't (current)
//tell them that the account is can't get interest
void checkAccountInfo(struct User u);

//in this functin you must print all the informations of 
//all the account that owned by the user
void checkAllAccounts(struct User u);

// in this function you must ask the user for the account number 
// after checking he is the owner of the account, ask him if he want
// to deposit or withraw. then the budget
// if he enter deposit add the budget to the old amount, else the opposit
// and recreate a file with the ammount edited
void makeTransaction(struct User u);

// in this function you must to ask the user for the accountNumber 
// he want to remove, after checking he is the owner recreate the record file
//without this account
void removeAccount(struct User u);

//in this function ask the user for the account he want to 
// transfer it, after chekcing if he is the owner, ask him 
// who the one that will have the account, after checking if the user 
// is already in our file (users.txt) recreate the record file 
//but this specific file with the new username and new user id
void transferOwner(struct User u);

void displayInterestInfo(struct Record r);
int IsPrintableName(const char *name);
int GetUserId(void);
int getLastAccountId(void);
bool generateAccountNumber(void);
int IsNOTDigit(const char *valid);
int isValidAccountType(const char *type);
int isValidDate(int day, int month, int year);
int Onlyalphabetical(char *str);
void promptForDate(struct Record *newAccount);
void promptForAccountType(struct Record *newAccount);
void promptForCountry(struct Record *newAccount);
void promptForPhone(struct Record *newAccount);
void promptForAmount(struct Record *newAccount);
void clearScreen();
bool isValidPhone(int phone);
bool isValidCountry(const char *country);
void clear_buffer(char *buffer, size_t size);
bool isValidAccountNbr(int accountNbr);
void promptForAccountNbr(struct Record *newAccount);
bool istransaction(struct Record *r);