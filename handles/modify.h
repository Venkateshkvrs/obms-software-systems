void modify_customer_password(int cfd, int id){
struct customer my_customer;
struct message msg;
int readBytes, writeBytes;

write_msg(cfd,1,2,"\nEnter/ Set New Password: # ");
read_msg(cfd,&msg);

my_customer=get_customer_details(id);
if(my_customer.cust_id==-1){// Anyway this part is never executed...
write_msg(cfd,0,1,"\nInvalid Cust-Id...");
return;
}
bzero(my_customer.cust_password,sizeof(my_customer.cust_password));
strcpy(my_customer.cust_password,msg.body);

bool status=set_customer_details(id,&my_customer);
if(status==0){
write_msg(cfd,0,1,"\nPassword Setting Failed...");
}
else{
write_msg(cfd,0,1,"\nPassword Set Successfully...");
}
}

void modify_account_details(int cfd){
struct account my_account;
struct message msg;
int id,readBytes, writeBytes;
write_msg(cfd,1,0,"\nEnter Account Number: ");
read_msg(cfd,&msg);
id=atoi(msg.body);

my_account=get_account_details(id);
if(my_account.acc_no==-1){
write_msg(cfd,0,1,"\nInvalid Account Number...");
return;
}
write_msg(cfd,1,0,"\nSet Account Balance: ");
read_msg(cfd,&msg);
my_account.acc_balance=atoi(msg.body);

bool status=set_account_details(id,&my_account);
if(status==0){
write_msg(cfd,0,1,"\nUpdation Failed...");
}
else{
write_msg(cfd,0,1,"\nDetails Updated Successfully...");
}

}

void modify_customer_details(int cfd){
struct customer my_customer;
struct message msg;
int readBytes, writeBytes;

write_msg(cfd,1,0,"\nEnter Cust-Id: ");
read_msg(cfd,&msg);
int id=atoi(msg.body);
my_customer=get_customer_details(id);
if(my_customer.cust_id==-1){
write_msg(cfd,0,1,"\nInvalid Cust-Id...");
return;
}
// Getting New Details
write_msg(cfd,1,1,"\nEnter (New) Name: ");
read_msg(cfd,&msg);

strcpy(my_customer.cust_name,msg.body);

write_msg(cfd,1,0,"\nEnter (New) Age: ");
read_msg(cfd,&msg);

int age=atoi(msg.body);
if(age>0 && age<120)
my_customer.cust_age=age;
else
my_customer.cust_age=-1;

write_msg(cfd,1,0,"\nEnter (New) UID: ");
read_msg(cfd,&msg);

strcpy(my_customer.cust_uid,msg.body);

write_msg(cfd,1,0,"\nEnter (New) Gender: ");
read_msg(cfd,&msg);

my_customer.cust_gender=msg.body[0];

write_msg(cfd,1,2,"\nEnter (New) Password: # ");
read_msg(cfd,&msg);

strcpy(my_customer.cust_password,msg.body);


bool status=set_customer_details(id,&my_customer);
if(status==0){
write_msg(cfd,0,1,"\nUpdation Failed...");
}
else{
write_msg(cfd,0,1,"\nDetails Updated Successfully...");
}
}
