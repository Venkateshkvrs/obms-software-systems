# CS 513 Software Systems Mini Project - OBMS
$$Title:\ \  Design\  and\ Development\ of\ Online\ Banking\ Management\ System\ (OBMS)$$
$\bigstar$ &nbsp; <b><i>Project Goal:</i></b><br> To develop a banking management system which is user-friendly and multi functional 
(as specified in Project Description, Reference: <a href="https://iiitbac-my.sharepoint.com/:b:/g/personal/boppana_venkatesh_iiitb_ac_in/EcQK-nE5hIVGq1gRtiPuFREBCUhx7QMWPTDrTH2OehJ5iw?e=vEONVo">link</a> )
<p>
This project mainly deals with concurrent banking operations in a safe and secure environment.<br/>
The term ‘safe’ refers to consistent transaction management system especially when used by Joint Account holders (to avoid race conditions).<br/>
The term ‘secure’ refers to authorized Access by Users (Admin/ Customer) and no other person can view/ access the data of concern.<br/>
</p>
$\bigstar$ &nbsp; <b><i>Features of OBMS are listed as follows:</i></b><br>
1. Handling multiple connections. i.e., Serving multiple clients simultaneously.<br/>
2. Socket programming is used to implement client-server model.<br/>
3. TCP connection for effective and reliable communication.<br/>
Storage/ Database: Unix File System. i.e., data is stored typically in files only.<br/><br/>
4. Concurrency:<br/>
&nbsp;&nbsp;&nbsp; a) File Locking (both mandatory and advisory locking) while operating files for consistency.<br/>
&nbsp;&nbsp;&nbsp; b) Semaphores protection as mutex/ binary locks for Critical Section of the code especially while adding new transactions, accounts, etc.,<br/>
5. Only system calls are used directly, avoiding use of library functions. <br/>
6. System calls related to process management,  file management, file locking, multi-threading and inter process communication mechanisms were used.<br/>
7. Password protected login system for both admins and customers.<br/>
$\bigstar$ &nbsp; <b><i>Operational Workflow:</i></b><br>
<img src="./images/ss2.png" width="" height=""><br/>
$\bigstar$ &nbsp; <b><i>Functionalities:</i></b><br>	
Admin Module: Administrator after successful login, able to do the following operations<br/>
1.Add new Customer Details,<br/>
2.Add new Accounts (Regular or Joint) for the above generated customers,<br/>
3.View the Customer Details,<br/>
4.View the Account Details,<br/>
5.Activate and Block Accounts for Transaction hold/ block<br/>
6.Update/ Modify the Account, Customer Details.<br/><br/>

Customer Module: Customer after login, can do any of these operations,<br/>
1.Add New Transaction ( Deposit/ Withdraw ),<br/>
2.View Transactions generated above,<br/>
3.Update Password,<br/>
4.Check the balance(s) of all the accounts he/she had.<br/>
<br><br>
$\bigstar$ &nbsp; <b><i>Implementation:</i></b><br>
a. getters and setters folder deals all operations related to getting and setting of data into files (database).<br/>
b. handles folder include method definitions to: <br>
&nbsp;&nbsp;&nbsp; 1. ```add.h``` - add new accounts, customers and transactions.<br/>
&nbsp;&nbsp;&nbsp; 2. ```view.h ```- view accounts, customers and transactions. <br/>
&nbsp;&nbsp;&nbsp; 3. ```login.h``` - login handler <br/>
&nbsp;&nbsp;&nbsp; 4. ```menu.h``` - menu handler <br/>
c. records-header file includes all structures of defined types in this system.<br/>
d. ```my_client.c``` - client program, specific to a user process.<br/>
e. ```my_server.c``` - server program, handles multiple client requests.<br/>

$\bigstar$ &nbsp; <b><i>Execution:</i></b><br>
Server Program could be run on background or in another terminal 
by the command ```./server.out ```<br/>
since, we already saved executable code in <br> ```server.out``` by the command, <br> ```gcc my_server.c -o server.out ```<br> and of ```client.out``` by the command, <br> ```gcc my_client.c -o client.out```
<br/>
similarly, run client program by the command ```./client.out```
<br/>
Admin default credentials
<br>Login-Id: ```iiitb```
<br>Password: ```sslab```
<br>Customer Login-Id Format: ```OBMS-{cust-Id.}```
<br/><br/>
$\bigstar$ &nbsp; <b><i>Notable Results:</i></b><br>
Only Valid User/ Customer can login in only one terminal at a time (only one active session)<br>
Multiple Users can login at a time and OBMS serves them efficiently <br>
Joint Account holders can access the account at the same time but, will be blocked if the other joint account holder is making a transcation. After it is done he/she can continue.<br/>
***
<br> 
$$Report\ by$$
$$Boppana\ Venkatesh$$
$$MTech\ CSE\ [MT2022140]$$
$$International\ Institute\ of\ Information\ Technology,\ Bangalore.$$
