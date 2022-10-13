#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>


struct customer get_customer_details(int customer_id){
int rBytes,wBytes, fd, offset;
struct customer my_customer;
//check validity of customer-Id
if(is_validId(1,customer_id)){
//valid
fd=open("./records/customer_file",O_RDONLY);
if(fd==-1){
perror("Cannot Open File");
return (struct customer){-1,"NULL",0,"NULL",'\0',"NULL","NULL",{0,0,0,0,0,0,0,0,0,0},0};
}
offset=lseek(fd,customer_id*sizeof(struct customer),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in customer File");
return (struct customer){-1,"NULL",0,"NULL",'\0',"NULL","NULL",{0,0,0,0,0,0,0,0,0,0},0};
}
/*Setting Lock*/
struct flock lock = {F_RDLCK, SEEK_SET, customer_id*sizeof(struct customer), sizeof(struct customer), getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining lock");
return (struct customer){-1,"NULL",0,"NULL",'\0',"NULL","NULL",{0,0,0,0,0,0,0,0,0,0},0};
}
/*...........*/
rBytes=read(fd,&my_customer,sizeof(my_customer));
/*Unlocking....*/
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);
/*...........*/
if(rBytes<=0){
perror("Cannot read the file");
return (struct customer){-1,"NULL",0,"NULL",'\0',"NULL","NULL",{0,0,0,0,0,0,0,0,0,0},0};
}
close(fd);
}
else{
printf("Record Not Exists\n");
return (struct customer){-1,"NULL",0,"NULL",'\0',"NULL","NULL",{0,0,0,0,0,0,0,0,0,0},0};
}

//success...
return my_customer;
}

/* Setter Program */

bool set_customer_details(int customer_id, struct customer *my_customer){
int rBytes,wBytes, fd, offset;
//check validity of customer-Id
if(is_validId(1,customer_id)){
//valid
my_customer->cust_id=customer_id;
fd=open("./records/customer_file",O_WRONLY);
if(fd==-1){
perror("Cannot Open File");
return 0;
}
offset=lseek(fd,customer_id*sizeof(struct customer),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in customer File");
return 0;
}
/*Setting Lock*/
struct flock lock = {F_WRLCK, SEEK_SET, customer_id*sizeof(struct customer), sizeof(struct customer), getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining lock!");
return 0;
}
/*...........*/
wBytes=write(fd,&(*my_customer),sizeof(*my_customer));
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
Create/ Save new customer:: Update to file...
*/

int save_customer(struct customer *my_customer){
int customer_id;
int rBytes,wBytes, fd, offset;
customer_id=get_record_details(1);
if(customer_id==-1) return -1;

my_customer->cust_id=customer_id;
fd=open("./records/customer_file",O_WRONLY | O_CREAT,0777);
if(fd==-1){
perror("Cannot Open File");
return -1;
}
offset=lseek(fd,customer_id*sizeof(struct customer),SEEK_SET);
if(offset==-1){
perror("Error while Repositioning in customer File");
return -1;
}
/*Setting Lock*/
struct flock lock = {F_WRLCK, SEEK_SET, customer_id*sizeof(struct customer), sizeof(struct customer), getpid()};
int lockingStatus = fcntl(fd, F_SETLKW, &lock);
if (lockingStatus == -1)
{
perror("Error obtaining lock!");
return -1;
}
/*...........*/
wBytes=write(fd,&(*my_customer),sizeof(*my_customer));
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
if(!set_record_details(1,customer_id+1)){
printf("Error while updating record file\n");
return -1;
}

//success...
return customer_id;
}

