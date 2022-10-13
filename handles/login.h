bool admin_login(char* id,char* password){
	struct administrator my_admin;
	my_admin=get_admin_details(0);//default admin credentials
	if(my_admin.id==-1) return 0;
	if(strcmp(id,my_admin.login_id) == 0 && strcmp(password,my_admin.password) == 0)
	return 1;
return 0;
}
bool customer_login(char* id,char* password){
	struct customer my_customer;
	my_customer=get_customer_details(atoi(id+5));
	if(my_customer.cust_id==-1)return 0;
	if(my_customer.cust_login_status==1) return 0;
	if(strcmp(my_customer.cust_login_id,id)==0 && strcmp(my_customer.cust_password,password)==0){
	my_customer.cust_login_status=1;
	set_customer_details(atoi(id+5),&my_customer);
	return 1;
	}
return 0;
}
void login_handle(int cfd,int type){
struct message msg;
char id[11];
char password[11];
int readBytes, writeBytes;
//Login Menu
//Getting Login_id
write_msg(cfd,1,0,"\nEnter Login-Id: ");
read_msg(cfd,&msg);

strcpy(id,msg.body);
id[10]='\0';

//Getting Password
write_msg(cfd,1,2,"\nEnter Password: ");
read_msg(cfd,&msg);


strcpy(password,msg.body);
password[10]='\0';

if(type==0 && admin_login(id,password)){
write_msg(cfd,0,1,"\nLogin Successful! ");
//goto menu
admin_menu(cfd);
}
else if(type==1 && customer_login(id,password)){
write_msg(cfd,0,1,"\nLogin Successful! ");

//goto menu
customer_menu(cfd,atoi(id+5));
}
else{
write_msg(cfd,1,0,"\nLogin Failed:Invalid Details or Already Login...\nDo you want to Try again(Y/N): ");
read_msg(cfd,&msg);

if((msg.body[0]=='Y' && msg.body[1]=='\0') || (msg.body[0]=='y' && msg.body[1]=='\0'))
login_handle(cfd,type);
else{
write_msg(cfd,0,0,"");
}

}
}

