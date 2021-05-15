// fruit_bot.c
// This program by YOUR-NAME-HERE (z5207001) on 30/05/18


#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "fruit_bot.h"

void print_player_name(void);
void print_move(struct bot *b);
void run_unit_tests(void);

// # DEFINES:

#define TRUE        1
#define FALSE       0

//prototypes for functions

int should_buy_electricity(struct bot *b);
void buy_electricity(struct bot *b); 
int should_buy(struct bot *b);
void buy(struct bot *b);
int should_sell(struct bot *b);
void sell(struct bot *b);
void move_east(struct bot *b);
int nearest_fruit_store_to_sell(struct bot *b);
int selling_fruit_distance(struct location *l);
int nearest_fruit_store_to_buy(struct bot *b);
int buying_fruit_distance(struct location *l);
int nearest_electricity_to_buy(struct bot *b);
int buying_electricity_distance(struct location *l);


int main(int argc, char *argv[]) {

    if (argc > 1) {
        // supply any command-line argument to run unit tests
        run_unit_tests();
        return 0;
    }

    struct bot *me = fruit_bot_input(stdin);
    if (me == NULL) {
        print_player_name();
    } else {
        print_move(me);
    }

    return 0;
}

void print_player_name(void) {
    // CHANGE THIS PRINTF TO YOUR DESIRED PLAYER NAME
    printf("Search Party for Aaron Tan");

}

void print_move(struct bot *b) {

    if (should_buy_electricity(b) == TRUE) {
        buy_electricity(b); 
    } else if (should_buy(b) == TRUE) {
        buy(b);   
    } else if (should_sell(b) == TRUE) {
        sell(b); 
    } else {
        move_east(b);
    }
    
}

// additional functions

int should_buy_electricity(struct bot *b) { //When bot should buy electricity  
    int buy = TRUE;
        
    //is capacity of bot = amount of electricty bot has?
    if (b->battery_capacity == b->battery_level) {
        buy = FALSE;
    }
    //does location have electricity to sell
    else if (strcmp(b->location->fruit, "Electricity") != 0) {
        buy = FALSE;
    }
    //if location buys electricity
    else if (b->location->price > 0) {
        buy = FALSE;
    }
    //if location has no electricity to sell
    else if (b->location->quantity == 0) {
        buy = FALSE;
    }
    //location must sell electricity
    else if (strcmp(b->location->fruit, "other") == 0) {
        buy = FALSE;
    }
    //if charge left in bot is greater than the turns in game left
    else if (b->battery_level > b->turns_left) {
        buy = FALSE;
    }//if charge left in bot is less than the turns in game left
    else if (b->battery_level < b->turns_left) {
        buy = TRUE;
    }
    return buy;
}

void buy_electricity(struct bot *b) { //How much electricity bot should buy
    int distance = nearest_electricity_to_buy(b);
    int buy = should_buy_electricity(b);
    int quantity = (b->battery_capacity - b->battery_level);
    printf("Buy %d\n", quantity);
}

int should_buy(struct bot *b) { //When bot should buy a fruit
    int buy = TRUE;
    int kg_of_fruit = b->fruit_kg;
    int capacity = b->maximum_fruit_kg;
    int distance_to_buy = nearest_fruit_store_to_buy(b);
    
    //If location does not sell Electricity
    if (strcmp(b->location->fruit, "Electricity") == 0) { 
        buy = FALSE;
    }
    //If location sells fruit
    else if (b->location->price > 0) {
        buy = FALSE;
    }
    //If location has quantity to sell
    else if (b->location->quantity == 0) {
        buy = FALSE;
    }
    //If cash on hand is less than money need to buy, do not buy
    else if ((-1)*(b->cash) > (b->location->price)) {
        buy = FALSE;
    }
    //If buying fruit overloads capacity, do not buy
    else if ((kg_of_fruit + 1) > capacity) {
        buy = FALSE;
    }
    //If less than 3 turns left in round, do not buy
    else if (b->turns_left < 3) {
        buy = FALSE;
    }
    //If location has no quantity to sell, do not buy
    if ((strcmp(b->location->fruit, "Electricity") != 0) && (b->location->price > 0) 
        && (b->location->quantity == 0)) {
        buy = FALSE;
    }
    
    return buy;
}

void buy(struct bot *b) { //Quantity to buy
    int quantity = 1000; //Maximum amount for bot to buy
        printf("Buy %d\n", quantity);
}

int should_sell(struct bot *b) { //When bot should sell
    int sell = TRUE;
    //Do not sell if price is 0
    if (b->location->price == 0) { 
        sell = FALSE;
    }
    //Do not sell if location will buy "Anything"
    else if (strcmp(b->location->fruit, "Anything") == 0) {
        sell = FALSE;
    }
    //Do not sell to a location that wants to sell
    else if (b->location->price < 0) {
        sell = FALSE;
    }
    //Do not sell if location is unwilling to buy anymore
    else if (b->location->quantity == 0) {
        sell = FALSE;
    }
    
    return sell;
}

void sell(struct bot *b) { //Quantity to sell
    int quantity = b->fruit_kg; //Sells amount of fruit in bot
    
    if (quantity == 0) {
        printf("Move 1\n");
    } else {
        printf("Sell %d\n", quantity);
    }
    
}

void move_east(struct bot *b) { //Movement of bot
    int distance_to_sell = nearest_fruit_store_to_sell(b);
    int distance_to_buy = nearest_fruit_store_to_buy(b);
    int to_buy_electricity = should_buy_electricity(b);
    int distance_to_buy_electricity = nearest_electricity_to_buy(b);
    if (to_buy_electricity == TRUE) { //Moves exact distance to location that sells electricity
        printf("Move %d\n", distance_to_buy_electricity);
    }
    else if (b->fruit_kg > 0) { //Moves exact distance to location that buys fruit
        printf("Move %d\n", distance_to_sell);
    }
    else if (b->fruit_kg == 0) { //Moves exact distance to location that sells fruit
        printf("Move %d\n", distance_to_buy);
    }
    else { 
        printf("Move 1\n");
    }
}

int nearest_fruit_store_to_sell(struct bot *b) { //Finds nearest location that buys fruit
    
    struct location *start = b->location;
    struct location *eastwards = b->location;
    struct location *westwards = b->location;
    int distance = 0;
    while (distance == 0 || eastwards != start) {
        if (selling_fruit_distance(eastwards)) {
            return distance;
        }
        if (selling_fruit_distance(westwards)) {
            return -distance;
        }
        eastwards = eastwards->east;
        westwards = westwards->west;
        distance = distance + 1;
    }
    return distance;
}    
int selling_fruit_distance(struct location *l){ //Needed for the above function
    return strcmp(l->fruit, "Electricity") != 0 &&
            strcmp(l->fruit, "Anything") != 0 &&
           l->price > 0 &&
           l->quantity > 0;
}

int nearest_fruit_store_to_buy(struct bot *b) { //Finds nearest location that sells fruit
    
    struct location *start = b->location;
    struct location *eastwards = b->location;
    struct location *westwards = b->location;
    int distance = 0;
    while (distance == 0 || eastwards != start) {
        if (buying_fruit_distance(eastwards)) {
            return distance;
        }
        if (buying_fruit_distance(westwards)) {
            return -distance;
        }
        eastwards = eastwards->east;
        westwards = westwards->west;
        distance = distance + 1;
    }
    return distance;
}  

int buying_fruit_distance(struct location *l){ //Needed for above function
    return strcmp(l->fruit, "Electricity") != 0 &&
            strcmp(l->fruit, "Anything") != 0 &&
           l->price < 0 &&
           l->quantity > 0;
}

int nearest_electricity_to_buy(struct bot *b) { //Finds nearest location that sells electricity

    struct location *start = b->location;
    struct location *eastwards = b->location;
    struct location *westwards = b->location;
    int distance = 0;
    while (distance == 0 || eastwards != start) {
        if (buying_electricity_distance(eastwards)) {
            return distance;
        }
        if (buying_electricity_distance(westwards)) {
            return -distance;
        }
        eastwards = eastwards->east;
        westwards = westwards->west;
        distance = distance + 1;
    }
    return distance;
}
 
int buying_electricity_distance(struct location *l){ //Needed for above function
    return strcmp(l->fruit, "Electricity") == 0 &&
           l->price < 0 &&
           l->quantity > 0;
}          

// overall testing strategy

//1.) Test the functions: 
//  -should_buy_electricity(struct bot *b)
//  -should_buy(struct bot *b)
//  -should_sell(struct bot *b)
//  -move_east(struct bot *b)

//2.) Create more specific conditions to test the functions inside the functions
//  listed above.

//3.) Testing is done by creating seperate worlds in different files.
// For should_buy_electricity, make a world where the maximum_battery of the bot
// is significantly less than the rounds in the game.
// E.g. maximum_capacity = 5, turns_left = 30.
// This will test to see if the bot will continuously buy electricity

//4.) For should_buy, test for the maximum_capacity.
// E.g. if maximum_capacity = 20 kg, and current fruit_kg = 20,
// the bot should not buy additional fruit. Rather it should try to sell.

//5.) For should_sell, test whether the bot will ever sell to "Anything"
// E.g. create a world where there are no locations to sell the fruits.
// This will leave only the "Anything" locations available to buy the fruits.

//6.) For move_east, test whether the bot immediately moves to the buying and selling
// locations.
//E.g. create a world in which the buying and selling locations are seperated by a 
// noticeable distance. This will make it evident whether the bot is moving without 
// exception to those specific market locations.

//7.) If these 4 functions work, then by effect the smaller functions work as well.
// void run_unit_tests(void) {

// }


