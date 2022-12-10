#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20000

void find_fruit(char input[],char fruit[][15]);

int main(void) {
	char fruit[20][15] = {
        "guava",
        "litchi",
        "longan",
        "watermelon",
        "pomelo",
        "pear",
        "banana",
        "papaya",
        "pumpkin",
        "tomato",
        "mango",
        "kiwi",
        "persimmon",
        "cantaloupe",
        "strawberry",
        "grape",
        "peach",
        "orange",
        "coconut",
        "lemon"
    };
    char input[N];
    scanf("%s", input);
    find_fruit(input,fruit);
    return 0;
}void find_fruit(char input[],char fruit[][15]){
// input is the fruit sequence eg: watermelon+_+watermelon+_+coconut+_+grape+_+coconut
// fruit is 20 kinds of fruit name eg: fruit[0] is "guava", fruit[1] is litchi and so on.
// you need to check the Loader code to understand above meaning
    char *p,*c;
    p = strtok(input, "+_+");

    int n=0;
    int fru[20] = {0}, big = 0;
    while(p != NULL) {
        c = p;
        for(int i=0 ; i<20 ; i++){
            int counter = 0;
            for(int h=0 ; h<strlen(fruit[i]) ; h++){
                if(fruit[i][h] == *(c+h)){
                    counter++;
                }else{
                    break;
                }
            }
            if(counter == strlen(fruit[i])){
                fru[i]++;
            }
        }
        n++;
        p = strtok(NULL,"+_+");
    }
    for(int i=0 ; i<20 ; i++){
        if(fru[i] > big) {
            big = fru[i];
        }
    }
    for(int i=0 ; i<20 ; i++){
        if(fru[i] == big){
            printf("%s\n",fruit[i]);
        }
    }


////watermelon+_+watermelon+_+coconut+_+grape+_+coconut
//pumpkin+_+coconut+_+guava+_+longan+_+peach+_+pomelo+_+coconut+_+litchi+_+lemon+_+coconut+_+persimmon+_+pear+_+coconut+_+banana+_+strawberry+_+tomato+_+cantaloupe+_+orange+_+coconut+_+pear+_+tomato+_+orange+_+grape+_+pomelo+_+litchi+_+grape+_+pumpkin+_+cantaloupe+_+strawberry+_+coconut+_+kiwi+_+grape+_+pumpkin+_+pomelo+_+tomato+_+pear+_+guava+_+papaya+_+coconut+_+kiwi+_+pear+_+mango+_+orange+_+peach+_+tomato+_+watermelon+_+orange+_+longan+_+guava+_+papaya+_+papaya+_+tomato+_+pomelo+_+grape+_+pear+_+pear+_+longan+_+pear+_+kiwi+_+pumpkin+_+watermelon+_+strawberry+_+watermelon+_+pomelo+_+mango+_+persimmon+_+litchi+_+kiwi+_+persimmon+_+lemon+_+longan+_+orange+_+litchi+_+kiwi+_+pear+_+mango+_+strawberry+_+longan+_+pomelo+_+grape+_+tomato+_+pomelo+_+peach+_+banana+_+lemon+_+longan+_+watermelon+_+litchi+_+papaya+_+strawberry+_+litchi+_+watermelon+_+litchi+_+pear+_+lemon+_+kiwi+_+tomato+_+guava+_+strawberry+_+litchi+_+kiwi+_+orange+_+kiwi+_+persimmon+_+pumpkin+_+pumpkin+_+watermelon+_+grape+_+watermelon+_+lemon+_+mango+_+pomelo+_+watermelon+_+coconut+_+mango+_+longan+_+guava+_+strawberry+_+grape+_+guava+_+guava+_+orange+_+grape+_+litchi+_+strawberry+_+strawberry+_+pear+_+grape+_+banana+_+lemon+_+tomato+_+orange+_+peach+_+persimmon+_+tomato+_+orange+_+guava+_+pomelo+_+persimmon+_+watermelon+_+pomelo+_+strawberry+_+pumpkin+_+papaya+_+persimmon+_+mango+_+longan+_+pear+_+pomelo+_+tomato+_+orange+_+pear+_+banana+_+pomelo+_+coconut+_+persimmon+_+mango+_+watermelon+_+pumpkin+_+peach+_+watermelon+_+tomato+_+cantaloupe+_+kiwi+_+litchi+_+strawberry+_+guava+_+cantaloupe+_+lemon+_+persimmon+_+tomato+_+grape+_+coconut+_+tomato+_+strawberry+_+watermelon+_+lemon+_+peach+_+pumpkin+_+pomelo+_+coconut+_+orange+_+litchi+_+pomelo+_+litchi+_+lemon+_+tomato+_+kiwi+_+grape+_+tomato+_+papaya+_+coconut+_+coconut+_+persimmon+_+litchi+_+kiwi+_+banana+_+longan+_+pomelo+_+pear+_+strawberry+_+pear+_+persimmon+_+pear+_+strawberry+_+papaya+_+pumpkin+_+strawberry+_+grape+_+pumpkin+_+coconut+_+cantaloupe+_+orange+_+kiwi+_+mango+_+coconut+_+mango+_+lemon+_+litchi+_+pear+_+pumpkin+_+guava+_+grape+_+coconut+_+persimmon+_+orange+_+tomato+_+mango+_+lemon+_+pear+_+pumpkin+_+pear+_+kiwi+_+guava+_+mango+_+pear+_+papaya+_+mango+_+lemon+_+watermelon+_+mango+_+tomato+_+peach+_+papaya+_+guava+_+banana+_+pear+_+watermelon+_+orange+_+coconut+_+guava+_+pear+_+coconut+_+peach+_+grape+_+longan+_+pear+_+peach+_+cantaloupe+_+pomelo+_+longan+_+cantaloupe+_+tomato+_+pear+_+cantaloupe+_+persimmon+_+mango+_+cantaloupe+_+strawberry+_+longan+_+peach+_+pear+_+kiwi+_+pomelo+_+persimmon+_+pomelo+_+watermelon+_+mango+_+papaya+_+guava+_+pumpkin+_+papaya+_+coconut+_+lemon+_+grape+_+pear+_+cantaloupe+_+guava+_+longan+_+coconut+_+peach+_+pomelo+_+kiwi+_+coconut+_+tomato+_+pear+_+mango+_+kiwi+_+coconut+_+peach+_+cantaloupe+_+banana+_+litchi+_+orange+_+longan+_+banana+_+cantaloupe+_+orange+_+peach+_+persimmon+_+mango+_+peach+_+lemon+_+guava+_+papaya+_+papaya+_+pear+_+cantaloupe+_+lemon+_+papaya+_+kiwi+_+peach+_+watermelon+_+grape+_+strawberry+_+pomelo+_+guava+_+peach+_+peach+_+mango+_+persimmon+_+litchi+_+pumpkin+_+banana+_+mango+_+longan+_+persimmon+_+watermelon+_+guava+_+guava+_+grape+_+mango+_+pumpkin+_+papaya+_+mango+_+peach+_+strawberry+_+grape+_+tomato+_+cantaloupe+_+grape+_+persimmon+_+litchi+_+coconut+_+papaya+_+grape+_+watermelon+_+lemon+_+kiwi+_+lemon+_+litchi+_+pomelo+_+persimmon+_+litchi+_+mango+_+watermelon+_+pomelo+_+strawberry+_+banana+_+pomelo+_+banana+_+strawberry+_+banana+_+strawberry+_+litchi+_+peach+_+longan+_+papaya+_+watermelon+_+kiwi+_+persimmon+_+coconut+_+peach+_+strawberry+_+tomato+_+watermelon+_+tomato+_+persimmon+_+grape+_+cantaloupe+_+watermelon+_+peach+_+orange+_+papaya+_+coconut+_+lemon+_+mango+_+strawberry+_+pear+_+tomato+_+mango+_+kiwi+_+watermelon+_+peach+_+pear+_+peach+_+pomelo+_+pumpkin+_+grape+_+papaya+_+kiwi+_+papaya+_+coconut+_+lemon+_+cantaloupe+_+papaya+_+grape+_+watermelon+_+kiwi+_+mango+_+peach+_+banana+_+coconut+_+pear+_+cantaloupe+_+pumpkin+_+peach+_+pomelo+_+longan+_+cantaloupe+_+pear+_+persimmon+_+pomelo+_+pumpkin+_+pumpkin+_+litchi+_+peach+_+persimmon+_+litchi+_+kiwi+_+persimmon+_+pear+_+coconut+_+mango+_+pomelo+_+persimmon+_+tomato+_+lemon+_+papaya+_+guava+_+tomato+_+grape+_+coconut+_+pumpkin+_+persimmon+_+kiwi+_+pumpkin+_+guava+_+papaya+_+kiwi+_+cantaloupe+_+persimmon+_+watermelon+_+tomato+_+persimmon+_+pomelo+_+mango+_+pumpkin+_+pumpkin+_+pomelo+_+lemon+_+guava+_+tomato+_+mango+_+longan+_+pear+_+strawberry+_+watermelon+_+pear+_+cantaloupe+_+grape+_+banana+_+pumpkin+_+cantaloupe+_+banana+_+persimmon+_+pear+_+grape+_+persimmon+_+persimmon+_+banana+_+orange+_+pear+_+litchi+_+banana+_+orange+_+orange+_+pumpkin+_+coconut+_+coconut+_+pomelo+_+orange+_+mango+_+cantaloupe+_+lemon+_+cantaloupe+_+lemon+_+cantaloupe+_+peach+_+peach+_+banana+_+pomelo+_+longan+_+banana+_+orange+_+tomato+_+coconut+_+longan+_+peach+_+longan+_+grape+_+longan+_+lemon+_+persimmon+_+grape+_+orange+_+litchi+_+pear+_+coconut+_+lemon+_+watermelon+_+longan+_+tomato+_+cantaloupe+_+pumpkin+_+pumpkin+_+banana+_+lemon+_+longan+_+grape+_+grape+_+guava+_+lemon+_+tomato+_+papaya+_+peach+_+mango+_+orange+_+kiwi+_+banana+_+guava+_+coconut+_+guava+_+kiwi+_+mango+_+peach+_+litchi+_+watermelon+_+litchi+_+lemon+_+watermelon+_+peach+_+litchi+_+persimmon+_+tomato+_+tomato+_+guava
}