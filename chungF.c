/*********************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the
concepts learned. I have constructed the functions and their respective algorithms and corresponding code by
myself. The program was run, tested, and debugged by my own efforts. I further certify that I have not copied in
part or whole or otherwise plagiarized the work of other students and/or persons.
 Ford Ainstein Chung, DLSU ID# 12205826
*********************************************************************************************/

/*
	This program is called a gummiphone, the gummiphone is a offline messenger that can communicate with other user. 
	It lets you create accounts and lets you create different kinds of messages. 
	You can add different users in a Personal connecion and it lets you sent messages in a group. 
	You can also send personal messages to a specific user and announce a message that will be sent to everyone. For the admin,
	the admin can delete lock, unlock, reset password and other more features related to users, the admin can also view and 
	delete messages and etc. Overall this program works offline using one device that can send messages to different users.

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define BLUE "\x1b[36m"
#define PURPLE "\033[0;35m"

#define MAX_MESS 1005
#define MAX_STR 55
#define MAX_USER 30
#define MAX_PASS 21
#define MAX_SEC 101

//typdef variables
typedef char mess[MAX_MESS];
typedef char string[MAX_STR];
typedef char QA[10][MAX_SEC];

//structure for admin Datas
typedef struct
{
	string password;			//password of the admin
	int PassReq[MAX_USER];		//contains the ID of the user who requested to reset password
	int nPassReq;				//number of user requested to reset password
}admin;

//structure for global Datas
typedef struct
{
	int nMessages;				//number of messages
	int nAnnouncement;			//number of the annoucements
	int announcement[MAX_MESS];	//contains the IDs of the annoucements
	admin admin;				//structure of admin 
	int nUsersID;				//number of used ID's and holder for ID's
	int nMessID;				//number of used ID for messages
}global;

//structure for connections of accounts
typedef struct
{
	int n;						//total Number of Connections of user
	int connected[MAX_USER];	//Array that Contains the ID of users in Connection
}group;

//structure for Message Datas
typedef struct
{
	int id;						//ID of the message				
	int type;					//Type of Messages, 1 for PM , 2 for PC, 3 for Annoucement
	int sender;					//ID of sender of message
	int reciever;				//ID of reciever of message
	string subject;				//subject of message
	mess message;				//Message itself
}message[1000];

//Array Structure for users INFO
typedef struct 
{
	int id;							//ID of User
	mess description;				//Description of User
	int status;						//Status of User, 0 for unlocked, 1 for locked, 2 for password change
	string name;					//name of user
	string username;				//username of user
	char password[MAX_PASS];		//password of user
	char securityQuestion[MAX_SEC];	//security question asked to user
	char securityAns[MAX_SEC];		//answer of user to the security question
	group connection;				//strucutre for the personal connection infos
	int sent[MAX_MESS];				//array containing the ID of the sent messages
	int nSent;						//number of sent messages
	int inbox[MAX_MESS];			//array containing the ID of the recieved messages
	int nInbox;						//number of recieved messages
	
}account[MAX_USER];

/*****************************************
saving data in accounts .txt list by line

	-1st line of the whole txt is the number of accounts
	
	1. user id - the index or id of the user
	2. user status - status of account, lock or unlocked
					- 0 unlocked and otherwise locked
					- 1 if locked
					- 2 if password needs to get changed before logging in
	3. name - name of the user
	4. username - username of the user
	5. password - encrypted message
	6. security question - the question given to the user when creating
	7. securityAns - the answer to the security question
	8. description - the description of the user to themselves, default user is the default
	9. nSent - the number of sent messages to the messenger
		-the next numbers is based on the number of sent
		-it is the id of the message
	10. nInbox -number of inboxed message and the next numbers are just the same with the nSent

message.txt guide line by line values

	-1st line contains the number of message in the app
	-2nd line of the whole code contain num of announcement

	parts if the message is PM - type = 1
		1. ID of the message
		2. username of the sender
		3. username of the reciever
		4. subject 
		5. message
		6. type of message ex pm for 1
		
	parts if the message is PC - type = 2
		1. ID of the message
		2. username of the sender
		3. ID of the PC
		4. subject 
		5. message
		6. type of message = 2
	parts of announcement type = 3
		1. ID of the message
		2. username of the sender
		3. reciever - Everyone
		4. subject 
		5. message
		6. type of message = 3

admin.txt guid line per line
	1. Admin password
	2. number of users who requested password rest
	3. lines that contain the ID of the users Indicated above
	
security Questions.txt line per line
	1. number of security question that exist
	2. the security questions

connections.txt guide line per line
	1.every first number corresponds to the number of user in the Personal conncetion
	2. next are the ID of the users, limit is the number 1.
	3. loop
******************************************/

//test cases for 20, 100, 1000, contains 1 more for all so its invalid
//ybozvldiafqtskunxjepmghcrwpyzvbrkjnmxdgfuhscteqoawliytnpsuxehrmldqgkcjafoinwbptyvszxfdasfasfasfasdfas
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
////asdfajsdhfjahsfajdfss
//asdfajsdhfjahsfajdfsskjfasdfdsjkfdksfdsjjfkdjskjfds


/*
	encrypt() encrypt the string at its parameter by 3 ascii places
	@param pass - contains word to encrypt
*/
void encrypt(string pass)
{
	int i;
	
	for(i = 0; i < strlen(pass); i++)
	{
		pass[i] = pass[i] + 3;
	}
}

/*
	decrypt() decrypt the string at its parameter by minus 3 ascii places
	@param pass - contains word to encrypt
*/
void decrypt(string pass)
{
	int i;
	
	for(i = 0; i < strlen(pass); i++)
	{
		pass[i] = pass[i] - 3;
	}	
}

/*
	userFinder() gets the ID of the user and reutrn the Index of user
	@user - structure array containing the Info of users
	@n -	contains the number of users in the Program
	@ID -	contains the ID of the user need to be found
	@returns the Index of the user in the user[]
*/
int userFinder(account user, int ID, int n)
{
	int nIndex;
	int found = 0;
	int i;
	
	for(i = 0; i < n && !found; i++)
	{
		if(user[i].id == ID)
		{
			nIndex = i;	
			found = 1;
		} 
	}
	
	return nIndex;
}

/*
	messIndFinder() gets the ID of messages and return its index in the array
	@message - structure array containing the Info of messages
	@global	- structure for global datas / miscellaneous
	@ID -	contains the ID of the message need to be found
	@returns the Index of the user in the user[]
*/
int messIndFinder(message message, global global, int nID)
{
	int i;
	int nIndex, nFound = 0;
	
	for(i = 0;i < global.nMessages && !nFound; i++)
	{
		if(message[i].id == nID)
		{
			nIndex = i;
			nFound = 1;
		}
	}
	
	return nIndex;
}

/*
	displayMessageInfo() Displays the Datas of a message
	@user - structure array containing the Info of users
	@message - structure array containing the Info of messages
	@n -	contains the number of users in the Program
	@global	- structure for global datas / miscellaneous
	@ID -	contains the ID of the message need to be found
	@messInd - Index of the messages needed to be displayed
*/
void displayMessageInfo(account user, int n, message message, global global, int messInd)
{
	int nIndex;
	int i, j;
	int nCount;
	
	system("cls");
		
	printf("MESSAGE INFO\n\n");
	
	if(message[messInd].type == 1)
	{
		printf("Personal Message\n\n");
	}
	else if(message[messInd].type == 2)
	{
		printf("Personal Connection\n\n");
	}
	else if(message[messInd].type == 3)
	{
		printf("Annoucement\n\n");
	}
	
	//sender
	printf(BLUE "Sender: \n" RESET);
	if(message[messInd].sender != -1)
	{
		printf("%s\n\n", user[userFinder(user, message[messInd].sender, n)].username);
	}
	else
	{
		printf("DELETED USER.\n\n");
	}
	
	
	//recipient
	printf(BLUE "Recipients: \n" RESET);
	
	nIndex = userFinder(user, message[messInd].reciever, n);
		
	//display the recipients of the message
	if(message[messInd].type == 3)
	{
		for(i = 0; i < n; i++)
		{
			printf("[%d] %s\n", i, user[i].username);
		}
	}
	else if(message[messInd].type == 1)
	{
		printf("%s\n", user[nIndex].username);
	}
	else if(message[messInd].type == 2)
	{
		nCount = 0;
		for (i = 0; i < user[nIndex].connection.n; i++)
		{
			if(message[messInd].sender != user[nIndex].connection.connected[i])
			{
				for(j = 0; j < user[userFinder(user, user[nIndex].connection.connected[i], n)].nInbox; j++)
				{
					if(message[messInd].id == user[userFinder(user, user[nIndex].connection.connected[i], n)].inbox[j])
					{
						printf("[%d] %s\n", i, user[userFinder(user, user[nIndex].connection.connected[i], n)].username);
						nCount++;
					}
				}
				
			}
		}
		
		if(nCount == 0)
		{
			printf("[NO RECIPIENT]\n");
		}
	}
	else if(message[messInd].type == -1)
	{
		nCount = 0;
		
		for(i = 0; i < n; i++)
		{
			for(j = 0; j < user[i].nInbox; j++)
			{
				if(message[messInd].id == user[i].inbox[j])
				{
					printf("[%d] %s\n", nCount, user[i].username);
					nCount++;
				}
			}
		}
	}
	

	//more info
	printf(BLUE "\nSubject: \n" RESET);
	printf("%s\n\n", message[messInd].subject);
	printf(BLUE "Message: \n" RESET);
	printf("%s\n\n", message[messInd].message);
	printf("\n---------------------------------\n\n");
}

/*
	saveAdminInfo() saves the information of the parameters to admin.txt
	@user - structure array containing the Info of users
	@n -	contains the number of users in the Program
	@global	- structure for global datas / miscellaneous
*/
void saveAdminInfo(account user, int n, global global)
{
	FILE *pFile;
	int i;
	
	pFile = fopen("admin.txt", "wt");
	
	if(pFile != NULL)
	{
		fprintf(pFile, "%s\n", global.admin.password);	//password
		fprintf(pFile, "%d\n", global.admin.nPassReq);	//number of person who sent message
		
		for(i = 0; i < global.admin.nPassReq; i++)
		{
			fprintf(pFile, "%d\n", global.admin.PassReq[i]);
		}
		
		fprintf(pFile, "%d\n", global.nUsersID);
		fprintf(pFile, "%d\n", global.nMessID);
		
		fclose(pFile);
	}
	else
	{
		printf(RED "NOT ENOUGH STORAGE\n" RESET);
	}
}

/*
	saveData() saves the Data of the account user to the account.txt
	@user - structure array containing the Info of users
	@n -	contains the number of users in the Program
*/
void saveData(account user, int nUser)
{
	FILE *pFile;
	int i, j;
	
	pFile = fopen("accounts.txt", "w");
	
	if(pFile != NULL)
	{
		fprintf(pFile, "%d\n", nUser);
		
		//saves the user data in the txt file
		for (i = 0; i < nUser; i++)
		{
			fprintf(pFile, "%d\n", user[i].id);
			fprintf(pFile, "%d\n", user[i].status);
			fprintf(pFile, "%s\n", user[i].name);
			fprintf(pFile, "%s\n", user[i].username);
			fprintf(pFile, "%s\n", user[i].password);
			fprintf(pFile, "%s\n", user[i].securityQuestion);
			fprintf(pFile, "%s\n", user[i].securityAns);
			fprintf(pFile, "%s\n", user[i].description);
			
			fprintf(pFile, "%d\n", user[i].nSent);
			for(j = 0; j < user[i].nSent; j++)
				fprintf(pFile, "%d\n", user[i].sent[j]);
			
			
			fprintf(pFile, "%d\n", user[i].nInbox);
			for(j = 0; j < user[i].nInbox; j++)
				fprintf(pFile, "%d\n", user[i].inbox[j]);
		}
		
		fclose(pFile);
	}
	else printf(RED "Not Enough storage" RESET);
	
	
	//saves personal connection data
	pFile = fopen("connections.txt", "w");
	
	if(pFile != NULL)
	{
		for (i = 0; i < nUser; i++)
		{
			fprintf(pFile, "%d\n", user[i].connection.n);
			
			for (j = 0; j < user[i].connection.n; j++)
			{
				fprintf(pFile, "%d\n", user[i].connection.connected[j]);
			}
				
		}
		fclose(pFile);
	}
	else printf(RED "Not Enough storage" RESET);	
}

/*
	inputPassword() ask for the password of a new account
	@user - structure array containing the Info of users and where the password will be saved
	@n -	contains the number of users in the Program
*/
void inputPassword(account user, int n)
{
	char pass[100];
	char rePassword[100];
	int nValid = 0;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		nValid = 0;
		do
		{
			printf("%25s", "Create Account ^w^\n");
			printf("\n---------------------------------\n\n");
			printf("Enter Name: %s\n", user[n].name);
			printf("Enter Username: %s\n", user[n].username);
			
			//input password
			fflush(stdin);
			printf("Password (MAX LENGTH 20): ");
			fgets(pass, 100, stdin);
			pass[strlen(pass) - 1] = '\0';
			
			if (strlen(pass) > 20 || strlen(pass) == 0)
			{
				system("cls");
				printf(RED "Error: INVALID PASSWORD\n" RESET);
				printf("---------------------------------\n\n");
			}	
			
		}while(strlen(pass) > 20 || strlen(pass) == 0);
		
		system("cls");
		
		printf("\n---------------------------------\n\n");
	
		printf("%25s", "Create Account ^w^\n");
		printf("\n---------------------------------\n\n");
		printf("Enter Name: %s\n", user[n].name);
		printf("Enter Username: %s\n", user[n].username);
		
		//re-enter password
		fflush(stdin);
		printf("Re-enter Password: ");
		fgets(rePassword, 100, stdin);
		rePassword[strlen(rePassword) - 1] = '\0';
		
		if(strcmp(pass, rePassword) == 0)
		{
			//save the password
			nValid = 1;
			encrypt(pass);
			strcpy(user[n].password,pass);
		}
		else
		{
			system("cls");
			printf(RED "Error: PASSWORD DONT MATCH\n" RESET);
		}
			
		printf("---------------------------------\n\n");
	}while(!nValid);
	
}

/*
	inputName() ask for the name of a new user
	@user - structure array containing the Info of users and where the name will be saved
	@n -	contains the number of users in the Program
*/
void inputName(account user, int n)
{
	string newName;
	int nValid;
	int i;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		fflush(stdin);
		nValid = 1;	
		printf("%25s", "Create Account ^w^\n");
		printf("\n---------------------------------\n\n");
		printf("Enter Name [MAX 50] with \nNO SPECIAL CHARACTERS\n");
		printf("\n---------------------------------\n\n");
		printf("Name: ");
		
		//enter name
		fgets(newName, 55, stdin);
		newName[strlen(newName) - 1] = '\0';	
		
		if(strlen(newName) <= 50 && strlen(newName) >= 1)
		{
			for(i = 0; i < strlen(newName) && nValid; i++)
			{
				if(!((newName[i] >= 'a' && newName[i] <= 'z') || (newName[i] >= 'A' && newName[i] <= 'Z') || newName[i] == ' '))
				{
					nValid = 0;
				}
			}
		}
		else
		{
			nValid = 0;
		}
		
		if(!nValid)
		{
			system("cls");
			printf(RED "ERROR: INVALID NAME" RESET);
			printf("\n---------------------------------\n\n");
		}
	}while(!nValid);
	
	
	strcpy(user[n].name, newName);
}

/*
	inputUsername() ask for the username of a new user
	@user - structure array containing the Info of users and where the username will be saved
	@n -	contains the number of users in the Program
*/
void inputUsername(account user, int n)
{
	string sTemp;
	int nValid;
	int i;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		nValid = 1;
		
		printf("%25s", "Create Account ^w^\n");
		printf("\n---------------------------------\n\n");
		printf("Enter Username [MAX 20] with \nNO SPACES\n");
		printf("\n---------------------------------\n\n");
		printf("Username: ");
		
		//enter username
		fflush(stdin);
		fgets(sTemp, sizeof(sTemp), stdin);
		sTemp[strlen(sTemp) - 1] = '\0';
		
		//checks if lessthan 20
		if(strlen(sTemp) <= 20 && strlen(sTemp) >= 1)
		{
			//checks if there is a space
			for(i = 0; i < strlen(sTemp) && nValid; i++)
			{
				if(sTemp[i] == ' ')
				{
					nValid = 0;
				}
			}
		}
		else
		{
			nValid = 0;
		}
		
		if(nValid)
		{
			for (i = 0; i < n && nValid; i++)
			{
				if(strcmp(user[i].username, sTemp) == 0)
					nValid = 0;
			}
			
			if(!nValid)
			{
				system("cls");
				printf(RED "ERROR: Username already taken\n" RESET);
				printf("---------------------------------\n\n");
			}
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID USERNAME\n" RESET);
			printf("---------------------------------\n\n");
		}
	}while(!nValid);
	
	strcpy(user[n].username, sTemp);
}

/*
	securityQuestions() ask the user a question and they need to answer it within the constraints
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the Program
	@questions	- Contains the security questions
	@nQuestions	- contains the number of questions
*/
void securityQuestions(account user, int n, QA questions, int *nQuestions)
{
	char sTemp[200];
	int randIndex;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%25s", "Create Account ^w^\n");
		printf("\n---------------------------------\n\n");
		
		//randomizer
		srand(time(NULL));
		randIndex = rand() % *nQuestions;
		
		//this is just temporary  question, make a file text file and access it randomly
		printf("Answer the security question:\n\n");
		strcpy(user[n].securityQuestion, questions[randIndex]);
		printf("%s\n\n", user[n].securityQuestion);
		
		printf("Answer (MAX 100 WORDS):\n\n");
		fgets(sTemp, sizeof(sTemp), stdin);
		sTemp[strlen(sTemp) - 1] = '\0';
		
		if(strlen(sTemp) > 100)
		{
			system("cls");
			printf(RED "ERROR: EXCEEDS LIMIT\n" RESET);
			printf("---------------------------------\n\n");
		}
		else if(strlen(sTemp) == 0)
		{
			system("cls");
			printf(RED "ERROR: ENTER A ANSWER\n" RESET);
			printf("---------------------------------\n\n");
		}

	}while(strlen(sTemp) > 100 || strlen(sTemp) == 0);
	
	strcpy(user[n].securityAns, sTemp);
}

/*
	createAccount() create account page and calls the function for getting the information
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the Program gets appended 1 ever new user
	@questions	- Contains the security questions
	@nQuestions	- contains the number of questions
	@global	- structure for global datas / miscellaneous, contains the availabe ID of user
	
*/
void createAccount(account user, int *n, QA questions, int *nQuestions, global *global)
{
	if (*n < MAX_USER)
	{
		printf("\n---------------------------------\n\n");
		printf("%25s", "Create Account ^w^\n");
		printf("\n---------------------------------\n\n");
		
		//call the function that gets the data needed
		user[*n].id = global->nUsersID;
		global->nUsersID++;
		inputName(user, *n);
		inputUsername(user, *n);
		inputPassword(user, *n);
		securityQuestions(user, *n, questions, nQuestions);
		user[*n].connection.n = 1;
		user[*n].connection.connected[0] = user[*n].id;
		user[*n].nSent = 0;
		user[*n].nInbox = 0;
		strcpy(user[*n].description, "DEFAULT USER");
		user[*n].status = 0;
		(*n)++;
		
		saveData(user, *n);
		saveAdminInfo(user, *n, *global);
		system("cls");
		printf(BLUE "SUCCESSFULLY CREATED ACCOUNT <3" RESET);
	}
	else
	{
		system("cls");
		printf(RED "THE MESSENGER IS FULL" RESET);
	}

}

/*
	addPersonalConnections() adds a user in the personal connection of the nIndex
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
	@sTemp - sTemp contains the username of the user that is gonna be added if it exist
*/
void addPersonalConnections(account user, int n, int nIndex, string sTemp)
{
	int i, j;
	int nCount, nValid = 0;
	int nID;
	
	nCount = user[nIndex].connection.n;
					
	system("cls");
	
	for(i = 0; i < n; i++)
	{
		if(strcmp(sTemp, user[i].username) == 0)
		{
			nID = user[i].id;
		}
	}
	
	for (i = 0; i < n && !nValid; i++)
	{
		//if you added yourself
		if(strcmp(user[nIndex].username, sTemp) != 0)
		{
			if (strcmp(user[i].username, sTemp) == 0)
			{	
				//scan for you friendlist if you already added the person
				for(j = 0; j < nCount && !nValid; j++)
				{
					if(nID == user[nIndex].connection.connected[j])
					{
						system("cls");
						printf(RED "ERROR: USER ALREADY ADDED\n" RESET);
						printf("---------------------------------\n\n");
						nValid = 1;
					}
				}
				
				//if not yet added then add the user
				if(!nValid)
				{
					nValid = 1;
					
					user[nIndex].connection.connected[nCount] = nID;
					
					nCount++;
					user[nIndex].connection.n = nCount;
					saveData(user, n);
					system("cls");
					printf(BLUE "ADDED SUCCESSFULLY\n" RESET);
					printf("---------------------------------\n\n");
					
				}	
			}	
		}
		else
		{
			system("cls");
			printf(RED "ERROR: YOU CAN\'T ADD YOURSELF\n" RESET);
			printf("---------------------------------\n\n");
			
			nValid = 1;
		}
		
	}
	
	//if user doesnt exit or if the account is yours
	if(nValid == 0)
	{
		system("cls");
		printf(RED "ERROR: USER DONT EXIST\n" RESET);
		printf("---------------------------------\n\n");
	}
		
}

/*
	addPCModule() contains the menu for adding PC and depending on the choice it will call the function on that number
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
*/
void addPCModule(account user, int n, int nIndex)
{
	int i;
	string sTemp;
	int nOpt, nTemp;
	
	system("cls");
	do
	{
		printf("Add Personal Connections\n\n");
		printf("Option: \n");
		printf("[1] Enter username\n");
		printf("[2] Select from list of Users\n");
		printf("[3] Exit\n");
		printf("\nEnter Choice: ");
		scanf("%d", &nOpt);
		fflush(stdin);
		
		if (nOpt == 1)		//add by username
		{
			system("cls");
			printf("Add Personal Connections\n\n");
			printf("Username: ");
			fgets(sTemp, sizeof(sTemp), stdin);
			sTemp[strlen(sTemp) - 1] = '\0';
			
			addPersonalConnections(user, n, nIndex, sTemp);					
		}
		else if(nOpt == 2)	//add using a list
		{
			system("cls");
			printf("Add Personal Connections\n\n");
			
			for(i = 0; i < n; i++)
			{
				printf("[%d] %s\n", i, user[i].username);
			}
			printf("\nEnter ID: ");
			scanf("%d", &nTemp);
			
			//check if valid
			if(nTemp >= 0 && nTemp < n)
			{
				//add the user
				addPersonalConnections(user, n, nIndex, user[nTemp].username);
			}
			else
			{
				system("cls");
				printf(RED "ERROR: INVALID INPUT\n" RESET);
			}
			
		}
		else if(nOpt == 3)	//exit the adding area
		{
			system("cls");
		}
		else				//invalid input
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT\n" RESET);
		}
	}while(nOpt != 3);
				
}

/*
	removePC() removes the user in the parameter int remove in the current logged in user's Personal connection
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
	@message - contains the messages information
	@global - structure for global datas / miscellaneous, contains the availabe ID of user and avaialbe ID messages
*/
void removePC(account user, int n, int nIndex, message message, global *global, int remove)
{
	int i, j, k;
	int nTemp;
	char cTemp;
	int nFound;
	
	system("cls");
	fflush(stdin);
	printf("\n---------------------------------\n\n");
	printf("Remove Personal Connections\n\n");
	printf("\n---------------------------------\n\n");
	
	//if user is valid
	
	printf("Are you sure you want to remove \"%s\"?\n", user[userFinder(user, user[nIndex].connection.connected[remove], n)].username);
	printf("[Y][N]: ");
	scanf(" %c", &cTemp);
	
	// user delete
	if(cTemp == 'Y' || cTemp == 'y')
	{
		//move everything by removing the user
		for (i = remove; i < user[nIndex].connection.n - 1; i++)
		{
			user[nIndex].connection.connected[i] = user[nIndex].connection.connected[i + 1]; 
		}
		
		user[nIndex].connection.connected[user[nIndex].connection.n] = '\0';
		user[nIndex].connection.n--;
		
		//find the Index of the user
		
		for(i = 0; i < n; i++)
		{
			if(user[i].id == remove)
			{
				nTemp = i;	
			}
		}
			
		
		//check the words that is for a PC
		for(i = 0; i < global->nMessages; i++)
		{
			//if personal message
			if(message[i].type == 2)
			{
				//compare if the sender is the pc owner
				if(user[nIndex].id == message[i].reciever)
				{
					
					nFound = 0;
					
					//check if the message is in the removed personal connections inbox
					for (j = 0; j < user[nTemp].nInbox && !nFound; j++)
					{
						//if nakita sa inbox then remove by moving the messages inside the inbox to replace the mess
						if(user[nTemp].inbox[j] == message[i].id)
						{
							nFound = 1;
							
							for(k = j; k < user[nTemp].nInbox - 1; k++)
							{
								user[nTemp].inbox[k] = user[nTemp].inbox[k + 1];
							}
							
							user[nTemp].inbox[k] = '\0';
							user[nTemp].nInbox--;
						}
					}
				}
			}
		}

		saveData(user, n);
		
		system("cls");
		printf(BLUE "User Removed" RESET);
		printf("\n---------------------------------\n\n");
		
	}
	else
	{
		system("cls");
		printf(RED "User Not Removed" RESET);
		printf("\n---------------------------------\n\n");
	}
			

}

/*
	modifyPersonalConnections() menu where the user can modify or view their personal connections
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
	@message - contains the messages information
	@global - structure for global datas / miscellaneous, contains the availabe ID of user and avaialbe ID messages
*/
void modifyPersonalConnections(account user, int n, int nIndex, global *global, message message)
{
	int nOpt; //nOpt2;
	string sTemp;
	int i, j;
	int nCount, nTemp;
	
	
	system("cls");
	printf("\n---------------------------------\n\n");
	do
	{
		printf("%29s", "Modify Person Connections\n");
		printf("\n---------------------------------\n\n");
		printf("[1] Add Personal connections\n");
		printf("[2] View personal Connections\n");
		printf("[3] Remove personal Connections\n");
		printf("[4] Personal Connection you are in\n");
		printf("[5] Exit\n\n");
		fflush(stdin);
		printf("Enter Choice: ");
		scanf("%d", &nOpt);
		
		
		if (nOpt == 1)	//add users
		{
			//add user
			addPCModule(user, n, nIndex);
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");	
		}
		else if(nOpt == 2)	//display personal connections
		{
			system("cls");
			printf("Personal Connections\n\n");
			
			//display the personal connection
			for (i = 1; i < user[nIndex].connection.n; i++)
			{
				printf("[%d] %s\n", i, user[userFinder(user, user[nIndex].connection.connected[i], n)].username);
			}
			
			printf("\nType anything to exit: ");
			scanf("%s", sTemp);
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}	
		else if(nOpt == 3)	//remove personal connections
		{
			//call remove function
			system("cls");
			
			fflush(stdin);
			printf("Remove Personal Connections\n\n");
			printf("Enter \"-1\" to Exit\n\n");
			
			for (i = 1; i < user[nIndex].connection.n; i++)
			{
				printf("[%d] %s\n", i, user[userFinder(user, user[nIndex].connection.connected[i], n)].username);
			}
			
			printf("\nEnter: ");
			scanf("%d", &nTemp);

			//call function to remove user
			
			if(nTemp != -1 && nTemp >= 1 && nTemp < i)
			{
				nTemp = userFinder(user, user[nIndex].connection.connected[nTemp], n);
				removePC(user, n, nIndex, message, global, nTemp);
				printf("---------------------------------\n\n");
			}
			else if(nTemp == -1)
			{
				system("cls");
				printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
				printf("---------------------------------\n\n");
			}
			else
			{
				system("cls");
				printf(RED "ERROR: INVALID INPUT\n" RESET);
				printf("---------------------------------\n\n");
			}
			
		}
		else if(nOpt ==  4)
		{
			system("cls");
			printf("List of users that have you in their\n");
			printf("%32s", "their Personal connection\n\n");
			nCount = 0;
			for(i = 0; i < n; i++)
			{
				if(i != nIndex)
				{
					for(j = 0; j < user[i].connection.n; j++)
					{
						if(user[i].connection.connected[j] == user[nIndex].id)
						{
							nCount++;
							printf("[%d] %s\n", nCount, user[i].username);	
						}
					}
				}
			}
			
			if(nCount == 0)
			{
				printf(BLUE "%26s\n" RESET, "[NO ONE ADDED YOU]");
			}
			printf("\nEnter anything to exit: ");
			scanf("%s", sTemp);
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else if(nOpt == 5)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT\n" RESET);
			printf("---------------------------------\n\n");
		}
	}while(nOpt != 5);
}

/*
	saveMessages() save the messages data in the message.txt
	@message - contains the messages information
	@global - structure for global datas / miscellaneous, contains the availabe ID of user and avaialbe ID messages and other
*/
void saveMessages(message message, global *global)
{
	FILE *pFile;
	int i;
	
	pFile = fopen("message.txt", "w");
	
	if(pFile != NULL)
	{
		fprintf(pFile, "%d\n", global->nMessages);
		fprintf(pFile, "%d\n", global->nAnnouncement);
		
		for(i = 0; i < global->nMessages; i++)
		{
			fprintf(pFile, "%d\n", message[i].id);
			fprintf(pFile, "%d\n", message[i].type);
			fprintf(pFile, "%d\n", message[i].sender);
			fprintf(pFile, "%d\n", message[i].reciever);
			fprintf(pFile, "%s\n", message[i].subject);
			fprintf(pFile, "%s\n", message[i].message);
		}
		fclose(pFile);
	}
	else
		printf("Not enough storage");
	
}

/*
	composePM() function called to create a personal message by asking for user inputs
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
	@message - contains the messages information and where the messages will be saved
	@global - structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
	@nReciever - contains the ID of the reciever of the message
*/
void composePM(account user, int n, int nIndex, message message, global *global, int nReciever)
{
	string sTemp;
	int nTemp = nReciever;
	char inputMess[1005];
	int messCount;
	int length, nValid;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		nValid = 1;
		printf("%28s", "Compose Personal Message\n");
		printf("\n---------------------------------\n\n");
		fflush(stdin);	
		printf("Send to: \"%s\"\n", user[nReciever].username);
		
		//enter the subject
		printf("Subject [MAX 50]: ");
		fgets(sTemp, 55, stdin);
		sTemp[strlen(sTemp) - 1] = '\0';
		
		if(strlen(sTemp) > 50)
		{
			nValid = 0;
			system("cls");
			printf(RED "ERROR: EXCEEDS 50 CHARACTERS\n" RESET);
			printf("---------------------------------\n\n");
		}
		else if(strlen(sTemp) == 0)
		{
			nValid = 0;
			system("cls");
			printf(RED "ERROR: ENTER A SUBJECT\n" RESET);
			printf("---------------------------------\n\n");
		}
		
	}while(!nValid);
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		nValid = 1;
		
		printf("%28s", "Compose Personal Message\n");
		printf("\n---------------------------------\n\n");
		fflush(stdin);	
		printf("Send to: \"%s\"\n", user[nReciever].username);
		printf("Subject: %s\n\n", sTemp);
		
		//type in the message
		printf("Compose message [MAX 1000 characters]:\n\n");
		fgets(inputMess, 1005, stdin);
		inputMess[strlen(inputMess) - 1] = '\0';
		
		
		length = strlen(inputMess);
		
		if(length <= 1000 && length >= 1)
		{
			//initialize the count
			messCount = global->nMessages;
			
			//save the info of the message
			message[messCount].id = global->nMessID;
			message[messCount].sender = user[nIndex].id;
			message[messCount].reciever = user[nReciever].id;
			strcpy(message[messCount].subject, sTemp);
			strcpy(message[messCount].message, inputMess);
			message[messCount].type = 1;
			
			global->nMessID++;
			
			//save the message to the user.sent
			user[nIndex].sent[user[nIndex].nSent] = message[messCount].id;
			user[nIndex].nSent++;
			
			//save the message to the recipient of the message
			user[nTemp].inbox[user[nTemp].nInbox] = message[messCount].id;
			user[nTemp].nInbox++;
			
			saveData(user, n);
			saveAdminInfo(user, n, *global);
			
			messCount++;
			global->nMessages = messCount;
			saveMessages(message, global);
			
			system("cls");
			printf(BLUE "MESSAGE SENT\n" RESET);
			printf("---------------------------------\n\n");
		
		}
		else
		{
			nValid = 0;
			system("cls");
			printf(RED "ERROR: INVALID MESSAGE\n" RESET);
			printf("---------------------------------\n\n");
		}
		
	}while(!nValid);
	
}

/*
	composeMessage() main menu where the user can create messages of personal, group PC and annoucements
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
	@message - contains the messages information and where the messages will be saved
	@global - structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
*/
void composeMessage(account user, int n, int nIndex, message message, global *global)
{
	int i, j;
	int nTemp, nValid = 0, nTemp2, messCount, nOpt;
	string sendTo, sTemp;
	char mess[MAX_MESS];
	int PC[n], nPC = 0;
	

	printf("\n---------------------------------\n\n");
	do
	{
		nTemp = 0;
		nValid = 0;
		fflush(stdin);
		printf("%25s", "COMPOSE MESSAGES\n");
		printf("\n---------------------------------\n\n");
		printf("Send to ?\n\n");
		printf("[1] Personal Message\n");
		printf("[2] Group Message\n");
		printf("[3] Announcement\n");
		printf("[4] Exit\n\n");
		printf("\n---------------------------------\n\n");
		printf("Enter send to: ");
		scanf("%d", &nOpt);
		
		if(nOpt == 1)
		{	
			system("cls");	
			printf("\n---------------------------------\n\n");
			printf("%28s", "Compose Personal Message\n");
			printf("\n---------------------------------\n\n");
			printf("Send to: ");
			fflush(stdin);
			fgets(sendTo, sizeof(sendTo), stdin);
			sendTo[strlen(sendTo) - 1] = '\0';
			
			//check if the user exist
			for(i = 0; i < n && !nValid; i++)
			{
				if(strcmp(user[i].username, sendTo) == 0)
				{
					nTemp = i;	
					nValid = 1;
				}
			}
			
			if(nValid)
			{
				composePM(user, n, nIndex, message, global, nTemp);
			}
			else
			{
				system("cls");
				printf(RED "ERROR: USER DOESN\'T EXIST\n" RESET);
				printf("---------------------------------\n\n");
			}	
		}
		else if(nOpt == 2)
		{
			
			system("cls");
			printf("\n---------------------------------\n\n");
			
			do
			{
				nPC = 0;
				printf("List of Personal Connections\n\n");
				printf("Instruction:\n\n");
				printf("[-1] Exit\n");
				printf("\n---------------------------------\n\n");
				
				//display list of personal connections
				for (i = 0; i < n; i++)
				{
					for(j = 0; j < user[i].connection.n; j++)
					{
						if(user[i].id == user[nIndex].connection.connected[j])
						{
							printf("[%d] %s Personal Connection\n", nPC, user[i].username);
							PC[nPC] = user[i].id;
							nPC++;
						}
					}
				}
				
				printf("\nChoice: ");
				scanf("%d", &nTemp);	
				
				if(nTemp >= 0 && nTemp < nPC)
				{
					nTemp = userFinder(user, PC[nTemp], n);
					
					fflush(stdin);
					system("cls");
					printf("\n---------------------------------\n\n");
					
					//enter subject
					do
					{
						nValid = 1;
						fflush(stdin);
						printf("%30s", "Personal Connection Message\n");
						printf("\n---------------------------------\n\n");
						
						printf("Send to %s Personal Connection\n\n", user[nTemp].username);
						//enter the subject
						printf("Subject [MAX 50]: ");
						fgets(sTemp, 55, stdin);
						sTemp[strlen(sTemp) - 1] = '\0';
						
						if(strlen(sTemp) > 50)
						{
							nValid = 0;
							system("cls");
							printf(RED "ERROR: EXCEEDS 50 CHARACTERS\n" RESET);
							printf("---------------------------------\n\n");
						}
						else if(strlen(sTemp) == 0)
						{
							nValid = 0;
							system("cls");
							printf(RED "ERROR: ENTER A SUBJECT\n" RESET);
							printf("---------------------------------\n\n");
						}
						
					}while(!nValid);
			
					system("cls");
					
					printf("\n---------------------------------\n\n");
					
					//enter message
					do
					{
						nValid = 1;
						
						printf("%27s", "Compose Group Message\n");
						printf("\n---------------------------------\n\n");
						fflush(stdin);	
						printf("Send to %s Personal Connection\n\n", user[nTemp].username);
						printf("Subject: %s\n\n", sTemp);
						
						//type in the message
						printf("Compose message [MAX 1000 characters]:\n\n");
						//enter the message to be send to the personal connection
						
						fgets(mess, sizeof(mess), stdin);
						mess[strlen(mess) - 1] = '\0';
						
						
						if(strlen(mess) <= 1000 && strlen(mess) >= 1)
						{
							messCount = global->nMessages;
					
							//save the info of the message
							message[messCount].id = global->nMessID;		//id of the message
							global->nMessID++;
							message[messCount].sender = user[nIndex].id;	//sender
							message[messCount].reciever = user[nTemp].id;	//recievers id of the PC owner
							strcpy(message[messCount].subject, sTemp);		//subject
							strcpy(message[messCount].message, mess);		//message
							message[messCount].type = 2;					//message type
							
							//save the message to the user.sent and update number of sent messages
							user[nIndex].sent[user[nIndex].nSent] = message[messCount].id;
							user[nIndex].nSent++;
							
							//find the index of the PC owner
							for(i = 0; i < n; i++)
							{
								if(user[i].id == PC[nTemp])
								{
									nTemp2 = i;
								}
							}
							
							//save the message to the recipient's inbox
							for(i = 0; i < n; i++)
							{
								for (j = 0; j < user[nTemp2].connection.n; j++)
								{
									if(user[nTemp2].connection.connected[j] != user[nIndex].id)
									{
										if(user[nTemp2].connection.connected[j] == user[i].id)
										{
											user[i].inbox[user[i].nInbox] = message[global->nMessages].id;
											user[i].nInbox++;
										}		
									}	
								}
							}
							
							saveData(user, n);
							saveAdminInfo(user, n, *global);
							
							messCount++;
							global->nMessages = messCount;
							saveMessages(message, global);
							
							system("cls");
							printf(BLUE "MESSAGE SENT\n" RESET);
							printf("---------------------------------\n\n");
						
						}
						else
						{
							nValid = 0;
							system("cls");
							printf(RED "ERROR: INVALID MESSAGE\n" RESET);
							printf("---------------------------------\n\n");
						}
						
					}while(!nValid);	
					
				}
				else if(nTemp == -1)
				{
					system("cls");
					printf(BLUE "RETURNED TO MESSAGE PAGE" RESET);
					printf("\n---------------------------------\n\n");
				}
				else
				{
					system("cls");
					printf(RED"ERROR: INVALID INPUT" RESET);
					printf("\n---------------------------------\n\n");
				}
				
			}while(nTemp != -1);
	
		}
		else if(nOpt == 3)
		{
			fflush(stdin);
			system("cls");
			printf("\n---------------------------------\n\n");
			
			do
			{
				nValid = 1;
				fflush(stdin);
				printf("%22s", "Annoucement\n");
				printf("\n---------------------------------\n\n");
				
				printf("Send to Everyone\n\n");
				
				//enter the subject
				printf("Subject [MAX 50]: ");
				fgets(sTemp, 55, stdin);
				sTemp[strlen(sTemp) - 1] = '\0';
				
				if(strlen(sTemp) > 50)
				{
					nValid = 0;
					system("cls");
					printf(RED "ERROR: EXCEEDS 50 CHARACTERS\n" RESET);
					printf("---------------------------------\n\n");
				}
				else if(strlen(sTemp) == 0)
				{
					nValid = 0;
					system("cls");
					printf(RED "ERROR: ENTER A SUBJECT\n" RESET);
					printf("---------------------------------\n\n");
				}
						
			}while(!nValid);
			
			system("cls");
					
			printf("\n---------------------------------\n\n");
			
			//enter message
			do
			{
				nValid = 1;
				
				printf("%27s", "Compose Announcement\n");
				printf("\n---------------------------------\n\n");
				fflush(stdin);	
				printf("Send to Everyone\n");
				printf("Subject: %s\n\n", sTemp);
				
				//type in the message
				printf("Compose message [MAX 1000 characters]:\n\n");
				//enter the message to be send to the personal connection
				
				fgets(mess, sizeof(mess), stdin);
				mess[strlen(mess) - 1] = '\0';
				
				
				if(strlen(mess) <= 1000 && strlen(mess) >= 1)
				{
					messCount = global->nMessages;
			
					//save the info of the message
					message[messCount].id = global->nMessID;
					message[messCount].sender = user[nIndex].id;
					global->nMessID++;
					message[messCount].reciever = user[nIndex].id;
					strcpy(message[messCount].subject, sTemp);
					strcpy(message[messCount].message, mess);
					
					message[messCount].type = 3;
					
					//save the message to the user.sent
					user[nIndex].sent[user[nIndex].nSent] = message[messCount].id;
					user[nIndex].nSent++;
					
					global->nAnnouncement++;
					
					saveData(user, n);
					saveAdminInfo(user, n, *global);
					
					messCount++;
					global->nMessages = messCount;
					saveMessages(message, global);
					
					system("cls");
					printf(BLUE "MESSAGE SENT\n" RESET);
					printf("---------------------------------\n\n");
				
				}
				else
				{
					nValid = 0;
					system("cls");
					printf(RED "ERROR: INVALID MESSAGE\n" RESET);
					printf("---------------------------------\n\n");
				}
				
			}while(!nValid);	
		}
		else if(nOpt == 4)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n" RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT\n" RESET);
			printf("---------------------------------\n\n");
		}
	}while(nOpt != 4);
}

/*
	sent() displays the list of messages that the user has sent, and they can view the messages info
	@user - structure array containing the Info of users and where the question and answer will be saved
	@n -	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
	@message - contains the messages information and where the messages will be saved
	@global - structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
*/
void sent(account user, int n, int nIndex, message message, global *global)
{
	int i;
	int messInd;
	int nOpt;
	string sTemp;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf(BLUE "%23s" RESET, "Sent Messages\n");
		printf("\n---------------------------------\n\n");
		printf("%22s", "[-1] Exit\n");
		printf("\n---------------------------------\n\n");
		
		
		//display sent messages
		for(i = 0; i < user[nIndex].nSent; i++)
		{
			messInd = messIndFinder(message, *global, user[nIndex].sent[i]);
			
			if(message[messInd].type == 1)
			{
				printf(BLUE "[%d] To: %s\n" RESET, i, user[userFinder(user, message[messInd].reciever, n)].username);
			}
			else if(message[messInd].type == 2)
			{
				if(message[messInd].reciever != -1)
				{
					printf(BLUE "[%d] To: %s Personal Connection\n" RESET, i, user[userFinder(user, message[messInd].reciever, n)].username);
				}
				else
				{
					printf(BLUE "[%d] To: Deleted User's Personal Connection\n" RESET, i);
				}
				
			}
			else if(message[messInd].type == 3)
			{
				printf(BLUE "[%d] To: Everyone\n" RESET, i);
			}
			
			printf("%s\n\n", message[messInd].subject);
		}
		
		printf("\n---------------------------------\n\n");
		printf("Number: ");
		fflush(stdin);
		scanf("%d", &nOpt);
		
		if(nOpt != -1 && nOpt >= 0 && nOpt < user[nIndex].nSent)
		{
			messInd = messIndFinder(message, *global, user[nIndex].sent[nOpt]);
			system("cls");
			displayMessageInfo(user, n, message, *global, messInd);
			printf("Enter Any Key to Exit: ");
			scanf("%s", sTemp);
			system("cls");
			printf(BLUE "RETURNED TO SENT\n"RESET);
			printf("---------------------------------\n\n");
		}
		else if(nOpt == -1)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED" INVALID INPUT \n"RESET);
			printf("---------------------------------\n\n");
		}
	}while(nOpt != -1);
	
	
}

/*
	reply() this function is called if the users wants to reply to a message, this basically reply to a specific message
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@nIndex 	- contains the index of the current user logged in
	@message 	- contains the messages information and where the messages will be saved
	@global 	- structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
	@messInd 	- contains the index of the message that the user wants to reply to
	@type		- contains the type of a message, 1 for PM, 2 for PC, 3 for annoucement
*/
void reply(account user, int n, int nIndex, message messages, global *global, int messInd, int type)
{
	char subject[100];
	int reciever, sender;
	char tempMess[1010];
	char cOpt;
	int nTemp;
	int i, j;
	
	system("cls");
	fflush(stdin);
		
	
	printf("\n---------------------------------\n\n");
	printf("%27s", "Replying to Message\n");
	printf("\n---------------------------------\n\n");
	
	sender = user[nIndex].id;
	
	//save the recipient for the type of message
	if(messages[messInd].type == 1)
	{
		reciever = messages[messInd].sender;
	}
	else
	{
		reciever = messages[messInd].reciever;
	}
	
	
	//subject is replied"subject"
	strcpy(subject, "Replied to \"");
	strcat(subject, messages[messInd].subject);
	strcat(subject, "\"");
	
	if(strlen(subject) > 50)
	{
		subject[50 - 3] = '.';
		subject[50 - 2] = '.';
		subject[50 - 1] = '.';
		subject[50] = '\0';
	}
	
	
	printf(BLUE "To: " RESET);
	if(type == 1)
	{
		printf("%s\n\n", user[userFinder(user, reciever, n)].username);
	}
	else if(type == 2)
	{
		printf("%s Personal Connection\n\n", user[userFinder(user, messages[messInd].reciever, n)].username);
	}
	else if(type == 3)
	{
		printf("Everyone\n\n");
	}
	
	
	printf(BLUE "Subject: " RESET);
	printf("%s\n\n", subject);
	
	//message
	printf(BLUE "Reply: \n\n" RESET);
	fgets(tempMess, 1005, stdin);
	tempMess[strlen(tempMess) - 1] = '\0';
	
	
	if(strlen(tempMess) <= 1000 && strlen(tempMess) >= 1)
	{
		//ask to send or not
	
		fflush(stdin);
		printf("\n---------------------------------\n\n");
		printf("Send Message [Y]es or [ANY KEYS EXCEPT Y] No\n\n");
		printf("Choice: ");
		scanf("%c", &cOpt);

		if(cOpt == 'y' || cOpt == 'Y')
		{	
			messages[global->nMessages].id = global->nMessID;			//ID
			global->nMessID++;
			messages[global->nMessages].type = messages[messInd].type;	//Types
			messages[global->nMessages].sender = sender;			//sender
			messages[global->nMessages].reciever = reciever;		//reciever
			strcpy(messages[global->nMessages].subject, subject);		//subject
			strcpy(messages[global->nMessages].message, tempMess);		//tempMess
			
			//save the message to the user.sent
			user[nIndex].sent[user[nIndex].nSent] = messages[global->nMessages].id;
			user[nIndex].nSent++;
			
			if(type == 1)
			{
				//contains the index of the user in the user array
				nTemp = userFinder(user, reciever, n);
				
				//save the message to the recipient of the message
				user[nTemp].inbox[user[nTemp].nInbox] = messages[global->nMessages].id;
				user[nTemp].nInbox++;
			}
			else if(type == 2)
			{
				//id of the owner of the PC
				nTemp = userFinder(user, reciever, n);
				
				//save the message to the recipient of the message
				for(i = 0; i < n; i++)
				{
					for (j = 0; j < user[nTemp].connection.n; j++)
					{
						if(user[nTemp].connection.connected[j] != user[nIndex].id)
						{
							if(user[nTemp].connection.connected[j] == user[i].id)
							{
								user[i].inbox[user[i].nInbox] = messages[global->nMessages].id;
								user[i].nInbox++;
							}		
						}	
					}
				}	
			}
			
			
			saveData(user, n);
			
			global->nMessages++;
			saveMessages(messages, global);
			saveAdminInfo(user, n, *global);
			
			system("cls");
			printf(BLUE "MESSAGE SENT\n" RESET);
			printf("---------------------------------\n\n");
		}
		else if(cOpt == 'N' || cOpt == 'n')
		{
			system("cls");
			printf(RED "MESSAGE NOT SENT\n" RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR INVALID INPUT\n" RESET);
			printf("---------------------------------\n\n");
		}
	}
	else
	{
		system("cls");
		printf(RED "ERROR: INVALID MESSAGE\n" RESET);
		printf("---------------------------------\n\n");
	}
	
}

/*
	inbox() displays the list of messages that the user has recieved that is filtered out by PM or PC, they can view the full details of the message
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@nIndex 	- contains the index of the current user logged in
	@message 	- contains the messages information and where the messages will be saved
	@global 	- structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
*/
void inbox(account user, int n, int nIndex, message message, global *global)
{
	int i, j;
	int nTemp, nOpt, tempCount, nTemp2, nID;
	int arrTemp[MAX_MESS];
	int PC[n];
	int nPC;
	int nOpt2;
	string sTemp, sTemp2;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{	
		nID = 1;
		tempCount = 0;
		nPC = 0;
		strcpy(sTemp, "");
		fflush(stdin);
		printf("%24s", "Inbox Messages\n");
		printf("\n---------------------------------\n\n");
		printf("[1] Personal Messages\n");
		printf("[2] Group Messages\n");
		printf("[3] Exit\n");
		printf("\n---------------------------------\n\n");
		fflush(stdin);
		
		//ask for what to do
		printf("Choice: ");
		scanf("%d", &nOpt2);
		
		if(nOpt2 == 1)
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			
			do
			{
				printf("%29s", "Viewing Personal Messages\n");
				printf("\n---------------------------------\n\n");
				printf("%22s", "[-1] Exit\n\n");
				printf("\n---------------------------------\n\n");
				
				tempCount = 0;
				
				//see if the message type is PM
				for(i = 0; i < user[nIndex].nInbox; i++)
				{
					if(message[messIndFinder(message, *global, user[nIndex].inbox[i])].type == 1)
					{
						tempCount++;
					}
				}
				
				//if no message recieved
				if(tempCount == 0)
				{
					printf(BLUE "%28s" RESET, "[NO MESSAGE RECIEVED]\n\n");
				}
				
				
				tempCount = 0;
				
				//display message preview snder and subject
				for(i = 0; i < global->nMessages; i++)
				{
					if(message[i].type == 1 && message[i].reciever == user[nIndex].id)
					{
						if(message[i].sender != -1)	
						{
							printf(BLUE"[%d] From: \"%s\"\n" RESET, tempCount, user[userFinder(user, message[i].sender, n)].username);
						}	
						else
						{
							printf(BLUE"[%d] From: DELETED USER.\n" RESET, tempCount);
						}
						
						printf("Subject: %s\n\n", message[i].subject);
						arrTemp[tempCount] = i;
						tempCount++;
					}
				}
				
				printf("---------------------------------\n\n");
				fflush(stdin);
				printf("Number: ");	
				scanf("%d", &nTemp);	//enter the Index of the message
				
				if (nTemp < tempCount && nTemp >= 0)
				{
					system("cls");
					printf("\n---------------------------------\n\n");
					do
					{

						printf(BLUE"Viewing Message:\n"RESET);
						printf("\n---------------------------------\n\n");
						printf("Instruction:\n\n");
						printf("[0] Reply\n");
						printf("[1] Exit\n");
						printf("\n---------------------------------\n\n");
						
						//display sender
						printf(BLUE "From: \n" RESET);
						if(message[arrTemp[nTemp]].sender != -1)
						{
							printf("%s\n\n", user[userFinder(user, message[arrTemp[nTemp]].sender, n)].username);
						}
						else
						{
							printf("DELETED USER.\n\n");
						}
						
						printf(BLUE "Subject: \n" RESET);
						printf("%s\n\n", message[arrTemp[nTemp]].subject);
						printf(BLUE"Message: \n"RESET);
						printf("%s\n\n", message[arrTemp[nTemp]].message);
						printf("\n---------------------------------\n\n");
						
						//if reply or exit
						fflush(stdin);
						printf("Choice: ");
						scanf("%d", &nOpt);
						
						if(nOpt == 0)
						{
	
							if(message[arrTemp[nTemp]].sender != -1)
							{
								reply(user, n, nIndex, message, global, arrTemp[nTemp], message[arrTemp[nTemp]].type);
							}
							else
							{
								system("cls");
								printf(RED "ERROR: USER DELETED\n" RESET);
								printf("---------------------------------\n\n");
							}
								
						}
						else if(nOpt == 1)
						{
							system("cls");
							printf(BLUE "RETURNED TO VIEWING\n"RESET);
							printf("---------------------------------\n\n");
						}
						else
						{
							system("cls");
							printf(RED"INVALID INPUT\n"RESET);
							printf("---------------------------------\n\n");
						}
					}while(nOpt != 1);
					
					printf("\n---------------------------------\n\n");
				}
				else if (nTemp == -1)
				{
					system("cls");
					printf(BLUE "RETURNED TO INBOX\n"RESET);
					printf("---------------------------------\n\n");
				}
				else
				{
					system("cls");
					printf(RED"INVALID INPUT\n"RESET);
					printf("---------------------------------\n\n");
				}
			}while(nTemp != -1);
			
			
		}
		else if(nOpt2 == 2)			//if personal connection messages
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			
			do
			{
				nPC = 0;
				printf("%31s", "Viewing Personal Connections\n");
				printf("\n---------------------------------\n\n");
				printf("%24s", "Instruction:\n\n");
				printf("%21s", "[-1] Exit\n");
				printf("\n---------------------------------\n\n");
				
				for(i = 0; i < n; i++)
				{
					for(j = 0; j < user[i].connection.n; j++)
					{
						if(user[nIndex].id == user[i].connection.connected[j])
						{
							//display the personal connection connected to the user
							printf("[%d] %s Personal Connection\n", nPC, user[i].username);
							PC[nPC] = user[i].id;
							nPC++;
						}
					}
				}
				
				printf("\n---------------------------------\n");
				printf("\nChoice: ");
				scanf("%d", &nTemp);
				
				//if valid then show messages
				if(nTemp != -1 && nTemp >= 0 && nTemp < nPC)
				{
					system("cls");
					printf("\n---------------------------------\n\n");
					
					
					nID = user[nTemp].id;
					nTemp = userFinder(user, PC[nTemp], n);
					do
					{	
						strcpy(sTemp, user[PC[nTemp]].username);
						
						
						tempCount = 0;
	
						printf("%s\n", sTemp);
						printf("Personal Connection Messages\n");
						printf("\n---------------------------------\n\n");
						printf("%24s", "Instruction:\n\n");
						printf("%21s", "[-1] Exit\n");
						printf("\n---------------------------------\n\n");
						
						//displays messages inside the Personal Connection
						for (i = 0; i < global->nMessages; i++)
						{
							for (j = 0; j < user[nIndex].nInbox; j++)
							{
								if(message[i].type == 2 && message[i].id == user[nIndex].inbox[j])
								{
									if(message[i].reciever == nID)
									{
										//display the message inside the PC
										if(message[i].sender != -1)
										{
											printf(BLUE "[%d] From: %s\n" RESET, tempCount, user[userFinder(user, message[i].sender, n)].username);
										}
										else
										{
											printf(BLUE "[%d] From: DELETED USER.\n" RESET, tempCount);
										}
										
										printf("Subject: ");
										printf("%s\n\n", message[i].subject);
										
										//save the id to the array
										arrTemp[tempCount] = message[i].id;
										tempCount++;
									}
								}
							}
						}
						
						//if no messages found
						if(tempCount == 0)
						{
							printf(BLUE "%26s\n" RESET, "[NO MESSAGE FOUND]\n\n");
						}
						printf("---------------------------------\n\n");
						printf("Number: ");
						scanf("%d", &nTemp2);
						
						if (nTemp2 < tempCount && nTemp2 >= 0)
						{
							system("cls");
							printf("\n---------------------------------\n\n");
							nTemp2 = messIndFinder(message, *global, arrTemp[nTemp2]);
							do
							{
								//displays message info
								printf(BLUE"Viewing Message:\n\n"RESET);
								printf("Instruction:\n\n");
								printf("[1] Reply\n");
								printf("[2] Exit\n");
								printf("\n---------------------------------\n\n");
								
								
								printf(BLUE "From: \n" RESET);
								
								if(message[nTemp2].sender != -1)
								{
									printf("%s\n\n", user[userFinder(user, message[nTemp2].sender, n)].username);
								}
								else
								{
									printf("DELETED USER.\n\n");
								}
								
								
								printf(BLUE "Recipients: \n" RESET);
								printf("[0] %s\n\n", sTemp);
								printf(BLUE "Subject: \n" RESET);
								printf("%s\n\n", message[nTemp2].subject);
								printf(BLUE"Message: \n"RESET);
								printf("%s\n\n", message[nTemp2].message);
								printf("\n---------------------------------\n\n");
								
								fflush(stdin);
								printf("Choice: ");
								scanf("%d", &nOpt);
								
								if(nOpt == 0)
								{
									system("cls");
									printf("\n---------------------------------\n\n");
									printf("Recipients\n\n");
									printf("\n---------------------------------\n\n");
									
									tempCount = 0;
									//display the recipients of the message
									for (i = 0; i < user[nTemp].connection.n; i++)
									{
										if(user[userFinder(user, user[nTemp].connection.connected[i], n)].id != message[nTemp2].sender)
										{
											for(j = 0; j < user[userFinder(user, user[nTemp].connection.connected[i], n)].nInbox; j++)
											{
												if(message[nTemp2].id == user[userFinder(user, user[nTemp].connection.connected[i], n)].inbox[j])
												{
													printf("[%d] %s\n", tempCount, user[userFinder(user, user[PC[nTemp]].connection.connected[i], n)].username);
													tempCount++;
												}
											}
											
										}
									}
									
									printf("\nType anything to exit: ");
									scanf("%s", sTemp2);
									system("cls");
									
									printf(BLUE "RETURNED TO MESSAGE\n" RESET);
									printf("---------------------------------\n\n");
								}
								else if(nOpt == 1)
								{
									reply(user, n, nIndex, message, global, nTemp2, message[nTemp2].type);	
										
								}
								else if(nOpt == 2)
								{
									system("cls");
									printf(BLUE "RETURNED TO MESSAGE\n" RESET);
									printf("---------------------------------\n\n");
								}
								else
								{
									system("cls");
									printf(RED"INVALID INPUT\n"RESET);
									printf("---------------------------------\n\n");
								}
								
							}while(nOpt != 2);
							
						}
						else if(nTemp2 == -1)
						{
							system("cls");
							printf(BLUE "RETURND TO VIEWING\n" RESET);
							printf("---------------------------------\n\n");
						}
						else
						{
							system("cls");
							printf(RED"INVALID INPUT\n"RESET);
							printf("---------------------------------\n\n");
						}
					}
					while(nTemp2 != -1);
					
					
				}
				else if(nTemp == -1)
				{
					system("cls");
					printf(BLUE "RETURNED TO INBOX\n" RESET);
					printf("---------------------------------\n\n");
				}
				else
				{
					system("cls");
					printf(RED "INVALID INPUT\n" RESET);
					printf("---------------------------------\n\n");
				}
			}while(nTemp != -1);
			
		}
		else if(nOpt2 == 3)
		{
			system("cls");
			printf(BLUE "RETURN TO MAIN PAGE\n" RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED"INVALID INPUT\n"RESET);
			printf("---------------------------------\n\n");
		}

	}while(nOpt2 != 3);
	
}

/*
	annoucement() displays the list of annoucement in the program, they can view the full details of the annoucement
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@nIndex 	- contains the index of the current user logged in
	@message 	- contains the messages information and where the messages will be saved
	@global 	- structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
*/
void announcements(account user, int n, int nIndex, message message, global *global)
{
	int i;
	int nTemp, nOpt;
	int arrTemp[MAX_MESS], nCount;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		//display announcement
		printf("%23s", "Annoucements\n");
		printf("\n---------------------------------\n\n");
		printf("%24s", "Instruction:\n\n");
		printf("%21s", "[-1] Exit\n");
		printf("\n---------------------------------\n\n");
		
		nCount = 0;
	
		for(i = 0; i < global->nMessages; i++)
		{
			if(message[i].type == 3)
			{
				if(message[i].sender != -1)
				{
					printf(BLUE"[%d] Annoucement From: %s\n" RESET, nCount, user[userFinder(user, message[i].sender, n)].username);
				}
				else
				{
					printf(BLUE"[%d] Annoucement From: Deleted User.\n" RESET, nCount);
				}
				
				printf("Subject: %s\n\n", message[i].subject);
				arrTemp[nCount] = i;
				nCount++;
			}
		}
		fflush(stdin);
		printf("Number: ");
		scanf("%d", &nTemp);
		
		if (nTemp < nCount && nTemp >= 0)
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			
			do
			{
				printf(BLUE"Viewing Annoucement:\n\n"RESET);
				printf("Instruction:\n\n");
				printf("[0] Reply\n");
				printf("[1] Exit\n");
				printf("\n---------------------------------\n\n");
				
				printf(BLUE "From: \n" RESET);
				if(message[arrTemp[nTemp]].sender != -1)
				{
					printf("%s\n\n", user[userFinder(user, message[arrTemp[nTemp]].sender, n)].username);
				}
				else
				{
					printf("Deleted User.\n\n");
				}
				
				printf(BLUE "Subject: \n" RESET);
				printf("%s\n\n", message[arrTemp[nTemp]].subject);
				printf(BLUE"Message: \n"RESET);
				printf("%s\n\n", message[arrTemp[nTemp]].message);
				printf("\n---------------------------------\n\n");
				
				fflush(stdin);
				printf("Choice: ");
				scanf("%d", &nOpt);
				
				if(nOpt == 0)
				{
					reply(user, n, nIndex, message, global, arrTemp[nTemp], message[arrTemp[nTemp]].type);
				}
				else if(nOpt == 1)
				{
					system("cls");
					printf(BLUE "RETURNED TO ANNOUCEMENT\n"RESET);
					printf("---------------------------------\n\n");
				}
				else
				{
					system("cls");
					printf(RED "ERROR: INVALID INPUT\n" RESET);
					printf("---------------------------------\n\n");
				}
			}while(nOpt != 1);
			
		}
		else if(nTemp == -1)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT\n" RESET);
			printf("---------------------------------\n\n");
		}
	}while(nTemp != -1);	
}

/*
	displayProfile() displays the profile or details of the user in the Find index, the logged in user can send a PM or add the User in the PC
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@nIndex 	- contains the index of the current user logged in
	@message 	- contains the messages information and where the messages will be saved
	@global 	- structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
	@Find 		- Index of the user that the user wanted to view
*/
void displayProfile(account user, int n, int nIndex, message message, global *global, int Find)
{
	char choice;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		//display message info
		fflush(stdin);
		printf(BLUE "[%d] "RESET, user[Find].id);
		printf("User Profile ID \n");
		printf(BLUE "\nUserame: \n" RESET);
		printf("%s\n\n", user[Find].username);
		printf(BLUE "Name: \n" RESET);
		printf("%s\n\n" , user[Find].name);
		printf(BLUE "Description: \n\n" RESET);
		printf("%s\n\n", user[Find].description);
		printf("\n---------------------------------\n\n");
		printf(BLUE "[1] " RESET);
		printf("Add Personal Connection\n");
		printf(BLUE "[2] " RESET);
		printf("Send Personal Message\n");
		printf(BLUE "[3] " RESET);
		printf("Exit\n");
		
		printf("\nChoice: ");
		choice = getchar();
		
		if(choice == '1')	//add user
		{
			printf(BLUE "Add User [Y][N]: " RESET);
			
			fflush(stdin);
			scanf(" %c", &choice);
			
			if(choice == 'y' || choice == 'Y')
			{
				addPersonalConnections(user, n, nIndex, user[Find].username);	
			}
			else 
			{
				system("cls");
				printf(RED "ERROR\n" RESET);
				printf("---------------------------------\n\n");
			}
			
		}
		else if(choice == '2')		//send personal message
		{
			composePM(user, n, nIndex, message, global, Find);
		}
		else if(choice == '3')		//exit
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT\n" RESET);
			printf("---------------------------------\n\n");
		}
	}while(choice != '3');
	
}

/*
	browseUsers() main menu where the user can find other users of the application by list or by filtering out their name
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@nIndex		- contains the index of the current user logged in
	@message	- contains the messages information and where the messages will be saved
	@global 	- structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
*/
void browseUsers(account user, int n, int nIndex, message message, global *global)
{
	int nOpt;
	int i, nTemp = 0, nInput;
	int arrTemp[MAX_USER];
	string sTemp;
	int found = 0;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		found = 0;
		
		printf("%22s", "Browse User\n");
		printf("\n---------------------------------\n\n");
		printf("[1] View list of Users\n");
		printf("[2] Filter by Name\n");				
		printf("[3] Exit\n\n");
		
		printf("Choice: ");
		scanf("%d", &nOpt);	//get input
		
		if(nOpt == 1)	//if by list
		{
			nTemp = 0;
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%23s", "List of Users\n");
			printf("\n---------------------------------\n\n");
			
			//display list of user
			for(i = 0; i < n; i++)
			{
				if(i != nIndex)
				{
					printf("[%d] %s\n", nTemp, user[i].username);
					arrTemp[nTemp] = user[i].id;
					nTemp++;
				}
			}
			
			if(nTemp == 0)
			{
				printf(BLUE "%25s" RESET, "[NO OTHER USERS]");
			}
			
			fflush(stdin);
			printf("\nEnter Number: ");
			scanf("%d", &nInput);
			fflush(stdin);
			
			if(nInput >= 0 && nInput < nTemp)	//if valid
			{
				displayProfile(user, n, nIndex, message, global, userFinder(user, arrTemp[nInput], n));
			}
			else
			{
				system("cls");
				printf(RED "ERROR: INVALID INPUT\n" RESET);
				printf("---------------------------------\n\n");	
			}
		}
		else if(nOpt == 2)		//by name
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%23s", "Find of Users\n");
			printf("\n---------------------------------\n\n");
			printf("Username: ");
			fflush(stdin);
			fgets(sTemp, sizeof(sTemp), stdin);
			sTemp[strlen(sTemp) - 1] = '\0';
			
			//find the user
			for(i = 0; i < n && !found; i++)
			{
				if(strcmp(user[i].username, sTemp) == 0)
				{
					displayProfile(user, n, nIndex, message, global, i);
					found = 1;
				}
			}
			
			if(!found)
			{
				system("cls");
				printf(RED "ERROR: USER DOESNT EXIST\n" RESET);
				printf("---------------------------------\n\n");
			}
		}
		else if(nOpt == 3)	//exit
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT\n" RESET);
			printf("---------------------------------\n\n");
		}
	}while(nOpt != 3);
	
		
}

/*
	modifyPersonalContents() main menu where the user can choose to change their name or personal description
	@user	- structure array containing the Info of users and where the question and answer will be saved
	@n 		-	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
*/
void modifyPersonalContents(account user, int n, int nIndex)
{
	int i;
	int nOpt, nValid;
	char cOpt;
	string sTemp;
	mess mTemp;
	char pass[100];
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%29s", "Modify Personal Contents\n");
		printf("\n---------------------------------\n\n");
		printf(BLUE "[1] Name:" RESET);
		printf("\n%s\n\n", user[nIndex].name);
		printf(BLUE "[2] Description:\n" RESET);
		printf("%s\n\n", user[nIndex].description);
		printf(BLUE "[3] Done\n\n" RESET);
		printf("\n---------------------------------\n\n");
		printf("Modify[?]: ");
		scanf("%d", &nOpt);	//ask for input
		fflush(stdin);
		
		if(nOpt == 1)	//modify name
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			
			do
			{
				nValid = 1;
				printf("%22s", "Modify Name\n");
				printf("\n---------------------------------\n\n");
				printf("%29s", "Enter Name [MAX 50] with\n");
				printf("%27s", "NO SPECIAL CHARACTERS\n");
				printf("\n---------------------------------\n\n");
				
				printf("Previous Name: %s\n", user[nIndex].name);
				
				
				fflush(stdin);
				printf(BLUE "New Name: " RESET);
				fgets(sTemp, sizeof(sTemp), stdin);	//gets new name
				sTemp[strlen(sTemp) - 1] = '\0';	
				
				if(strlen(sTemp) <= 50 && strlen(sTemp) >= 1)
				{
					for(i = 0; i < strlen(sTemp) && nValid; i++)
					{
						if(!((sTemp[i] >= 'a' && sTemp[i] <= 'z') || (sTemp[i] >= 'A' && sTemp[i] <= 'Z') || sTemp[i] == ' '))
						{
							nValid = 0;
						}
					}
				}
				else
				{
					nValid = 0;
				}
				
				if(!nValid)
				{
					system("cls");
					printf(RED "ERROR: INVALID NAME" RESET);
					printf("\n---------------------------------\n\n");
				}
			}while(!nValid);
			
			fflush(stdin);
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%22s", "Modify Name\n");
			printf("\n---------------------------------\n\n");
			printf("%29s", "Enter Name [MAX 50] with\n");
			printf("%27s", "NO SPECIAL CHARACTERS\n");
			printf("\n---------------------------------\n\n");
			printf("Previous Name: %s\n", user[nIndex].name);
			printf("New Name: %s\n", sTemp);
			printf("\n---------------------------------\n\n");
			printf("Save Changes [Y]es or [ANY KEYS EXCEPT Y] No\n\n");
			printf("Choice: ");
			scanf(" %c", &cOpt);	//save changes or not
			
			if(cOpt == 'Y' || cOpt == 'y')
			{
				system("cls");
				printf("\n---------------------------------\n\n");
				printf("%22s", "Modify Name\n");
				printf("\n---------------------------------\n\n");
				printf("Enter Password: ");
				fflush(stdin);
				fgets(pass, sizeof(pass), stdin);
				pass[strlen(pass) - 1] = '\0';
				
				encrypt(pass);
				
				if(strcmp(user[nIndex].password, pass) == 0)
				{
					strcpy(user[nIndex].name, sTemp);
					system("cls");
					printf(BLUE "CHANGES SAVED\n"RESET);
					printf("---------------------------------\n\n");
				}
				else
				{
					system("cls");
					printf(RED"ERROR: WRONG PASSWORD\n" RESET);
					printf("---------------------------------\n\n");
				}					
			}
			else
			{
				system("cls");
				printf(RED "CHANGES NOT SAVED\n"RESET);
				printf("---------------------------------\n\n");
			}
		}
		else if(nOpt == 2)	//modify the description
		{
			system("cls");
			printf("\n---------------------------------\n\n");

			do
			{
				printf("%26s", "Modify Description\n");
				printf("\n---------------------------------\n\n");
				printf(BLUE "New Description [MAX 100]: \n" RESET);
				fgets(mTemp, sizeof(mTemp), stdin);	//gets the new description
				mTemp[strlen(mTemp) - 1] = '\0';
				
				if(strlen(mTemp) > 100)
				{
					system("cls");
					printf(RED "ERROR: EXCEEDS LIMIT" RESET);
					printf("\n---------------------------------\n\n");
				}
				else if(strlen(mTemp) == 0)
				{
					system("cls");
					printf(RED "ERROR: NO INPUT" RESET);
					printf("\n---------------------------------\n\n");
				}
			}while(strlen(mTemp) > 100 || strlen(mTemp) == 0);
			
			
			printf("\n---------------------------------\n\n");
			printf("Save Changes [Y]es or [ANY KEYS EXCEPT Y] No\n\n");
			scanf(" %c", &cOpt);	//if save the changes
			
			if(cOpt == 'Y' || cOpt == 'y')
			{
				system("cls");
				printf("\n---------------------------------\n\n");
				
				printf("%26s", "Modify Description\n");
				printf("\n---------------------------------\n\n");
				printf("Enter Password: ");
				fflush(stdin);
				fgets(pass, sizeof(pass), stdin);
				pass[strlen(pass) - 1] = '\0';
				
				encrypt(pass);
				
				if(strcmp(user[nIndex].password, pass) == 0)
				{
					strcpy(user[nIndex].description, mTemp);
					system("cls");
					printf(BLUE "CHANGES SAVED\n"RESET);
					printf("---------------------------------\n\n");
				}
				else
				{
					system("cls");
					printf(RED"ERROR: WRONG PASSWORD\n" RESET);
					printf("---------------------------------\n\n");
				}
			}
			else	
			{
				system("cls");
				printf(RED "CHANGES NOT SAVED\n"RESET);
				printf("---------------------------------\n\n");
			}	
			
		}
		else if(nOpt == 3)	//exit
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT\n" RESET);
			printf("---------------------------------\n\n");
		}
		
		saveData(user, n);
		
	}while(nOpt != 3);
	
}

/*
	modifyAccountSecurity() main menu where the user can change their password and security answer for the security question
	@user	- structure array containing the Info of users and where the question and answer will be saved
	@n 		-	contains the number of users in the program
	@nIndex - contains the index of the current user logged in
*/
void modifyAccountSecurity(account user, int n, int nIndex)
{
	int nOpt;
	char tempPass[25], rePass[MAX_PASS];
	char sTemp[200], sTemp2[200];
	
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%29s", "Modify Personal Contents\n");
		printf("\n---------------------------------\n\n");
		printf("[1] Change Password\n");
		printf("[2] Change Security Question Answer\n");
		printf("[3] Exit\n");
		printf("\n---------------------------------\n\n");
		
		fflush(stdin);
		printf("Enter: ");
		scanf("%d", &nOpt);	//what to do
		
		if(nOpt == 1)	//change password
		{
			fflush(stdin);
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%24s", "Change Password\n");
			printf("\n---------------------------------\n\n");
			printf("Old Password: ");
			fgets(tempPass, sizeof(tempPass), stdin);	//ask for password
			tempPass[strlen(tempPass) - 1] = '\0';
			fflush(stdin);
			
			encrypt(tempPass);
			
			if(strcmp(user[nIndex].password, tempPass) == 0)
			{
				system("cls");
				printf("\n---------------------------------\n\n");
				
				do
				{
					printf("%24s", "Change Password\n");
					printf("\n---------------------------------\n\n");
					printf("Enter New Password (MAX LENGTH 20): \n");
					printf("Password: ");
					
					fflush(stdin);
					fgets(tempPass, sizeof(tempPass), stdin);	//enter for password
					tempPass[strlen(tempPass) - 1] = '\0';
					
					if (strlen(tempPass) > 20)
					{
						system("cls");
						printf(RED "ERROR: EXCEEDS MAX LENGTH\n" RESET);
						printf("---------------------------------\n\n");
					}	
					else if(strlen(tempPass) == 0)
					{
						system("cls");
						printf(RED "ERROR: ENTER A PASSWORD\n" RESET);
						printf("---------------------------------\n\n");
					}
					
				}while(strlen(tempPass) > 20 || strlen(tempPass) == 0);
				
				fflush(stdin);
				system("cls");
				printf("\n---------------------------------\n\n");
				printf("%24s", "Change Password\n");
				printf("\n---------------------------------\n\n");
				printf("Re-Password: ");
				
				fgets(rePass, sizeof(rePass), stdin);	//re-enter password
				rePass[strlen(rePass) - 1] = '\0';
				
				if(strcmp(tempPass, rePass) == 0)
				{
					encrypt(rePass);
					strcpy(user[nIndex].password, rePass);
					saveData(user, n);
					
					system("cls");
					printf(BLUE "PASSWORD CHANGED SUCCESSFULLY\n" RESET);
					printf("---------------------------------\n\n");
				}
				else
				{
					system("cls");
					printf(RED "ERROR: PASSWORD DOESN\'T MATCH\n" RESET);
					printf("---------------------------------\n\n");
				}
			}
			else
			{
				system("cls");
				printf(RED "ERROR: PASSWORD DOESN\'T MATCH\n" RESET);
				printf("---------------------------------\n\n");
			}
		}
		else if(nOpt == 2)	//change security answer
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			
			do
			{
				printf("%32s", "Change Security Question Answer\n");
				printf("\n---------------------------------\n\n");
				printf("Security Question: \n\n");
				printf("%s\n\n", user[nIndex].securityQuestion);
				printf("Answer [MAX 100]: \n\n");
				fflush(stdin);
				fgets(sTemp, sizeof(sTemp), stdin);
				sTemp[strlen(sTemp) - 1] = '\0';
				
				if(strlen(sTemp) > 100)
				{
					system("cls");
					printf(RED "ERROR: EXCEEDS LIMIT\n" RESET);
					printf("---------------------------------\n\n");
				}
				else if(strlen(sTemp) == 0)
				{
					system("cls");
					printf(RED "ERROR: NO INPUT" RESET);
					printf("\n---------------------------------\n\n");
				}
			}while(strlen(sTemp) > 100 || strlen(sTemp) == 0);
			
			
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%32s", "Change Security Question Answer\n");
			printf("\n---------------------------------\n\n");
			printf("Security Question: \n\n");
			printf("%s\n\n", user[nIndex].securityQuestion);
			printf("Re-Enter Answer: \n\n");
			fgets(sTemp2, sizeof(sTemp2), stdin);
			sTemp2[strlen(sTemp2) - 1] = '\0';
			
			if(strcmp(sTemp, sTemp2) == 0)
			{
				strcpy(user[nIndex].securityAns, sTemp2);
				saveData(user, n);
				system("cls");
				printf(BLUE "ANSWER CHANGED SUCCESSFULLY\n" RESET);
				printf("---------------------------------\n\n");
			}
			else
			{
				system("cls");
				printf(RED "ERROR: ASNWERS DONT MATCH\n" RESET);
				printf("---------------------------------\n\n");
			}	
		}
		else if(nOpt == 3)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT\n" RESET);
			printf("---------------------------------\n\n");
		}
	}while(nOpt != 3);
	
}

/*
	userModule() main menu page of the user where they can choose what to do in the messenger
				calls the function of what they want to do
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@nIndex 	- contains the index of the current user logged in
	@message 	- contains the messages information
	@global 	- structure for global datas, contains number of messages that exist and the availabe ID of messages and etc.
*/
void userModule(account user, int n, int nIndex, message message, global *global)
{
	int nOpt;

	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%22s", "GUMMIPHONE\n");
		printf("\n---------------------------------\n\n");
		printf("Welcome user ");
		printf(BLUE "%s\n\n" RESET, user[nIndex].username);
		printf("[1] Compose Message\n");
		printf("[2] Inbox\n");
		printf("[3] Sent\n");
		printf("[4] View Announcements\n");
		printf("[5] Modify Personal Contents\n");
		printf("[6] Modify Account Security\n");
		printf("[7] Modify Personal Connections\n");
		printf("[8] Browse Users\n");
		printf("[9] Log Out\n\n");
		
		printf("Enter Choice: ");
		scanf("%d", &nOpt);	//what to do
		
		system("cls");
		switch(nOpt)
		{
			case 1: composeMessage(user, n, nIndex, message, global); break;
			case 2:	inbox(user, n, nIndex, message, global); break;
			case 3:	sent(user, n, nIndex, message, global); break;
			case 4:	announcements(user, n, nIndex, message, global); break;
			case 5:	modifyPersonalContents(user, n, nIndex); break;
			case 6:	modifyAccountSecurity(user, n, nIndex); break;
			case 7:	modifyPersonalConnections(user, n, nIndex, global, message);	break;
			case 8:	browseUsers(user, n, nIndex, message, global); break;
			case 9: printf(BLUE "RETURNED TO LOGIN PAGE" RESET); break;
			default: system("cls");
					printf(RED "ERROR: INVALID INPUT" RESET);
					printf("\n---------------------------------\n\n");
		}
	}while(nOpt != 9);
	
}

/*
	requestPassword() this function sends a request password reset to the admin of the user 
	@user 	- structure array containing the Info of users and where the question and answer will be saved
	@n 		-	contains the number of users in the program
	@global - contains miscellaneous data and also admin data where the requst will be saved
*/
void requestPassword(account user, int n, global *global)
{
	string sUsername;
	int nIndex, nFound = 0, nValid;
	int i, j;
	mess mAnswer;
	
	printf("\n---------------------------------\n\n");
	printf("Request Password Reset\n");
	printf("\n---------------------------------\n\n");
	printf(RED "WARNING!!\n\n" RESET);
	printf("Account will be locked until \nthe admin resets the password\n");
	printf("\n---------------------------------\n\n");
	
	
	fflush(stdin);
	printf("Username: ");
	fgets(sUsername, sizeof(sUsername), stdin);	//reset password
	sUsername[strlen(sUsername) - 1]  = '\0';
	
	for(i = 0; i < n && !nFound; i++)
	{
		if(strcmp(sUsername, user[i].username) == 0)
		{
			nValid = 1;
			//if the user already sent a request
			for(j = 0; j < global->admin.nPassReq && nValid; j++)
			{
				//if user is found then it already exist
				if(user[i].id == global->admin.PassReq[j])
				{
					nValid = 0;
				}
			}
			
			//if it doesnt exist then add it
			if(nValid)
			{
				nIndex = i;
				nFound = 1;
			}
			
		}
	}
	
	//if user is valid
	if(nFound)
	{
		system("cls");
		printf("\n---------------------------------\n\n");
		printf("Request Password Reset\n");
		printf("\n---------------------------------\n\n");
		printf(RED "WARNING!!\n\n" RESET);
		printf("Account will be locked until \nthe admin resets the password\n");
		printf("\n---------------------------------\n\n");
		
		//ask for username
		printf("Username: %s\n", user[nIndex].username);
		
		system("cls");
		printf("\n---------------------------------\n\n");
		printf("Request Password Reset\n");
		printf("\n---------------------------------\n\n");
		
		//ask for the answer to the security question
		printf(BLUE "Answer Security Question: \n\n" RESET);
		printf("%s\n\n", user[nIndex].securityQuestion);
		
		fgets(mAnswer, sizeof(mAnswer), stdin);
		mAnswer[strlen(mAnswer) - 1]  = '\0';
		
		//checks if answer is the same
		if(strcmp(mAnswer, user[nIndex].securityAns) == 0)
		{
			//send a request password reset
			global->admin.PassReq[global->admin.nPassReq] = user[nIndex].id;
			global->admin.nPassReq++;
			saveAdminInfo(user, n, *global);
			user[nIndex].status = 1;
			saveData(user, n);
			
			system("cls");
			printf(BLUE "REQEUST SENT" RESET);
		}
		else
		{
			system("cls");
			printf(RED "ANSWER NOT SAME" RESET);
		}
	}
	else
	{
		system("cls");
		printf(RED "ERROR: USER DOESN\'T EXIST" RESET);
	}
	
}

/*
	loginUser() main menu page of the user where they can choose what to do in the messenger
				calls the function of what they want to do
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@message 	- contains the messages information
	@global 	- structure for global datas,admin data, contains number of messages that exist and the availabe ID of messages and etc.
*/
void loginUser(account user, int n, message message, global *global)
{
	int nOpt, nValid, nIndex;
	char sTemp[100];
	char pass[100];
	char rePassword[100];
	int i;
	mess mAnswer;
	
	
	//ask username
	printf("\n---------------------------------\n\n");
	
	do
	{
		nValid = 0;
		fflush(stdin);
		printf("%22s", "LOGIN PAGE\n");
		printf("\n---------------------------------\n\n");
		printf("Options:\n\n");
		printf("[1] Login\n");
		printf("[2] Forgot Password\n");
		printf("[3] Exit\n");
		printf("\n---------------------------------\n\n");
		
		printf("Enter Choice: ");
		scanf("%d", &nOpt);
		
		
		if(nOpt == 1)
		{
			system("cls");
			
			printf("\n---------------------------------\n\n");
			printf("%22s", "LOGIN PAGE\n");
			printf("\n---------------------------------\n\n");
			fflush(stdin);
			
			//enter username
			printf("Username: ");
			fgets(sTemp, sizeof(sTemp), stdin);
			sTemp[strlen(sTemp) - 1] = '\0';
			
			//find the user index
			for (i = 0; i < n && !nValid; i++)
			{
				if (strcmp(user[i].username, sTemp) == 0)
				{
					nIndex = i;
					nValid = 1;
				}
			}
			
			if(!nValid)
			{
				system("cls");
				printf(RED "ERROR: USERNAME DOESN\'T EXIST" RESET);
			}
			else
			{
				
				//if user is not on change password mode
				if(user[nIndex].status != 2)
				{
					printf("Password: ");
					fgets(sTemp, sizeof(sTemp), stdin);
					sTemp[strlen(sTemp) - 1] = '\0';
					
					encrypt(sTemp);
					
					system("cls");
					
					//checks if the password is correct
					if(strcmp(user[nIndex].password, sTemp) != 0)
					{
						//if it doesnt exit display error
						printf(RED "ERROR: INCORRECT PASSWORD" RESET);
						nValid = 0;
					}
					else
					{
						//if it exist go to the main page
						if(user[nIndex].status == 0)
						{
							userModule(user, n, nIndex, message, global);
						}
						else if(user[nIndex].status == 1)
						{
							printf(RED "ERROR: ACCOUNT LOCKED" RESET);
						}
						
					}	
				}
				else if(user[nIndex].status == 2)		//if the password needs to be changed first because of reset password
				{
					system("cls");
					
					printf("\n---------------------------------\n\n");
					printf("%22s", "LOGIN PAGE\n");
					printf("\n---------------------------------\n\n");
					printf("Username: %s\n", user[nIndex].username);
					
					//get the answer to the security question
					printf(BLUE "\nAnswer Security Question: \n\n" RESET);
					printf("%s\n\n", user[nIndex].securityQuestion);
					
					fgets(mAnswer, sizeof(mAnswer), stdin);
					mAnswer[strlen(mAnswer) - 1]  = '\0';
					
					//if answer is the same ask the user to rest password
					if(strcmp(mAnswer, user[nIndex].securityAns) == 0)
					{
						system("cls");
						printf("\n---------------------------------\n\n");
						//ask for default
						do
						{
							
							printf("%22s", "LOGIN PAGE\n");
							printf("\n---------------------------------\n\n");
							
							fflush(stdin);
							
							//enter default password
							printf("Default Password is \"CCPROG2\"\n\n");
							printf("Enter Default Password: ");
							fgets(pass, sizeof(pass), stdin);
							pass[strlen(pass) - 1] = '\0';
							
							encrypt(pass);
							
							if(strcmp(pass, user[nIndex].password) != 0)
							{
								system("cls");
								printf(RED "ERROR: INCORRECT PASSWORD" RESET);
								printf("\n---------------------------------\n\n");
							}
									
						}while(strcmp(pass, user[nIndex].password) != 0);
						
						//ask for new password
						system("cls");
						printf("\n---------------------------------\n\n");
						
						do
						{
							printf("%22s", "LOGIN PAGE\n");
							printf("\n---------------------------------\n\n");
							
							fflush(stdin);
							
							printf("New Password [Max 20]: ");
							fgets(pass, sizeof(pass), stdin);
							pass[strlen(pass) - 1] = '\0';
							
							//if exceed max length display error
							if(strlen(pass) > 20)
							{
								system("cls");
								printf(RED "ERROR: EXCEEDS MAX LENGTH" RESET);
								printf("\n---------------------------------\n\n");
							}
						}while(strlen(pass) > 20);
						
						
						
						system("cls");
						printf("\n---------------------------------\n\n");
						printf("%22s", "LOGIN PAGE\n");
						printf("\n---------------------------------\n\n");
						printf("Re-Enter Password: ");
						
						//ask to re enter password
						fgets(rePassword, sizeof(rePassword), stdin);
						rePassword[strlen(rePassword) - 1] = '\0';
						
						//if password are the same then change the password
						if(strcmp(pass, rePassword) == 0)
						{
							user[nIndex].status = 0;
							encrypt(pass);
							strcpy(user[nIndex].password, pass);
							saveData(user, n);
							
							system("cls");
							printf(BLUE "ACCOUNT UNLOCKED" RESET);
						}
						else
						{
							system("cls");
							printf(RED "PASSWORD DONT MATCH" RESET);
						}					
					}
					else
					{
						system("cls");
						printf(RED "WRONG ANSWER" RESET);
					}
				}	
			}
		}
		else if(nOpt == 2)
		{
			system("cls");
			//enter for the forgot password
			requestPassword(user, n, global);
		}
		else if(nOpt == 3)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE" RESET);
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT" RESET);
		}
		
		if(nOpt != 3)
			printf("\n---------------------------------\n\n");
		
		
	}while(nOpt != 3);
	
}

/*
	loadAccount() this loads every datas of the txt files into the structure array user, 
					the security questions, messages, and connections
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@nUser 		-	contains the number of users in the Program gets appended 1 ever new user
	@questions	- Contains the security questions
	@nQuestions	- contains the number of questions
	@message	- contains the information of the messages
	@global		- structure for global datas / miscellaneous, contains the availabe ID of user and messages
	
*/
void loadAccount(account user, int *nUser, QA questions, int *nQuestions, message message, global *global)
{
	int i, j;
	char sTemp[200];
	char sSentence[MAX_SEC];
	FILE *pFile;
	int nTemp = 0;
	
	pFile = fopen("message.txt", "r");
	
	if(pFile != NULL)
	{
		nTemp = 0;
		//numbers
		fgets(sTemp, sizeof(sTemp), pFile);
		global->nMessages = atoi(sTemp);
		
		//numbers of annoucement
		fgets(sTemp, sizeof(sTemp), pFile);
		global->nAnnouncement = atoi(sTemp);
		
		for (i = 0; i < global->nMessages; i++)
		{
			//ID
			fgets(sTemp, sizeof(sTemp), pFile);
			message[i].id = atoi(sTemp);
			
			//type of message, PM GC ALL
			fgets(sTemp, sizeof(sTemp), pFile);
			message[i].type = atoi(sTemp);
			
			//sender
			fgets(sTemp, sizeof(sTemp), pFile);
			message[i].sender = atoi(sTemp);
			
			//reciever
			fgets(sTemp, sizeof(sTemp), pFile);
			message[i].reciever = atoi(sTemp);
			
			//subject
			fgets(sTemp, sizeof(sTemp), pFile);
			sTemp[strlen(sTemp) - 1] = '\0';
			strcpy(message[i].subject, sTemp);
			
			//message
			fgets(sTemp, sizeof(sTemp), pFile);
			sTemp[strlen(sTemp) - 1] = '\0';
			strcpy(message[i].message, sTemp);
		}
		
		for(i = 0; i < global->nAnnouncement; i++)
		{
			if(message[i].type == 3)
			{
				global->announcement[nTemp] = message[i].id;
				nTemp++;
			}	
		}
		fclose(pFile);
		
	}
	else
	{
		global->nMessages = 0;
		global->nAnnouncement = 0;
	}
	
	
	pFile = fopen("accounts.txt", "r");
	
	//load user info
	if (pFile != NULL)
	{
		//number of account
		fgets(sTemp, sizeof(sTemp), pFile);
		*nUser = atoi(sTemp);
		
		for (i = 0; i < *nUser; i++)
		{
			//index / id of the user
			fgets(sTemp, sizeof(sTemp), pFile);
			user[i].id = atoi(sTemp);
			
			//status of account
			fgets(sTemp, sizeof(sTemp), pFile);
			user[i].status = atoi(sTemp);
			
			//name
			fgets(sTemp, sizeof(sTemp), pFile);
			sTemp[strlen(sTemp) - 1] = '\0';
			strcpy(user[i].name, sTemp);
			
			//username
			fgets(sTemp, MAX_STR, pFile);
			sTemp[strlen(sTemp) - 1] = '\0';
			strcpy(user[i].username, sTemp);
			
			//password
			fgets(sTemp, MAX_STR, pFile);
			sTemp[strlen(sTemp) - 1] = '\0';
			strcpy(user[i].password, sTemp);
			
			//security question
			fgets(sSentence, sizeof(sSentence), pFile);
			sSentence[strlen(sSentence) - 1] = '\0';
			strcpy(user[i].securityQuestion, sSentence);
			
			//security answer
			fgets(sSentence, sizeof(sSentence), pFile);
			sSentence[strlen(sSentence) - 1] = '\0';
			strcpy(user[i].securityAns, sSentence);
			
			//description
			fgets(sSentence, sizeof(sSentence), pFile);
			sSentence[strlen(sSentence) - 1] ='\0';
			strcpy(user[i].description, sSentence);
			
			//number of sent
			fgets(sTemp, MAX_STR, pFile);
			user[i].nSent = atoi(sTemp);
			
			for(j = 0; j < user[i].nSent; j++)
			{
				//id of the message
				fgets(sTemp, MAX_STR, pFile);
				user[i].sent[j] = atoi(sTemp);
			}
			
			//number of inbox messages
			fgets(sTemp, MAX_STR, pFile);
			user[i].nInbox = atoi(sTemp);
			
			for(j = 0; j < user[i].nInbox; j++)
			{
				//id of the message recieved
				fgets(sTemp, MAX_STR, pFile);
				user[i].inbox[j] = atoi(sTemp);
			}
				
		
			if(global->nMessages == 0)
			{
				user[i].nSent = 0;
				user[i].nInbox = 0;
			}
		}
		
		
		fclose(pFile);
	}
	else
	{
		*nUser = 0;
	}
		
		

	//load security questions
	
	pFile = fopen("securityQuestions.txt", "r");
	
	if(pFile != NULL)
	{
		
		fgets(sTemp, sizeof(sTemp), pFile);
		*nQuestions = atoi(sTemp);
		
		for (i = 0; i < *nQuestions; i++)
		{
			fgets(sTemp, sizeof(sTemp), pFile);
			sTemp[strlen(sTemp) - 1] = '\0';
			strcpy(questions[i], sTemp);
		}
		
		fclose(pFile);
	}
	else
	{
		strcpy(questions[0], "Favorite Disney Character? ");
		*nQuestions = 1;
	}
	
	pFile = fopen("connections.txt", "r");
	
	
	if(pFile != NULL)
	{
		for (i = 0; i < *nUser; i++)
		{
			fgets(sTemp, sizeof(sTemp), pFile);
			user[i].connection.n = atoi(sTemp);
			
			for(j = 0; j < user[i].connection.n; j++)
			{
				fgets(sTemp, sizeof(sTemp), pFile);
				sTemp[strlen(sTemp) - 1] = '\0';
				user[i].connection.connected[j] = atoi(sTemp);
			}
			
		}
		
		fclose(pFile);
	}
	else
	{
		for (i = 0; i < *nUser; i++)
		{
			user[i].connection.n = 1;
			user[i].connection.connected[0] = user[i].id;
		}	
	}
	
}
 
/*
	adminSecurityQuestion()  this is where the admin can add a new security question
	@n			- contains the number of questions that exist
	@questions	- Contains the security questions
	
*/        
void adminSecurityQuestion(QA questions, int *n)
{
	int nOpt;
	int i;
	FILE *pFile;
	char sTemp[105];
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%21s", "QUESTIONS\n");
		printf("\n---------------------------------\n\n");
		printf("[1] Add more Questions\n");
		printf("[2] Exit\n");
		printf("\n---------------------------------\n\n");
		
		printf("%25s\n\n", "LIST OF QUESTIONS");
		for (i = 0; i < *n; i++)
		{
			printf("%d. %s\n", i + 1, questions[i]);
		}
		
		printf("\n---------------------------------\n\n");
		printf("What to do: ");
		scanf("%d", &nOpt);
		fflush(stdin);
		
		if(nOpt == 1)
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			
			do
			{
				fflush(stdin);
				printf("%21s", "QUESTIONS\n");
				printf("\n---------------------------------\n\n");
				printf("[1] Add more Questions\n");
				printf("[2] Exit\n");
				printf("\n---------------------------------\n\n");
				
				printf("\nLIST OF QUESTIONS\n\n");
				for (i = 0; i < *n; i++)
				{
					printf("%d. %s\n", i + 1, questions[i]);
				}
				printf("\n---------------------------------\n\n");
				
				printf("\nAdd Question [MAX 100]:\n\n");
				fgets(sTemp, sizeof(sTemp), stdin);
				sTemp[strlen(sTemp) - 1] = '\0'; 
				
				if(strlen(sTemp) > 100)
				{
					system("cls");
					printf(RED "Error: EXCEED LIMIT\n" RESET); 
					printf("---------------------------------\n\n");
				}
				else if(strlen(sTemp) == 0)
				{
					system("cls");
					printf(RED "Error: NO INPUT\n" RESET); 
					printf("---------------------------------\n\n");
				}
			}while(strlen(sTemp) > 100 || strlen(sTemp) == 0);
			
			
			strcpy(questions[*n], sTemp);
			(*n)++;
			
			pFile = fopen("securityQuestions.txt", "w");
	
				if(pFile != NULL)
				{
					fprintf(pFile, "%d\n", *n);
					for(i = 0; i < *n; i++)
					{
						fprintf(pFile, "%s\n", questions[i]);
					}
				}
					
				fclose(pFile);
				system("cls");
				printf(BLUE "QUESTION ADDED\n"RESET);
				printf("---------------------------------\n\n");
		}
		else if(nOpt == 2)
		{
			system("cls");
			printf(BLUE "RETURNED TO USER MODULE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "Error: INVALID INPUT\n" RESET); 
			printf("---------------------------------\n\n");
		}
	}while(nOpt != 2);
	
}

/*
	adminModifyUsers() this function displays a list of user and the admin can modify 
						the user's name, username, description and personal connection
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			- contains the number of users in the program
	@message 	- contains the messages information
	@global 	- structure for global datas, admin data number of messages and etc
*/
void adminModifyUsers(account user, int n, global *global, message message)
{
	int i;
	int nIndex, nTemp;
	int arrTemp[MAX_USER];
	char cOpt;
	string sTemp;
	int nValid;
	mess mTemp;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%23s", "List of Users\n");
		printf("\n---------------------------------\n\n");
		printf("%22s", "[-1] Exit\n");
		printf("\n---------------------------------\n\n");
		nTemp = 0;
	
		//print the list of users and their ID
		for(i = 0; i < n; i++)
		{	
			printf("[%d] %s\n", nTemp, user[i].username);
			arrTemp[nTemp] = user[i].id;						//Temp to save the id of the user
			nTemp++;
		}
		
		fflush(stdin);
		printf("\nEnter Number: ");
		scanf("%d", &nIndex);
		
		if(nIndex >= 0 && nIndex < nTemp)
		{
			//save the index of the user
			nIndex = userFinder(user, arrTemp[nIndex], n);
			
			system("cls");
			printf("\n---------------------------------\n\n");
			do
			{
				fflush(stdin);	
				//Display the status
				printf(BLUE "Account Status: " RESET);
				
				if(user[nIndex].status == 0)
				{
					printf("Unlocked\n\n");
				}
				else
				{
					printf("Locked\n\n");
				}
				
				
				//display the infos
				printf(BLUE "User Profile ID: "RESET);
				printf("%d \n\n", user[nIndex].id);
				printf(BLUE "[a] Name: \n" RESET);
				printf("%s\n\n" , user[nIndex].name);
				printf(BLUE "[b] Userame: \n" RESET);
				printf("%s\n\n", user[nIndex].username);
				printf(BLUE "Password: \n" RESET);
				printf("%s\n\n", user[nIndex].password);
				printf(BLUE "[c] Description: \n\n" RESET);
				printf("%s\n\n", user[nIndex].description);
				printf(BLUE "[d] Personal Connections:\n\n" RESET);
				
				//print personal connections
				for(i = 1; i < user[nIndex].connection.n; i++)
				{
					printf("[%d] %s\n", i, user[userFinder(user, user[nIndex].connection.connected[i], n)].username);	
				}		
				
				if(user[nIndex].connection.n == 1)
				{
					printf("[NO USER ADDED TO PERSONAL CONNECTION]\n");
				}
				
				printf("\n---------------------------------\n\n");
				printf(BLUE "[e] Exit\n\n" RESET);
				//nExit = 0;
				
				fflush(stdin);
				printf("Enter Letter to Edit: ");
				cOpt = getchar();
				
				
				//enter what to modify
				if(cOpt == 'a')
				{
					fflush(stdin);
					system("cls");
					printf("\n---------------------------------\n\n");
					
					do
					{
						nValid = 1;
						printf("%22s", "Modify Name\n");
						printf("\n---------------------------------\n\n");
						printf("%29s", "Enter Name [MAX 50] with\n");
						printf("%27s", "NO SPECIAL CHARACTERS\n");
						printf("\n---------------------------------\n\n");
						
						printf("Previous Name: %s\n", user[nIndex].name);
						
						
						fflush(stdin);
						printf(BLUE "New Name: " RESET);
						fgets(sTemp, sizeof(sTemp), stdin);
						sTemp[strlen(sTemp) - 1] = '\0';	
						
						if(strlen(sTemp) <= 50 && strlen(sTemp) >= 1)
						{
							for(i = 0; i < strlen(sTemp) && nValid; i++)
							{
								if(!((sTemp[i] >= 'a' && sTemp[i] <= 'z') || (sTemp[i] >= 'A' && sTemp[i] <= 'Z') || sTemp[i] == ' '))
								{
									nValid = 0;
								}
							}
						}
						else
						{
							nValid = 0;
						}
						
						if(!nValid)
						{
							system("cls");
							printf(RED "ERROR: INVALID NAME" RESET);
							printf("\n---------------------------------\n\n");
						}
					}while(!nValid);
					
					printf(BLUE "Confrim Change [Y]es or [N]o: " RESET);
					scanf(" %c", &cOpt);
					
					if(cOpt == 'Y' || cOpt == 'y')
					{
						strcpy(user[nIndex].name, sTemp);
						saveData(user, n);
						system("cls");
						printf(BLUE "CHANGES SAVED\n"RESET);
						printf("---------------------------------\n\n");
					}
					else 
					{
						system("cls");
						printf(RED "CHANGES DISCARTED\n" RESET);
						printf("---------------------------------\n\n");
					}
				}
				else if(cOpt == 'b')
				{
					fflush(stdin);
					system("cls");
					printf("\n---------------------------------\n\n");
					do
					{
						nValid = 1;
						printf("%24s", "Modify UserName\n");
						printf("\n---------------------------------\n\n");
						printf("Enter Userame [MAX 20]\n");
						printf("\n---------------------------------\n\n");
						printf("Modify Username\n");
						printf("\n---------------------------------\n\n");
						printf("Previous Username: %s\n", user[nIndex].username);
						fflush(stdin);
						printf(BLUE "New Userame: " RESET);
						fgets(sTemp, sizeof(sTemp), stdin);
						sTemp[strlen(sTemp) - 1] = '\0';
						
						if(strlen(sTemp) <= 20 && strlen(sTemp) >= 1)
						{
							for(i = 0; i < strlen(sTemp) && nValid; i++)
							{
								if(sTemp[i] == ' ')
								{
									nValid = 0;
								}
							}
							if(nValid)
							{
								printf(BLUE "Confrim Change [Y]es or [N]o: " RESET);
								scanf(" %c", &cOpt);
								
								if(cOpt == 'Y' || cOpt == 'y')
								{
									strcpy(user[nIndex].username, sTemp);
									saveData(user, n);
									system("cls");
									printf(BLUE "CHANGES SAVED\n"RESET);
									printf("---------------------------------\n\n");
								}
								else 
								{
									system("cls");
									printf(RED "CHANGES DISCARTED\n" RESET);
									printf("---------------------------------\n\n");
								}
							}
							else
							{
								system("cls");
								printf(RED "ERROR: INVALID USERNAME" RESET);
								printf("\n---------------------------------\n\n");
							}
							
						}
						else
						{
							system("cls");
							printf(RED "ERROR: INVALID USERNAME" RESET);
							printf("\n---------------------------------\n\n");
						}
						
					}while(!nValid);
					
				}			
				else if(cOpt == 'c')
				{
					system("cls");
					printf("\n---------------------------------\n\n");
					do
					{
						printf("%26s", "Modify Description\n");
						printf("\n---------------------------------\n\n");
						printf(BLUE "New Description: \n" RESET);
						fflush(stdin);
						fgets(mTemp, sizeof(mTemp), stdin);
						mTemp[strlen(mTemp) - 1] = '\0';
						
						if(strlen(mTemp) <= 100 && strlen(mTemp) >= 1)
						{
							printf("\n---------------------------------\n\n");
							printf(BLUE "Confrim Change [Y]es or [N]o: " RESET);
							fflush(stdin);
							scanf(" %c", &cOpt);
							
							if(cOpt == 'Y' || cOpt == 'y')
							{
								strcpy(user[nIndex].description, mTemp);
								saveData(user, n);
								system("cls");
								printf(BLUE "CHANGES SAVED\n"RESET);
								printf("---------------------------------\n\n");
							}
							else
							{
								system("cls");
								printf(RED "CHANGES DISCARTED\n" RESET);
								printf("---------------------------------\n\n");
							}
						}
						else
						{
							system("cls");
							printf(RED "ERROR: INVALID DESCRIPTION" RESET);
							printf("\n---------------------------------\n\n");
						}
						
					}while(strlen(mTemp) > 100 || strlen(mTemp) == 0);
					
					
				}
				else if(cOpt == 'd')
				{
					//print personal connections
					system("cls");
					printf("\n---------------------------------\n\n");
					
					do
					{
						printf("Modify User Personal Connection\n");
						printf("\n---------------------------------\n\n");
						printf("Enter The Number to Remove User or Add by [0]\n\n");
						printf("[0] Add Personal Connection\n");
						
						//print list of Personal connection of the user
						for(i = 1; i < user[nIndex].connection.n; i++)
						{
							printf("[%d] %s\n", i, user[userFinder(user, user[nIndex].connection.connected[i], n)].username);	
						}		
						printf("\n---------------------------------\n\n");
						printf("[-1] Exit\n");
						printf("\n---------------------------------\n\n");
						
						fflush(stdin);
						printf("Enter: ");
						scanf("%d", &nTemp);
						
						
						//check if nTemp if valid
						if(nTemp > 0 && nTemp < user[nIndex].connection.n)
						{	
							//call the remove PC function
							removePC(user, n, nIndex, message, global, nTemp);
						}
						else if(nTemp == 0)
						{
							addPCModule(user, n, nIndex);
						}
						else if(nTemp == -1)
						{
							system("cls");
							printf(BLUE "RETURNED TO VIEWING\n"RESET);
							printf("---------------------------------\n\n");
						}
						else	
						{
							system("cls");
							printf(RED "Error: INVALID INPUT\n" RESET); 
							printf("---------------------------------\n\n");
						}
					}while(nTemp != -1);
					
					
				}
				else if(cOpt == 'e')
				{
					system("cls");
					printf(BLUE "RETURNED TO LIST\n"RESET);
					printf("---------------------------------\n\n");
				}
				else
				{
					system("cls");
					printf(RED "Error: INVALID INPUT\n" RESET); 
					printf("---------------------------------\n\n");
				}
			}while(cOpt != 'e');	
		}
		else if(nIndex == -1)
		{
			system("cls");
			printf(BLUE "RETURNED TO USER MODULE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "Error: INVALID INPUT\n" RESET); 
			printf("---------------------------------\n\n");
		}
	}while(nIndex != -1);

}

/*
	adminViewUsers() this function displays the information of the user, real name, username, password, description PC and etc
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			- contains the number of users in the program
	@message 	- contains the messages information
	@global 	- structure for global datas, admin data number of messages and etc
*/
void adminViewUsers(account user, int n, global *global, message message)
{
	int i;
	int nIndex, nTemp;
	int arrTemp[MAX_USER];
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%23s", "List of Users\n");
		printf("\n---------------------------------\n\n");
		printf("%22s", "[-1] Exit\n");
		printf("\n---------------------------------\n\n");
	
		nTemp = 0;
	
		
		for(i = 0; i < n; i++)
		{	
			printf("[%d] %s\n", nTemp, user[i].username);
			arrTemp[nTemp] = user[i].id;
			nTemp++;
		}
		
		fflush(stdin);
		printf("\nEnter Number: ");
		scanf("%d", &nIndex);
		
		if(nIndex >= 0 && nIndex < nTemp)
		{
			//save the index of the user
			nIndex = userFinder(user, arrTemp[nIndex], n);
			
			system("cls");
			fflush(stdin);
			printf(BLUE "Account Status: " RESET);
				
			if(user[userFinder(user, nIndex, n)].status == 0)
			{
				printf("Unlocked\n\n");
			}
			else
			{
				printf("Locked\n\n");
			}
				
				printf(BLUE "User Profile ID: "RESET);
				printf("%d \n\n", user[nIndex].id);
				printf(BLUE "Name: \n" RESET);
				printf("%s\n\n" , user[nIndex].name);
				printf(BLUE "Userame: \n" RESET);
				printf("%s\n\n", user[nIndex].username);
				printf(BLUE "Password: \n" RESET);
				printf("%s\n\n", user[nIndex].password);
				printf(BLUE "Description: \n\n" RESET);
				printf("%s\n\n", user[nIndex].description);
				printf(BLUE "Personal Connections:\n\n" RESET);
				
				for(i = 1; i < user[nIndex].connection.n; i++)
				{	
					printf("[%d] %s\n", i, user[userFinder(user, user[nIndex].connection.connected[i], n)].username);	
				}
				
				if(user[nIndex].connection.n == 1)
				{
					printf("[NO USER ADDED TO PERSONAL CONNECTION]\n");
				}
				printf("\n---------------------------------\n\n");
				
				printf("Enter Anything to Exit: ");
				fflush(stdin);
				scanf("%d", &nTemp);
				system("cls");
				printf(BLUE "RETURNED TO USER MODULE\n"RESET);
				printf("---------------------------------\n\n");
		}
		else if(nIndex == -1)
		{
			system("cls");
			printf(BLUE "RETURNED TO USER MODULE\n"RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "Error: INVALID INPUT\n" RESET); 
			printf("---------------------------------\n\n");
		}
	}while(nIndex != -1);
	
}

/*
	adminResetPass() here the admin can reset a users password, this is also
					where the user that requested their password to be reset is located
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			- contains the number of users in the program
	@global 	- structure for global datas, admin data number of messages and etc
*/
void adminResetPass(account user, int n, global *global)
{
	int i;
	int nOpt, nIndex, nTemp;
	char cChoice;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%27s", "Refresh Account Password\n");
		printf("\n---------------------------------\n\n");
		printf("[1] List of Users\n");
		printf("[2] User Request\n");
		printf("[3] Exit\n");
		printf("\n---------------------------------\n\n");
		
		fflush(stdin);
		printf("Enter: ");
		scanf("%d", &nOpt);
		
		if(nOpt == 1)
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%23s", "List of Users\n");
			printf("\n---------------------------------\n\n");
			
			for(i = 0; i < n; i++)
			{
				printf("[%d] %s\n", i, user[i].username);
			}
			printf("\n---------------------------------\n\n");
			printf("%22s", "[-1] Exit\n");
			printf("\n---------------------------------\n\n");
			fflush(stdin);
			printf("\nEnter: ");
			scanf("%d", &nIndex);
			
			if(nIndex >= 0 && nIndex < n)
			{
				system("cls");
				printf("\n---------------------------------\n\n");
				printf("%24s", "Password Reset\n");
				printf("\n---------------------------------\n\n");
				printf("Reset User %s\' Password?\n\n", user[nIndex].username);
				printf("[Y]es or [N]o??\n\n");
				printf("Choice: ");
				fflush(stdin);
				scanf(" %c", &cChoice);
				
				if(cChoice == 'y' || cChoice == 'Y')
				{
					system("cls");
					printf(BLUE "PASSWORD RESET SUCCESSFULLY" RESET);
					printf("\n---------------------------------\n\n");
					
					//save default password
					strcpy(user[nIndex].password, "CCPROG2");
					
					encrypt(user[nIndex].password);
					
					//Reset on Progress
					user[nIndex].status = 2;
					
					//save datas
					saveData(user, n);
				}
				else
				{
					system("cls");
					printf(RED "PASSWORD RESET CANCELED" RESET);
					printf("\n---------------------------------\n\n");
				}
			}
			else if(nIndex == -1)
			{
				system("cls");
				printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
				printf("---------------------------------\n\n");
			}
			else
			{
				system("cls");
				printf(RED "INVALID INPUT" RESET);
				printf("\n---------------------------------\n\n");
			}
			
		}
		else if(nOpt == 2)
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%32s", "List of Users Requesting RESET\n");
			printf("\n---------------------------------\n\n");
			
			
			if(global->admin.nPassReq == 0)
			{
				printf(BLUE "%22s\n" RESET, "[NO REQUEST]");
			}
			
			
			for(i = 0; i < global->admin.nPassReq; i++)
			{
				nIndex = userFinder(user, global->admin.PassReq[i], n); 
				printf("[%d] %s\n", i, user[nIndex].username);
			}
			
			printf("\n---------------------------------\n\n");
			printf("%22s", "[-1] Exit\n");
			printf("\n---------------------------------\n\n");
			printf("Number: ");
			fflush(stdin);
			scanf("%d", &nTemp);
			
			if(nTemp >= 0 && nTemp < global->admin.nPassReq)
			{
				//save the index of the user inside of the array
				nIndex = userFinder(user, global->admin.PassReq[nTemp], n);
				
				system("cls");
				printf("\n---------------------------------\n\n");
				printf("%24s", "Password Reset\n"); 
				printf("\n---------------------------------\n\n");
				printf("Reset User %s\' Password?\n", user[nIndex].username);
				printf("[Y]es or [N]o??\n\n");
				printf("Choice: ");
				scanf(" %c", &cChoice);
				
				if(cChoice == 'y' || cChoice == 'Y')
				{
					system("cls");
					printf(BLUE "PASSWORD RESET SUCCESSFULLY" RESET);
					printf("\n---------------------------------\n\n");
					
					//save default password
					strcpy(user[nIndex].password, "CCPROG2");
					
					encrypt(user[nIndex].password);
					
					//on Progress
					user[nIndex].status = 2;
					
					//overwrite the current index by the next number
					for(i = nIndex; i < global->admin.nPassReq; i++)
					{
						global->admin.PassReq[i] = global->admin.PassReq[i + 1];
					}
					
					//last value becomes empty
					global->admin.PassReq[i] = '\0';
					//less the count of the users that send request
					global->admin.nPassReq--;
					
					//save datas
					saveAdminInfo(user, n, *global);
					saveData(user, n);
				}
				else
				{
					system("cls");
					printf(RED "PASSWORD RESET CANCELED" RESET);
					printf("\n---------------------------------\n\n");
				}
			}
			else if(nTemp == -1)
			{
				system("cls");
				printf(BLUE "RETURNED TO MAIN PAGE\n"RESET);
				printf("---------------------------------\n\n");
			}
			else
			{
				system("cls");
				printf(RED "INVALID INPUT" RESET);
				printf("\n---------------------------------\n\n");
			}
			
		}
		else if(nOpt == 3)
		{
			system("cls");
			printf(BLUE "Return To Main Page" RESET);
			printf("\n---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT" RESET);
			printf("\n---------------------------------\n\n");
		}
	}while(nOpt != 3);
	
	
}

/*
	adminDeleteUsers() this is where the admin can delete a user, When a user is deleted they disappear 
						from all their connections but the messages they sent out remain.
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			- contains the number of users in the program
	@global 	- structure for global datas, admin data number of messages and etc
	@messages	- contains the messages information
*/
void adminDeleteUsers(account user, int *n, global *global, message message)
{
	int i, j, k;
	int nIndex;
	char nChoice;
	int nFound;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%22s", "Delete Users\n");
		printf("\n---------------------------------\n\n");
		printf("%21s", "[-1] Exit\n");
		printf("\n---------------------------------\n\n");
		
	
		//print usernames
		for(i = 0; i < *n; i++)
		{	
			printf("[%d] %s\n", i, user[i].username);
		}
		printf("\n---------------------------------\n\n");
		
		fflush(stdin);
		printf("Enter: ");
		scanf("%d", &nIndex);
		
		if(nIndex >= 0 && nIndex < *n)
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%22s", "Delete Users\n");
			printf("\n---------------------------------\n\n");
			printf("Delete User %s?\n", user[nIndex].username);
			printf("[Y]es or [N]o\n");
			
			fflush(stdin);
			printf("Choice: ");	
			scanf(" %c", &nChoice);
			
			if(nChoice == 'y' || nChoice == 'Y')
			{
				//delete the users from the personal connections of other user
				for(i = 0; i < *n; i++)
				{
					nFound = 0;
					
					for(j = 1; j < user[i].connection.n && !nFound; j++)
					{
						//if the user is found in a users personal connection
						if(user[nIndex].id == user[i].connection.connected[j])
						{
							//delete the user from the connection of that user
							for(k = j; k < user[i].connection.n - 1; k++)
								user[i].connection.connected[k] = user[i].connection.connected[k + 1];
								
							user[i].connection.n--;
							nFound = 1;
						}
					}
				}
				
				//update the senders ID
				for(i = 0; i < global->nMessages; i++)
				{
					if(message[i].sender == user[nIndex].id)
					{
						message[i].sender = -1;
					}
				}
				
				//update the recievers ID
				for(i = 0; i < global->nMessages; i++)
				{
					if(message[i].reciever == user[nIndex].id)
					{
						message[i].reciever = -1;
					}
				}
				
				//overwrite the current index of the user by the next users
				for(i = nIndex; i < *n - 1; i++)
				{
					user[i] = user[i + 1];
				}
				
				(*n)--;
				
				system("cls");
				printf(BLUE "USER DELETED" RESET);
				printf("\n---------------------------------\n\n");
				saveData(user, *n);
				saveMessages(message, global);
	
			}
			else
			{
				system("cls");
				printf(RED "ACTION CANCELED" RESET);
				printf("\n---------------------------------\n\n");
			}
		}
		else if(nIndex == -1)
		{
			system("cls");
			printf(BLUE "Return To Main Page" RESET);
			printf("\n---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "ERROR: INVALID INPUT" RESET);
			printf("\n---------------------------------\n\n");
		}
	}while(nIndex != -1);
	
	
}

/*
	adminUserModule() this function is the main user module of the admin, where the admin can choose things to do with a user
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n	 		-	contains the number of users in the Program gets appended 1 ever new user
	@questions	- Contains the security questions
	@nQuestions	- contains the number of questions
	@message	- contains the information of the messages
	@global		- structure for global datas / miscellaneous, contains the availabe ID of user and messages
	
*/
void adminUserModule(account user, int *n, QA questions, int *nQuestions, message message, global *global)
{
	int nOpt;

	system("cls");
	printf("\n---------------------------------\n\n");
	do
	{
		printf("%22s", "USER MODULE\n");
		printf("\n---------------------------------\n\n");
		printf("[1] View User\n");
		printf("[2] Modify Users\n");
		printf("[3] Refresh User Account Password\n");
		printf("[4] Delete Users\n");
		printf("[5] Security Questions\n");
		printf("[6] Return\n\n");
		
		fflush(stdin);
		printf("Enter Choice: ");
		scanf("%d", &nOpt);
		
		switch(nOpt)
		{
			case 1: adminViewUsers(user, *n, global, message);			//call user viewer
					break;
			case 2: adminModifyUsers(user, *n, global, message);		//call modify user admin
					break;
			case 3: adminResetPass(user, *n, global); 					//reset password module
					break;
			case 4: adminDeleteUsers(user, n, global, message);			//Delete users
					break;
			case 5: adminSecurityQuestion(questions, nQuestions);		//admin security question
					break;
			case 6: system("cls");
					printf(BLUE "RETURNED TO MAIN PAGE\n" RESET);
					printf("---------------------------------\n\n");
					break;
			default: system("cls");
					printf(RED "Error: INVALID INPUT\n" RESET); 
					printf("---------------------------------\n\n");
		}
	}while(nOpt != 6);
	
}

/*
	adminViewMessages() this is where the admin can view all the messages in the program, and the specific info of the message
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@message 	- contains the messages information
	@global 	- structure for global datas,admin data, contains number of messages that exist and the availabe ID of messages and etc.
*/
void adminViewMessages(account user, int n, message message, global global)
{
	int i;
	int messInd;
	string sTemp;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%23s", "View Messages\n");
		printf("\n---------------------------------\n\n");
		printf("%22s", "[-1] Exit\n");
		printf("\n---------------------------------\n\n");
		
		
		//display all messages by subjects and index
		for(i = 0; i < global.nMessages; i ++)
		{
			printf("[%d] %s\n", i, message[i].subject);
		}
		
		fflush(stdin);
		printf("\nEnter: ");
		scanf("%d", &messInd);
		
		if(messInd >= 0 && messInd < global.nMessages)
		{
			//display the message infos based on the index chosen by the admin
		
			if(messInd >= 0 && messInd < global.nMessages)
			{
				//display the message info
				displayMessageInfo(user, n, message, global, messInd);
				
				printf("Enter Anykey to exit: ");
				scanf("%s", sTemp);		
				system("cls");
				printf(BLUE "RETURNED TO MAIN PAGE\n" RESET);
				printf("---------------------------------\n\n");
			}
		}
		else if (messInd == -1)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n" RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "Error: INVALID INPUT\n" RESET); 
			printf("---------------------------------\n\n");
		}
		
	}while(messInd != -1);

}

/*
	adminViewbyFilter() this is where the admin can view all the messages in the program but filterd by a specific user either
						be reciever or sender of the messages
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@message 	- contains the messages information
	@global 	- structure for global datas,admin data, contains number of messages that exist and the availabe ID of messages and etc.
*/
void adminViewbyFilter(account user, int n, message message, global global)
{
	int i;
	int nOpt, nFound, nIndex, messInd;
	string sTemp;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		nFound = 0;
		printf("%28s", "View Message by Filter\n");
		printf("\n---------------------------------\n\n");
		printf("[1] Filter by Sender\n");
		printf("[2] Filter by Recipient\n");
		printf("[3] Exit\n");
		printf("---------------------------------\n\n");	
		
		printf("\nEnter: ");
		fflush(stdin);
		scanf("%d", &nOpt);
		
		if(nOpt == 1)	//If filter by sender
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%28s", "View Message by Filter\n");
			printf("\n---------------------------------\n\n");
			printf("Instruction:\n\n");
			printf("Enter Name or Username of Sender\n");
			printf("---------------------------------\n\n");	
			printf("\nSearch: ");
			
			fflush(stdin);
			//get the name or username of user
			fgets(sTemp, sizeof(sTemp), stdin);
			sTemp[strlen(sTemp) - 1] = '\0';
			
			for(i = 0; i < n && !nFound; i++)
			{
				if(strcmp(user[i].username, sTemp) == 0 || strcmp(user[i].name, sTemp) == 0)
				{
					nIndex = i;
					nFound = 1;
				}
			}
			
			
			if(nFound)
			{
				system("cls");
				
				printf(BLUE "Messages Sent By \"%s\" (Aka %s)\n\n" RESET, user[nIndex].name, user[nIndex].username);
				//scan the whole messages for the same user searched
				for(i = 0; i < user[nIndex].nSent; i++)
				{
					messInd = messIndFinder(message, global, user[nIndex].sent[i]);
					printf("[%d] %s\n", i, message[messInd].subject);
				}
				printf("---------------------------------\n\n");	
				printf("[-1] EXIT MESSAGES\n");
				printf("\n---------------------------------\n\n");	
		
				//if the user didnt sent any messages yet
				if(user[nIndex].nSent == 0)
				{
					printf("[NO MESSAGES SENT]\n\n");
					printf("---------------------------------\n\n");	
					printf("Enter Anykey to exit: ");
					scanf("%s", sTemp);		
					system("cls");
					printf(BLUE "RETURNED TO MAIN PAGE\n" RESET);
					printf("---------------------------------\n\n");	
				}
				else
				{
					system("cls");
					do
					{
						printf(BLUE "Messages Sent By \"%s\" (Aka %s)\n\n" RESET, user[nIndex].name, user[nIndex].username);
						//scan the whole messages for the same user searched
						for(i = 0; i < user[nIndex].nSent; i++)
						{
							messInd = messIndFinder(message, global, user[nIndex].sent[i]);
							printf("[%d] %s\n", i, message[messInd].subject);
						}
						
						fflush(stdin);
						printf("---------------------------------\n\n");	
						printf("[-1] EXIT MESSAGES\n");
						printf("\n---------------------------------\n\n");	
						printf("\nNumber: ");
						scanf("%d", &messInd);
						
						if(messInd >= 0 && messInd < user[nIndex].nSent)
						{
							//display the message info
							
							messInd = messIndFinder(message, global, user[nIndex].sent[messInd]);
							
							displayMessageInfo(user, n, message, global, messInd);
							
							fflush(stdin);
							printf("Enter Anykey to exit: ");
							scanf("%s", sTemp);			
							system("cls");
							printf(BLUE "RETURNED TO MAIN PAGE\n" RESET);
							printf("---------------------------------\n\n");
						}
						else if(messInd == -1)
						{
							system("cls");
							printf(BLUE "RETURNED TO MESSAGE MODULE\n" RESET);
							printf("---------------------------------\n\n");
						}
						else
						{
							system("cls");
							printf(RED "ERROR: INVALID INPUT\n" RESET);
							printf("---------------------------------\n\n");
						}
					}while(messInd != -1);
					
				}
					
			}
			else
			{
				system("cls");
				printf(RED "ERROR: USER NOT FOUND\n" RESET);
				printf("---------------------------------\n\n");
			}
		}
		else if(nOpt == 2)		//filter by recipient
		{
			system("cls");
			printf("\n---------------------------------\n\n");
			printf("%28s", "View Message by Filter\n");
			printf("\n---------------------------------\n\n");
			printf("Instruction:\n\n");
			printf("Enter Name or Username of Recipient\n");
			printf("---------------------------------\n\n");
			printf("\nSearch: ");
			
			fflush(stdin);
			//get the name or username of user
			fgets(sTemp, sizeof(sTemp), stdin);
			sTemp[strlen(sTemp) - 1] = '\0';
			
			//find the index of the username
			for(i = 0; i < n && !nFound; i++)
			{
				if(strcmp(user[i].username, sTemp) == 0 || strcmp(user[i].name, sTemp) == 0)
				{
					nIndex = i;
					nFound = 1;
				}
			}
			
			//if it exist
			if(nFound)
			{
				system("cls");
				
				printf(BLUE "Messages Recieved By \"%s\" (Aka %s)\n\n" RESET, user[nIndex].name, user[nIndex].username);
				
				//scan the inbox of the user
				for(i = 0; i < user[nIndex].nInbox; i++)
				{
					messInd = messIndFinder(message, global, user[nIndex].inbox[i]);
					printf("[%d] %s\n", i, message[messInd].subject);
				}
				printf("---------------------------------\n\n");
				printf("[-1] EXIT MESSAGES\n");
				printf("\n---------------------------------\n\n");
				
				//display the message info 
				if(user[nIndex].nInbox == 0)
				{
					printf("NO MESSAGES RECIEVED\n\n");
					printf("---------------------------------\n\n");
					printf("Enter Anykey to exit: ");
					scanf("%s", sTemp);	
					system("cls");
					printf(BLUE "RETURNED TO MESSAGE MODULE\n" RESET);
					printf("---------------------------------\n\n");
						
				}
				else
				{
					system("cls");
					
					do
					{
						printf(BLUE "Messages Recieved By \"%s\" (Aka %s)\n\n" RESET, user[nIndex].name, user[nIndex].username);
						//scan the whole messages for the same user searched
						
						for(i = 0; i < user[nIndex].nInbox; i++)
						{
							messInd = messIndFinder(message, global, user[nIndex].inbox[i]);
							printf("[%d] %s\n", i, message[messInd].subject);
						}
						printf("---------------------------------\n\n");
						printf("[-1] EXIT MESSAGES\n");
						printf("\n---------------------------------\n\n");
						
						fflush(stdin);
						printf("\nNumber: ");
						scanf("%d", &messInd);
						
						if(messInd >= 0 && messInd < user[nIndex].nInbox)
						{
							messInd = messIndFinder(message, global, user[nIndex].inbox[messInd]);
							
							displayMessageInfo(user, n, message, global, messInd);
							
							fflush(stdin);
							printf("Enter Anykey to exit: ");
							scanf("%s", sTemp);		
							system("cls");
							printf(BLUE "RETURNED TO MESSAGE MODULE\n" RESET);
							printf("---------------------------------\n\n");
						}
						else if (messInd == -1)
						{
							system("cls");
							printf(BLUE "RETURNED TO MESSAGE MODULE\n" RESET);
							printf("---------------------------------\n\n");
						}
						else
						{
							system("cls");
							printf(RED "ERROR: INVALID INPUT\n" RESET);
							printf("---------------------------------\n\n");
						}	
					}while(messInd != -1);
					
				}
			}
			else
			{
				system("cls");
				printf(RED "ERROR: USER NOT FOUND\n" RESET);
				printf("---------------------------------\n\n");
			}
			
		}
		else if(nOpt == 3)	//if exit
		{
			system("cls");
			printf(BLUE "RETURNED TO MESSAGE MODULE\n" RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "Error: INVALID INPUT\n" RESET); 
			printf("---------------------------------\n\n");
		}
	}while (nOpt != 3);
	
}

/*
	adminDeleteMess() this is where the admin can delete a message, deleted message will disapear in all users inbox or sent
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@message 	- contains the messages information
	@global 	- structure for global datas,admin data, contains number of messages that exist and the availabe ID of messages and etc.
*/
void adminDeleteMess(account user, int n, message message, global *global)
{
	int i, j, k;
	int messInd, type;
	char cChoice;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%23s", "View Messages\n");
		printf("\n---------------------------------\n\n");
		printf("%22s", "[-1] Exit\n");
		printf("\n---------------------------------\n\n");
		
		//display all messages by subjects and index
		for(i = 0; i < global->nMessages; i ++)
		{
			printf("[%d] %s\n", i, message[i].subject);
		}
		
		printf("\n---------------------------------\n\n");
		fflush(stdin);
		printf("\nEnter: ");
		scanf("%d", &messInd);
		
		
		//display the message infos based on the index chosen by the admin
		
		if(messInd >= 0 && messInd < global->nMessages)
		{
			//display the message info
			displayMessageInfo(user, n, message, *global, messInd);
			
			printf("Delete message?\n");
			printf("[Y] for Yes | Anykey to cancel\n");
			printf("\n---------------------------------\n\n");
			printf("Choice: ");
			fflush(stdin);
			scanf(" %c", &cChoice);
			
			if(cChoice == 'y' || cChoice == 'Y')
			{
				//check every user
				for(i = 0; i < n; i++)
				{
					//delete in sent if any
					for(j = 0; j < user[i].nSent; j++)
					{
						if(message[messInd].id == user[i].sent[j])
						{
							for(k = j; k < user[i].nSent - 1; k++)
							{
								user[i].sent[k] = user[i].sent[k + 1];
							}
							user[j].nSent--;
						}
					}
					
					//delete in inbox if any
					for(j = 0; j < user[i].nInbox; j++)
					{
						if(message[messInd].id == user[i].inbox[j])
						{
							for(k = j; k < user[i].nInbox - 1; k++)
							{
								user[i].inbox[k] = user[i].inbox[k + 1];
							}
							
							user[j].nInbox--;
						}
					}
					
					saveData(user, n);
				}
				
				type = message[messInd].type;
				
				//delete message
				for(i = messInd; i < global->nMessages - 1; i++)
				{	
					message[i] = message[i + 1];
				}
				
				global->nMessages--;
				
				if(type == 3)
				{
					global->nAnnouncement--;
				}
				
				saveMessages(message, global);
				saveData(user, n);
				system("cls");
				printf(BLUE "SUCCESSFULLY DELETED\n" RESET);
				printf("---------------------------------\n\n");
			}
			else
			{
				system("cls");
				printf(RED "Error: CANCELED\n" RESET); 
				printf("---------------------------------\n\n");
			}
			
			
		}
		else if (messInd == -1)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n" RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "Error: INVALID INPUT\n" RESET); 
			printf("---------------------------------\n\n");
		}	
	}while(messInd != -1);
	
}

/*
	adminMessageModule() this is the admin's main module for messages where they can do specific thins related to messages
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@message 	- contains the messages information
	@global 	- structure for global datas,admin data, contains number of messages that exist and the availabe ID of messages and etc.
*/
void adminMessageModule(account user, int *n, message message, global *global)
{
	int nOpt;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%24s", "Message Module\n\n");
		printf("\n---------------------------------\n\n");
		printf("[1] View Messages\n");
		printf("[2] View by Filter\n");
		printf("[3] Delete Message\n");
		printf("[4] Exit\n\n");
		
		printf("Enter: ");
		fflush(stdin);
		scanf(" %d", &nOpt);
		
		if(nOpt == 1)
		{
			adminViewMessages(user, *n, message, *global);
		}
		else if(nOpt == 2)
		{
			adminViewbyFilter(user, *n, message, *global);
		}
		else if(nOpt == 3)
		{
			adminDeleteMess(user, *n, message, global);
		}
		else if(nOpt == 4)
		{
			system("cls");
			printf(BLUE "RETURNED TO MAIN PAGE\n" RESET);
			printf("---------------------------------\n\n");
		}
		else
		{
			system("cls");
			printf(RED "Error: INVALID INPUT\n" RESET); 
			printf("---------------------------------\n\n");
		}
	}while(nOpt != 4);
	
}

/*
	adminChangePass() this is the where the admin can change the password for the admin page
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n 			-	contains the number of users in the program
	@global 	- structure for global datas,admin data, contains number of messages that exist and the availabe ID of messages and etc.
*/
void adminChangePass(account user, int n, global *global)
{
	string pass, rePass;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	printf("%27s", "Change Admin Password\n");
	printf("\n---------------------------------\n\n");
	printf("Enter Old Password\n");
	printf("Password: ");
	
	fflush(stdin);
	fgets(pass, sizeof(pass), stdin);
	pass[strlen(pass) - 1] = '\0';
	
	encrypt(pass);
	
	if(strcmp(pass, global->admin.password) == 0)
	{
		system("cls");
		printf("\n---------------------------------\n\n");
		
		do
		{
			printf("%27s", "Change Admin Password\n");
			printf("\n---------------------------------\n\n"); 
			printf("New Password [MAX 20]: ");
			
			fflush(stdin);
			fgets(pass, sizeof(pass), stdin);
			pass[strlen(pass) - 1] = '\0';
			
			if(strlen(pass) > 20)
			{
				system("cls");
				printf(RED "Error: EXCEEDS MAX\n" RESET); 
				printf("---------------------------------\n\n");
			}
			else if(strlen(pass) == 0)
			{
				system("cls");
				printf(RED "Error: NO INPUT\n" RESET); 
				printf("---------------------------------\n\n");
			}
		}while(strlen(pass) > 20 || strlen(pass) == 0);
		
		
		system("cls");
		printf("\n---------------------------------\n\n");
		printf("%27s", "Change Admin Password\n");
		printf("\n---------------------------------\n\n");
		
		printf("Re-Enter Password: ");
		fflush(stdin);
		fgets(rePass, sizeof(rePass), stdin);
		rePass[strlen(rePass) - 1] = '\0';
		
		if(strcmp(pass, rePass) == 0)
		{
			system("cls");
			printf(BLUE "PASSWORD SUCCESSFULLY CHANGED\n" RESET);
			printf("---------------------------------\n\n");
			
			encrypt(pass);
			strcpy(global->admin.password, pass);
			saveAdminInfo(user, n, *global);
		}
		else
		{
			system("cls");
			printf(RED "ERROR: PASSWORD DONT MATCH\n" RESET);
			printf("---------------------------------\n\n");
		}
		
	}
	else
	{
		system("cls");
		printf(RED "ERROR: INCORRECT PASSWORD\n" RESET);
		printf("---------------------------------\n\n");
		
	}
}

/*
	adminPage() this is the main page of the admin where they can choose what to do in the admin page 
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n	 		- contains the number of users in the Program gets appended 1 ever new user
	@questions	- Contains the security questions
	@nQuestions	- contains the number of questions
	@message	- contains the information of the messages
	@global		- structure for global datas / miscellaneous, contains the availabe ID of user and messages
	
*/
void adminPage(account user, int *n, QA questions, int *nQuestions, message message, global *global)
{
	int nOpt;
	
	system("cls");
	printf("\n---------------------------------\n\n");
	do
	{
		
		printf("%28s", "WELCOME GUARDIAN MASTER\n");
		printf("\n---------------------------------\n\n");
	
		printf("[1] Users Module\n");
		printf("[2] Messages Module\n");
		printf("[3] Change Password Module\n");
		printf("[4] Log off\n\n");
		
		printf("Enter Choice: ");
		scanf("%d", &nOpt);
		fflush(stdin);
		
		switch(nOpt)
		{
			case 1: adminUserModule(user, n, questions, nQuestions, message, global);			//userModule
					break;
			case 2: adminMessageModule(user, n, message, global);								//messageModule
					break;
			case 3: adminChangePass(user, *n, global);
					break;
			case 4: system("cls"); 
					printf(BLUE "RETURNED TO MAIN PAGE" RESET);
					break;
			default: system("cls");
					printf(RED "Error: INVALID INPUT\n" RESET); 
					printf("---------------------------------\n\n");
			
		}
		
	}while(nOpt != 4);
	
}

/*
	adminLoginPage() this is where a user can login to the admin page
	@user 		- structure array containing the Info of users and where the question and answer will be saved
	@n	 		- contains the number of users in the Program gets appended 1 ever new user
	@questions	- Contains the security questions
	@nQuestions	- contains the number of questions
	@message	- contains the information of the messages
	@global		- structure for global datas / miscellaneous, contains the availabe ID of user and messages
	
*/
void adminLoginPage(account user, int *n, QA questions, int *nQuestions, message message, global *global)
{
	string inputPass;
	
	printf("\n---------------------------------\n\n");
	printf("%29s\n", "GUARDIAN MASTER ADMIN ^w^");
	printf("\n---------------------------------\n\n");
	fflush(stdin);
	printf("Password: ");
	fgets(inputPass, sizeof(inputPass), stdin);
	inputPass[strlen(inputPass) - 1] = '\0';
	
	encrypt(inputPass);
	
	//nextime have encryption	
	if (strcmp(inputPass, global->admin.password) == 0)
	{
		adminPage(user, n, questions, nQuestions, message, global);
	}
	else
	{
		system("cls");
		printf(RED "Error: WRONG PASSWORD" RESET); 
	}

}

/*
	loadAdminInfo() this loads the data from the admin.txt to the global.admin structure
	@global		- structure for global datas / miscellaneous and admin data
	
*/
void loadAdminInfo(global *global)
{
	FILE *pFile;
	string sTemp;
	int i;
	
	pFile = fopen("admin.txt", "rt");
	
	if(pFile != NULL)
	{
		//password of the admin
		fgets(sTemp, sizeof(sTemp), pFile);
		sTemp[strlen(sTemp) - 1] = '\0';
		strcpy(global->admin.password, sTemp);
		
		//number of user who requested password reset
		fgets(sTemp, sizeof(sTemp), pFile);
		global->admin.nPassReq = atoi(sTemp);
		
		for (i = 0; i < global->admin.nPassReq; i++)
		{
			//gets the ID of the user that request password reset
			fgets(sTemp, sizeof(sTemp), pFile);
			global->admin.PassReq[i] = atoi(sTemp);
		}
		
		//Count of IDs for user
		fgets(sTemp, sizeof(sTemp), pFile);
		global->nUsersID = atoi(sTemp);
		
		//count of IDs for messages
		fgets(sTemp, sizeof(sTemp), pFile);
		global->nMessID = atoi(sTemp);
		
		fclose(pFile);
	}
	else
	{
		global->nUsersID = 0;
		global->nMessID = 0;
		strcpy(global->admin.password, "");
		strcpy(global->admin.password, "password");
		encrypt(global->admin.password);
		global->admin.nPassReq = 0;		//0 users = 0 request
	}
	
}

/*
	main() - the main function where the user can choose to create an account, login to the messenger, 
				or access the admin moduels
*/
int main()
{
	int nOpt;
	account user;
	int nUsers, nQuestions;	//number of users
	QA SecQuestions;		//security questions
	message message;		//messages
	global global;			//structure for the global
	
	
	//load the data
	loadAdminInfo(&global);
	loadAccount(user, &nUsers, SecQuestions, &nQuestions, message, &global);
	
	saveData(user, nUsers);
	saveMessages(message, &global);
	saveAdminInfo(user, nUsers, global);
	
	printf("\n---------------------------------\n\n");
	
	do
	{
		printf("%29s\n", "WELCOME TO GUMMIPHONE ^w^");
		printf("\n---------------------------------\n\n");
		
		printf("[1] Create new Account - %d\n", nUsers);
		printf("[2] Login\n");
		printf("[3] Admin Page\n");
		printf("[4] Exit Messenger\n");
		
		fflush(stdin);
		printf("\nEnter Choice: ");
		scanf("%d", &nOpt);
		
		system("cls");
		
		switch(nOpt)
		{
			case 1: createAccount(user, &nUsers, SecQuestions, &nQuestions, &global);
				break;
			case 2: loginUser(user, nUsers, message, &global); 
				break;
			case 3: adminLoginPage(user, &nUsers, SecQuestions, &nQuestions, message, &global);
				break;
			case 4: printf(BLUE "BAI BAI ^_^\n" RESET);
				break;
			default: printf(RED "Error: Invalid Input" RESET); 
		}
		
		if (nOpt != 4)
			printf("\n---------------------------------\n\n");
		
		
	}while(nOpt != 4);
	

	return 0;
}