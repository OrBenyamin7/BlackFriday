#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<unistd.h>
#include <sys/stat.h>

#define ITEMS 100
#define MAX 256

//struct for the item
typedef struct Item {
    char productName[MAX];
	int amount;
	float price;
}Item;

//print an error to the screen and close the program
void Error(char* msg) { fprintf(stderr, "Error:%s\n", msg); exit(1); }
int receivingItems(Item* items[]);
void GarbageRemove(char* line);
void split_productName_amount(Item* items, char* line);
void updatePrice( Item* item,int fd_from);
int searchPrice_afterSale(char* buff, char* ptr, Item* item);
void totalPriceAndFile(Item* item [], int countItems, char* nameCompany, char* invitingName);
void createOrderFile(Item** item,char* nameComp, char* invitingName,int counter,double totalPrice);
void titleOrderFile(char* nameRest, int fdOrder);
void itemsOrderFile(char* nameRest, int fdOrder, Item *item[],int counter);
void timeOrderFile(int fdOrder);

int main(int argc, char* argv[]) 
{
	char from[MAX] = "/home/braude/";
    char path_GetFlyer[MAX]="/home/braude/";
    int pid,countItems, fd_from, i=0;
    Item items[ITEMS],* items_ptr[ITEMS];

    if (argc!=3)
		Error("wrong number of paramaters");

	strcat(from, "Black_Friday/");
    strcat(from, argv[1]);
	strcat(from, ".txt");
    strcat(path_GetFlyer, "GetFlyer");

    pid=fork();
    if (pid==0){ // The Child Process 
        if (execl(path_GetFlyer, "GetFlyer", argv[1], 0) == -1)
        {		 
            perror("error"); 
            return(-1); 
        } 
    }

    else if (pid==-1){
		perror("error"); //fork error
		return(-1);
	}
	else //parent Process
		wait();
	if ((fd_from = open(from, O_RDONLY)) == -1) //open the file
	{
		printf("Company Not Found!\n");
		exit(1);
	}

    for (i = 0; i < ITEMS; i++) //initallize the struct array
		items_ptr[i] = &items[i];
	countItems=receivingItems(items_ptr); //getting all the items in the order
	for (i = 0; i < countItems; i++)
		updatePrice(items_ptr[i],fd_from); //updating the price of each item
	totalPriceAndFile(items_ptr, countItems, argv[1], argv[2]); //calculate the total price and creating the file
	close(fd_from);
    return 0;
}

//keep getting the order items
int receivingItems(Item * items[]) 
{
    char tempItems[ITEMS];
	int i=0, countItems=0;

    printf("Insert your order (STOP to finish):\n");
    fgets(tempItems, ITEMS, stdin);
	GarbageRemove(tempItems); //reinitallize the string
    while (strcmp(tempItems, "STOP") != 0) { //while the input isn't 'STOP'
		countItems++;
		split_productName_amount(items[i], tempItems); //get the product and the amount
		i++;
		fgets(tempItems, ITEMS, stdin);
        GarbageRemove(tempItems);
	}

    return countItems;
}

//initallize string and "cleaning it"
void GarbageRemove(char* line) 
{
	int i;
	if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';
	while (strlen(line) > 0 && line[strlen(line) - 1] == ' ') //spacing reduction in the end
		line[strlen(line) - 1] = '\0';
    while (strlen(line) > 0 && line[0] == ' '){//spacing reduction in the beginning
		for (i = 1; i < strlen(line) - 1; i++)
			line[i - 1] = line[i];
	}
}

//seperate the price and product name
void split_productName_amount(Item* items, char* line) {
	int i, j;
	i = strlen(line) - 1;
	while (i >= 0 && line[i] != ' ') //while there isnt space
		i--;
    items->amount = atoi(line + i);
	for (j = 0; j < i; j++) //loop over the product name
		items->productName[j] = line[j];
	items->productName[j] = '\0';
	GarbageRemove(items->productName);
}

//updating the price for all the items
void updatePrice( Item* item,int fd_from) 
{
	int rbytes=0,i,sum=0;
	char buff[MAX] ,*ptrInstance=NULL,tempName[MAX];
    if(lseek(fd_from, 0, SEEK_SET)==-1) 
    {
		perror("lseek1 error");
		exit(1);
	}

    if ((rbytes = read(fd_from, buff, MAX - 1)) == -1)	
    { 
		perror("read1 ");
		exit(1);
	}

    while (rbytes > 0)
    {
        sprintf(tempName, "%s", item->productName);
		ptrInstance = strstr(buff, tempName); //find the product name
		if (ptrInstance != NULL) 
        {
			if(searchPrice_afterSale(buff, ptrInstance, item)==1)
            {
				break;
			}    
		}

        for (i = 0; i < MAX; i++) //reinitallize the buffer
			buff[i] = '\0';
		sum = sum-50+MAX-1; //in case the string not found
		if(lseek(fd_from, sum, SEEK_SET)==-1) { //get the pointer forword sum bytes
			perror("lseek2 error");
			exit(1);
		}

        if ((rbytes = read(fd_from, buff, MAX - 1)) == -1)
        { 
			perror("read2 from flyer for price");
			exit(1);
		}
	}

    if (ptrInstance == NULL) { //in case the company name not found
        printf("%s - product Not Found!\n", item->productName);
		item->price = 0;
	}
}

//search the price with given item
int searchPrice_afterSale(char* buff, char* ptr, Item* item) 
{
	char* ptrNis = NULL;
    char *line;
    double sale_prc;
    int price_m;
	ptr=ptr+strlen(item->productName);
    ptrNis = strstr(ptr, "NIS");
	if (ptrNis == NULL) //if the price isnt found
		return 0;

    ptrNis--;
	while (*ptrNis != ' '){// keep reading the price
		ptrNis--;
	} 
    price_m =  atoi(ptrNis);
    line = strtok(buff, "\n"); 
	line = strtok(NULL, "%"); //find discount
    sale_prc =atoi(line);
    sale_prc = 1 - sale_prc/100;
    item->price=price_m*sale_prc; //update after discount
	return 1;
}

//calculate the total price and printing to the file
void totalPriceAndFile(Item* item [], int countItems, char* nameCompany, char* invitingName) 
{ 
	double sum = 0;
    int i;
	char checkConfirm[MAX];
	for (i = 0; i < countItems; i++){//sum all the items price
		sum += item[i]->price*item[i]->amount;
	} 
		
	if (sum==0)
		printf("Total Price: %.1f NIS, can not make an order\n",sum);
    
    else 
    {
		printf("Total Price: %.1f NIS (Confirm to approve/else cancle)\n",sum);
		fgets(checkConfirm, MAX, stdin);
		GarbageRemove(checkConfirm);
		if (strcmp(checkConfirm, "Confirm") == 0) { //if the user inserted "Confirm"
			createOrderFile(item, nameCompany, invitingName, countItems, sum); //create the order file
			printf("Order created! // %s.txt created in %s_Order Dir with Read Mode\n",invitingName,nameCompany);
		}
		else
			printf("Order did not create!\n");
	}
}

//create the file and write into it
void createOrderFile(Item** item,char* nameComp, char* invitingName,int counter,double totalPrice) 
{
	int fdOrder;
	char path[MAX]="/home/braude/Black_Friday/",total[MAX] = {'\0'};
    sprintf(total,"Total Price: %.1f NIS\n",totalPrice);
	strcat(path, nameComp);
	strcat(path, "_Order/");
	strcat(path, invitingName);
	strcat(path, ".txt");

    if ((fdOrder = open(path, O_WRONLY | O_CREAT, 0664)) == -1) 
    { 
		perror("open"); exit(1);
	}
	titleOrderFile(nameComp, fdOrder); //printing the title
	itemsOrderFile(nameComp, fdOrder, item,counter); //priting all the items with the prices
	if (write(fdOrder, total, strlen(total)) == -1) { //write the total price
		perror("write title order"); exit(1);
	}
	timeOrderFile(fdOrder); //printing the time
	close(fdOrder);
    chmod(path, 0444); //change the file permission's 
}

//write the title to the file
void titleOrderFile(char* nameRest, int fdOrder) 
{
	char title[MAX] = { '\0' };
	GarbageRemove(nameRest);
	strcpy(title, nameRest);
	strcat(title, " Order\n\n");
	if (write(fdOrder, title, strlen(title)) == -1) { //write to the file
		perror("write title order"); exit(1);
	}
}

//writing all the order with the price to the file
void itemsOrderFile(char* nameRest, int fdOrder, Item *item[],int counter) 
{
	int i;
	char itemOrder[MAX]= { '\0' };
	for (i = 0; i < counter; i++) { //loop over all the items
        if(item[i]->price==0);
        else
        {
		    sprintf(itemOrder, "%s - %d\n\n",item[i]->productName, item[i]->amount);
		    if (write(fdOrder, itemOrder, strlen(itemOrder)) == -1) { //write to the file
				perror("write item order"); exit(1);
            }
		}
	}
}

//printing the current date to the file
void timeOrderFile(int fdOrder) 
{
	char timeStr[MAX] = { '\0' };
	int day, mois, an;
	time_t now;

    // localtime converts a 'time_t' value to calendar time
	time(&now);
	struct tm* local = localtime(&now);
	day = local->tm_mday; //get the day
	mois = local->tm_mon + 1; //get the month
	an = local->tm_year + 1900; //get the year
	sprintf(timeStr, "\n%02d/%02d/%d", day, mois, an);
	if (write(fdOrder, timeStr, strlen(timeStr)) == -1) { //write to file
		perror("write time"); exit(1);
	}
}