#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MOVIENUMBER 15000
#define MAX 170000
#define TABLESIZE 200000
typedef struct queue Q;
void put(int, int, int, int);
Q get();
int rear = -1;
int front = -1;
int addActor(char*);

typedef struct movie M;
typedef struct actor A;


struct queue {
	int value, count, previous,movie;
};
Q queue_array[170000];


struct movie {
	char name[50];
	bool marked;
	int neighbors[250], count;
};

struct actor {
	char name[30];
	bool marked;
	int neighbors[100], count;
};

int moviecount = 0, actorcount = 0;
M movies[MOVIENUMBER];
A actors[TABLESIZE];

void checkNeighborhood(char* actor, char* actor2) {
	int i, j, k, a, count = 0;
	int index = addActor(actor);
	int index2 = addActor(actor2);
	int movs[10],path[10];
	Q q;
	bool flag = true;
	put(index, count, -1,-1);
	while (flag) {
		//is queue empty?
		if (front == -1 || front > rear) {
			flag = false;
			printf("bu iki oyuncu arasinda bag yok\n");
		}
		else {
			q = get();
			index = q.value;
			count = q.count;
			int prev = q.previous;
			int mov = q.movie;
			actors[index].marked = true;

			if (index == index2) {
				a = count;
				printf("distance between %s and %s: %d\n",actor, actor2, count);
				path[count] = index;
				movs[count] = mov;
				count--;
				//
				while (count >= 0) {
					for (k = 0; k <= rear; k++) {
						if (prev == queue_array[k].value) {
							path[count] = prev;
							movs[count] = queue_array[k].movie;
							prev = queue_array[k].previous;
							count--;
							break;
						}
					}
				}
				
				for (k = a; k >= 1; k--) {
					printf("%s - %s '%s'\n", actors[path[k]].name,actors[path[k-1]].name,movies[movs[k]].name);
				}
				flag = false;
			}
			count++;
			for (i = 0; i < actors[index].count && flag; i++)
				for (j = 0; j < movies[actors[index].neighbors[i]].count && j <= 250; j++) { 
					a = movies[actors[index].neighbors[i]].neighbors[j];

					if (actors[a].marked == false) {
						if (count < 7)
							put(a, count, index, actors[index].neighbors[i]);
						actors[a].marked = true;
					}
				}
		}
	}
}

int int_pow(int base, int exp) {
	int result = 1;
	while (exp){
		if (exp & 1)
			result *= base;
		exp /= 2;
		base *= base;
	}
	return result;
}


int addMovie(char name[20]) {
	strcpy(movies[moviecount].name, name);
	movies[moviecount].marked = false;
	movies[moviecount].count = 0;
	moviecount++;
	return moviecount - 1;
}

int addActor(char name[20]) {
	int collusion_count = -1, size, i, power, key = 0, akey;
	size = strlen(name);
	for (i = 0; i < size; i++) {
		key += (int)name[i] * 1;
	}
	do {
		collusion_count++;
		akey = ((key % TABLESIZE) + int_pow(collusion_count, 2)) % TABLESIZE;
	} while (strlen(actors[akey].name) > 0 && strcmp(name, actors[akey].name) != 0);
	if (strlen(actors[akey].name) <= 0) {
		strcpy(actors[akey].name, name);
		actors[akey].count = 0;
		actors[akey].marked = false;
		actorcount++;
	}
	return akey;
}

int main() {
	int k;
	FILE* fp = fopen("input-mpaa.txt", "r");
	char buff[10000], * name;
	int i = 0, movieindex, actorindex;
	if (fp == NULL)
		return 1;
	while (fgets(buff, sizeof(buff), fp) != NULL && strlen(buff) > 1) {
		name = strtok(buff, "/");
		movieindex = addMovie(name);
		while (name != NULL) {
			name = strtok(NULL, "/");
			if (name != NULL) {
				if (name[strlen(name) - 1] == '\n')
					name[strlen(name) - 1] = '\0';
				actorindex = addActor(name);
				actors[actorindex].neighbors[actors[actorindex].count] = movieindex;
				actors[actorindex].count++;
				movies[movieindex].neighbors[movies[movieindex].count] = actorindex;
				movies[movieindex].count++;
			}
		}
		if (i % 1000 == 0)
			printf("%d movies saved\n", i);
		i++;
	}
	printf("total # of actors : %d\n", actorcount);
	//checkNeighborhood("Bacon, Kevin", "Allen, Todd");
	//checkNeighborhood("Reno, Kelly", "Scott, Ivan");
	//checkNeighborhood("Bacon, Kevin", "Schwarzenegger, Arnold");
	//checkNeighborhood("Nicholson, Jack", "Streep, Meryl");
	//checkNeighborhood("Allen, Woody", "Goodman, John");
	checkNeighborhood("Brent, Eve", "Sokoloski, Magdalena");
	//checkNeighborhood("Wilder, Scott", "Rose, Al");
	//checkNeighborhood("A", "Trujillo, Raoul");
	
	
	printf("\n");
	return 0;
}

void put(int item, int count, int previous,int movie) {
	if (rear == MAX - 1)
		printf("Queue Overflow \n");
	else {
		if (front == -1)
			front = 0;
		rear = rear + 1;
		queue_array[rear].value = item;
		queue_array[rear].count = count;
		queue_array[rear].previous = previous;
		queue_array[rear].movie = movie;
	}
}

Q get() {
	Q item;
	if (front == -1 || front > rear) {
		printf("Queue Underflow \n");
		//item = -1;
		return;
	}
	else {
		item = queue_array[front];
		front++;
		return item;
	}
	//return item;
}




