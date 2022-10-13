/*
Author: BOPPANA VENKATESH [MT2022140]
File Description: Method definitions of adding new account/customer/transaction...
*/
void add_account_handle(int cfd){
int account_no;
struct account my_account;
struct message msg;
struct customer my_customer1, my_customer2;
int rwstatus, pcid, scid;
int n1, n2;
//Getting details
rwstatus=write_msg(cfd,1,0,"\nEnter Account Type (J: Joint , R or any other key for Regular): ");
if(!rwstatus){
perror("Error while writing...");
return;
}
rwstatus=read_msg(cfd,&msg);
if(!rwstatus){
perror("Error while reading...");
return;
}

if(strcmp(msg.body,"J")==0 || strcmp(msg.body,"j")==0)
my_account.acc_type='J';
else
my_account.acc_type='R';

rwstatus=write_msg(cfd,1,0,"\nEnter Cust-Id. (Primary): ");
if(!rwstatus){
perror("Error while writing...");
return;
}
rwstatus=read_msg(cfd,&msg);
if(!rwstatus){
perror("Error while reading...");
return;
}
pcid=atoi(msg.body);
scid=-1;

if(my_account.acc_type=='J'){
write_msg(cfd,1,0,"\nEnter Cust-Id. (Secondary): ");
read_msg(cfd,&msg);

scid=atoi(msg.body);
}
if(scid != -1 && pcid == scid){
write_msg(cfd,0,1,"\nEnter Valid Different Cust-Id.'s...");
return;
}
if(is_validId(1,pcid) && (scid==-1 || is_validId(1,scid))){

//Setting Semaphore Details...
key_t semKey;      
int semIdentifier; 
int semctlStatus;  
int semopStatus;  

    union semun
    {
        int val;               
        struct semid_ds *buf;  
        unsigned short *array; 
        struct seminfo *__buf; 
    } semSet;

    semKey = ftok(".", 121);
    if (semKey == -1)
    {
        perror("Error while getting key!");
        return;
    }

    semIdentifier = semget(semKey, 1, 0);
    if (semIdentifier == -1)
    {
        semIdentifier = semget(semKey, 1, IPC_CREAT | 0700);
        if (semIdentifier == -1)
        {
            perror("Error while semaphore creation...");
            return;
        }

        semSet.val = 1; 
        semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
        if (semctlStatus == -1)
        {
            perror("Error while binary semaphore initialization...");
            return;
        }
    }

    struct sembuf semOp;
    semOp.sem_num = 0;
    semOp.sem_op = -1;
    semOp.sem_flg = SEM_UNDO;
    semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error: semaphore opn failed...");
        return;
    }
/*
Critical Section..............Start
*/
my_customer1=get_customer_details(pcid);
if(scid != -1)
my_customer2=get_customer_details(scid);
if(my_customer1.n_accounts==10 || (scid != -1 && my_customer2.n_accounts==10)){
write_msg(cfd,0,1,"\nMax-Limit of Accounts Reached...");
 
/*
Critical Section................End
*/
// unlocking the critical section
    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        return;
    }
return;
}

my_customer1.accounts[my_customer1.n_accounts]=pcid;
n1=my_customer1.n_accounts;
my_customer1.n_accounts+=1;
set_customer_details(pcid,&my_customer1);

if(scid != -1){
my_customer2.accounts[my_customer2.n_accounts]=scid;
n2=my_customer2.n_accounts;
my_customer2.n_accounts+=1;
set_customer_details(scid,&my_customer2);

}
/*
Critical Section................End
*/
// unlocking the critical section
    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        return;
    }
}
else{
write_msg(cfd,0,1,"\nInvalid Cust-Id.'s...");
return;
}
my_account.acc_holders[0]=pcid;
my_account.acc_holders[1]=scid;
//Initializing status: active, balance: 0
my_account.acc_status=1;
my_account.acc_balance=0;

//Update to file
account_no=save_account(&my_account);

if(account_no==-1){
write_msg(cfd,0,1,"\nCannot Save the Account...");
}
else{
my_customer1.accounts[n1]=account_no;
set_customer_details(pcid,&my_customer1);

if(scid != -1){
my_customer2.accounts[n2]=account_no;
set_customer_details(scid,&my_customer2);
}
write_msg2(cfd,0,1,"\nAccount Created Successfully!\nAccount No. Generated is",account_no);

}
}
/*............................Add Transaction....................................................*/
void add_txn_handle(int cfd,int session_id){
int txn_no;
struct txn my_txn;
struct message msg;

int readBytes, writeBytes;
//Getting details
write_msg(cfd,1,0,"\nEnter Account Number: ");
read_msg(cfd,&msg);

int account_no=atoi(msg.body);

struct account my_account;


my_account=get_account_details(account_no);

if(my_account.acc_no==-1 || my_account.acc_status==0){
write_msg(cfd,0,1,"\nInvalid Accunt Number or Account is Inactive...");

return;
}
if(my_account.acc_holders[0] != session_id && my_account.acc_holders[1] != session_id){
write_msg(cfd,0,1,"\nNo Match with your Accounts...");

return;
}

if(my_account.acc_type=='J'){
//Setting Semaphore Details...
key_t semKey;      
int semIdentifier; 
int semctlStatus;  
int semopStatus;  

    union semun
    {
        int val;               
        struct semid_ds *buf;  
        unsigned short *array; 
        struct seminfo *__buf; 
    } semSet;

    semKey = ftok(".", 121);
    if (semKey == -1)
    {
        perror("Error while getting key...");
        return;
    }

    semIdentifier = semget(semKey, 1, 0);
    if (semIdentifier == -1)
    {
        semIdentifier = semget(semKey, 1, IPC_CREAT | 0700);
        if (semIdentifier == -1)
        {
            perror("Error while semaphore creation...");
            return;
        }

        semSet.val = 1; 
        semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
        if (semctlStatus == -1)
        {
            perror("Error while binary sempahore initialization...");
            return;
        }
    }

    struct sembuf semOp;
    semOp.sem_num = 0;
    semOp.sem_op = -1;
    semOp.sem_flg = SEM_UNDO;
    semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error: semop failed...");
        return;
    }
/*
Critical Section..............Start
*/
write_msg(cfd,1,0,"\nEnter Transaction Type: (0:WDL, 1:Dep): ");
read_msg(cfd,&msg);

int txn_type=atoi(msg.body);

write_msg(cfd,1,0,"\nEnter Transaction Amount: ");
read_msg(cfd,&msg);

int txn_amount=atoi(msg.body);
my_account=get_account_details(account_no);
if((txn_type == 0) && (my_account.acc_balance-txn_amount < 0)){
write_msg(cfd,0,1,"\nInsufficient Funds...");
/*
Critical Section................End
*/
// unlocking the critical section
    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        return;
    }
return;
}
my_txn.prev_bal=my_account.acc_balance;
if(txn_type==0){
my_txn.cur_bal=my_account.acc_balance-txn_amount;
}
else if(txn_type==1){
my_txn.cur_bal=my_account.acc_balance+txn_amount;
}

else{
write_msg(cfd,0,1,"\nInvalid Choice...");
/*
Critical Section................End
*/
// unlocking the critical section
    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        return;
    }

return;
}

my_txn.txn_type=txn_type;
my_txn.txn_amount=txn_amount;
my_txn.account_no=account_no;
my_txn.txn_time=time(0);

//Update to file
txn_no=save_txn(&my_txn);


if(txn_no==-1){
write_msg(cfd,0,1,"\nError while Adding Transaction...");
/*
Critical Section................End
*/
// unlocking the critical section
    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        return;
    }
return;
}
else{
my_account.acc_balance=my_txn.cur_bal;
set_account_details(account_no,&my_account);
write_msg2(cfd,0,1,"\nTransaction Successful!\nTxn-Id. Generated is ",txn_no);
}
/*
Critical Section................End
*/
// unlocking the critical section
    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        return;
    }
}
//...........
else{
write_msg(cfd,1,0,"\nEnter Transaction Type: (0:WDL, 1:Dep): ");
read_msg(cfd,&msg);

int txn_type=atoi(msg.body);

write_msg(cfd,1,0,"\nEnter Transaction Amount: ");
read_msg(cfd,&msg);

int txn_amount=atoi(msg.body);
if((txn_type == 0) && (my_account.acc_balance-txn_amount < 0)){
write_msg(cfd,0,1,"\nInsufficient Funds...");
return;
}
my_txn.prev_bal=my_account.acc_balance;
if(txn_type==0){
my_txn.cur_bal=my_account.acc_balance-txn_amount;
}
else if(txn_type==1){
my_txn.cur_bal=my_account.acc_balance+txn_amount;
}

else{
write_msg(cfd,0,1,"\nInvalid Choice...");
return;
}
my_txn.txn_type=txn_type;
my_txn.txn_amount=txn_amount;
my_txn.account_no=account_no;
my_txn.txn_time=time(0);

//Update to file
txn_no=save_txn(&my_txn);


if(txn_no==-1){
write_msg(cfd,0,1,"\nError while Adding Transaction...");
return;
}
else{
my_account.acc_balance=my_txn.cur_bal;
set_account_details(account_no,&my_account);
write_msg2(cfd,0,1,"\nTransaction Successful!\nTxn-Id. Generated is ",txn_no);
}

}
//..............
}

void add_customer_handle(int cfd){
int customer_id;
struct customer my_customer;
struct message msg;
int readBytes, writeBytes;

//Getting details
write_msg(cfd,1,1,"\nEnter Name: ");
read_msg(cfd,&msg);

strcpy(my_customer.cust_name,msg.body);

write_msg(cfd,1,0,"\nEnter Age: ");
read_msg(cfd,&msg);

int age=atoi(msg.body);
if(age>0 && age<120)
my_customer.cust_age=age;
else
my_customer.cust_age=-1;//Error default...

write_msg(cfd,1,0,"\nEnter UID: ");
read_msg(cfd,&msg);

strcpy(my_customer.cust_uid,msg.body);

write_msg(cfd,1,0,"\nEnter Gender: ");
read_msg(cfd,&msg);

my_customer.cust_gender=msg.body[0];

write_msg(cfd,1,2,"\nEnter Password: # ");
read_msg(cfd,&msg);

strcpy(my_customer.cust_password,msg.body);

//Initializing...
strcpy(my_customer.cust_login_id,"");
my_customer.n_accounts=0;
for(int i=0;i<10;i++)
my_customer.accounts[i]=-1;
my_customer.cust_login_status=0;
//Update to file
customer_id=save_customer(&my_customer);
if(customer_id==-1){
write_msg(cfd,0,1,"\nCannot Save the Customer...");
return;
}
else{
write_msg2(cfd,0,1,"\nCustomer Created Successfully!\nNote: Login-Id is OBMS-{customer-Id.}\nCust-Id. Generated is ",customer_id);
}
//set Login-Id...
sprintf(my_customer.cust_login_id,"OBMS-%d",customer_id);
bool status=set_customer_details(customer_id,&my_customer);
if(status==0){
write_msg(cfd,0,1,"\nLogin-Id Setting Failed...");
}
else{
write_msg2(cfd,0,1,"\nLogin-Id Settings Completed Successfully... Login-Id: OBMS-",customer_id);
}
}
