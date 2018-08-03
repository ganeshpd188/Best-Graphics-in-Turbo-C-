#include<fstream.h>
#include<stdio.h>
#include<process.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<conio.h>
#include<fcntl.h>
//#include<types.h>
//#include<stat.h>
#include<dir.h>
#include<dos.h>
#include<iomanip.h>
#define VIDEO 0x10

char *main_menu[]=
{
	"^Books Department",
	"^Members Department",
	"^Book Issuing Department",
	"^Exit"
};

char *book_menu[]=
{
	"^Purchase New Books",
	"^Display Stock",
	"^Search Book",
	"^Remove Book",
	"^reCall Book",
	"^Modify Book",
	"^reTurn to main menu"
};
char *issue_menu[]=
{
	"^Issue Books",
	"^Return Book",
	"^Check Book ",
	"^Search Book (Member Wise)",
	"^Modify Transaction",
	"^Display Book",
	"^reTurn to main menu"
};
char *member_menu[]=
{
	"^New Membership",
	"^Display Members",
	"^Search Members",
	"^Remove Members",
	"^Modify Members Details",
	"^Issue Card",
	"^reCall Members",
	"^reTurn to Main Menu"
};

char *messages[]=
{
	"                      SILVER.SYS                     ",
	"                         Select using Enter or Hot key                      ",
	//2-5
	"Includes the Transacation for Books ",
	"Maintaining the Member Details",
	"Book Issueing and Maintaince",
	"Exit Program",

	//6-12
	"Purchase New Books for Library",
	"Display the total number of Book in Library",
	"Check for the Existance of Book",
	"Remove Books from the stock",
	"Get back the Removed Books",
	"To do alteration in Book details",
	"Return to the main menu",

	//13-20
	"Enrole the new Membership",
	"Display all Members present in Library",
	"Search for Members by Member Code",
	"To Cancel the Membership",
	"Modify Members Details",
	"Get the Print Out of Membership Card",
	"Members",
	"Return to main menu",

	//21-27
	"Issue Books to a Member",
	"Returning Book from the Member",
	"Check the book Issued for Member",
	"Search Book According to Member Wise",
	"To Modify Issue Details",
	"Display all BookIssued by Library",
	"Return to main menu",

};

struct dates
{
	char dd[4],mm[4],yy[10];
};

struct times
{
	char hh[4],mm[4],ss[4];
};

char far *vid_mem;
int ascii,scan;


//****************************OTHER FUNCTION**********************************

void hline(int sx,int ex,int y , char c)
{
	for(int i=sx;i<=ex;i++)
	{
		gotoxy(i,y);
		cprintf("%c",c);
	}
}

void vline(int sy,int ey,int x , char c)
{
	for(int i=sy;i<=ey;i++)
	{
		gotoxy(x,i);
		cprintf("%c",c);
	}
}

void box(int sx,int sy,int ex,int ey,char x)
{
	textcolor(2);
	hline(sx,ex,sy,x);
	hline(sx,ex,ey,x);
	vline(sy,ey,sx,x);
	vline(sy,ey,ex,x);
	textcolor(2);
}
void con_str(int x,char str[])
{
	for(int i=0;x>0;i++)
	{
		str[i]=((x%10)+48);
		x=x/10;
	}
	str[i]='\0';
	strrev(str);
}

int con_int(char str[])
{
	int ten=1;
	int rev=0;

	for(int i=0;i<(strlen(str)-1);i++)
		ten=ten*10;

	for(i=0;str[i]!='\0';i++)
	{
		rev=rev+(str[i]-48)*ten;
		ten=ten/10;
	}
	return rev;
}

dates get_dates()
{
	struct date d;
	struct dates dt;
	getdate(&d);
	con_str(d.da_year,dt.yy);
	con_str(d.da_day,dt.dd);
	con_str(d.da_mon,dt.mm);
	return dt;
}
times get_time()
{
	struct  time t;
	struct times tm;
	gettime(&t);
	con_str(t.ti_hour,tm.hh);
	con_str(t.ti_min,tm.mm);
	con_str(t.ti_sec,tm.ss);
	return tm;
}

void writechar(int r,int c,char ch,int attb)
{
	char far *v;
	v = vid_mem + r* 160 + c *2;
	*v=ch;
	v++;
	*v=attb;
}
void writestring(char*s,int r,int c,int attb)
{
	while(*s!='\0')
	{
		if(*s=='^')
		{
			s++;
			writechar(r,c,*s,126);
		}
		else
			writechar(r,c,*s,attb);
			s++;
			c++;
	}
}

drawbox(int sr,int sc,int er,int ec,int attr)
{
	int i;
	for(i=sc+1;i<ec;i++)
	{
		writechar(sr,i,205,attr);
		writechar(er,i,205,attr);
	}
	for(i=sr+1;i<er;i++)
	{
		writechar(i,sc,186,attr);
		writechar(i,ec,186,attr);
	}
	writechar(sr,sc,201,attr);
	writechar(sr,ec,187,attr);
	writechar(er,sc,200,attr);
	writechar(er,ec,188,attr);
	return 0;
}

void setmode (int mode)
{
	union REGS i,o;
	i.h.ah=0;
	i.h.al=mode;
	int86(16,&i,&o);
}

void mainscreen()
{
	clrscr();
	int i,j;

	drawbox(1,0,23,79,7);
	drawbox(3,0,21,79,7);

	writechar(3,0,204,7);
	writechar(3,79,185,7);
	writechar(21,0,204,7);
	writechar(21,79,185,7);

	for(i=4;i<=20;i++)
	{
		for(j=1;j<=78;j=j+2)
		{
			writechar(i,j,177,7);
			writechar(i,j+1,177,7);
		}
	}
	writestring(messages[0],2,14,112);
	writestring(messages[1],22,2,128+78);


	struct dates dt=get_dates();
	struct times tm=get_time();
	char d[30];
	sprintf(d," %s/%s/%s ",dt.dd,dt.mm,dt.yy);
	writestring(d,2,1,79);
	sprintf(d," %s:%s:%s ",tm.hh,tm.mm,tm.ss);
	writestring(d,2,69,79);
}

void size(int ssl,int esl)
{
	union REGS i,o;
	i.h.ah=1;
	i.h.ch=ssl;
	i.h.cl=esl;
	i.h.bh=0;
	int86(16,&i,&o);
}

void getkey()
{
	union REGS ii,oo;

	while(!kbhit());
	ii.h.ah=0;
	int86(22,&ii,&oo);
	scan=oo.h.ah;
	ascii=oo.h.al;
}

void savevideo(int sr,int sc,int er,int ec,char* buffer)
{
	char far *v;
	int i,j;
	for(i=sr;i<=er;i++)
	{
		for(j=sc;j<=ec;j++)
		{
			v=vid_mem + i * 160 + j * 2;
			*buffer=*v;
			v++;
			buffer++;
			*buffer=*v;
			buffer++;
		}
	}
}

void restorevideo(int sr,int sc,int er,int ec,char* buffer)
{
	char far *v;
	int i,j;
	for(i=sr;i<=er;i++)
	{
		for(j=sc;j<=ec;j++)
		{
			v=vid_mem + i * 160 + j * 2;
			*v=*buffer;
			v++;
			buffer++;
			*v=*buffer;
			buffer++;
		}
	}
}

void menubox(int sr,int sc,int er,int ec,char fil,char shad)
{
	int i,j;
	for(i=sr;i<er;i++)
	{
		for(j=sc;j<(ec-1);j++)
			writechar(i,j,' ',fil);
	}

	if(shad==0)
	{
		for(i=sr;i<=er;i++)
		{
			writechar(i,ec,' ',fil);
			writechar(i,(ec-1),' ',fil);
		}
		for(j=sc;j<=ec;j++)
			writechar(er,j,' ',fil);
	}
	else
	{
		for(i=sr+1;i<er;i++)
		{
			writechar(i,ec,' ',shad);
			writechar(i,(ec-1),' ',shad);
		}
		for(j=sc+2;j<=ec;j++)

			writechar(er,j,' ',shad);
	}
}

void displaymenu(char **menu,int count,int sr,int sc)
{
	int i;
	for(i=0;i<count;i++)
	{
		writestring(menu[i],sr,sc,112);
		sr++;
	}
}

getresponce(char **menu,char *hotkeys,int sr,int sc,int count,int helpnumber)
{
	int choice=1,len,hotkeychoice;
	writestring(menu[choice-1],sr+choice,sr+1,111);

	while(1)
	{
		getkey();
		if(ascii==0)
		{
			switch(scan)
			{
				case 80 :	writestring(menu[choice-1],sr+choice,sc+1,112);
								choice++;
								helpnumber++;
								break;
				case 72 :   writestring(menu[choice-1],sr+choice,sc+1,112);
								choice--;
								helpnumber--;
								break;
			}
			if(choice==0)
			{
				choice=count;
				helpnumber=helpnumber+count;
			}
			if(choice>count)
			{
				choice=1;
				helpnumber=helpnumber-count;
			}
			writestring (menu[choice-1],sr+choice,sc+1,111);
			menubox(6,35,9,78,112,66);

			writestring(messages[helpnumber],7,36,112);
		}
		else
		{
			if(ascii==13)
				return choice;
			if(ascii==27)
				return 27;

			ascii=toupper(ascii);
			hotkeychoice=1;

			while(*hotkeys!='\0')
			{
				if(*hotkeys==ascii)
					return hotkeychoice;
				else
				{
					hotkeys++;
					hotkeychoice++;
				}
			}
		}
	}
}

int popupmenu(char **menu,int count,int sr,int sc,char *hotkey,int helpnumber)
{
	int er,ec,i,l=0,areareqd,areaforhelp,choice;
	char *p,*h;

	er=sr+count+2;

	for(i=0;i<count;i++)
	{
		if(strlen(menu[i])>l)
			l=strlen(menu[i]);
	}
	ec=sc+l+3;
	areareqd=(er-sr+1)*(ec-sc+1)*2;
	p=(char*)malloc(areareqd);
	if(p==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();
		exit(2);
	}
	savevideo(sr,sc,er,ec,p);
	menubox(sr,sc,er,ec,112,66);
	displaymenu(menu,count,sr+1,sc+1);
	areaforhelp = (9-6+1) * (78-35+1)*2;
	h=(char*)malloc(areaforhelp);
	if(h==NULL)
	{
		writestring(messages[29],22,14,112);
		getch();
		exit(3);
	}
	savevideo(6,35,9,78,h);
	menubox(6,35,9,78,112,66);

	writestring(messages[helpnumber],7,36,112);

	choice=getresponce(menu,hotkey,sr,sc,count,helpnumber);

	restorevideo(sr,sc,er,ec,p);
	restorevideo(6,35,9,78,h);
	free(p);
	free(h);
	return(choice);
}

void movetoxy(int x, int y)
{
	union REGS regs;

	regs.h.ah = 2;  /* set cursor position */
	regs.h.dh = y;
	regs.h.dl = x;
	regs.h.bh = 0;  /* video page 0 */
	int86(VIDEO, &regs, &regs);
}
////////////////////////////////////////////////////////////////////////

class library
{
	char b_code[20];
	char b_name[100];
	char b_price[10];
	char b_no_copy[10];
	int flag;
	dates dop;
public:
	library();
	~library();
	void diskin();
	void diskout();
	void search();
	void deletes();
	void modify();
	void sorting();
	void b_recall();
	void increment(char*);
	int ret_b_code(char*);
}lib;

class member_genral
{
	char m_code[20];
	char m_fathers_name[100];
	char m_name[100];
	char m_address[200];
	char m_phone[30];
	char m_age[5];
	char m_membership[10];
	int flag;
	dates dob;

public:

	member_genral();
	~member_genral();
	void accept();
	void display();
	void search();
	void modify();
	void deletes();
	void card();
	void sort();
	void increment(char*);
	void recall();
	int ret_m_code(char*);
}mg;

class issue_book
{
	char m_code[20];
	char b_code[20];
	char status;
	dates doi;
	dates dor;

public:

	issue_book();
	~issue_book();
	void accept();
	void display();
	void msearch();
	void search();
	void modify();
	void book_return();
	void sort();
	dates ret_date(dates);
	int diff_day(int dd,int mm,int yy);
	int r_m_code(char*);
	int r_status(char*);
}ib;
//********************Library class function*********************************
library::library()
{}
library::~library()
{}

void library::diskin()
{
	char ch='y',d[80];
	ifstream out("lib",ios::in|ios::trunc);
	out.seekg(0,ios::end);
	out.seekg(out.tellg()-sizeof(library),ios::beg);
	out.read((char*)&lib,sizeof(library));
	if(out.tellg()<0)
		strcpy(lib.b_code,"B00000");
	out.close();

	while(ch=='y'||ch=='Y')
	{
		ofstream in("lib",ios::app);
		mainscreen();
		menubox(6,10,18,68,112,79);
		writestring("                   NEW BOOK ENTRY                   ",2,14,112);

		increment(lib.b_code);
		lib.dop=get_dates();

		sprintf(d,"Book Code        :   %s        ",lib.b_code);
		writestring(d,8,20,15);
		writestring("Book Name        :                 ",10,20,15);
		writestring("Book Price       :                 ",12,20,15);
		writestring("Number of copies :                 ",14,20,15);
		sprintf(d,"Date of purchase : %s/%s/%s       ",lib.dop.dd,lib.dop.mm,lib.dop.yy);
		writestring(d,16,20,15);

		writestring("                     Please Enter the Name of new Book                    ",22,2,78);
		gotoxy(42,11);	gets(lib.b_name);		fflush(stdin);
		writestring("                    Please Enter the Price for new Book                   ",22,2,78);
		gotoxy(42,13);	gets(lib.b_price);	fflush(stdin);
		writestring("                Please Enter the Number of copies of new Book             ",22,2,78);
		gotoxy(42,15);	gets(lib.b_no_copy);	fflush(stdin);

		lib.flag=1;
		in.write((char*)&lib,sizeof(library));
		in.close();

		writestring("                           Do you wish to cont...y/n                        ",22,2,128+78);
		ch=getche();
	}
}

void library::increment(char* code)
{
	int x=0,ten=10000;
	for(int i=1;code[i]!='\0';i++)
	{
		x=x+((code[i]-48)*ten);
		ten=ten/10;
	}
	x++;
	for(i=5;x>0;i--)
	{
		code[i]=((x%10)+48);
		x=x/10;
	}
}
void library::diskout()
{
	clrscr();
	int pos=5;
	drawbox(1,0,23,79,7);
	drawbox(3,0,23,79,7);
	writechar(3,0,204,7);
	writechar(3,79,185,7);
	writestring("Book Code      Book Name     Book Price   No.of copies   Date of Purchase",2,3,15);

	ifstream out("lib",ios::in);
	while(out.read((char*)&lib,sizeof(library)))
	{
		if(pos>=23)
		{
			getch();
			clrscr();
			drawbox(1,0,23,79,7);
			drawbox(3,0,23,79,7);
			writechar(3,0,204,7);
			writechar(3,79,185,7);
			writestring("Book Code      Book Name     Book Price   No.of copies   Date of Purchase",2,3,15);
			pos=5;
		}
		if(lib.flag==1)
		{
			gotoxy(3,pos);
			cout<<setw(6)<<lib.b_code<<setw(20)<<lib.b_name<<setw(9)<<lib.b_price<<setw(14)<<lib.b_no_copy<<setw(13)<<lib.dop.dd<<"/"<<lib.dop.mm<<"/"<<lib.dop.yy;
			pos++;
		}
	}
	out.close();
	getch();
}

void library::search()
{
	char search_code[10];
	int flag=0;
	char d[200];

	mainscreen();
	menubox(6,10,18,68,112,79);
	writestring("                    BOOK SEARCH ENTRY                ",2,14,112);
	writestring("Enter the Book Code :                 ",8,20,15);
	writestring("                Please Enter the Book Code You Want to search               ",22,2,78);

	gotoxy(43,9);	cin>>search_code;


	ifstream f("lib",ios::in);
	while(f.read((char*)&lib,sizeof(library)))
	{
		if(strcmpi(lib.b_code,search_code)==0 && (lib.flag==1))
		{
			sprintf(d,"Book Name           : %s               ",lib.b_name);										writestring(d,10,20,15);
			sprintf(d,"Book Price          : %s               ",lib.b_price);										writestring(d,12,20,15);
			sprintf(d,"Number of copies    : %s               ",lib.b_no_copy);									writestring(d,14,20,15);
			sprintf(d,"Date of purchase    : %s/%s/%s       ",lib.dop.dd,lib.dop.mm,lib.dop.yy); writestring(d,16,20,15);
			flag=1;
		}
	}
	if(flag==0)
		writestring("                 Your Entered book code is Not exist ! Try Again            ",22,2,128+78);
	else
		writestring("                             Searching Seccussful                           ",22,2,128+78);
	f.close();
	getch();
}

void library::deletes()
{
	clrscr();
	mainscreen();
	menubox(6,10,18,68,112,79);
	char delete_code[10];
	int f=0;

	writestring("                    BOOK REMOVE ENTRY                ",2,14,112);
	menubox(6,10,18,68,112,79);
	writestring("Enter the Book Code :               ",8,20,15);
	writestring("                Please Enter the Book Code You Want to Remove               ",22,2,78);
	gotoxy(43,9);	cin>>delete_code;

	ifstream out("lib", ios::in );
	ofstream in("t", ios::trunc);

	while(out.read((char*)&lib,sizeof(library)))
	{
		if(strcmpi(delete_code,lib.b_code)==0 && (lib.flag==1))
		{
			f=1;
			lib.flag=0;
		}
		in.write((char*)&lib,sizeof(library));
	}
	in.close();
	out.close();
	remove("lib");
	rename("t","lib");
	if(f==1)
		writestring("                  Your Entered Book Code is Deleted  From List              ",22,2,128+78);
	if(f==0)
		writestring("                 Your Entered book code is Not exist ! Try Again            ",22,2,128+78);
	getch();
}

void library::b_recall()
{
	clrscr();
	mainscreen();
	char rec_code[10];
	int f=0;

	writestring("                    BOOK RECALL ENTRY                ",2,14,112);
	menubox(6,10,18,68,112,79);
	writestring("Enter the Book Code :               ",8,20,15);
	writestring("                Please Enter the Book Code You Want to Recall               ",22,2,78);
	gotoxy(43,9);
	cin>>rec_code;

	ifstream out("lib", ios::in );
	ofstream in("t", ios::trunc);
	while(out.read((char*)&lib,sizeof(library)))
	{
		if(strcmpi(rec_code,lib.b_code)==0 && (lib.flag==0))
		{
			f=1;
			lib.flag=1;
		}
		in.write((char*)&lib,sizeof(library));
	}
	in.close();
	out.close();
	remove("lib");
	rename("t","lib");

	if(f==1)
		writestring("                Your Entered Book Code is Now Recalled From List            ",22,2,128+78);
	else
		writestring("                 Your Entered book code is Not exist ! Try Again            ",22,2,128+78);
	getch();
}

void library::modify()
{
	clrscr();
	mainscreen();
	char modify_code[10],d[80];
	int f=0;

	writestring("                    BOOK MODIFY ENTRY                ",2,14,112);
	menubox(6,10,18,68,112,79);
	writestring("Enter the Book Code :               ",8,20,15);
	writestring("                Please Enter the Book Code You Want to Modify               ",22,2,78);
	gotoxy(43,9);
	cin>>modify_code;

	ifstream out("lib", ios::in );
	ofstream in("t", ios::trunc);

	while(out.read((char*)&lib,sizeof(library)))
	{
		if(strcmpi(modify_code,lib.b_code)==0 && (lib.flag==1))
		{
			lib.dop=get_dates();
			writestring("Book Name          :                ",10,20,15);
			writestring("Book Price         :                ",12,20,15);
			writestring("Number of copies   :                ",14,20,15);
			sprintf(d,"Date of purchase   : %s/%s/%s      ",lib.dop.dd,lib.dop.mm,lib.dop.yy);
			writestring(d,16,20,15);

			writestring("                     Please Enter the Name of new Book                    ",22,2,78);
			gotoxy(42,11);	gets(lib.b_name);		fflush(stdin);
			writestring("                    Please Enter the Price for new Book                   ",22,2,78);
			gotoxy(42,13);	gets(lib.b_price);	fflush(stdin);
			writestring("                Please Enter the Number of copies of new Book             ",22,2,78);
			gotoxy(42,15);	gets(lib.b_no_copy);	fflush(stdin);
			f=1;
		}
		in.write((char*)&lib,sizeof(library));
	}
	in.close();
	out.close();
	remove("lib");
	rename("t","lib");

	if(f==1)
		writestring("                Your Entered Book Code is Now Modified From List            ",22,2,128+78);
	else
		writestring("                 Your Entered book code is Not exist ! Try Again            ",22,2,128+78);

	getch();
}


int library::ret_b_code(char* code)
{
	char a[10];
	ifstream out("lib", ios::in);
	while(out.read((char*)&lib,sizeof(library)))
	{
		if(strcmpi(code,lib.b_code)==0 && lib.flag==1)
		{
		return 1;
		}
	}
	out.close();
	return 0;
}
//**********************Member_genral functions*******************************
member_genral::member_genral()
{}
member_genral::~member_genral()
{}
void member_genral::accept()
{
	char year[4],ch='y';
	while(ch=='y'||ch=='Y')
	{
		ofstream in("MEM",ios::app);
		ifstream out("MEM",ios::in);
		clrscr();
		mainscreen();
		writestring(" NEW MEMBER ENTRY ",2,30,112);
		textcolor(7);
		out.seekg(0,ios::end);
		out.seekg(out.tellg()-sizeof(member_genral),ios::beg);
		out.read((char*)&mg,sizeof(member_genral));

		if(out.tellg()<=0)
			strcpy(mg.m_code,"M0001");
		else
			increment(mg.m_code);
		out.close();

		writestring(" Member Name      : ",7,14,112);
		writestring(" Member Age       : ",9,14,112);
		writestring(" Father's name    : ",11,14,112);
		writestring(" Member Address   : ",13,14,112);
		writestring(" Member Phone No. : ",15,14,112);
		writestring(" Years 1 or 2     : ",17,14,112);

		fflush(stdin);
		mg.dob=get_dates();

		gotoxy(46,8);	gets(mg.m_name);
		gotoxy(46,10);	gets(mg.m_age);
		gotoxy(46,12);	gets(mg.m_fathers_name);
		gotoxy(46,14);	gets(mg.m_address);
		gotoxy(46,16);	gets(mg.m_phone);

		while(1)
		{
			gotoxy(46,18);	gets(year);
			if(strcmpi(year,"1")==0 || strcmpi(year,"2")==0)
			{
				strcpy(mg.m_membership,year);
				break;
			}
			else
				writestring("            Invalid Fee Structure! Try Again           ",22,14,112);
		}
		mg.flag=1;
		in.write((char*)&mg,sizeof(member_genral));
		in.close();
		writestring("               Do you wish to cont...y/n          ",22,14,112);
		ch=getche();
	}
}

void member_genral::increment(char* code)
{
	int x=0,ten=1000;
	for(int i=1;code[i]!='\0';i++)
	{
		x=x+((code[i]-48)*ten);
		ten=ten/10;
	}
	x++;
	for(i=4;x>0;i--)
	{
		code[i]=((x%10)+48);
		x=x/10;
	}
}

void member_genral::display()
{
	clrscr();
	int pos=5;
	drawbox(1,0,23,79,7);
	drawbox(3,0,23,79,7);
	writechar(3,0,204,7);
	writechar(3,79,185,7);
	writestring("Mem Code  Mem Name  Age   Fat Name       Address         Phone    DOJ   Term",2,3,15);
	ifstream out("MEM",ios::in);
	while(out.read((char*)&mg,sizeof(member_genral)))
	{
		if(pos>=23)
		{
			getch();
			clrscr();
			drawbox(1,0,23,79,7);
			drawbox(3,0,23,79,7);
			writechar(3,0,204,7);
			writechar(3,79,185,7);
			writestring("Mem Code  Mem Name  Age   Fat Name       Address         Phone    DOJ   Term",2,3,15);
			pos=5;
		}

		if(mg.flag==1)
		{
			gotoxy(3,pos);
			cout<<setw(5)<<mg.m_code<<setw(12)<<mg.m_name<<setw(4)<<mg.m_age<<setw(12)<<mg.m_fathers_name<<setw(20)<<mg.m_address<<setw(10)<<mg.m_phone<<setw(3)<<mg.dob.dd<<"/"<<mg.dob.mm<<"/"<<mg.dob.yy<<setw(2)<<mg.m_membership;
			pos++;
		}
	}
	out.close();
	getch();
}

void member_genral::search()
{
	clrscr();
	mainscreen();
	char searchcode[10];
	int flag=0;

	box(10,5,70,21,'Û');
	box(11,5,71,21,'Û');
	gotoxy(30,8);
	textcolor(15);
	cprintf(" MEMBER CODE SEARCH ENTRY ");
	textcolor(7);

	gotoxy(30,12);
	cprintf("Enter the Member Code : ");
	gotoxy(53,12);
	cin>>searchcode;
	clrscr();
	mainscreen();
	box(10,5,60,24,'Û');
	box(11,5,61,24,'Û');

	ifstream f("MEM",ios::in);
	while(f.read((char*)&mg,sizeof(member_genral)))
	{
		if(strcmpi(searchcode,mg.m_code)==0 && (mg.flag==1))
		{
			gotoxy(25,7);
			textcolor(15);
			cprintf(" MEMBER SEARCH RESULT ");
			textcolor(7);

			gotoxy(20,9);	cprintf("Member Code  : %s",mg.m_code);
			gotoxy(20,11);	cprintf("Name         : %s",mg.m_name);
			gotoxy(20,13);	cprintf("Age          : %s",mg.m_age);
			gotoxy(20,15);	cprintf("Father Name  : %s",mg.m_fathers_name);
			gotoxy(20,17);	cprintf("Address      : %s",mg.m_address);
			gotoxy(20,19);	cprintf("Phone        : %s",mg.m_phone);
			gotoxy(20,21);	cprintf("Date Of Join : %s/%s/%s",mg.dob.dd,mg.dob.mm,mg.dob.yy);
			gotoxy(20,23);	cprintf("Term         : %s",mg.m_membership);
			flag=1;
		}
	}

	if(flag==0)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(10,11);
		cout<<"Your Entered Member Code is Not exist ! Try Again ";
	}
	f.close();
}

void member_genral::modify()
{
	clrscr();
	mainscreen();
	char modify_code[10],year[4];
	int f=0;
	box(10,5,70,18,'Û');
	box(11,5,71,18,'Û');
	gotoxy(30,8);
	textcolor(15);
	cprintf(" MODIFY RECORD ");
	textcolor(7);

	gotoxy(15,12);
	cprintf("Enter the Member Code : ");
	gotoxy(53,12);
	cin>>modify_code;
	clrscr();
	mainscreen();
	box(10,5,60,24,'Û');
	box(11,5,61,24,'Û');

	ifstream out("MEM", ios::in );
	ofstream in("t", ios::trunc);
	while(out.read((char*)&mg,sizeof(member_genral)))
	{
		if(strcmpi(modify_code,mg.m_code)==0 && (mg.flag==1))
			{
				f=1;
				gotoxy(20,12);	cprintf("ENTER THE NAME          :");
				gotoxy(20,14);	cprintf("ENTER THE AGE           :");
				gotoxy(20,16);	cprintf("ENTER THE FATHER'S NAME :");
				gotoxy(20,18);	cprintf("ENTER THE ADDRESS       :");
				gotoxy(20,20);	cprintf("ENTER THE PHONE NUMBER  :");
				gotoxy(20,22);	cprintf("ENTER THE MEMBERSHIP FEE:");

				fflush(stdin);
				mg.dob=get_dates();

				gotoxy(46,12);	cin>>mg.m_name;
				gotoxy(46,14);	cin>>mg.m_age;
				gotoxy(46,16);	cin>>mg.m_fathers_name;
				gotoxy(46,18);	cin>>mg.m_address;
				gotoxy(46,20);	cin>>mg.m_phone;
				while(1)
				{
					gotoxy(46,22);	gets(year);
					if(strcmpi(year,"1")==0 || strcmpi(year,"2")==0)
					{
						strcpy(mg.m_membership,year);
						break;
					}
					else
					{
						textcolor(128+4);  gotoxy(25,25);  cout<<"INVALID FEE STRUCTURE! TRY AGAIN";
						getch();
					}
				}
				mg.flag=1;
			}
		in.write((char*)&mg,sizeof(member_genral));
	}

	in.close();
	out.close();
	remove("MEM");
	rename("t","MEM");
	if(f==1)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(15,11);
		cout<<"Your Entered Member Code is now modified ";
	}

	if(f==0)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(10,11);
		cout<<"Your Entered Member Code is Not exist ! Try Again ";
	}
}

void member_genral::deletes()
{
	clrscr();
	mainscreen();
	char delete_code[10];
	box(10,5,70,21,'Û');
	box(11,5,71,21,'Û');
	gotoxy(30,8);
	textcolor(15);
	cprintf(" REMOVE CODE DELETE ENTRY ");
	textcolor(7);
	gotoxy(30,12);
	cprintf("Enter the Member Code : ");
	gotoxy(53,12);
	cin>>delete_code;
	clrscr();
	mainscreen();
	box(10,5,60,24,'Û');
	box(11,5,61,24,'Û');

	ifstream out("MEM", ios::in );
	ofstream in("t", ios::trunc);
	int f=0,f2=0;

	while(out.read((char*)&mg,sizeof(member_genral)))
	{
		if(strcmpi(delete_code,mg.m_code)==0 && (mg.flag==1))
		{
			f2=1;
			if(ib.r_status(delete_code)==1)
			{
				f=1;
				mg.flag=0;
			}
		}
		in.write((char*)&mg,sizeof(member_genral));
	}

	if(f==1)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(15,11);
		cout<<"Your Entered Member Code is Now Deleted ";
	}
	in.close();
	out.close();
	remove("MEM");
	rename("t","MEM");

	if(f==0)
	{
		clrscr();
		box(8 ,8 ,70 ,14 ,'Û' );
		gotoxy(10,11);
		cout<<" FIRST BOOK RETURN OTHERWISE MEMBERSHIP CANNOT BE CANCELED";
	}
	if(f2==0)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(10,11);
		cout<<"Your Entered Member Code is Not exist ! Try Again ";
	}
}

void member_genral::recall()
{
	clrscr();
	mainscreen();
	char call_code[10];
	box(10,5,70,21,'Û');
	box(11,5,71,21,'Û');
	gotoxy(30,8);
	textcolor(15);
	cprintf(" RECALL MEMBER CODE ENTRY ");
	textcolor(7);
	gotoxy(30,12);
	cprintf("Enter the Member Code : ");
	gotoxy(53,12);
	cin>>call_code;
	clrscr();
	mainscreen();
	box(10,5,60,24,'Û');
	box(11,5,61,24,'Û');

	ifstream out("MEM", ios::in );
	ofstream in("t", ios::trunc);
	int f=0;

	while(out.read((char*)&mg,sizeof(member_genral)))
	{
		if(strcmpi(call_code,mg.m_code)==0 && (mg.flag==0))
		{
				f=1;
				mg.flag=1;
		}
		in.write((char*)&mg,sizeof(member_genral));
	}

	if(f==1)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(15,11);
		cout<<"Your Entered Member Code is Now Recalled ";
	}
	in.close();
	out.close();
	remove("MEM");
	rename("t","MEM");

	if(f==0)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(10,11);
		cout<<"Your Entered Member Code is Not exist ! Try Again ";
	}
}


void member_genral::card()
{
	union REGS regs;

	int f=0;
	clrscr();
	mainscreen();
	char t_m_code[10];
	box(10,9,65,19,'Û');
	box(11,9,66,19,'Û');
	gotoxy(30,12);
	textcolor(15);
	cprintf(" ISSUE CARD ");
	textcolor(7);
	gotoxy(18,15);
	cprintf("Enter the Member Code : ");
	gotoxy(48,15);
	cin>>t_m_code;
	clrscr();
	mainscreen();
	box(13,9,60,19,'Û');
	box(14,9,61,19,'Û');
	ifstream out("MEM", ios::in );
	while(out.read((char*)&mg,sizeof(member_genral)))
	{
		if(strcmpi(t_m_code,mg.m_code)==0 && (mg.flag==1))
		{
			f=1;
			gotoxy(18,11);	cprintf("Member Code  : %s",mg.m_code);
			gotoxy(18,13);	cprintf("Name         : %s",mg.m_name);
			gotoxy(18,15);	cprintf("Address      : %s",mg.m_address);
			int x;
			if(strcmpi(m_membership,"1")==0)
			{
				x=con_int(mg.dob.yy);
				x=x+1;
				con_str(x,mg.dob.yy);
			}
			else
			{
				x=con_int(mg.dob.yy);
				x=x+2;
				con_str(x,mg.dob.yy);
			}
			gotoxy(18,17);	cprintf("Valid Upto   : %s/%s/%s",mg.dob.dd,mg.dob.mm,mg.dob.yy);
		}
		int86(5, &regs, &regs);
		getch();
	}
	out.close();

	if(f==0)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(10,11);
		cout<<"Your Entered Member Code is Not exist ! Try Again ";
	}
}
int member_genral::ret_m_code(char* code)
{
	char a[10];
	ifstream out("MEM", ios::in);
	while(out.read((char*)&mg,sizeof(member_genral)))
	{
		if(strcmpi(code,mg.m_code)==0 && mg.flag==1)
		{
		return 1;
		}
	}
	out.close();
	return 0;
}

void member_genral::sort()
{
}

//****************************issue class function************************************

issue_book::issue_book()
{
}

issue_book::~issue_book()
{
}
	char m_code[10];
	char b_code[10];
	dates doi;
	dates dor;

void issue_book::accept()
{
	char t_m_code[10],t_b_code[10],ch='y';
	int f=0,flag=0,flag2=0;
	while(ch=='y'||ch=='Y')
	{
		ofstream in("isu",ios::app);
		ifstream out("isu",ios::in);
		clrscr();
		mainscreen();
		box(10,5,63,22,'Û');
		box(11,5,64,22,'Û');
		gotoxy(30,7);
		textcolor(15);
		cprintf(" ISSUE BOOK ENTRY ");
		textcolor(7);
		gotoxy(20,12);	cprintf("Member Code      : ");
		gotoxy(42,12);	gets(t_m_code);
		if(mg.ret_m_code(t_m_code)==0)
		{
			flag=1;
			clrscr();
			box(8 ,8 ,62 ,17 ,'Û' );
			gotoxy(15,11);cout<<"Your Entered Member Code is Not exist ";
			gotoxy(14,12);cout<<"                   Or                 ";
			gotoxy(14,13);cout<<"            MEMBERSHIP CANCELED       ";
			gotoxy(14,14);cout<<"               !Try Again             ";
		}
		if(flag==0)
		{
			while(out.read((char*)&ib,sizeof(issue_book)))
			{
				if(strcmpi(t_m_code,ib.m_code)==0 && ib.status=='y')
				{
					clrscr();
					box(8 ,8 ,62 ,14 ,'Û' );
					gotoxy(20,11);
					cout<<"BOOK ISSUED ALLREADY";
					f=1;
				}
			}
			if(f==0)
			{
				gotoxy(20,14);	cprintf("Book Code        : ");
				gotoxy(42,14);	gets(t_b_code);
				if(lib.ret_b_code(t_b_code)==0)
				{
					flag2=1;
					clrscr();
					box(8 ,8 ,62 ,14 ,'Û' );
					gotoxy(10,11);
					cout<<"Your Entered Book Code is Not exist ! Try Again ";
				}
				if(flag2==0)
				{
					strcpy(ib.m_code,t_m_code);
					strcpy(ib.b_code,t_b_code);
					gotoxy(20,16);	cprintf("Date Of Issue    : ");
					gotoxy(20,18);	cprintf("Date Of Return   : ");
					ib.status='y';
					ib.doi=get_dates();
					gotoxy(42,16);	cout<<ib.doi.dd<<"/"<<ib.doi.mm<<"/"<<ib.doi.yy;
					dor=ret_date(ib.doi);
					gotoxy(42,18);	cout<<ib.dor.dd<<"/"<<ib.dor.mm<<"/"<<ib.dor.yy;
					in.write((char*)&ib,sizeof(issue_book));
					in.close();
					out.close();
				}
			}
		}
		gotoxy(25,24);
		cout<<"Do you wish to cont...y/n :";		ch=getche();
	}
}

void issue_book::display()
{
	clrscr();
	int pos=5;
	ifstream f("isu",ios::in);
	textcolor(15);
	hline(2,78,2,'Í');
	gotoxy(2,3);
	cprintf("Mem Code  Book Code   Date Of Issue   Date Of Return   status");
	hline(2,78,4,'Í');
	hline(2,78,25,'Í');
	textcolor(7);

	while(f.read((char*)&ib,sizeof(issue_book)))
	{
		if(pos>=23)
		{
			pos=5;
			getch();
			clrscr();
			textcolor(15);
			hline(2,78,2,'Í');
			gotoxy(2,3);
			cprintf("Mem Code  Book Code   Date Of Issue   Date Of Return   status");
			hline(2,78,4,'Í');
			hline(2,78,23,'Í');
			textcolor(7);
		}

		gotoxy(3,pos);

		if(ib.status=='y')
		{
			cout<<setw(6)<<ib.m_code<<setw(11)<<ib.b_code<<setw(10)<<ib.doi.dd<<"/"<<ib.doi.mm<<"/"<<ib.doi.yy<<setw(10)<<ib.dor.dd<<"/"<<ib.dor.mm<<"/"<<ib.dor.yy<<"      "<<ib.status;
			pos++;
		}
	}
	f.close();
}

void issue_book::msearch()
{
	clrscr();
	mainscreen();
	char searchcode[10];
	int flag=0,flag2=0;;

	box(10,5,70,21,'Û');
	box(11,5,71,21,'Û');
	gotoxy(27,10);
	textcolor(15);
	cprintf(" BOOK  SEARCH ENTRY ");
	textcolor(7);
	gotoxy(22,15);
	cprintf("Enter the Member Code : ");
	gotoxy(46,15);
	cin>>searchcode;
	if(mg.ret_m_code(searchcode)!=1 )
	{
		flag=1;
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(10,11);
		cout<<"Your Entered Member Code is Not exist ! Try Again ";
	}
	if(flag==0)
	{
		ifstream f("isu",ios::in);

		while(f.read((char*)&ib,sizeof(issue_book)))
		{
			if(strcmpi(searchcode,ib.m_code)==0 && ib.status=='y')
			{
				clrscr();
				mainscreen();
				box(10,5,60,24,'Û');
				box(11,5,61,24,'Û');
				gotoxy(18,9);
				textcolor(15);
				cprintf(" FOLLOWING BOOK ISSUED TO THAT MEMBER ");
				textcolor(7);

				gotoxy(20,12);	cprintf("Member Code  : %s",ib.m_code);
				gotoxy(20,14);	cprintf("Book code    : %s",ib.b_code);
				gotoxy(20,16);	cprintf("Issue date   : %s/%s/%s",ib.doi.dd,ib.doi.mm,ib.doi.yy);
				gotoxy(20,18);	cprintf("Return date  : %s/%s/%s",ib.dor.dd,ib.dor.mm,ib.dor.yy);
				flag2=1;
			}
		}
		f.close();
		if(flag2==0)
		{
			flag=1;
			clrscr();
			box(8 ,8 ,62 ,14 ,'Û' );
			gotoxy(25,11);
			cout<<"No Book Issued ";
		}
	}
}

void issue_book::search()
{
	int flag=0;
	clrscr();
	mainscreen();
	char searchcode[10];
	int pos=5;
	box(10,5,70,21,'Û');
	box(11,5,71,21,'Û');
	gotoxy(27,10);
	textcolor(15);
	cprintf(" BOOK  SEARCH ENTRY ");
	textcolor(7);
	gotoxy(22,15);
	cprintf("Enter the Book Code : ");
	gotoxy(44,15);
	cin>>searchcode;
	if(lib.ret_b_code(searchcode)==0)
	{
			flag=1;
			clrscr();
			box(8 ,8 ,62 ,14 ,'Û' );
			gotoxy(10,11);
			cout<<"Your Entered Book Code is Not exist ! Try Again ";
	}
	if(flag==0)
	{
		clrscr();
		textcolor(15);
		hline(2,78,2,'Í');
		gotoxy(2,3);
		cprintf("Mem Code  Book Code   Date Of Issue   Date Of Return");
		hline(2,78,4,'Í');
		hline(2,78,25,'Í');
		textcolor(7);

		ifstream f("isu",ios::in);
		while(f.read((char*)&ib,sizeof(issue_book)))
		{
			if(strcmpi(searchcode,ib.b_code)==0)
			{
				if(pos>=23)
				{
					pos=5;
					getch();
					clrscr();
					textcolor(15);
					hline(2,78,2,'Í');
					gotoxy(2,3);
					cprintf("Mem Code  Book Code   Date Of Issue   Date Of Return");
					hline(2,78,4,'Í');
					hline(2,78,23,'Í');
					textcolor(7);
				}
				gotoxy(3,pos);
				if(ib.status=='y')
				{
					cout<<setw(6)<<ib.m_code<<setw(11)<<ib.b_code<<setw(9)<<ib.doi.dd<<"/"<<ib.doi.mm<<"/"<<ib.doi.yy<<setw(9)<<ib.dor.dd<<"/"<<ib.dor.mm<<"/"<<ib.dor.yy;
					pos++;
				}
			}
		}
		f.close();
	}
}

void issue_book::modify()
{
	clrscr();
	mainscreen();
	char modify_code[10];
	int f=0;
	box(10,5,70,21,'Û');
	box(11,5,71,21,'Û');
	gotoxy(29,10);
	textcolor(15);
	cprintf(" MODIFY RECORD ");
	textcolor(7);

	gotoxy(22,15);
	cprintf("Enter the Member Code : ");
	gotoxy(45,15);
	cin>>modify_code;
	clrscr();
	mainscreen();
	box(10,5,60,24,'Û');
	box(11,5,61,24,'Û');

	ifstream out("isu", ios::in );
	ofstream in("t", ios::trunc);
	while(out.read((char*)&ib,sizeof(issue_book)))
	{
		if(strcmpi(modify_code,ib.m_code)==0)
			{
				f=1;
				gotoxy(20,14);	cprintf("Book Code        : ");
				gotoxy(20,16);	cprintf("Date Of Issue    : ");
				gotoxy(20,18);	cprintf("Date Of Return   : ");
				gotoxy(42,14);	gets(ib.b_code);
				ib.doi=get_dates();
				gotoxy(42,16);	cout<<ib.doi.dd<<"/"<<ib.doi.mm<<"/"<<ib.doi.yy;
				dor=ret_date(ib.doi);
				gotoxy(42,18);	cout<<ib.dor.dd<<"/"<<ib.dor.mm<<"/"<<ib.dor.yy;
				ib.status='y';
			}
			in.write((char*)&ib,sizeof(issue_book));
	}
	in.close();
	out.close();
	remove("isu");
	rename("t","isu");
	if(f==0)
	{
		clrscr();
		box(8 ,8 ,62 ,14 ,'Û' );
		gotoxy(10,11);
		cout<<"Your Entered Member Code is Not exist ! Try Again ";
	}
}

int leap(int yy)
{
	if(yy%400==0)
		return 1;
	else if(yy%4==0 && yy%100!=0)
		return 1;
	return 0;
}

int issue_book::diff_day(int dd,int mm,int yy)
{
	int rem=0;
	int mrem[12]={3,0,3,2,3,2,3,3,2,3,2,3};

	yy=(yy-1)%400;

	if(yy<400 && yy>=300)
		rem=(yy-300)+((yy-300)/4)+1;

	else if(yy<300 && yy>=200)
		rem=(yy-200)+((yy-200)/4)+3;

	else if(yy<200 && yy>=100)
		rem=(yy-100)+((yy-100)/4)+5;

	else if(yy<100 && yy>=0)
		rem=yy+(yy/4);

	for(int i=0;i<mm-1;i++)
	{
		rem=rem+mrem[i];
	}

	if(leap(yy+1)==1 && mm>2)
		rem=rem+1;

	rem=rem+dd;

	return rem;
}

void issue_book::book_return()
{
	dates d;
	char t_m_code[10],t_b_code[10],ch='y';
	int rd=0,ad=0,fine=0,f=0;
	while(ch=='y'||ch=='Y')
	{
		ifstream out("isu",ios::in);
		ofstream in("t",ios::trunc);
		clrscr();
		mainscreen();
		box(10,5,63,25,'Û');
		box(11,5,64,25,'Û');
		gotoxy(30,7);
		textcolor(15);
		cprintf(" RETURN BOOK ENTRY ");
		textcolor(7);
		gotoxy(20,12);	cprintf("Member Code      : ");
		gotoxy(42,12);	gets(t_m_code);
		gotoxy(20,14);	cprintf("Book Code        : ");
		gotoxy(42,14);	gets(t_b_code);
		while(out.read((char*)&ib,sizeof(issue_book)))
		{
			if((strcmpi(t_m_code,ib.m_code)==0) && (strcmpi(t_b_code,ib.b_code)==0) && ib.status=='y')
			{
				f=1;
				gotoxy(20,16);	cprintf("Date Of Issue    : ");
				gotoxy(20,18);	cprintf("Date Of Return   : ");
				gotoxy(20,20);	cprintf("Today Date       : ");
				gotoxy(42,16);	cout<<ib.doi.dd<<"/"<<ib.doi.mm<<"/"<<ib.doi.yy;
				gotoxy(42,18);	cout<<ib.dor.dd<<"/"<<ib.dor.mm<<"/"<<ib.dor.yy;
				d=get_dates();
				gotoxy(42,20);	cout<<d.dd<<"/"<<d.mm<<"/"<<d.yy;
				rd=ib.diff_day(con_int(ib.dor.dd) , con_int(ib.dor.mm) , con_int(ib.dor.yy));
				ad=ib.diff_day(con_int(d.dd),con_int(d.mm),con_int(d.yy));
				fine=ad-rd;
				fine=fine*100;
				gotoxy(20,22);	cprintf("Fine             : ");
				if(fine>0)
				{
					gotoxy(42,22); cout<<fine;
				}
				else
				{
					fine=0;
					gotoxy(42,22); cout<<fine;
				}
				ib.status='n';
			}
		in.write((char*)&ib,sizeof(issue_book));
		}
		in.close();
		out.close();
		remove("isu");
		rename("t","isu");
		if(f==0)
		{
			clrscr();
			box(8 ,8 ,62 ,14 ,'Û' );
			gotoxy(20,11);
			cout<<"MISMATCH MEMBER OR BOOK";
		}
		gotoxy(25,24);
		cout<<"Do you wish to cont...y/n :";		ch=getche();
	}
}

dates issue_book::ret_date(dates rdt)
{
	int ad=7;
	int dd=con_int(rdt.dd);
	int mm=con_int(rdt.mm);
	int yy=con_int(rdt.yy);

	if(yy>0)
	{
		if(mm==1||mm==3||mm==5|| mm==7||mm==8||mm==10||mm==12)
		{
			if(dd>=1 && dd<=31)
			{
				dd=dd+ad;
				if(dd>31 && mm!=12)
				{
					dd=dd-31;
					mm=mm++;
				}
				else if(dd>31 && mm==12)
				{
					dd=dd-31;
					mm=1;
					yy++;
				}
			}
		}
		else if(mm==4||mm==6||mm==9||mm==11)
		{
			if(dd>=1 && dd<=30)
			{
				dd=dd+ad;
				if(dd>30)
				{
					dd=dd-30;
					mm++;
				}
			}
		}
		else if(mm==2)
		{
			if(yy%400==0 || yy%4==0 && yy%100!=0)
			{
				dd=dd+ad;
				if(dd>29)
				{
					dd=dd-29;
					mm++;
				}
			}
			else if(dd>=1 && dd<=28)
			{
				dd=dd+ad;
				if(dd>28)
				{
					dd=dd-28;
					mm++;
				}
			}
		}
	}
	con_str(mm,rdt.mm);
	con_str(dd,rdt.dd);
	con_str(yy,rdt.yy);
	return rdt;
}

int issue_book::r_m_code(char* code)
{
	char a[10];
	ifstream out("isu", ios::in);
	while(out.read((char*)&ib,sizeof(issue_book)))
	{
		if(strcmpi(code,ib.m_code)==0)
		{
		return 1;
		}
	}
	out.close();
	return 0;
}

int issue_book::r_status(char* code)
{
	char a[10];
	ifstream out("isu", ios::in);
	while(out.read((char*)&ib,sizeof(issue_book)))
	{
		if(strcmpi(code,ib.m_code)==0 && ib.status =='n')
		{
		return 1;
		}
	}
	out.close();
	return 0;
}

void book_menu_ch()
{
	int fs_choice;
	while(1)
	{
		mainscreen();
		fs_choice=popupmenu(book_menu,7,5,5,"PDSRCMT",6);
		switch(fs_choice)
		{
			case 1:lib.diskin();	break;
			case 2:lib.diskout();	break;
			case 3:lib.search();	break;
			case 4:lib.deletes();	break;
			case 5:lib.b_recall();  break;
			case 6:lib.modify(); 	break;
			case 7:
			case 27: size(6,7);
				 clrscr();
				 return;
		}
	}
}

void member_menu_ch()
{
	int ds_choice;
	while(1)
	{
		mainscreen();
		ds_choice=popupmenu(member_menu,8,5,5,"NDSRMICT",13);
		switch(ds_choice)
		{
			case 1:   mg.accept();	    break;
			case 2:   mg.display();	    break;
			case 3:   mg.search();	    break;
			case 4:   mg.deletes();	    break;
			case 5:   mg.modify();      break;
			case 6:   mg.card();        break;
			case 7:   mg.recall();      break;
			case 8:
			case 27:  size(6,7);
				  clrscr();
				  return;
		}
	}
}

void issue_menu_ch()
{
	int ds_choice;
	while(1)
	{
		mainscreen();
		ds_choice=popupmenu(issue_menu,7,5,5,"IRCSMDT",21);
		switch(ds_choice)
		{
			case 1:  ib.accept();		break;
			case 2:  ib.book_return(); break;
			case 3:  ib.search();      break;
			case 4:  ib.msearch();     break;
			case 5:  ib.modify();      break;
			case 6:  ib.display();     break;
			case 7:
			case 27:  size(6,7);
				  clrscr();
				  return;
		}
	}
}
void main()
{
	int mm_choice;
	//store base address of VDU memory and set approprate vidio mode

	#ifdef MA
		//store base address for MA
		vid_mem=(char far*)0xb8000000L;
		setmode(7);
	#else
		vid_mem=(char far*)0xb8000000L;
		setmode(3);
	#endif

	size(32,0);		//hide cursor
	menubox(0,0,24,79,7,7);
	drawbox(1,0,21,79,7);
	drawbox(1,0,23,79,7);
	while(1)
	{
		mainscreen();
		mm_choice=popupmenu(main_menu,4,5,5,"BMIE",2);

		switch(mm_choice)
		{
			case 1 :      book_menu_ch();break;
			case 2 :      member_menu_ch(); break;
			case 3 :      issue_menu_ch(); break;
			case 4 :
			case 27:	size(6,7);
					clrscr();
					exit(1);
		}

	}
}