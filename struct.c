#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define MAX 10

struct name {
	int a;
	double b;
	char msg[50];
};

struct name *Person_create(char str[] , int a , double b){
	struct name *person = malloc(sizeof(struct name));
	assert(person != NULL);
	assert(strlen(str) < 50);

	strncpy(person->msg,str , strlen(str));
	person->a = a;
	person->b = b;

	return person; 
}
void print(struct name *person){
  printf("%s\t%d\t%f\t \n" , person->msg , person->a , person->b);
}

void destroy(struct name *objects[]){
	int i = 0;
	for(i = 0 ; i < MAX ; i++){
		free(objects[i]);
	}
}

int main(){
	struct name *ptr = Person_create("RICHMOND" , 24 , 2000000.50);
	print(ptr);
	int i = 0;
	struct name *objects[MAX];
	for ( i= 0 ; i < MAX ; i++){
		//new object every time;
		objects[i] = (struct name *) malloc(sizeof(struct name));
		objects[i]->a = i;
		objects[i]->b = i + 1.0	;
		strcpy(objects[i]->msg , "NULL HAHA");	
	}
	for(i = 0 ; i < MAX ; i++){
		printf("----------------------------------------------\n");
		printf("%p=> %d	   %f	%s\n" , objects[i] ,(*objects[i]).a ,(*objects[i]).b , (*objects[i]).msg); 
	}
	//now create a copy  and modify
	struct name *copy[MAX];
	memcpy(&copy , &objects , sizeof(objects)); //not exactly what i want
	printf("Object in bytes %d \n" , sizeof(objects) / 4);
	copy[4]->a = 500;
	copy[6]->b = 5005.67 + 4005.09;
	strcpy(copy[8]->msg , "FUCK");
	  for(i = 0 ; i < MAX ; i++){
                printf("================================================\n");
                printf("%p=> %d    %f   %s\n" , copy[i] ,(*copy[i]).a ,(*copy[i]).b , (*copy[i]).msg);
        }
	printf("objects still have intial values????\n");
	   for(i = 0 ; i < MAX ; i++){
                printf("----------------------------------------------\n");
                printf("%p=> %d    %f   %s\n" , objects[i] ,(*objects[i]).a ,(*objects[i]).b , (*objects[i]).msg);
        }
	destroy(objects);
	destroy(copy);
	struct name *names[MAX];
	int j = 0;
	for(j = 0 ; j < MAX ; j++){
	  names[j] = Person_create("read some thing" , j , i+2);//constructor like in java
	}
	for (j = 0 ; j < MAX ; j++){
	  printf("*************************************************\n");
	  printf("%p-->" , names[j]);
	  print(names[j]);
	}
}
