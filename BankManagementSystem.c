#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include<dos.h>
#include<dir.h>
#define ACCOUNTS "AccountRecord.dat"
#define TRANSC "Transactions.dat"
#define BR_CODE 1822

typedef union  {float savings; float current; float fixed;} Deposit;
typedef struct {short int day; short int month; short int year;} Date;
typedef struct {char street[20]; char city[16]; char state[20]; int pincode; short int houseNo;} Address;
typedef struct {char name[32]; char citizenshipNo[20]; Address addr; Date dob; Deposit amount; char type; short int fxd_yrs; int acc_no; long long int phoneNo;} Customer;
typedef struct {int from_acc; int to_acc; float amt; Date date;} Transaction;

void setColor(int ForgC)
{
 	WORD wColor;

  	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  	CONSOLE_SCREEN_BUFFER_INFO csbi;                       //We use csbi for the wAttributes word.
 	if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 	{
    	wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);	//Mask out all but the background attribute, and add in the forgournd color
    	SetConsoleTextAttribute(hStdOut, wColor);
 	}
 	return;
}

void getAddress(Address *a)
{
	printf("\n\tEnter House No. : ");		fflush(stdin);	scanf("%x", &a->houseNo);
	printf("\tEnter Street Name : ");		fflush(stdin);  gets(a->street);
	printf("\tEnter City : ");				fflush(stdin);  gets(a->city);
	printf("\tEnter Pincode : ");			fflush(stdin);	scanf("%d", &a->pincode);
	printf("\tEnter State : ");				fflush(stdin);  gets(a->state);
}
int addrcmp(Address a, Address b)
{
	return (!strcmp(a.street, b.street) && !strcmp(a.city, b.city) && !strcmp(a.state, b.state) && a.pincode == b.pincode && a.houseNo == b.houseNo);
}
void getDate(Date *d)
{
	char date[11];
	fflush(stdin);  gets(date);
	d->day = ((int)date[0] - 48) * 10 + ((int)date[1] - 48);
	d->month = ((int)date[3] - 48) * 10 + ((int)date[4] - 48);
	d->year = ((int)date[6] - 48) * 1000 + ((int)date[7] - 48) * 100 + ((int)date[8] - 48) * 10 + ((int)date[9] - 48);
}
char getDeposit(Deposit *deposit, short int *fdyrs)
{
	char type; float amount; 
	printf("\n\tEnter type of Deposit : ");	scanf("%c", &type);
	printf("\n\tEnter Deposit Amount : ");	scanf("%f", &amount);	
	switch(type)
	{
		case 's': case 'S': deposit->savings = amount; break;		
		case 'c': case 'C': deposit->current = amount; break;		
		case 'f': case 'F': deposit->fixed = amount;
				  printf("\n\tEnter Period in Years : ");	  scanf("%d", fdyrs); 
				  break;				  
	}
	return type;
}
void viewlist(Customer x)
{	
	if(strlen(x.name) > 15)
	{
		setColor(12);	printf("%d\t", x.acc_no);	setColor(7);	printf("%s\t", x.name);		setColor(14);	printf("%lld\t", x.phoneNo);
	}
	else
	{
		setColor(12);	printf("%d\t", x.acc_no);	setColor(7);	printf("%s\t\t", x.name);	setColor(14);	printf("%lld\t", x.phoneNo);
	}
	setColor(15);
	printf("%d, %s, %s - %d, %s\n", x.addr.houseNo, x.addr.street, x.addr.city, x.addr.pincode, x.addr.state);
	printf("\n");
}
void view()
{
	FILE *fp = fopen(ACCOUNTS, "rb");	Customer x;
	printf("\n\nA/C No.\tName\t\t\tPhone No.\tAddress\n");
	while(fread(&x, sizeof(Customer), 1, fp))
		viewlist(x);
}
void see(Customer x)	
{
	printf("\n\tName : %s", x.name);
	printf("\n\tA/C No. : %d", x.acc_no);
	printf("\n\tPhone No. : %lld", x.phoneNo);
	printf("\n\tCitizenship No. : %s", x.citizenshipNo);
	printf("\n\tAddress : %x, %s, %s - %d, %s", x.addr.houseNo, x.addr.street, x.addr.city, x.addr.pincode, x.addr.state);
	printf("\n\tDate of Birth: %d/%d/%d", x.dob.day, x.dob.month, x.dob.year);	
	switch(x.type)
	{
		case 's': case 'S': printf("\n\tA/C Type : Savings");
							printf("\n\tDeposit Amount : Rs. %.2f", x.amount.savings);
							printf("\n\tInterest @ 3 %c Per Annum = Rs. %.2f", 37, (x.amount.savings * 0.03));
							break;
		case 'c': case 'C': printf("\n\tA/C Type : Current");
							printf("\n\tDeposit Amount : Rs. %.2f", x.amount.current);
							break;
		case 'f': case 'F': printf("\n\tA/C type : Fixed");
							printf("\n\tDeposit Amount: Rs. %.2f", x.amount.fixed);
							printf("\n\tInterest @ 7.75 %c Per Annum = Rs. %.2f", 37,  x.amount.fixed * 0.0775);
							break;
	}
}
void new_acc(Customer *x) 	
{
	printf("\n\tEnter the Name : ");							fflush(stdin);  gets(x->name);
	printf("\n\tEnter Address: ");								fflush(stdin);  getAddress(&x->addr);
	printf("\n\tEnter Phone No. : ");							scanf("%lld", &x->phoneNo);
	printf("\n\tEnter Citizenship No. (PAN / AADHAR ): ");		fflush(stdin);  gets(x->citizenshipNo);
	printf("\n\tEnter Date of Birth as (DD/MM/YYYY) : ");		getDate(&x->dob);
	printf("\n\tEnter type of Deposit. \n\t 'S' -> Savings 'C' -> Current 'F' -> Fixed \n");	
	x->type = getDeposit(&x->amount, &(x->fxd_yrs));	
}
void storeAccount(Customer x)	
{
	FILE *fp; 
	fp = fopen(ACCOUNTS, "ab");
	fwrite(&x, sizeof(Customer), 1, fp);
	fclose(fp);
}
Customer setAccNo()				
{
	FILE *fp;	Customer x; int totalrecords;
	fp = fopen(ACCOUNTS, "rb+");	
	fseek(fp, 0, SEEK_END);
	totalrecords = ftell(fp) / sizeof(Customer);	
	fseek(fp, -sizeof(Customer), SEEK_END);	
	fread(&x, sizeof(Customer), 1, fp);
	if(totalrecords == 1)
		x.acc_no = (BR_CODE * 1000) + 1;
	else
		x.acc_no = (BR_CODE * 1000) + totalrecords;
	fseek(fp, -sizeof(Customer), SEEK_END);
	fwrite(&x, sizeof(Customer), 1, fp);
	fclose(fp);
	return x;
}
Customer search()		
{
	FILE *fp; int c = 0; Customer x; char name[32]; long long int phNo;
	fp = fopen(ACCOUNTS, "rb");	
	printf("\n\tEnter Name : ");			fflush(stdin);	gets(name);
	printf("\n\tEnter Phone No. : ");						scanf("%lld", &phNo);		
	while(fread(&x, sizeof(Customer), 1, fp))	
		if(strcmp(x.name, name) == 0 && x.phoneNo == phNo)
		{
			printf("\n\t\tAccount Found");
			return x;
		}	
	x.acc_no = -1; printf("\n\t\tAccount Not Found !");
	return x;
}
void recordTransaction(Transaction t)
{
	FILE *fp;	
	fp = fopen(TRANSC, "ab");
	fwrite(&t, sizeof(Transaction), 1, fp);
	fclose(fp);
}
void viewTransactionList()
{
	FILE *fp = fopen(TRANSC, "rb"); Transaction t;
	printf("\n\tDate\t\tPayer's A/c No.\tPayee's A/c No.\tAmount");
	while(fread(&t, sizeof(Transaction), 1, fp))
	{
		setColor(14);	printf("\n\t%d/%d/%d\t", t.date.day, t.date.month, t.date.year);
		setColor(12);	printf("%d\t\t", t.from_acc);  setColor(10);	printf("%d\t\t", t.to_acc); setColor(15);	printf("%f\n", t.amt);
	}
	fclose(fp);
}
void transact()	
{
	Customer payer, recipient;	float amount, payerBalance, recpBalance; 	Transaction t;	Date d;	
	printf("\n\n\t\tPlease Enter Payer's Details : \n\n ");		payer = search();
	printf("\n\n\t\tPlease Enter Recipient's Details : \n\n");	recipient = search();	
	if(payer.acc_no != -1 && recipient.acc_no != 1)
	{
		switch(payer.type)
		{
			case 'c': case 'C': payerBalance = payer.amount.current; break;			
			case 's': case 'S': payerBalance = payer.amount.savings; break;			
			default: printf("Invalid Choice");
		}		
		printf("\n\tEnter Amount : ");		scanf("%f", &amount);
		printf("\n\tEnter Date : ");		getDate(&d);		
		if(payerBalance >= amount)
		{
			switch(payer.type)
			{
				case 'c': case 'C': payer.amount.current -= amount; break;				
				case 's': case 'S': payer.amount.savings -= amount; break;
			}			
			switch(recipient.type)
			{
				case 'c': case 'C': recipient.amount.current += amount; break;				
				case 's': case 'S': recipient.amount.savings += amount; break;
			}
			
			t.amt = amount; 	t.date = d; 	t.from_acc = payer.acc_no;	t.to_acc = recipient.acc_no;
			recordTransaction(t);
			printf("\t\t\t\tTransaction Successfull.");
		}
		else
			printf("\n\n\t\tInsuffiecient Balance");		
	}
	else
		printf("\n\n\t\tInvalid Transaction");	
}
void erase(Customer x)
{
	FILE *fp, *fpt; Customer y;	
	fp = fopen(ACCOUNTS, "rb");		fpt = fopen("temp.dat", "wb");	
	while(fread(&y, sizeof(Customer), 1, fp))	
		if(y.acc_no != x.acc_no)
			fwrite(&y, sizeof(Customer), 1, fpt);	
	fclose(fp);	fclose(fpt);	
	fp = fopen(ACCOUNTS, "wb"); 	fpt = fopen("temp.dat", "rb");	
	while(fread(&y, sizeof(Customer), 1, fpt))
		fwrite(&y, sizeof(Customer), 1, fp);		
	fclose(fp); 	remove("temp.dat");
}
void edit(Customer x)
{
	FILE *fp; Customer y;	
	fp = fopen(ACCOUNTS, "rb+");	
	printf("\tEnter Address : ");										fflush(stdin);  getAddress(&x.addr);
	printf("\tEnter Phone No. (in Case of No Change Enter Zero): ");	fflush(stdin);	scanf("%lld", &x.phoneNo);	
	while(fread(&y, sizeof(Customer), 1, fp))
	{
		if(y.acc_no == x.acc_no)
		{
			if(addrcmp(x.addr, y.addr) == 0)
			{
				if(x.addr.houseNo != 0 && y.addr.houseNo != x.addr.houseNo)
					y.addr.houseNo = x.addr.houseNo;
				if(x.addr.pincode != 0 && y.addr.pincode != x.addr.pincode)
					y.addr.pincode = x.addr.pincode;
				if(strlen(x.addr.city) && strcmp(y.addr.city, x.addr.city))
					strcpy(y.addr.city, x.addr.city);
				if(strlen(x.addr.state) && strcmp(y.addr.state, x.addr.state))
					strcpy(y.addr.state, x.addr.state);
				if(strlen(x.addr.street) && strcmp(y.addr.street, x.addr.street))
					strcpy(y.addr.street, x.addr.street);
			} 			
		}
	}
}
void menu()
{
	printf("\n\t Enter 1 to Add New Account.");
	printf("\n\t Enter 2 to Search Account.");
	printf("\n\t Enter 3 to Transact.");
	printf("\n\t Enter 4 to view A/C List.");
	printf("\n\t Enter 5 to View Transactions");
	printf("\n\t Enter 6 to Exit");
}
void run()
{
	int choice; Customer x; char c;
	
	printf("\n\n\tEnter Choice : ");		scanf("%d", &choice);
	
	switch(choice)
	{
		case 1: new_acc(&x);	
				storeAccount(x);	
				x = setAccNo();		
				see(x);	
				run();	
				break;
		
		case 2: x = search();	
				printf("\n\tEnter 'D' to Erase account. \n\tEnter 'E' to Edit Account. \n\tEnter 'S' to See Account. \n\t Enter Choice: ");
				fflush(stdin);	scanf("%c", &c);
				switch(c)
				{
					case 'd': case 'D': erase(x); break;
					case 'e': case 'E': edit(x); break;
					case 's': case 'S': see(x);	break;
				}
				run();
				break;
		case 3: transact(); run(); break;
		case 4: view(); run();	break;
		case 5: viewTransactionList(); break;
		case 6: break;
		default: printf("\n Wrong choice! Try Again"); run();
	}	
}
int main()
{
	printf("\n\t\tCOPYRIGHT EXCLUSIVELY RESERVED TO KALPADIPTYA ROY");
	printf("\n\n\t\t\tWELCOME TO BANK MANAGEMENT SYSTEM\n\n");	
	menu();
	run();
	getch();
	return 0;	
}
