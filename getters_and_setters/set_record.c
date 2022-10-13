/*
Author: BOPPANA VENKATESH [MT2022140]
File Description: Program to set record details Initially.{0-Accounts, 0-Customers, 1-Default Admin, 0-Transactions}
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
struct record{
int accounts;
int customers;
int admins;
int transactions;

};

void main(){
struct record my_record;
int fd,writeBytes;
fd=open("/home/venkatesh/obms/records/record_file",O_CREAT | O_RDWR,0777);
if(fd==-1){
perror("Cannot Open File");
exit(1);
}
//Set credentials
my_record.accounts=0;
my_record.customers=0;
my_record.admins=1;
my_record.transactions=0;
writeBytes=write(fd,&my_record,sizeof(my_record));
if(writeBytes<=0){
perror("Cannot write into file");
exit(1);
}
close(fd);
}
