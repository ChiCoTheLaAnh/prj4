#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

int countWordsOfLength(char* filename, int wordSize) { 
    FILE* fptr = fopen(filename, "r"); 
    char word[256];
    int count = 0;
    
    if (fptr == NULL) {
        return -1; // Return -1 if the file could not be opened
    }
    
    while (fscanf(fptr, "%255s", word) == 1) {
        if (strlen(word) == wordSize) {
            count++; // Increment count if word length matches wordSize
        }
    }
    
    fclose(fptr);
    return count;
}

bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    FILE* fptr = fopen(filename, "r");
    char word[256];
    int count = 0;
    
    if (fptr == NULL) {
        return false; // Return false if the file could not be opened
    }
    
    while (fscanf(fptr, "%255s", word) == 1) {
        if (strlen(word) == wordSize) {
            strcpy(words[count], word);
            count++;
        }
    }
    
    fclose(fptr);
    if (count == numWords) {
        return true; // Return true if the number of words matches numWords
    }
    return false;
}

int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    while (loInd <= hiInd) {
        int midInd = loInd + (hiInd - loInd) / 2;
        int cmp = strcmp(words[midInd], aWord);
        if (cmp == 0) {
            return midInd; // Return the index if the word is found
        } else if (cmp < 0) {
            loInd = midInd + 1;
        } else {
            hiInd = midInd - 1;
        }
    }
    return -1; // Return -1 if the word is not found
}

void freeWords(char** words, int numWords) {
    for (int i = 0; i < numWords; i++) {
        free(words[i]); // Free each allocated C-string
    }
    free(words); // Free the array of pointers itself
}

void insertWordAtFront(WordNode** ladder, char* newWord) {
    WordNode* newWordNode = (WordNode*)malloc(sizeof(WordNode));
    if (newWordNode == NULL) {
        return; // Return if memory allocation fails
    }
    newWordNode->myWord = newWord;
    newWordNode->next = *ladder;
    *ladder = newWordNode;
}

int getLadderHeight(WordNode* ladder) {
    WordNode* curr = ladder;
    int height = 0;

    while (curr != NULL) {
        curr = curr->next;
        height++; // Increment height for each node
    }
    return height;
}

WordNode* copyLadder(WordNode* ladder) {
    if (ladder == NULL) {
        return NULL; // Return NULL if the ladder is empty
    }
    WordNode* copied = (WordNode*)malloc(sizeof(WordNode));
    copied->myWord = ladder->myWord;
    copied->next = copyLadder(ladder->next);
    return copied;
}

void freeLadder(WordNode* ladder) {
    WordNode* currentNode = ladder;

    // Traverse and free each node in the ladder
    while (currentNode != NULL) {
        WordNode* nextNode = currentNode->next;
        free(currentNode);  // Free current node
        currentNode = nextNode;  // Move to next node
    }
}

void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    LadderNode* newLadderNode = (LadderNode*)malloc(sizeof(LadderNode));
    newLadderNode->topWord = newLadder;
    newLadderNode->next = NULL;

    if (*list == NULL) {
        *list = newLadderNode; // Set new node as the head if the list is empty
    } else {
        LadderNode* curLadderNode = *list;
        while (curLadderNode->next != NULL) {
            curLadderNode = curLadderNode->next;
        }
        curLadderNode->next = newLadderNode; // Append the new node at the end
    }
}

WordNode* popLadderFromFront(LadderNode** list) {
    if (*list == NULL) {
        return NULL; // Return NULL if the list is empty
    } else {
        LadderNode* tempLadderNode = *list;
        WordNode* popWordNode = tempLadderNode->topWord;
        *list = (*list)->next;
        free(tempLadderNode); // Free the removed node
        return popWordNode;
    }
}

void freeLadderList(LadderNode* myList) {
    LadderNode* currentLadder = myList;

    // Traverse through each ladder 
    while (currentLadder != NULL) {
        // Free each WordNode 
        freeLadder(currentLadder->topWord);

        // Store the next ladder before freeing the current one
        LadderNode* nextLadder = currentLadder->next;
        free(currentLadder);  // Free the current LadderNode

        // Move to next ladder in the list
        currentLadder = nextLadder;
    }
}

bool isNeighbor(char* word1, char* word2, int wordSize) {
    int dif = 0;
    for (int i = 0; i < wordSize; i++) {
        if (word1[i] != word2[i]) {
            dif++;
        }
        if (dif > 1) {
            return false; // Return false if more than one difference is found
        }
    }
    return dif == 1; // Return true if exactly one difference is found
}

WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    LadderNode* queueLadder = NULL;
    WordNode* ladder = NULL;
    
    // Insert the start word at the front of the ladder
    insertWordAtFront(&ladder, startWord);
    
    // Insert the ladder into the back of the queue
    insertLadderAtBack(&queueLadder, ladder);
    
    // Process the queue until it's empty
    while (queueLadder) {
        // Pop the ladder from the front of the queue
        ladder = popLadderFromFront(&queueLadder);
        char* currentWord = ladder->myWord;
        
        // Check if the current word is the final word
        if (strcmp(currentWord, finalWord) == 0) {
            // Free the remaining ladders in the queue
            freeLadderList(queueLadder);
            return ladder;
        }
        
        // Iterate through all words to find the neighbors
        for (int i = 0; i < numWords; i++) {
            if (!usedWord[i] && isNeighbor(currentWord, words[i], wordSize)) {
                usedWord[i] = true; // Mark the word as used
                WordNode* newLadder = copyLadder(ladder); // Copy the current ladder
                insertWordAtFront(&newLadder, words[i]); // Insert the new word at the front of the new ladder
                insertLadderAtBack(&queueLadder, newLadder); // Insert the new ladder into the back of the queue
            }
        }
        // Free the current ladder
        freeLadder(ladder);
    }
    return NULL; // Return NULL if no ladder is found
}

// interactive user-input to set a word; 
//  ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}


//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main()
//-----------------------------------------------------
int main() {
    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
