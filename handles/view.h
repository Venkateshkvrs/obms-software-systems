#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<string.h>
#include<errno.h>
#include<stdbool.h>
#include<time.h>

void view_account_handle(int cfd){
struct account my_account;
struct message msg;
int id;
int readBytes, writeBytes;
write_msg(cfd,1,0,"\nEnter Account Number: ");
read_msg(cfd,&msg);

id=atoi(msg.body);
my_account=get_account_details(id);

if(my_account.acc_no==-1){
write_msg(cfd,0,1,"\nCannot view the Account...");
return;
}
char buffer[1000];

sprintf(buffer,"\n...................Account Details..................\nAccount Type: %c((J)oint/ (R)egular)\nAccount Holders Cust-Id's: %d(primary) %d(secondary)\nAccount Status %d(1: Activen 0:Inactive) \nAccount Balance: %ld\n...................End of Statement..................",my_account.acc_type,my_account.acc_holders[0],my_account.acc_holders[1],my_account.acc_status,my_account.acc_balance);
write_msg(cfd,0,1,buffer);

}


void view_txn_handle(int cfd){
struct txn my_txn;
struct tm * timeinfo;
struct message msg;
int id;
int readBytes, writeBytes;
write_msg(cfd,1,0,"\nEnter Transaction Id: ");
read_msg(cfd,&msg);

id=atoi(msg.body);
my_txn=get_txn_details(id);
if(my_txn.txn_id==-1) {
write_msg(cfd,0,1,"\nCannot view the Transaction...");
return;
}

timeinfo=localtime(&my_txn.txn_time);
char buffer[1000];
sprintf(buffer,"...................Transaction Details..................\nTransaction Type: %d(0:WDL 1:Dep)\nTransaction Amount: %ld\nTransaction Account No: %d\nPrev Balance %ld\nCurrent Balance %ld\nTransaction Time %s\n...................End of Statement..................\n",my_txn.txn_type,my_txn.txn_amount,my_txn.account_no,my_txn.prev_bal,my_txn.cur_bal,asctime(timeinfo));
write_msg(cfd,0,1,buffer);

}


void view_customer_handle(int cfd){
struct customer my_customer;
struct message msg;
int id;
int readBytes, writeBytes;
write_msg(cfd,1,0,"\nEnter Cust-Id: ");
read_msg(cfd,&msg);

id=atoi(msg.body);

my_customer=get_customer_details(id);
if(my_customer.cust_id==-1){
write_msg(cfd,0,1,"\nCannot view the Customer...");
return;
}

char buffer[1000];
sprintf(buffer,"...................Customer Details..................\nName: %s\nAge: %d\nGender: %c\nUid: %s\nLogin-Id: %s\nPassword: %s\nAccounts: %d\nAccount Numbers: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n...................End of Statement..................\n",my_customer.cust_name,my_customer.cust_age,my_customer.cust_gender,my_customer.cust_uid,my_customer.cust_login_id,my_customer.cust_password,my_customer.n_accounts,my_customer.accounts[0],my_customer.accounts[1],my_customer.accounts[2],my_customer.accounts[3],my_customer.accounts[4],my_customer.accounts[5],my_customer.accounts[6],my_customer.accounts[7],my_customer.accounts[8],my_customer.accounts[9]);
write_msg(cfd,0,1,buffer);

}
void check_balance(int cfd,int customer_id){
struct customer my_customer;
struct message msg;
my_customer=get_customer_details(customer_id);
if(my_customer.cust_id==-1){
write_msg(cfd,0,1,"\nCannot view the Customer...");
return;
}
if(my_customer.n_accounts==0){
write_msg(cfd,0,1,"\nNo Accounts to display Balance");
return;
}
for(int i=0;i<my_customer.n_accounts;i++){
int account_no=my_customer.accounts[i];
struct account my_account;
my_account=get_account_details(account_no);
if(my_account.acc_no==-1){
write_msg2(cfd,0,1,"\nCannot view the Account for Balance...",account_no);
}
write_msg2(cfd,0,1,"\nAccount Number ",account_no);
write_msg2(cfd,0,1,"Balance Rs ",my_account.acc_balance);
}
write_msg(cfd,0,1,"\n...................End of Statement..................\n");
}

