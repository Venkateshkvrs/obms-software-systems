void admin_menu(int cfd){

struct message msg;
int readBytes, writeBytes;
begin2:
write_msg(cfd,1,0,"\n....... Welcome to Admin Menu .......\n1. Add Account\n2. View Account\n3. Add Customer\n4. View Customer\n5. Activate Account\n6. De-activate Account\n7. Modify Account Details\n8. Modify Customer Details\n9. / any key to Exit\nEnter Your Choice: ");
read_msg(cfd,&msg);
int choice=atoi(msg.body);
switch(choice){
case 1: add_account_handle(cfd); 	
	goto begin2;
	break;
case 2: view_account_handle(cfd); 	
	goto begin2; 
	break;
case 3: add_customer_handle(cfd); 	
	goto begin2; 
	break;
case 4: view_customer_handle(cfd); 	
	goto begin2; 
	break;
case 5: 
	write_msg(cfd,1,0,"\nEnter Account Number: ");
	read_msg(cfd,&msg);
	
	if(activate_account(atoi(msg.body))){
	write_msg(cfd,0,1,"\nAccount Activation Successful! ");
	}
	else{
	write_msg(cfd,0,1,"\nError while Activating! ");
	}
	goto begin2; 
	break;
case 6: 
	write_msg(cfd,1,0,"\nEnter Account Number: ");
	read_msg(cfd,&msg);
	
	if(activate_account(atoi(msg.body))){
	write_msg(cfd,0,1,"\nAccount De-activation Successful! ");
	}
	else{
	write_msg(cfd,0,1,"\nError while De-activating! ");
	}
	goto begin2; 
	break;
case 7: modify_account_details(cfd);
	goto begin2; 
	break;
case 8: modify_customer_details(cfd);
	goto begin2; 
	break;
default:
	write_msg(cfd,0,0,"");
	break;
}

}

void customer_menu(int cfd, int id){

struct message msg;
int readBytes, writeBytes;
begin3:
write_msg(cfd,1,0,"\n....... Welcome to Customer Menu .......\n1. Add Transaction\n2. View Transaction\n3. Check Balance\n4. Change Password \n5./ any key to Exit\nEnter Your Choice: ");
read_msg(cfd,&msg);

int choice=atoi(msg.body);
switch(choice){
case 1: add_txn_handle(cfd,id); 	
goto begin3; 
break;
case 2: view_txn_handle(cfd); 		
goto begin3; 
break;
case 3: check_balance(cfd,id);
goto begin3;
break;
case 4: modify_customer_password(cfd,id);
goto begin3;
break;
default:
	struct customer my_customer;
	my_customer=get_customer_details(id);
	my_customer.cust_login_status=0;
	set_customer_details(id,&my_customer);
	write_msg(cfd,0,0,"");
	break;
}

}


