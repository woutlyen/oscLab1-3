#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main (int argc, char **argv){

int MAX = 20;
char first[MAX];
char second[MAX];
char name[MAX*2];
char str[MAX];
int year;

printf("Enter firstname: ");
scanf("%s",first);
printf("Enter Second Name: ");
scanf("%s", second);

//printf("%s %s\n",first,second);

for(int i=0; i<MAX; i++){
	str[i] = toupper(second[i]);
}

printf("%s\n",str);

printf("Return value of strcmp(): %d\n", strcmp(second, str));
printf("Return value of strcasecmp(): %d\n", strcasecmp(second, str));

sprintf(name, "%s %s", first, second);
printf("%s\n", name);

printf("Enter birthyear: ");
scanf("%d", &year);
printf("%d\n", year);

return 0;
}

