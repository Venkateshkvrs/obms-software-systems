#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>


struct txn get_txn_details(int txn_no){
int rBytes,wBytes, fd, offset;
struct txn my_txn;
//check validity of txn_no
if(is_validId(3,txn_no)){
//valid

fd=open("./records/txn_file",O_RDONLY);
if(fd==-1){
perror("Cannot Open File");
return (struct txn){-1,0,0,0,0,0,0};
}
offset=lseek(fd,txn_no*sizeof(struct txn),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in txn File");
return (struct txn){-1,0,0,0,0,0,0};
}
/*Setting Lock*/
struct flock lock = {F_RDLCK, SEEK_SET, txn_no*sizeof(struct txn), sizeof(struct txn), getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining lock!");
return (struct txn){-1,0,0,0,0,0,0};
}
/*...........*/
rBytes=read(fd,&my_txn,sizeof(my_txn));
/*Unlocking....*/
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);
/*...........*/
if(rBytes<=0){
perror("Cannot read the file");
return (struct txn){-1,0,0,0,0,0,0};
}
close(fd);
}
else{
printf("Record Not Exists\n");
return (struct txn){-1,0,0,0,0,0,0};
}
//success...
return my_txn;
}

/* Setter Program */

bool set_txn_details(int txn_no, struct txn *my_txn){
int rBytes,wBytes, fd, offset;
//check validity of txn_no
if(is_validId(3,txn_no)){
//valid
my_txn->txn_id=txn_no;
fd=open("./records/txn_file",O_WRONLY);
if(fd==-1){
perror("Cannot Open File");
return 0;
}
offset=lseek(fd,txn_no*sizeof(struct txn),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in account File");
return 0;
}
/*Setting Lock*/
struct flock lock = {F_WRLCK, SEEK_SET, txn_no*sizeof(struct txn), sizeof(struct txn), getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining lock!");
return 0;
}
/*...........*/
wBytes=write(fd,&(*my_txn),sizeof(*my_txn));
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

/*
Create/ save new Transaction to file..........
*/
int save_txn(struct txn *my_txn){
int txn_no;
int rBytes,wBytes, fd, offset;

//set available txn-no...
txn_no=get_record_details(3);
if(txn_no==-1) return -1;

my_txn->txn_id=txn_no;
fd=open("./records/txn_file",O_RDWR | O_CREAT,0777);
if(fd==-1){
perror("Cannot Open File");
return -1;
}
offset=lseek(fd,txn_no*sizeof(struct txn),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in account File");
return -1;
}
/*Setting Lock*/
struct flock lock = {F_RDLCK, SEEK_SET, txn_no*sizeof(struct txn), sizeof(struct txn), getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining lock!");
return -1;
}
/*...........*/
wBytes=write(fd,&(*my_txn),sizeof(*my_txn));
/*Unlocking....*/
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);
/*...........*/
if(wBytes<=0){
perror("Cannot write into the file");
return -1;
}
close(fd);

//upadate available txn_no...
if(!set_record_details(3,txn_no+1)){
printf("Error while updating record file\n");
return -1;
}

//success...
return txn_no;
}
