#include "header.h"
#include "stdbool.h"
#include "ctype.h"

const char *RECORDS = "./data/records.txt";
const char *USER = "./data/user.txt";


/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/


/***************************************************************************************************************************************************************************************************/
                                                     /*   SUCCES  */
/***************************************************************************************************************************************************************************************************/
void Succes(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        clearScreen();
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {

            printf("Insert a valid operation!\n");
    
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        clearScreen();
        mainMenu(u);
    }
    else
    {
        clearScreen();
        exit(1);
    }
}

/***************************************************************************************************************************************************************************************************/
                                                     /*   REGISTRATION  */
/***************************************************************************************************************************************************************************************************/

void registerMenu() {
    FILE *fp;
    struct User newUser;
    char password;
    printf("\033[95m");
    printf("\n\t\t\t===== Register =====\n");

    do {
        printf("\n\t\tEnteur ur username (max 49 characters): ");
        scanf("%s", newUser.name);
        if (strlen(newUser.name) > 49 || !Onlyalphabetical(newUser.name)) {
            clearScreen();
            printf("\033[31m Error: Username is Invalid. Please try again.\033[39m\n");
    
            continue;
        }
        fp = fopen("./data/users.txt", "r");
        if (fp == NULL) {
            printf("\033[31m Error: Error opening file.\033[39m\n");
            exit(1);
        }

        int found = 0;
        struct User tempUser;
        while (fscanf(fp, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF) {
            if (strcmp(newUser.name, tempUser.name) == 0) {
                found = 1;
                break;
            } else if (!IsPrintableName(newUser.name)){
                found = 2;
            }
        }
        fclose(fp);

        if (found == 1) {
            printf("\033[31m Error: Username already exists. Please choose another.\033[39m\n");
    
        } else if (found == 2){
            printf("\033[31m Error: Username invalid cause countains inprintibale character .\033[39m\n");
    
        } else {
            break;
        }
    } while (1);

    do {
        printf("\n\t\tEnter your password (max 49 characters): ");
        scanf("%s", newUser.password);
        if (strlen(newUser.password) > 49) {
            clearScreen();
            printf("\033[31m Error: Password is too long. Please try again.\033[39m\n");
            registerMenu();
        } 
        printf("\n\t\tConfirm your password: ");
        scanf("%s", newUser.confirmPassword);
        
        if (strcmp(newUser.password, newUser.confirmPassword) != 0) {
            printf("\033[31m Error: Passwords do not match. Please try again.\033[39m\n");
            registerMenu();
            return;
        } else {
            break;
        }
    } while (1);

    newUser.id = GetUserId();  

    fp = fopen("./data/users.txt", "a");
    if (fp == NULL) {
        printf("\033[31m Error: Error opening file . \033[39m\n");
        exit(1);
    }
    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    fclose(fp);
    clearScreen();
    printf("\nRegistration successful!\n");
    Succes(1, mainMenu, newUser);
}

/***************************************************************************************************************************************************************************************************/
                                                     /*   LOGIN TO SESSION  */
/***************************************************************************************************************************************************************************************************/

void login(char *username, char *password) {
     FILE *fp;
     int found = 0;
    do {
        clearScreen();
        printf("\033[95m");
        printf("\n\n\t\t Bank Management System\n");
        clear_buffer(username, 100);
        clear_buffer(password, 100);
        printf("\n\t\t Username: ");
        scanf("%s", username);
        printf("\t\t Password: ");
        scanf("%s", password);


        fp = fopen("./data/users.txt", "r");
        if (fp == NULL) {
            printf("\n\t\t Cannot open users file!\n");
            exit(1);
        }
        struct User tempUser;

        while (fscanf(fp, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF) {
            if (strcmp(tempUser.name, username) == 0 && strcmp(tempUser.password, password) == 0) {
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found) {
            printf("\n\t\t Login successful!\n");
            mainMenu(tempUser);
            return;
        } else {
             printf("\033[31m \n\t\tError:  Wrong username or password!.\033[39m\n");
    
            printf("\n\t\t Press Enter to try again...");
            getchar();
            getchar();
        }
         
    } while (!found);
}


/***************************************************************************************************************************************************************************************************/
                                                     /*   CREAT NEW ACCOUNT  */
/***************************************************************************************************************************************************************************************************/

void createNewAcc(struct User u) {
    struct Record r;
    FILE *pf;

    clearScreen();
    printf("\033[95m");
    printf("\t\t\t\tCreating new account:\n");
    memset(&r, 0, sizeof(r));
    int accountnbr;
    
    r.id = getLastAccountId();
    r.userId = u.id;
    strcpy(r.name, u.name);

    promptForAccountNbr(&r);
    promptForCountry(&r);
    promptForPhone(&r);
    promptForAmount(&r);
    promptForAccountType(&r);
    promptForDate(&r);

    pf = fopen("./data/records.txt", "a");
    if (pf == NULL) {
        printf("\n\033[31m Error: opening file. Please try again later..\033[39m\n");

        Succes(1, mainMenu, u);
        return;
    }

    fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id, r.userId, r.name, r.accountNbr,
            r.deposit.day, r.deposit.month, r.deposit.year,
            r.country, r.phone, r.amount, r.accountType);
    fclose(pf);

    printf("\nAccount created successfully!\n");
    Succes(1, mainMenu, u);
}

/***************************************************************************************************************************************************************************************************/
                                                     /*   CHECK ALL ACCOUNTS OF USER  */
/***************************************************************************************************************************************************************************************************/

void checkAllAccounts(struct User u) {
    struct Record r;
    int accountsFound = 0;
    FILE *pf = fopen(RECORDS, "r");
    
    if (pf == NULL) {
        printf("nError: Unable to open records file. Please try again later.\n");

        Succes(1, mainMenu, u);
        return;
    }

    clearScreen();
    printf("\033[95m");
    printf("\t====== All Accounts for %s ======\n\n", u.name);

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        
        if (strcmp(r.name, u.name) == 0) {
                accountsFound++;
                printf("     ╔═══════════════════════════════════════╗\n");
                printf("     ║\tAccount #%-23d     ║\n", accountsFound);
                printf("     ╠═══════════════════════════════════════╣\n");
                printf("     ║ ==> Account Number : %-17d║\n", r.accountNbr);
                printf("     ║ ==> Date Opened    : %02d/%02d/%02d       ║\n", 
                    r.deposit.day, r.deposit.month, r.deposit.year);
                printf("     ║ ==> Country        : %-17s║\n", r.country);
                printf("     ║ ==> Phone Number   : %-17d║\n", r.phone);
                printf("     ║ ==> Balance        : $%-16.2f║\n", r.amount);
                printf("     ║ ==> Account Type   : %-17s║\n", r.accountType);
                printf("     ╚═══════════════════════════════════════╝\n");
                
        }
    }

    if (accountsFound == 0) {
        printf("\nNo accounts found for user %s.\n", u.name);

    } else {
        printf("\nTotal accounts found: %d\n", accountsFound);
    }

    if (ferror(pf)) {
        printf("\nError occurred while reading the file.\n");

    }

    fclose(pf);
    
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); 
    getchar(); 
    
    Succes(1, mainMenu, u);
}

/***************************************************************************************************************************************************************************************************/
                                                     /*   UPDATE INFORMATION (COUNTRY/ PHONE NUMBER)  */
/***************************************************************************************************************************************************************************************************/

void updateAccount(struct User u) {
    struct Record r;
    int accountNBR, choice;
    FILE *pf = fopen(RECORDS, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    int found = 0;

    if (pf == NULL || tempFile == NULL) {
        perror("Error opening file");

        if (pf) fclose(pf);
        if (tempFile) fclose(tempFile);
        Succes(1, mainMenu, u);
        return;
    }

    printf("\033[95m");
    printf("\t\t====== Update Account Information ======\n\n");
    printf("Enter the account number you want to update: ");
    scanf("%d", &accountNBR);

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        if (r.accountNbr == accountNBR) {
            found = 1;
            
            while (1) {
                printf("\nChoose the information you want to update:\n");
                printf("\t1. Country\n");
                printf("\t2. Phone Number\n");
                printf("\t3. Exit\n");
                printf("\nEnter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1: {
                        char newCountry[100];
                        printf("\nEnter the new country: ");
                        scanf("%s", newCountry);
                        if (!Onlyalphabetical(newCountry) || !isValidCountry(newCountry)) {
                            printf("\n\033[101m Invalid country name. Country should be 2-99 characters and contain only letters. \033[39m\n");
                            continue;
                        }
                        strcpy(r.country, newCountry);
                        break;
                    }
                    case 2: {
                        int newPhone;
                        printf("\nEnter the new phone number: ");
                        scanf("%d", &newPhone);
                        if (!isValidPhone(newPhone)) {
                            printf("\nInvalid phone number. Please enter a 9-digit number.\n");
                            continue;
                        }
                        r.phone = newPhone;
                        break;
                    }
                    case 3:
                        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                                r.id, r.userId, r.name, r.accountNbr,
                                r.deposit.day, r.deposit.month, r.deposit.year,
                                r.country, r.phone, r.amount, r.accountType);
                        fclose(pf);
                        fclose(tempFile);
                        remove(RECORDS);
                        rename("temp.txt", RECORDS);
                        Succes(1, mainMenu, u);
                        return;
                    default:
        
                        printf("\nInvalid choice. Please try again.\n");
                
                        continue; 
                }
                break;
            }
        }

        
        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                r.id, r.userId, r.name, r.accountNbr,
                r.deposit.day, r.deposit.month, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(pf);
    fclose(tempFile);

    if (!found) {
        printf("\nAccount not found.\n");

        remove("temp.txt");
        Succes(1, mainMenu, u);
        return;
    }

    remove(RECORDS);
    rename("temp.txt", RECORDS);

    printf("\nAccount information updated successfully.\n");
    Succes(1, mainMenu, u);
}

/***************************************************************************************************************************************************************************************************/
                                                     /*   CHECK ACCOUNT INFORMATIONS  */
/***************************************************************************************************************************************************************************************************/

void checkAccountInfo(struct User u)
{
    struct Record r;
    int accountNumber;
    FILE *pf = fopen(RECORDS, "r");

    if (pf == NULL) {
        printf("Error opening file!\n");


        Succes(1, mainMenu, u);
        return;
    }

    clearScreen();
    printf("\033[95m");
    printf("\t\t====== Check Account Info =====\n\n");
    printf("\nEnter the account number:");
    scanf("%d", &accountNumber);

    int found = 0;
    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accountNumber) {
            found = 1;
            printf("     ╔═══════════════════════════════════════╗\n");
                printf("     ║\t ########## Account #########        ║\n");
                printf("     ╠═══════════════════════════════════════╣\n");
                printf("     ║ ==> Account Number : %-17d║\n", r.accountNbr);
                printf("     ║ ==> Date Opened    : %02d/%02d/%02d       ║\n", 
                    r.deposit.day, r.deposit.month, r.deposit.year);
                printf("     ║ ==> Country        : %-17s║\n", r.country);
                printf("     ║ ==> Phone Number   : %-17d║\n", r.phone);
                printf("     ║ ==> Balance        : $%-16.2f║\n", r.amount);
                printf("     ║ ==> Account Type   : %-17s║\n", r.accountType);
                printf("     ╚═══════════════════════════════════════╝\n");

                displayInterestInfo(r);
        
            break;
        }
    }
    
    fclose(pf);
    
    if (!found) {
        printf("\nAccount not found or you don't have access to this account!\n");

        Succes(1, mainMenu, u);
    } else {
        Succes(1, checkAccountInfo, u);
    }
}

/***************************************************************************************************************************************************************************************************/
                                                     /*   MAKE TRANSACTION (WHIDRAW OR DEPOSIT) */
/***************************************************************************************************************************************************************************************************/

void makeTransaction(struct User u) {
    int accountNumber, choice;
    double amount;
    struct Record r;
    FILE *pf = fopen(RECORDS, "r+");
    int found = 0;

    if (pf == NULL) {
        printf("Error opening file!\n");
        Succes(1, mainMenu, u);
        return;
    }

    clearScreen();
    printf("\033[95m");
    printf("\t\t====== Make Transaction =====\n\n");
    printf("\nEnter the account number: ");
    scanf("%d", &accountNumber);
    // check if accounttype is saving or current 
    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        if (r.accountNbr == accountNumber) {
             if (strcmp(r.accountType, "current") != 0 && strcmp(r.accountType, "saving") != 0) {
                printf("\nThis account type does not allow transactions. Only saving and current accounts are allowed.\n");
                fclose(pf);
                Succes(1, mainMenu, u);
                return;
            }
            found = 1;
            break;
        } 
    }

    if (!found) {
        printf("\nAccount not found or you are not allowed to make transaction cause u have a fixed account!\n");
        fclose(pf);
        Succes(1, mainMenu, u);
        return;
    } 

    while (1) {
        printf("\nChoose the type of transaction:\n");
        printf("\t1. Deposit\n");
        printf("\t2. Withdraw\n");
        printf("\t3. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                clearScreen();
                printf("\nEnter the amount to deposit: ");
                scanf("%lf", &amount);
                
                if (amount <= 0) {
        
                    printf("\nInvalid amount. Please enter a positive number.\n");
            
                    continue;
                }
                
                r.amount += amount;
                break;
            }
            case 2: {
                clearScreen();
                printf("\nEnter the amount to withdraw: ");
                scanf("%lf", &amount);
                
                if (amount <= 0 || amount > r.amount) {
        
                    printf("\nInvalid amount. Please enter a positive number less than or equal to your current balance.\n");
            
                    continue;
                }
                
                r.amount -= amount;
                break;
            }
            case 3:
                fclose(pf);
                Succes(1, mainMenu, u);
                return;
            default:
                clearScreen();
    
                printf("\nInvalid choice. Please try again.\n");
        
                continue;
        }

        rewind(pf);
        FILE *tempFile = fopen("temp.txt", "w");
        if (tempFile == NULL) {

            printf("Error creating temporary file.\n");
    
            fclose(pf);
            Succes(1, mainMenu, u);
            return;
        }

        struct Record tempRecord;
        while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                      &tempRecord.id, &tempRecord.userId, tempRecord.name, &tempRecord.accountNbr,
                      &tempRecord.deposit.day, &tempRecord.deposit.month, &tempRecord.deposit.year,
                      tempRecord.country, &tempRecord.phone, &tempRecord.amount, tempRecord.accountType) == 11) {
            
            if (tempRecord.accountNbr == accountNumber) {
                fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                        r.id, r.userId, r.name, r.accountNbr,
                        r.deposit.day, r.deposit.month, r.deposit.year,
                        r.country, r.phone, r.amount, r.accountType);
            } else {
                fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                        tempRecord.id, tempRecord.userId, tempRecord.name, tempRecord.accountNbr,
                        tempRecord.deposit.day, tempRecord.deposit.month, tempRecord.deposit.year,
                        tempRecord.country, tempRecord.phone, tempRecord.amount, tempRecord.accountType);
            }
        }

        fclose(pf);
        fclose(tempFile);

        remove(RECORDS);
        rename("temp.txt", RECORDS);

        printf("\nTransaction completed successfully.\n");

        Succes(1, mainMenu, u);
        return;
    }
}


/***************************************************************************************************************************************************************************************************/
                                                     /*   REMOVE ACCOUNT  */
/***************************************************************************************************************************************************************************************************/

void removeAccount(struct User u) {
    int accountNumber;
    struct Record r;
    FILE *pf, *tempFile;
    int found = 0;

    clearScreen();
    printf("\033[95m");
    printf("\t\t====== Remove Account =====\n\n");
    printf("Enter the account number to remove: ");
    scanf("%d", &accountNumber);

    pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file!\n");

        Succes(1, mainMenu, u);
        return;
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");

        fclose(pf);
        Succes(1, mainMenu, u);
        return;
    }

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        
        if (r.accountNbr == accountNumber && strcmp(r.name, u.name) == 0) {
            found = 1;
            printf("\nAccount #%d removed successfully.\n", accountNumber);
        } else {
            fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                    r.id, r.userId, r.name, r.accountNbr,
                    r.deposit.day, r.deposit.month, r.deposit.year,
                    r.country, r.phone, r.amount, r.accountType);
        }
    }

    fclose(pf);
    fclose(tempFile);
    if (!found) {
        remove("temp.txt");
        printf("\nAccount not found or you don't have permission to remove it.\n");

        Succes(1, mainMenu, u);
        return;
    }

    remove(RECORDS);
    rename("temp.txt", RECORDS);

    Succes(1, mainMenu, u);
}

/***************************************************************************************************************************************************************************************************/
                                                     /*   TRANSFERT OWNER TO ANOTHER USER  */
/***************************************************************************************************************************************************************************************************/

void transferOwner(struct User u) {
    int accountNumber;
    char newOwnerName[100];
    struct Record r;
    FILE *pf;
    int found = 0;
    int newUserExists = 0;  

    clearScreen();
    printf("\033[95m");
    printf("\t\t====== Transfer Account Ownership =====\n\n");
    
    printf("Enter the account number to transfer: ");
    scanf("%d", &accountNumber);
    getchar();

    printf("Enter the new owner's name: ");
    scanf("%s", newOwnerName);

    pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file!\n");

        Succes(1, mainMenu, u);
        return;
    }

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        if (strcmp(r.name, newOwnerName) == 0) {
            newUserExists = 1;
            break;
        }
    }
    fclose(pf);

    if (!newUserExists) {
        printf("\nError: The user %s does not exist in records.\n", newOwnerName);

        Succes(1, mainMenu, u);
        return;
    }

    pf = fopen(RECORDS, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (pf == NULL || tempFile == NULL) {
        printf("Error with files!\n");
        if (pf) fclose(pf);
        if (tempFile) fclose(tempFile);
        Succes(1, mainMenu, u);
        return;
    }

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.day, &r.deposit.month, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) == 11) {
        
        if (r.accountNbr == accountNumber && strcmp(r.name, u.name) == 0) {
            char confirm;
            printf("\nAre you sure you want to transfer account #%d to %s? (y/n): ", 
                   accountNumber, newOwnerName);
            scanf(" %c", &confirm);
            
            if (confirm == 'y' || confirm == 'Y') {
                found = 1;
                strcpy(r.name, newOwnerName);
                printf("\nAccount ownership transferred successfully to %s.\n", newOwnerName);
            } else {
                printf("\nTransfer cancelled.\n");
            }
        }
        
        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n",
                r.id, r.userId, r.name, r.accountNbr,
                r.deposit.day, r.deposit.month, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(pf);
    fclose(tempFile);

    if (!found) {
        remove("temp.txt");
        printf("\nAccount not found or you don't have permission to transfer it.\n");

        Succes(1, mainMenu, u);
        return;
    }

    remove(RECORDS);
    rename("temp.txt", RECORDS);

    Succes(1, mainMenu, u);
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

int IsNOTDigit(const char *valid) {
    int i;
    for (i = 0; valid[i] != '\0'; i++) {
        if (!isdigit(valid[i])) {
            return 0;
        }
    }
    return 1;
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

bool generateAccountNumber() {
    FILE *fp = fopen("./data/records.txt", "r");
    if (fp == NULL) {
          printf("\033[31m Error: Error opening file.\033[39m\n");
        exit(1);
    }

    int lastId = 0;
    struct Record tempRecord;
    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &tempRecord.id, &tempRecord.userId, tempRecord.name, &tempRecord.accountNbr,
                  &tempRecord.deposit.day, &tempRecord.deposit.month, &tempRecord.deposit.year,
                  tempRecord.country, &tempRecord.phone, &tempRecord.amount, tempRecord.accountType) != EOF) {
        if (tempRecord.accountNbr = lastId) {
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

void clear_buffer(char *buffer, size_t size) {
    memset(buffer, 0, size);
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

int getLastAccountId() {
    FILE *fp = fopen("./data/records.txt", "r");
    if (fp == NULL) {
          printf("\033[31m Error: Error opening file.\033[39m\n");
        exit(1);
    }

    int lastId = 0;
    struct Record tempRecord;
    char name[50];

    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &tempRecord.id, &tempRecord.userId, tempRecord.name, &tempRecord.accountNbr,
                  &tempRecord.deposit.day, &tempRecord.deposit.month, &tempRecord.deposit.year,
                  tempRecord.country, &tempRecord.phone, &tempRecord.amount, tempRecord.accountType) != EOF) {
        if (tempRecord.id > lastId) {
            lastId = tempRecord.id;
        }
    }

    fclose(fp);
    return lastId + 1;
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

int GetUserId() {
    FILE *fp = fopen("./data/users.txt", "r");
    if (fp == NULL) {
          printf("\033[31m Error: Error opening file.\033[39m\n");
        exit(1);
    }

    int lastId = 0;
    struct User tempUser;

    while (fscanf(fp, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF) {
        if (tempUser.id > lastId) {
            lastId = tempUser.id;
        }
    }

    fclose(fp);
    return lastId + 1;
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

int IsPrintableName(const char *str) {
    int i = 0;
    while (str[i]) {
        if (!((str[i] >= 32 && str[i] <= 126) || str[i] == '\n')) {
            return 0;
        }
        i++;
    }
    return 1;
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

int isValidAccountType(const char* type) {
    return (strcmp(type, "saving") == 0 || strcmp(type, "current") == 0 ||
            strcmp(type, "fixed01") == 0 || strcmp(type, "fixed02") == 0 ||
            strcmp(type, "fixed03") == 0);
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

int isValidDate(int day, int month, int year) {
    if (month < 1 || month > 12 || day < 1 || day > 31 || year < 2000 || year > 2050) {
        return false;
    }
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        daysInMonth[1] = 29;
    }
    
    return day <= daysInMonth[month - 1];
}


/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

int Onlyalphabetical(char *str) {
    int i = 0;
    while (str[i]) {
        if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))) {
            return 0;
        }
        i++;
    }

    return 1;
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

void promptForCountry(struct Record *newAccount) {
    while (true) {
        printf("\nEnter the country: ");
        scanf("%s", newAccount->country);
        
        if (!isValidCountry(newAccount->country)) {
            clearScreen();

          printf("\033[31m Error: Unable to open records file. Please try again later.\033[39m\n");
        } else {
            break;
        }
    }
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

void promptForPhone(struct Record *newAccount) {
    while (true) {
        printf("\nEnter the phone number: ");
        scanf("%d", &newAccount->phone);
        
        if (newAccount->phone < 100000000 || newAccount->phone > 999999999) {
            clearScreen();
            printf("\nInvalid phone number. Please enter a 9-digit number.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

void promptForAmount(struct Record *newAccount) {
    while (true) {
        printf("\nEnter the amount to deposit: $");
        if (scanf("%lf", &newAccount->amount) != 1 || newAccount->amount < 0) {
            clearScreen();
            printf("\nInvalid amount. Please enter a positive number.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

void promptForAccountType(struct Record *newAccount) {
    while (true) {
        printf("\nEnter the type of account : \n\t\t\t=====> . saving\n\t\t\t=====> . current\n\t\t\t=====> . fixed01\n\t\t\t=====> . fixed02\n\t\t\t=====> . fixed03\n=================>\t  ");
        scanf("%9s", newAccount->accountType);
        
        if (!isValidAccountType(newAccount->accountType)) {
            clearScreen(); 
            printf("\nInvalid account type. Please choose from the given options.\n");
        } else {
            break;
        }
    }
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

void promptForDate(struct Record *newAccount) {
    while (true) {
        printf("\nEnter today's date (dd/mm/yyyy): ");
        scanf("%d/%d/%d", &newAccount->deposit.day, &newAccount->deposit.month, &newAccount->deposit.year);
        
        if (!isValidDate(newAccount->deposit.day, newAccount->deposit.month, newAccount->deposit.year)) {
            clearScreen();
            printf("\nInvalid date format or date. Please use dd/mm/yyyy format.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

 void promptForAccountNbr(struct Record *newAccount) {
    int accountNbr;
    while (true) {
        printf("\nEnter the account number: ");
        scanf("%d", &newAccount->accountNbr);
        
        if (!isValidAccountNbr(newAccount->accountNbr)) {
            clearScreen();
            printf("\nInvalid account number. Please enter a valid account number.\n");
            while (getchar()!= '\n');
        } else {
            break;
        }
    }
 }

/***************************************************************************************************************************************************************************************************/
/************************************************************************************************************************************************************************************************/

 bool isValidAccountNbr(int accountNbr) {
    // The account number is the last 4 digits of the phone number
    // it needs to be unique
    // Also check if the account number already exists
    // in the records file
    if (accountNbr < 1 || accountNbr > 999999999) return false;
    
    if (accountNbr == generateAccountNumber()) {
        return false;
    }
    
    return true;
 }

 /***************************************************************************************************************************************************************************************************/
 /************************************************************************************************************************************************************************************************/
 bool isValidPhone(int phone) {
    return phone >= 100000000 && phone <= 999999999;
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

bool isValidCountry(const char *country) {
    if (strlen(country) < 2 || strlen(country) > 99) return false;
    
    for (int i = 0; country[i]; i++) {
        if (!isalpha(country[i])) return false;
    }
    return true;
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/

void clearScreen() {
   system("clear");
}

/***************************************************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************************************************/
void displayInterestInfo(struct Record r) {
    double interestRate = 0.0;
    double monthlyInterest = 0.0;

    printf("\033[95m");
    printf("\n     ╔═══════════════════════════════════════╗\n");
    printf("     ║\t ########## Interest #########       ║\n");
    printf("     ╠═══════════════════════════════════════╣\n");

    if (strcmp(r.accountType, "saving") == 0) {
        interestRate = 0.07; 
        monthlyInterest = (r.amount * interestRate) / 12;
        printf("     ║ Account Type: Saving                  ║\n");
        printf("     ║ Interest Rate: 7%%                     ║\n");
        printf("     ║ Monthly Interest: $%-17.2f  ║\n", monthlyInterest);
        printf("     ║ Payment Date: Day %-2d of every month   ║\n", r.deposit.day);
    }
    else if (strcmp(r.accountType, "fixed01") == 0) {
        interestRate = 0.04; 
        monthlyInterest = (r.amount * interestRate);
        printf("     ║ Account Type: Fixed Deposit - 1 Year  ║\n");
        printf("     ║ Interest Rate: 4%%                     ║\n");
        printf("     ║ Monthly Interest: $%-17.2f  ║\n", monthlyInterest);
        printf("     ║                                       ║\n");
        printf("     ║ Maturity Date: %02d/%02d/%-15d  ║\n", 
               r.deposit.day, r.deposit.month, r.deposit.year + 1);
    }
    else if (strcmp(r.accountType, "fixed02") == 0) {
        interestRate = 0.05; 
        monthlyInterest = (r.amount * interestRate) * 2;
        printf("     ║ Account Type: Fixed Deposit - 2 Years ║\n");
        printf("     ║ Interest Rate: 5%%                     ║\n");
        printf("     ║ Monthly Interest: $%-17.2f  ║\n", monthlyInterest);
        printf("     ║                                       ║\n");
        printf("     ║ Maturity Date: %02d/%02d/%-15d  ║\n", 
               r.deposit.day, r.deposit.month, r.deposit.year + 2);
    }
    else if (strcmp(r.accountType, "fixed03") == 0) {
        interestRate = 0.08; 
        monthlyInterest = (r.amount * interestRate) * 3;
        printf("     ║ Account Type: Fixed Deposit - 3 Years ║\n");
        printf("     ║ Interest Rate: 8%%                     ║\n");
        printf("     ║ Monthly Interest: $%-17.2f  ║\n", monthlyInterest);
        printf("     ║                                       ║\n");
        printf("     ║ Maturity Date: %02d/%02d/%-15d  ║\n", 
               r.deposit.day, r.deposit.month, r.deposit.year + 3);
    }
    else if (strcmp(r.accountType, "current") == 0) {
        printf("     ║ Account Type: Current                 ║\n");
        printf("     ║                                       ║\n");
        printf("     ║ You will not get interests because    ║\n");
        printf("     ║ the account is of type current        ║\n");
    }
    
    printf("     ╚═══════════════════════════════════════╝\n");
}