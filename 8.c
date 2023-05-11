#include<stdio.h>
#include<conio.h>
#include<string.h>
typedef struct
{
	char name[50];
	float price;
} Food;
typedef struct
{
	char customer[50];
	char food[50];
	int day;
	int month;
	int year;
} Order;
void addFood()
{
	Food newFood;
	FILE *ptf;
	if((ptf = fopen("foods.txt", "a")) == NULL)
	{
		printf("File could not be opened.\n");
		return;
	}
	printf("Please enter food's name and press Enter.\n(The name can contain \"space\".)\n");
	gets(newFood.name);
	/*???*///getchar();
	printf("Now enter food's price and press Enter.\n");
	scanf("%f", &newFood.price);
	fputs(newFood.name, ptf);
	fprintf(ptf, "^*%.2f^*", newFood.price);
	fclose(ptf);
}
void sortFoods(Food fl[], int n)
{
	int i, j;
	Food temp;
	for(i = 0; i < n - 1; i++)
		for(j = i + 1; j < n; j++)
			if(fl[i].price > fl[j].price)
			{
				temp = fl[i];
				fl[i] = fl[j];
				fl[j] = temp;
			}
	FILE *ptf;
	if((ptf = fopen("foods.txt", "w")) == NULL)
	{
		printf("File could not be opened.\n");
		return;
	}
	for(i = 0; i < n; i++)
	{
		fputs(fl[i].name, ptf);
		fprintf(ptf, "^*%.2f^*", fl[i].price);
	}
	fclose(ptf);
	printf("Foods have been sorted successfully.\n");
}
int checkOrder(char no[], Food fl[], int n)
{
	int i;
	for(i = 0; i < n; i++)
		if(strcmp(no , fl[i].name) == 0)
			return i;
	return -1;
}
void addOrder(Food fl[], int n)
{
	int x, y;
	float income;
	Order newOrder;
	FILE *ptf;
	if((ptf = fopen("orders.txt", "a")) == NULL)
	{
		printf("File could not be opened.\n");
		return;
	}
	printf("Please enter customer's name and press Enter.\n");
	gets(newOrder.customer);
	while(1)
	{
		printf("Please enter food's name and press Enter.\n");
		gets(newOrder.food);
		if((y = checkOrder(newOrder.food, fl, n)) != -1)
		{
			printf("Please enter date in (d m y) order and press Enter.\n");
			scanf("%d", &newOrder.day);
			scanf("%d", &newOrder.month);
			scanf("%d", &newOrder.year);
			fputs(newOrder.customer, ptf);
			fprintf(ptf, "*%s", newOrder.food);
			fprintf(ptf, "*%d", newOrder.day);
			fprintf(ptf, "*%d", newOrder.month);
			fprintf(ptf, "*%d*", newOrder.year);
			printf("New order added successfully.\n");
			break;
		}
		else
		{
			printf("Food not found. Do you want to try again or dismiss this order?\n(enter 1 to retry and 2 to dismiss)\n");
			scanf("%d", &x);
			getchar();
			if(x == 2)
				break;
		}
	}
	fclose(ptf);
	if((ptf = fopen("income.txt", "r")) == NULL)
	{
		printf("File could not be opened.\n");
		return;
	}
	fscanf(ptf, "%f", &income);
	income += fl[y].price;
	fclose(ptf);
	if((ptf = fopen("income.txt", "w")) == NULL)
	{
		printf("File could not be opened.\n");
		return;
	}
	fprintf(ptf, "%.2f", income);
	fclose(ptf);
	printf("The current income of the restaurant is %.2f .\n", income);
}
void fillfl(Food fl[], int *pnf)
{
	int i, j;
	char *ps;
	char temp[50000], p[50];
	*pnf = 0;
	FILE *ptf;
	if((ptf = fopen("foods.txt", "r")) == NULL)
	{
		printf("File could not be opened.\n");
		return;
	}
	fseek(ptf, 0, SEEK_END);
	if(ftell(ptf) != 0)
	{
		fseek(ptf, 0, SEEK_SET);
		fgets(temp, 50000, ptf);
		if(temp[0] != '\0')
		{
			ps = strtok(temp, "*");
			for(j = 0; *(ps + j) != '^'; j++)
				fl[0].name[j] = *(ps + j);
			ps = strtok(NULL, "*");
			for(j = 0; *(ps + j) != '^'; j++)
				p[j] = *(ps + j);
			p[j] = '\0';
			sscanf(p, "%f", &fl[0].price);
			*pnf += 1;
		}
		for(i = 1; (ps = strtok(NULL, "*")) != NULL; i++)
		{
			for(j = 0; *(ps + j) != '^'; j++)
				fl[i].name[j] = *(ps + j);
			ps = strtok(NULL, "*");
			for(j = 0; *(ps + j) != '^'; j++)
				p[j] = *(ps + j);
			p[j] = '\0';
			sscanf(p, "%f", &fl[i].price);
			*pnf += 1;
		}
	}
	fclose(ptf);
}
int main()
{
	int i, nf, *pnf;
	pnf = &nf;
	char c;
	Food foodList[500];
	printf("Welcome to the restaurant system programm.\n\n");
	FILE *ptf;
	if((ptf = fopen("income.txt", "a")) == NULL)
	{
		printf("File could not be opened.\n");
		return 0;
	}
	fclose(ptf);
	if((ptf = fopen("foods.txt", "a")) == NULL)
	{
		printf("File could not be opened.\n");
		return 0;
	}
	fclose(ptf);
	fillfl(foodList, pnf);
	while(1)
	{
		printf("What do you want to do?\n(enter 1 to add a new food, 2 to sort entered foods, and 3 to add an order)\n");
		scanf("%d", &i);
		getchar();/*just to remove enter from buffer*/
		while(i != 1 && i != 2 && i != 3)
		{
			printf("Incorrect input. Please try again.\n");
			scanf("%d", &i);
			getchar();/*just to remove enter from buffer*/
		}
		switch(i)
		{
			case(1):
				addFood();
				fillfl(foodList, pnf);
				break;
			case(2):
				sortFoods(foodList, nf);
				break;
			case(3):
				addOrder(foodList, nf);
				break;
		}
		printf("Do you whant to continue?\n(enter y/Y to continue, or n/N to end the programm)\n");
		scanf(" %c", &c);
		while(c != 'y' && c != 'Y' && c != 'n' && c != 'N')
		{
			printf("Incorrect input. Please try again.\n");
			scanf(" %c", &c);
		}
		if(c == 'n' || c == 'N')
			break;
	}
	printf("Thanks for using this programm");
	return 0;
}
