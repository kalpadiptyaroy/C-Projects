#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include<dir.h>
#include<dos.h>
#include<math.h>
#define RECORD "CustomerRecord.dat"

typedef struct
{
	int customerNo;
	long long int phoneNo;
	char name[28];
	char address[32];
	float balance;
} Customer;

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

void input(Customer *x)
{
	fflush(stdin);
	printf("\n\tEnter the name of Customer: ");
	gets(x->name); 
	printf("\tEnter the Address : ");
	gets(x->address); fflush(stdin);
	printf("\tEnter the Phone No. : ");
	scanf("%lld", &x->phoneNo);
	printf("\tEnter the Amount to be Paid : ");
	scanf("%f", &x->balance);
	x->balance = -x->balance;
}

void output(Customer x, float amt)
{
	printf("\n\tCustomer Name :");   	setColor(14);   printf(" %s\t", x.name);  			setColor(15);
	printf("\n\tCustomer No. :");    	setColor(11);	printf(" %d\t", x.customerNo);  	setColor(15);
	printf("\n\tCustomer Phone No. : %lld\t", x.phoneNo);	
	printf("\n\tCustomer Address : %s\t", x.address);
	printf("\n\tBill Amount :"); 		setColor(10);	printf(" %.2f\t", amt - x.balance);	setColor(15);
	printf("\n\tAmount Paid :"); 		setColor(10);	printf(" %.2f\t",amt);				setColor(15);
	printf("\n\tRemaining Balance:"); 	setColor(12);	printf(" %.2f\t", x.balance);		setColor(15);
}

Customer setCustomerNo()
{
	FILE *fp;
	Customer x;
	int k, totalRecords;
	fp = fopen(RECORD, "rb+");
	fseek(fp, 0, SEEK_END);
	totalRecords = ftell(fp) / sizeof(Customer);	
	
	if(totalRecords == 1)
	{
		rewind(fp);
		fread(&x, sizeof(Customer), 1, fp);
		x.customerNo = 1;
		fseek(fp, -sizeof(Customer), SEEK_END);
		fwrite(&x, sizeof(Customer), 1 , fp);
	}
	else
	{
		fseek(fp, -2 * sizeof(Customer), SEEK_END);
		fread(&x, sizeof(Customer), 1, fp);
		k = x.customerNo + 1;
		fread(&x, sizeof(Customer), 1, fp);
		x.customerNo = k;
		fseek(fp, -sizeof(Customer), SEEK_END);
		fwrite(&x, sizeof(Customer), 1, fp);
	}
	
	fclose(fp);
	return x;	
}

void update(Customer x)
{
	FILE *fp;	Customer y;
	
	fp = fopen(RECORD, "rb+");
	
	while(fread(&x, sizeof(Customer), 1, fp))
	{
		if(y.customerNo == x.customerNo)
		{	
			fseek(fp, -sizeof(Customer), SEEK_CUR);
			fwrite(&x, sizeof(Customer), 1, fp);
			break;
		}
	}
	fclose(fp);
}

void writefile(Customer x)
{
	FILE *fp;
	fp = fopen("CustomerRecord.dat", "ab");
	fwrite(&x, sizeof(Customer), 1, fp);
	fclose(fp);
}

Customer searchByName(char *name)		
{
	FILE *fp;
	fp = fopen(RECORD, "rb");
	Customer x;
	while(fread(&x, sizeof(Customer), 1, fp))
	{
		if(strcmp(x.name, name) == 0)
		{
			printf("\n\tCustomer Record Found \n");
			fclose(fp);
			return x;
		}
	}
	printf("\n\tRecord not Found \n");
	x.customerNo = -1;
	fclose(fp);
	return x;
		
}

Customer searchByNo(int n)
{
	FILE *fp;
	fp = fopen(RECORD, "rb");
	Customer x;
	while(fread(&x, sizeof(Customer), 1, fp))
	{
		if(x.customerNo == n)
		{
			printf("\n\t Customer Record Found \n");
			fclose(fp);
			return x;
		}
	}	
	printf("\tRecord not Found");
	x.customerNo = -1;
	fclose(fp);
	return x;
}

Customer match(Customer x)
{
	FILE *fp; Customer y; char choice;
	fflush(stdin);
	printf("\n\n\tDo you want to EDIT Customer Details ?  (Y/N) : ");
	scanf("%c", &choice);
	
	if(choice == 'Y' || choice == 'y')
	{
		fflush(stdin);
		printf("\n\t Enter Name : ");
		gets(x.name);
		printf("\n\t Enter Address : ");
		gets(x.address);
		printf("\n\t Enter Phone No. : ");
		scanf("%lld", &x.phoneNo);
		
		fp = fopen(RECORD, "rb+");
		
		while(fread(&y, sizeof(Customer), 1, fp))
		{
			if(x.customerNo == y.customerNo)
			{
				if(strcmp(x.address, y.address) != 0 && strlen(x.address))
					strcpy(y.address, x.address);
				if(strcmp(x.name, y.name) != 0 && strlen(x.name))
					strcpy(y.name, x.name);
				if((x.phoneNo != y.phoneNo) && ((int)log10(x.phoneNo) + 1 == 10))
					y.phoneNo = x.phoneNo;
				
				fseek(fp, -sizeof(Customer), SEEK_CUR);
				fwrite(&y, sizeof(Customer), 1, fp);
				break;
			}
		}
		fclose(fp);
	}
	else
		y = x;
	return y;
}

Customer search(Customer x)
{
	int choice; 
	
	printf("\n\tEnter 1 to Search By Name \n\tEnter 2 to Search by Customer No.");
	printf("\n\n\tEnter Choice : ");
	scanf("%d", &choice);
	if(choice == 1)
	{	
		printf("\tEnter the Name : ");
		fflush(stdin);	gets(x.name);
		x = searchByName(x.name);
	}
	else if(choice == 2)
	{	
		printf("\tEnter the Customer No. : ");
		scanf("%d", &x.customerNo);
		x = searchByNo(x.customerNo);
	}
	else
	{
		printf("\tWRONG CHOICE. Try Again!");
		search(x);
	}
	
	return x;
}

float payBill(Customer *x)
{
	char c; float amt = 0.0;  fflush(stdin);
	printf("\tDo you want to pay the bill? (Y/N) : ");
	scanf("%c", &c);
	
	if(c == 'Y' || c == 'y')
	{
		printf("\tEnter Amount Paid : ");
		scanf("%f", &amt);
		
		x->balance = amt + x->balance;
	}
	else
		x->balance = x->balance - amt;
		
	return amt;
}

void displayMenu()
{
	system("cls");	//clrscr() is deprecated. So we are using cls command with the system() function to clear screen initially.
	printf("\n\t\t\tCOPYRIGHT 2019 : DEVELOPED BY KALPADIPTYA ROY\n");
	printf("\n\t\t\tWELCOME TO THE CUSTOMER BILLING SYSTEM\n\n");
	printf("\n\t\tEnter 1 to Add Account");
	printf("\n\t\tEnter 2 to Search Account");
	printf("\n\t\tEnter 3 to Exit");
}

void run()
{
	int choice;	Customer x; float amt;
	
	x.balance = 0;	x.phoneNo = 0;
	
	printf("\n\n \t\t\tEnter your Choice : ");
	scanf("%d", &choice);
	
	switch(choice)
	{
		case 1:	input(&x);
				amt = payBill(&x);
				writefile(x);
				x = setCustomerNo(); 
				output(x, amt);
				run();
				break;
				
		case 2: x = search(x);
				if(x.customerNo != -1)
				{
					amt = payBill(&x);
					update(x);
					x = match(x);
					output(x, amt);
				}
				run();
				break;
				
		case 3: break;
		
		default: printf("WRONG CHOICE !");	run(); 
	}
}

int main()
{	
	displayMenu();
	run();
	printf("\n\n \tThank you for using the our Billing System! \n\n");
	getch();	
	return 0;	
}
