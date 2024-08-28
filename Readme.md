# ATM Management System

## Authors

- **Mehdi** - Project creator and developer

## Overview

This project is an ATM management system built in C. The system allows users to perform several operations such as:
- Registering new users
- Creating and updating accounts
- Viewing account details
- Performing transactions (deposit/withdrawal)
- Deleting accounts
- Transferring account ownership between users

### Features Implemented

1. **User Registration**
   - Unique user registration with validation to ensure no duplicate usernames.

2. **Login**
   - Allows users to securely log into the system by verifying their credentials.

3. **Account Creation**
   - Users can create new bank accounts associated with their profile.

4. **Check Account Details**
   - Displays specific account information based on the account ID.

5. **Update Account Information**
   - Users can update the country or phone number for their accounts.

6. **Make Transactions**
   - Deposit and withdraw money from accounts (restrictions for certain account types).

7. **Account Removal**
   - Users can remove their accounts.

8. **Transfer Account Ownership**
   - Users can transfer account ownership to another registered user.

### Bonus Features

- **Instant Notification Using Pipes and Child Processes**
  - Users are instantly notified when they receive ownership of an account.

- **Encryption of Passwords**
  - User passwords are encrypted to enhance security.
