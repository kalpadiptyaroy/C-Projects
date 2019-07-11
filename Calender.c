#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<dos.h>
#include<dir.h>

typedef struct	{int first_day;	int total_days; int a[6][7];} Month;
typedef struct 	{int day; int month; int year;} Date;
typedef enum 	{Mon, Tue, Wed, Thu, Fri, Sat, Sun} Day;
char monthName[][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

Day firstDayYear; Date date;

void SetColor(int ForgC)
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
void getTotalDays(Month m[])
{
	int i;
	for(i = 0; i < 12; i++)	
		switch(i + 1)
		{
			case 2: m[i].total_days = (date.year % 400 == 0 || date.year % 4 == 0) ? 29 : 28; 
					break;			
			case 1:		case 3:		case 5:		case 7:		case 8:		case 10: 
			case 12: m[i].total_days = 31; break;
			case 4:		case 6:		case 9:
			case 11: m[i].total_days = 30; break;	
		}		
}
void getFirstDay(Month m[])
{
	int i;
	for(i = 1; i < 12; i++)	
		m[i].first_day = (m[i - 1].first_day + (m[i - 1].total_days % 7)) % 7;
	
}
Month fillMonth(Month m)
{
	int k = 0;	
	while(k < m.first_day)
		m.a[0][(k++ % 7)] = 0;
	while(k <= m.total_days)
		m.a[k / 7][(k % 7)] = (k++ - m.first_day) + 1;
	while(k - m .first_day + 1 <= m.total_days)
		m.a[k / 7][k % 7] = k++ - m.first_day + 1;
	while(k <= 42)	
		if(m.first_day != 0)
			m.a[k / 7][k++ % 7] = 0;
		else
			m.a[k / 7][k++ % 7 - 1] = 0;
	return m;	
}
void fillYear(Month *m)
{
	int i;	
	for(i = 0; i < 12; i++)
	 	m[i] = fillMonth(m[i]); 
}
void display(Month m, Date d, int n)
{
	int i, j;	
	SetColor(14);	printf("\n \tMon \tTue \tWed \tThu \tFri \tSat \t");
	SetColor(12);	printf("Sun \n");	SetColor(15);	
	for(i = 0; i < 6; i++)
	{
		printf("\t");
		for(j = 0; j < 7; j++)
			if(m.a[i][j] != 0 && m.a[i][j]== d.day && n == 1)
			{														//sleep(int seconds) is used to add delay. Generally to get the blink effect. 
				SetColor(10); sleep(1);	printf("%d \t", m.a[i][j]);	sleep(1);	SetColor(15);   
			}
			else if(m.a[i][j] != 0 && j == 6)
			{
				SetColor(12);	printf("%d \t", m.a[i][j]);		SetColor(15);
			}			
			else if(m.a[i][j] != 0)
				printf("%d \t", m.a[i][j]);
			else
				printf("\t");
		printf("\n");
	}
}
void displayYear(Month m[])
{
	int i;
	for(i = 0; i < 12; i++)
	{
		printf("\n \t\t[  %s  ] \n", monthName[i]);
		if(date.month == i + 1)
			display(m[i], date, 1);
		else
			display(m[i], date, 0);
		printf("\n");
	}
		
}
void getFirstDayOfYear(int year, Month *m)
{
	firstDayYear = (((year - 1) * 365) - ((year - 1) / 100) + ((year - 1) / 400) + ((year - 1) / 4)) % 7;
	m->first_day = firstDayYear;
}
void getDate(char *d)
{	
	date.day = 0;	date.month = 0;		date.year = 0;		int i = 0, k = 10;			
	while(d[i] != '/')
	{
		date.day = date.day + k * ((int)d[i++] - 48); 		k = k / 10;
	}
	++i; k = 10;
	while(d[i] != '/')
	{
		date.month = date.month + k * ((int)d[i++] - 48);	k = k / 10;
	}
	++i; k = 1000;
	while(d[i] != '\0')
	{
		date.year = date.year + k * ((int)d[i++] - 48);		k = k / 10;
	}
}
int main()
{
	system("cls");
	Month m[12];	char d[11];	
	printf("Please Maintain the DD/MM/YYYY Format during Input \n");
	printf("Enter the Date: ");
	scanf("%s", d);	
	d[10] = '\0';
	getDate(d);
	printf("\n\n\t\tCALENDER OF %d \n\n", date.year);			
	getFirstDayOfYear(date.year, &m[0]);
	getTotalDays(m);	getFirstDay(m);			
	fillYear(m);		displayYear(m);				
	getch();
}
