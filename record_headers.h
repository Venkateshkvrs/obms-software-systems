/*
Author: BOPPANA VENKATESH [MT2022140]
File Description: Structures that maintained for effective file access and advisory locking.
*/
/*Record structure maintains no. of accounts, customers, admins, transactions till date in OBMS...*/
struct record{
int accounts;
int customers;
int admins;
int transactions;

};
/*Message structure maintains Type of Message, Action to be taken by client program, along with Message Body (Actual Message)...*/
struct message{
int id;
bool type;
int action;
char body[1000];
};
/*
Transaction Details maintained like in this structure...
Transaction Types: Withdrawl, Deposit.
Id is generated automatically by the system
Prev bal and Cur Bal keeps track of previous and current account Balance after this transaction done successfully...
Txn Time: Time at which the transaction done...
*/
struct txn{
int txn_id;
bool txn_type; // 0. withdrawl, 1. deposit
long int txn_amount;
int account_no;
long int prev_bal;
long int cur_bal;
time_t txn_time;
};
/*
Customer Details maintained like in this structure ...
*/
struct customer{
int cust_id;
char cust_name[25];
int cust_age;
char cust_uid[12];
char cust_gender;
//Login Credentials
char cust_login_id[11]; // OBMS-{customer-Id}
char cust_password[11];
int accounts[10]; // Account Numbers Array of this customer.
int n_accounts;// number of accounts this customer had.
bool cust_login_status;
};
/*
Account Details maintained like in this structure ...
*/
struct account{
int acc_no;
int acc_holders[2];// cust.Id's of account holders.
char acc_type;//R-> regular, J->Joint Account.
bool acc_status;//0-> Inactivate, Other-> Active
long int acc_balance;
};
/*
Administrator Details maintained like in this structure ...
*/
struct administrator{
int id;
char name[25];
char login_id[10];
char password[10];
};


