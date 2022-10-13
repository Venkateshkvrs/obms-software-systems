#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>


struct account get_account_details(int account_no){
int rBytes,wBytes, fd, offset;
struct account my_account;
//check validity of account_no
if(is_validId(0,account_no)){
//valid

fd=open("./records/account_file",O_RDONLY);
if(fd==-1){
perror("Cannot Open File");
return (struct account){-1,{0,0},0,0,0};
}
offset=lseek(fd,account_no*sizeof(struct account),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in account File");
return (struct account){-1,{0,0},0,0,0};
}

/*Setting Lock*/
struct flock lock = {F_RDLCK, SEEK_SET, account_no*sizeof(struct account), sizeof(struct account), getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining read lock on Account record!");
return (struct account){-1,{0,0},0,0,0};
}
/*...........*/
rBytes=read(fd,&my_account,sizeof(my_account));
/*Unlocking....*/
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);
/*...........*/

if(rBytes<=0){
perror("Cannot read the file");
return (struct account){-1,{0,0},0,0,0};
}
close(fd);
}
else{
printf("Record Not Exists\n");
return (struct account){-1,{0,0},0,0,0};
}
//success...
return my_account;
}

/* Setter Program */

bool set_account_details(int account_no, struct account *my_account){
int rBytes,wBytes, fd, offset;
//check validity of customer-Id
if(is_validId(0,account_no)){
//valid
my_account->acc_no=account_no;
fd=open("./records/account_file",O_WRONLY);
if(fd==-1){
perror("Cannot Open File");
return 0;
}
offset=lseek(fd,account_no*sizeof(struct account),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in account File");
return 0;
}
/*Setting Lock*/
struct flock lock = {F_WRLCK, SEEK_SET, account_no*sizeof(struct account), sizeof(struct account), getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining read lock on Account record!");
return 0;
}
/*...........*/
wBytes=write(fd,&(*my_account),sizeof(*my_account));
/*Unlocking....*/
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);
/*...........*/
if(wBytes<=0){
perror("Cannot write into the file");
return 0;
}
close(fd);
}
else{
printf("Record Not Exists\n");
return 0;
}

//success...
return 1;
}


bool deactivate_account(int account_no){

struct account my_account;
my_account=get_account_details(account_no);
if(my_account.acc_no==-1)
return 0;

my_account.acc_status=0;
if(set_account_details(account_no,&my_account))
return 1;

return 0;
}


bool activate_account(int account_no){

struct account my_account;
my_account=get_account_details(account_no);
if(my_account.acc_no==-1)
return 0;

my_account.acc_status=1;//activate...
if(set_account_details(account_no,&my_account))
return 1;

return 0;
}

int save_account(struct account *my_account){
int account_no;
int rBytes,wBytes, fd, offset;
//set available account-no...
account_no=get_record_details(0);
if(account_no==-1) return -1;

my_account->acc_no=account_no;

fd=open("./records/account_file",O_WRONLY | O_CREAT,0777);
if(fd==-1){
perror("Cannot Open File");
return -1;
}
offset=lseek(fd,account_no*sizeof(struct account),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in account File");
return -1;
}
/*Setting Lock: Mandatory*/
struct flock lock = {F_WRLCK, SEEK_SET, 0,0, getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining read lock on Account record!");
return 0;
}
/*...........*/
wBytes=write(fd,&(*my_account),sizeof(*my_account));
/*Unlocking....*/
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);
/*...........*/
if(wBytes<=0){
perror("Cannot write into the file");
return -1;
}
close(fd);

//upadate available account-no...
if(!set_record_details(0,account_no+1)){
printf("Error while updating record file\n");
return -1;
}
//success...
return account_no;
}


