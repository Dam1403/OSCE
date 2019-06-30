#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct Node{
	int num;
	struct Node *next;	
}Node;

Node * gen_collatz(int start_num)
{

	Node* head_node = (Node*) malloc(sizeof(Node));

	Node* curr_node = head_node;
	curr_node->num = start_num;


	int curr_num = 0;	
	while(curr_node->num != 1)
	{
		
		if(curr_node->num % 2 == 0)
		{
			curr_num = curr_node->num / 2;
	
		}
		else
		{
			curr_num = (3 * curr_node->num) + 1;	
		}
	
		Node* new_node = malloc(sizeof(Node));
		new_node->num = curr_num;
		new_node->next = NULL;

		curr_node->next = new_node;
		curr_node = new_node;
	}
	return head_node;

	
}


int main(){
	char number[10];
	int start_num = 0;

	printf("Enter Starting Number: ");
	fgets(number,10,stdin);
	start_num = atoi(number);
	//printf("%d\n",start_num);

	int pid = fork();
	if(pid == 0){
		wait();
		printf("\n Main Process Exiting :D\n");
	}
	else
	{
		Node* head_node = gen_collatz(start_num);
		Node* curr_node = head_node;
		Node* to_free = NULL;	
		printf("Output: ");
		while(curr_node != NULL)
		{
			printf("%d, ",curr_node->num);
			to_free = curr_node;
			curr_node = curr_node->next;
			free(to_free);
		}
	}

}
