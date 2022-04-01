//Name: Victoria Moreno
//Dr. Steinberg
//COP3502 Spring 2022
//Programming Assignment 3 Skeleton

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct card_s{
	int rank;    //number
	char * type; //type of card
	struct card_s * nextptr;
}card_t;

//function prototypes
void rules(); //display rules
int playRound(); //simulate round
card_t * openCardDeck(); //open the card deck and place into a linkedlist setup
card_t * insertBackSetup(card_t *node, char *name, int cardrank); //take card from orginial deck and place in back of linked list for setup of game
int empty(card_t * node); //check to see if linked list is empty
void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * cardp1, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck


int main()
{
	int seed;
	printf("Enter Seed: ");
	scanf("%d", &seed);
	
	srand(seed); //seed set
	rules();
	
	int player; //1 or 2
	int result;
	
	printf("Would you like to be player 1 or 2?\n");
	printf("Enter 1 or 2: ");
	scanf("%d", &player); //choose player
	
	for(int game = 1; game <= 5; ++game) //simulate games
	{
		printf("Alright lets play game %d.\n", game);
		printf("Lets split the deck.\n");
		
		result = playRound(); //play game
		
		if((result == 1 && player == 1) || (result == 2 && player == 2)) //determine who won
			printf("You won game %d.\n", game);
		else
			printf("I won game %d.\n", game);
	}
	
	return 0;
}

int deckSize(card_t * head){

	if(head == NULL){
		return 0;
	}
    if(head->nextptr == NULL){
		return 1;
	}
    int count = 1;
    while(head->nextptr !=NULL){
		return 1 + deckSize(head->nextptr);
    }

    return count;
}
void cleanUp(card_t * head){
    
	while(head->nextptr != NULL)
	{
		card_t *temp = head->nextptr;
		free(head->type);
		free(head);
		head = temp;
	}
}

card_t * removeCard(card_t * node, int spot){


	card_t* nodeS;
	nodeS = search(node, spot);

	if(spot != 0){

		card_t *nodeB;
		nodeB = search(node, spot -1);

		//because spot is a random number from 0 - 51 
		// and size goes from 1-52
		if(spot != deckSize(node)-1){
			nodeB->nextptr = nodeB->nextptr->nextptr;
			nodeS->nextptr = NULL;
		}
		//if its the last node 
		else{
			nodeB->nextptr = NULL;
			nodeS->nextptr = NULL;
		}
		

	}
	//if the one being dealt is the first one 
	else if(spot ==0){
		//when its the last one in the deck you need to return something 
		if(node->nextptr == NULL){
			//printf("Last node... what do i do? \n" );
			return NULL;
		}
		card_t *temp = node->nextptr;

		node->nextptr = NULL;
		return temp;

	}
	//if the node is the last one only free it	
	if(nodeS != NULL)
		free(nodeS);

    return node;
}


card_t * copyCard(card_t * node){

	//printf("\n CREATING A COPY OF THE CARD %s\n", node->type);
	card_t *newCard = malloc(sizeof(card_t)*1);
	if (newCard == NULL){
        printf("Error in malloc...\n");
        exit(1);
    }
	newCard->type = (char *) malloc(sizeof(char) * 20);
	if(newCard->type == NULL){
		printf("Error with malloc...\n");
		exit(1);
	}
	newCard->nextptr = NULL;
	newCard->rank = node->rank;
	//newCard->type = node->type;  cant set two strings equal to eachother 

	strcpy(newCard->type, node->type);

    return newCard;
}

//looking for the node at the number spot so you need a counter
// the node passed in is the head
card_t * search(card_t * node, int spot){
    int counter = 0;
    while(counter != spot && node->nextptr != NULL){
        counter++;
        node = node->nextptr;
    }
    
    return node;
}

// this is where you put each players card on the back of their deck 
card_t * insertBackDeck(card_t *head, card_t *node){
	
	//temp is what youll use to traverse down the linked list bc you still need to return the head
	//have to make the head
	
	if(empty(head)) //check to see if list is empty
    {
		//printf("head was empty...");
		head = (card_t *) malloc(sizeof(card_t));
		
		if(empty(head)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			head->type = (char *) malloc(sizeof(char) * 20);
			
			if(head->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}

			strcpy(head->type, node->type);
			head->rank = node->rank;
			head->nextptr = NULL; //must make new tail nextptr NULL!!!
			
			return head;  //terminate
	
		}
      
    }


	card_t *temp = head; //used to go to the end of the linked list 

	while(temp->nextptr != NULL){
		temp = temp->nextptr;
	} //now youre at the end of the linked list 
	
	temp->nextptr = copyCard(node); //copying the card to the end of the list 


	return head;
}

//being used in playround to find when a players deck is >2
int playRound(){
    
    //playround also goes until someones decksize is <2. also need to check this in the war scenario
    
    
    card_t* wholeDeck = openCardDeck(); //open the deck
	//printing the whole deck to see what it looks like 
	card_t *tempDeck = wholeDeck;

  
    //shuffle it	
    int size = deckSize(wholeDeck);
	//printf("size is %d", size);	
	printf("There are 52 cards in the deck.\n");
	card_t *player1D = NULL;
	card_t *player2D = NULL;

    //while(size > 2 && wholeDeck != NULL){ //the dealing which is shuffled and empties the main deck 
    while(!empty(wholeDeck)){
	    size = deckSize(wholeDeck);
		//printf("size is %d", size);

		int card2 = rand()%size; //SWICTHED THE ORDER
        player2D = insertBackDeck(player2D, search(wholeDeck, card2));
        wholeDeck = removeCard(wholeDeck, card2);

        size = deckSize(wholeDeck);

        int card1 = rand()%size;
        player1D = insertBackDeck(player1D, search(wholeDeck, card1));
        wholeDeck = removeCard(wholeDeck, card1); 

	
    }
    int p1Count = deckSize(player1D); // so whoevers is bigger is the winner
	int p2Count = deckSize(player2D);



	int c = 0; //to check the rounds
	while(!empty(player1D) && !empty(player2D)){ //while hes not winning or losing 

		printf("Player 1 pulled out %s. \t Player 2 pulled out %s.\n", player1D->type, player2D->type);
		if(compareCard(player1D, player2D) == 1){
			printf("Player 1 won that round.\n");
			p1Count = deckSize(player1D);
			p2Count = deckSize(player2D);
			player1D = moveCardBack(player1D); //putting player1s card back at the bottom 1st
			player1D = insertBackDeck(player1D,player2D); //moving player2s card to the bottom of player1s 2nd 
			player2D = removeCard(player2D, 0);
		}
		else if(compareCard(player1D, player2D) == 2){
			printf("Player 2 won that round.\n");
			p1Count = deckSize(player1D);
			p2Count = deckSize(player2D);
			player2D = moveCardBack(player2D); //do p2 first
			player2D = insertBackDeck(player2D, player1D); // do p1 second
			player1D = removeCard(player1D,0);
			 
			
		}
		else{ // when its a tie
			//check the 3rd card 
			//keeping the head here just in case
            //MINIMUM CARDS HERE IS 3 
			if(deckSize(player1D) > 2 && deckSize(player1D)< 50 ){//if there are enough cards
				card_t *p1temp = player1D->nextptr->nextptr;
				card_t *p2temp = player2D->nextptr->nextptr;
				printf("Ugh oh...We have a tie! W-A-R!\n");
				
				int times = 1;
				while(compareCard(p1temp, p2temp) == 0){ //only runs if theres a double war 
					//printf("break not called");
                    if(deckSize(player1D) == 3 || deckSize(player2D) ==3 ){
                        if(deckSize(player1D) == 3){
							printf("Player 1 has 0 cards.\n");
							printf("Player 2 has 52 cards.\n");
							return 2;
						}
						if(deckSize(player2D) == 3){
							printf("Player 1 has 52 cards.\n");
							printf("Player 2 has 0 cards.\n");
							return 1;
						}
                    }
					printf("Player 1 pulled out %s. \t Player 2 pulled out %s.\n", p1temp->type, p2temp->type);
					p1temp = p1temp->nextptr->nextptr;  
					p2temp = p2temp->nextptr->nextptr;
					times++;
					int size = 1 + (2*times); // the initial tie plus the next amount of ties times two

					if(deckSize(player1D) < size || deckSize(player2D) < size){
						if(deckSize(player1D) < size){
							printf("Player 1 has 0 cards.\n");
							printf("Player 2 has 52 cards.\n");
							return 2;
						}
						if(deckSize(player2D) < size){
							printf("Player 1 has 52 cards.\n");
							printf("Player 2 has 0 cards.\n");
							return 1;
						}

					}

					if(times>1){
						printf("Player 1 has %d cards.\n", deckSize(player1D)-size);
						printf("Player 2 has %d cards.\n", deckSize(player2D)-size);
						printf("Ugh oh...We have a tie! W-A-R!\n");
					}
					
				
				} //while theyre the same then continue to go until theyre not the same

				printf("Player 1 pulled out %s. \t Player 2 pulled out %s.\n", p1temp->type, p2temp->type);

			//now the p1 and p2s arent the same and we need to put the cards back correctly 
				if(compareCard(p1temp, p2temp) == 1){ //if player 1 wins
					//printf("player1 won\n");
					
					printf("Player 1 won that W-A-R.\n");
					for(int i = 0; i < times; i++){ 
						
						//printf("time number %d", 1);
						player1D = moveCardBack(player1D);
						player1D = moveCardBack(player1D);
						player1D = insertBackDeck(player1D,player2D); //then player 2 card 1 
						player2D = removeCard(player2D, 0); //gotta remove right after cus the next call is to the front
						player1D = insertBackDeck(player1D,player2D); //player 2 card 2 
						player2D = removeCard(player2D, 0); //remove right after 
						
					}
					//war cards 
					player1D = moveCardBack(player1D);
					player1D = insertBackDeck(player1D,player2D);
					player2D = removeCard(player2D, 0);

					p1Count = deckSize(player1D);
					p2Count = deckSize(player2D);
				}

				if(compareCard(p1temp, p2temp) == 2){ //if player 2 wins
					//printf("player2 won");
					printf("Player 2 won that W-A-R.\n");
					for(int i = 0; i < times; i++){ 
						
						//printf("time number %d", 1);
						player1D = moveCardBack(player1D);
						player1D = moveCardBack(player1D);
						player1D = insertBackDeck(player1D,player2D); //then player 2 card 1 
						player2D = removeCard(player2D, 0); //gotta remove right after cus the next call is to the front
						player1D = insertBackDeck(player1D,player2D); //player 2 card 2 
						player2D = removeCard(player2D, 0); //remove right after
						
					}	
					//war cards 
					player2D = moveCardBack(player2D);
					player2D = insertBackDeck(player2D,player1D);
					player1D = removeCard(player1D, 0);

					p1Count = deckSize(player1D);
					p2Count = deckSize(player2D);			
				}

				//where i removed 
				//}
			}//end of if 
			else{


				if(deckSize(player1D) < 3){
					printf("Player 1 has 0 cards.\n");
					printf("Player 2 has 52 cards.\n");
					return 2;
				}
				if(deckSize(player2D) < 3){
					printf("Player 1 has 52 cards.\n");
					printf("Player 2 has 0 cards.\n");
					return 1;
				}

				break;
			}
			//printf("both players have enough cards\n");
		} // end of else which is the tie 
			
		printf("Player 1 has %d cards.\n", deckSize(player1D));
		printf("Player 2 has %d cards.\n", deckSize(player2D));
	}

	//printf("finished thru the cards");
	int winner= 2;

	if(p1Count > p2Count){
		winner = 1;
	}
    
    if(!empty(player1D)){
        cleanUp(player1D);
    }
    if(!empty(player2D)){
        cleanUp(player2D);
    }
	
    return winner;
}
card_t * moveCardBack(card_t *head){ //need to move the head to the back of the deck 

	if(deckSize(head) == 1){
		return head;
	}
	card_t *temp = head; //
	card_t *newHead = head->nextptr; //what you will return 

	while(temp->nextptr != NULL){ // going to the last node...
		temp = temp->nextptr;
	} //... to set its null pointer to the head and then delete the heads next pointer 

	temp->nextptr = head; 
	head->nextptr = NULL;
	temp->nextptr->nextptr = NULL;
	

	return newHead;

}
int compareCard(card_t * cardp1, card_t * cardp2){
	
	if(cardp1->rank > cardp2->rank){
		return 1;
	}
	else if(cardp1->rank < cardp2->rank){
		return 2;
	}

	return 0; //means theyre the same 
}

//open it in playround()
card_t * openCardDeck(){
	FILE *fptr = fopen("deck.txt", "r");
	
	char *name = (char *) malloc(sizeof(char) * 20);
	
	if (name == NULL)
	{
		printf("Error in malloc...\n");
		exit(1);
	}
	
	card_t * head = NULL;
	
	int cardrank = 13;
	int tracker = 1;
	while(fscanf(fptr, "%s", name) == 1)
	{			
		strcat(name, " ");
		
		if(tracker % 5 == 1)
		{
			strcat(name, "of Clubs");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 2)
		{
			strcat(name, "of Diamonds");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 3)
		{
			strcat(name, "of Hearts");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 4)
		{
			strcat(name, "of Spades");
			head = insertBackSetup(head, name, cardrank);
			tracker = 0;
			--cardrank;
		}
		
		++tracker;
		
	}
	
	free(name);
	fclose(fptr);
	
	return head;
}

//called in open deck and whenever else
card_t * insertBackSetup(card_t *node, char *name, int cardrank)
{
	
    if(empty(node)) //check to see if list is empty
    {
		node = (card_t *) malloc(sizeof(card_t));
		
		if(empty(node)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			node->type = (char *) malloc(sizeof(char) * 20);
			
			if(node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}
			
			strcpy(node->type, name);
			node->rank = cardrank;
			node->nextptr = NULL; //must make new tail nextptr NULL!!!
			
			return node;  //terminate
		}
      
    }
	
	//here we know that the list has at least one node
	
	card_t *head = node; //keep pointer to head since we will need to return this address
	
	while(node->nextptr != NULL) //traverse to tail
		node = node->nextptr;
	
	node->nextptr = (card_t *) malloc(sizeof(card_t)); 
	
	if(node->nextptr == NULL) //see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; //terminate if tail didn't get created
	}
	
	//populate new node
	node->nextptr->type = (char *) malloc(sizeof(char) * 20);
	
	if(node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}
	
	strcpy(node->nextptr->type, name);
	node->nextptr->rank = cardrank;
	node->nextptr->nextptr = NULL; //very important
			
	return head; //return head node since we need our starting point of the linked list
}

int empty(card_t * node)
{
	return (node == NULL); //return condition result
}

void rules()
{
    printf("Welcome to the card game war!\n");
    printf("Here are the rules.\n");
    printf("You have a pile of cards and I have a pile of cards.\n");
    printf("We will each pull the top card off of our respective deck and compare them.\n");
    printf("The card with the highest number will win the round and take both cards.\n");
    printf("However if there is a tie, then we have to we have to place one card faced down and the next one faced up and compare the results.\n");
    printf("Winner of the tie, will grab all the cards out. However if it's a tie again, then we repeat the same action.\n");
    printf("Ready? Here we go!\n");
}

