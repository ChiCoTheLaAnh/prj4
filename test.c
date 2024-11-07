#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

int countWordsOfLength(char* filename, int wordSize); 
bool buildWordArray(char* filename, char** words, int numWords, int wordSize); 
int findWord(char** words, char* aWord, int loInd, int hiInd); 
void insertWordAtFront(WordNode** ladder, char* newWord);
int getLadderHeight(WordNode* ladder);
WordNode* copyLadder(WordNode* ladder);
void insertLadderAtBack(LadderNode** list, WordNode* newLadder);
void freeLadder(WordNode* ladder);
WordNode* popLadderFromFront(LadderNode** list);
WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord );
void freeWords(char** words, int numWords);

bool test_countWordsOfLength() {
    int result1 = countWordsOfLength("simple3.txt", 3);
    if (result1 != 26) {
        printf("Test 1 Failed: Expected 26, but got %d\n", result1);
        return false;
    } else {
        printf("Test 1 Passed: Expected 26, got %d\n", result1);
    }

    int result2 = countWordsOfLength("abcde.txt", 3);
    if (result2 != -1) {
        printf("Test 2 Failed: Expected -1, but got %d\n", result2);
        return false;
    } else {
        printf("Test 2 Passed: Expected -1, got %d\n", result2);
    }

    int result3 = countWordsOfLength("simple4.txt", 40);
    if (result3 != 0) {
        printf("Test 3 Failed: Expected 0, but got %d\n", result3);
        return false;
    } else {
        printf("Test 3 Passed: Expected 0, got %d\n", result3);
    }

    return true;
}
bool test_buildWordArray() {
    int numWords = 26;
    int wordSize = 3;
    int wrongNumWords = 27;
    char* words[numWords];
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }
    int result1 = buildWordArray("simple3.txt",words, numWords, wordSize);
    if(!result1){
        printf("Expected: true");
        return false;
    }
    
    int result3 = buildWordArray("simple2.txt", words, wrongNumWords, wordSize);
    if(result3){
        return false;
    }
    return true;
}

bool test_findWord() {
    char* words[7] = {"apple", "bug", "codedata", "ear", "hope", "tap", "top"};
    char* aWord = "bug";
    char* aWord2 = "hello";
    
    int result1 = findWord(words, aWord, 0, 6);
    int result2 = findWord(words, aWord2, 0, 6);
    
    // Check result for finding "bug"
    if (result1 != 1) {
        printf("Test 1 Failed: Expected 1 (found 'bug'), but got %d\n", result1);
        return false;
    } else {
        printf("Test 1 Passed: Expected 1, got %d\n", result1);
    }
    
    // Check result for finding "hello"
    if (result2 != -1) {
        printf("Test 2 Failed: Expected -1 (not found 'hello'), but got %d\n", result2);
        return false;
    } else {
        printf("Test 2 Passed: Expected -1, got %d\n", result2);
    }
    
    return true;
}
bool test_insertWordAtFront() {
    WordNode* chain = NULL;

    // Test inserting the first word
    char* newWord1 = "apple";
    insertWordAtFront(&chain, newWord1);

    if (chain == NULL || strcmp(chain->myWord, newWord1) != 0) {
        printf("Test 1 Failed: Expected first word to be '%s', but got '%s'\n", newWord1, chain ? chain->myWord : "NULL");
        return false;
    } else {
        printf("Test 1 Passed: First word is '%s'\n", chain->myWord);
    }

    // Test inserting a second word
    char* newWord2 = "banana";
    insertWordAtFront(&chain, newWord2);

    if (chain == NULL || strcmp(chain->myWord, newWord2) != 0) {
        printf("Test 2 Failed: Expected first word to be '%s', but got '%s'\n", newWord2, chain ? chain->myWord : "NULL");
        return false;
    } else {
        printf("Test 2 Passed: First word is '%s'\n", chain->myWord);
    }

    // Check if the second node has the first word
    if (chain->next == NULL || strcmp(chain->next->myWord, newWord1) != 0) {
        printf("Test 3 Failed: Expected second word to be '%s', but got '%s'\n", newWord1, (chain->next) ? chain->next->myWord : "NULL");
        return false;
    } else {
        printf("Test 3 Passed: Second word is '%s'\n", chain->next->myWord);
    }

    // Free allocated memory for the test
    WordNode* current = chain;
    while (current != NULL) {
        WordNode* temp = current;
        current = current->next;
        free(temp);
    }

    return true;
}

bool test_getLadderHeight() {
    WordNode* ladder = NULL;

    // Test 1: Empty ladder should have a height of 0
    if (getLadderHeight(ladder) != 0) {
        printf("Test 1 Failed: Expected ladder height 0, but got %d\n", getLadderHeight(ladder));
        return false;
    } else {
        printf("Test 1 Passed: Ladder height is 0\n");
    }

    // Test 2: Ladder with one word should have a height of 1
    insertWordAtFront(&ladder, "hi");
    if (getLadderHeight(ladder) != 1) {
        printf("Test 2 Failed: Expected ladder height 1, but got %d\n", getLadderHeight(ladder));
        return false;
    } else {
        printf("Test 2 Passed: Ladder height is 1\n");
    }

    // Test 3: Ladder with two words should have a height of 2
    insertWordAtFront(&ladder, "lo");
    if (getLadderHeight(ladder) != 2) {
        printf("Test 3 Failed: Expected ladder height 2, but got %d\n", getLadderHeight(ladder));
        return false;
    } else {
        printf("Test 3 Passed: Ladder height is 2\n");
    }

    // Free the memory to avoid leaks
    WordNode* current = ladder;
    while (current != NULL) {
        WordNode* temp = current;
        current = current->next;
        free(temp);
    }

    return true;
}
bool test_copyLadder() {
    WordNode* ladder = NULL;

    // Test 1: Copying an empty ladder should return NULL
    if (copyLadder(ladder) != NULL) {
        printf("Test 1 Failed: Expected NULL for an empty ladder, but got a non-NULL value.\n");
        return false;
    } else {
        printf("Test 1 Passed: Empty ladder correctly returned NULL.\n");
    }

    // Setup the ladder with some words
    insertWordAtFront(&ladder, "worda");
    insertWordAtFront(&ladder, "wordb");
    insertWordAtFront(&ladder, "wordc");

    // Test 2: Copying a non-empty ladder
    WordNode* copiedLadder = copyLadder(ladder);
    WordNode* originalCurrent = ladder;
    WordNode* copiedCurrent = copiedLadder;

    while (originalCurrent != NULL && copiedCurrent != NULL) {
        // Check if the words match
        if (strcmp(originalCurrent->myWord, copiedCurrent->myWord) != 0) {
            printf("Test 2 Failed: Expected '%s' in copied ladder, but got '%s'.\n", originalCurrent->myWord, copiedCurrent->myWord);
            return false;
        }

        // Check if they are different nodes in memory
        if (originalCurrent == copiedCurrent) {
            printf("Test 2 Failed: Original and copied nodes are pointing to the same memory location.\n");
            return false;
        }

        originalCurrent = originalCurrent->next;
        copiedCurrent = copiedCurrent->next;
    }

    // Check if both lists have the same length
    if (originalCurrent != NULL || copiedCurrent != NULL) {
        printf("Test 2 Failed: Original and copied ladders have different lengths.\n");
        return false;
    } else {
        printf("Test 2 Passed: Ladder was successfully copied with independent nodes.\n");
    }

    // Cleanup the allocated memory for both lists
    originalCurrent = ladder;
    while (originalCurrent != NULL) {
        WordNode* temp = originalCurrent;
        originalCurrent = originalCurrent->next;
        free(temp);
    }

    copiedCurrent = copiedLadder;
    while (copiedCurrent != NULL) {
        WordNode* temp = copiedCurrent;
        copiedCurrent = copiedCurrent->next;
        free(temp);
    }

    return true;
}

bool test_insertLadderAtBack() {
    LadderNode* chain = NULL;
    WordNode* ladderA = NULL;
    WordNode* ladderB = NULL;

    // Insert words in ladderA
    insertWordAtFront(&ladderA, "worda");
    insertWordAtFront(&ladderA, "wordb");

    // Test 1: Insert first ladder (ladderA) at back of an empty chain
    insertLadderAtBack(&chain, ladderA);

    if (chain == NULL || chain->topWord != ladderA) {
        printf("Test 1 Failed: Expected chain's topWord to be ladderA, but got %p\n", (void*)chain->topWord);
        return false;
    } else {
        printf("Test 1 Passed: First ladder (ladderA) inserted correctly at the back.\n");
    }

    // Insert words in ladderB
    insertWordAtFront(&ladderB, "wordc");
    insertWordAtFront(&ladderB, "wordd");

    // Test 2: Insert second ladder (ladderB) at back of the chain
    insertLadderAtBack(&chain, ladderB);

    if (chain->next == NULL || chain->next->topWord != ladderB) {
        printf("Test 2 Failed: Expected chain's next topWord to be ladderB, but got %p\n", (void*)(chain->next ? chain->next->topWord : NULL));
        return false;
    } else {
        printf("Test 2 Passed: Second ladder (ladderB) inserted correctly at the back.\n");
    }

    // Check if ladderB is indeed the last node
    if (chain->next->next != NULL) {
        printf("Test 3 Failed: Expected ladderB to be the last node, but found additional nodes.\n");
        return false;
    } else {
        printf("Test 3 Passed: ladderB is correctly the last node.\n");
    }

    // Free the memory to avoid leaks (cleaning both WordNode and LadderNode chains)
    LadderNode* currentLadder = chain;
    while (currentLadder != NULL) {
        LadderNode* tempLadder = currentLadder;
        currentLadder = currentLadder->next;

        WordNode* currentWord = tempLadder->topWord;
        while (currentWord != NULL) {
            WordNode* tempWord = currentWord;
            currentWord = currentWord->next;
            free(tempWord);
        }
        free(tempLadder);
    }

    return true;
}

bool test_popLadderFromFront() {
    LadderNode* chain = NULL;
    WordNode* ladderA = NULL;
    WordNode* ladderB = NULL;

    // Set up ladderA with words
    insertWordAtFront(&ladderA, "worda");
    insertWordAtFront(&ladderA, "wordb");

    // Insert ladderA and ladderB into the chain
    insertLadderAtBack(&chain, ladderA);
    insertLadderAtBack(&chain, ladderB);

    // Test 1: Pop ladderA from the front
    WordNode* popped = popLadderFromFront(&chain);
    if (popped != ladderA || chain->topWord != ladderB) {
        printf("Test 1 Failed: Expected to pop ladderA, but got %p. Expected new front to be ladderB, but got %p\n",
               (void*)popped, (void*)(chain ? chain->topWord : NULL));
        return false;
    } else {
        printf("Test 1 Passed: Popped ladderA, new front is ladderB.\n");
    }

    // Test 2: Pop ladderB from the front, chain should now be empty
    popped = popLadderFromFront(&chain);
    if (popped != ladderB || chain != NULL) {
        printf("Test 2 Failed: Expected to pop ladderB and chain to be NULL, but got %p for popped and %p for chain\n",
               (void*)popped, (void*)chain);
        return false;
    } else {
        printf("Test 2 Passed: Popped ladderB, chain is now empty.\n");
    }

    return true;
}

bool test_findShortestWordLadder() {
    // Initialize test data
    char* wordList[] = {"apple", "bug", "codedata", "ear", "hope", "tap", "top"};
    bool wordUsage[7] = {false};
    int totalWords = 7;
    int wordLength = 3;
    char start[] = "tap";
    char end[] = "top";

    // Expected ladder sequence from start to end
    char* expectedLadder[] = {"top", "tap"};

    // Call the function to test
    WordNode* ladder = findShortestWordLadder(wordList, wordUsage, totalWords, wordLength, start, end);
    
    // Check if ladder is NULL
    if (ladder == NULL) {
        printf("Test failed: findShortestWordLadder returned NULL\n");
        return false;
    }

    // Verify that the returned ladder matches the expected sequence
    WordNode* current = ladder;
    int index = 0;
    while (current != NULL && index < 2) {
        if (strcmp(current->myWord, expectedLadder[index]) != 0) {
            printf("Test failed: expected %s, but got %s\n", expectedLadder[index], current->myWord);
            freeLadder(ladder);
            return false;
        }
        current = current->next;
        index++;
    }

    // Final validation: check if we have exactly two words in the ladder
    if (current != NULL || index != 2) {
        printf("Test failed: ladder length does not match expected length\n");
        freeLadder(ladder);
        return false;
    }

    // Free allocated memory for the ladder
    freeLadder(ladder);
    printf("Test passed: findShortestWordLadder\n");
    return true;
}



// int main() {
//     if (test_countWordsOfLength()) {
//         printf("test_countWordsOfLength passed.\n");
//     } else {
//         printf("test_countWordsOfLength failed.\n");
//     }

//     if (test_buildWordArray()) {
//         printf("test_buildWordArray passed.\n");
//     } else {
//         printf("test_buildWordArray failed.\n");
//     }

//     if (test_findWord()) {
//         printf("test_findWord passed.\n");
//     } else {
//         printf("test_findWord failed.\n");
//     }

//     if (test_insertWordAtFront()) {
//         printf("test_insertWordAtFront passed.\n");
//     } else {
//         printf("test_insertWordAtFront failed.\n");
//     }
    
    
//     if (test_getLadderHeight()) {
//         printf("test_getLadderHeight passed.\n");
//     } else {
//         printf("test_getLadderHeight failed.\n");
//     }
    
//     if (test_copyLadder()) {
//         printf("test_copyLadder passed.\n");
//     } else {
//         printf("test_copyLadder failed.\n");
//     }

//     if (test_insertLadderAtBack()) {
//         printf("test_insertLadderAtBack passed.\n");
//     } else {
//         printf("test_insertLadderAtBack failed.\n");
//     }

//     if (test_popLadderFromFront()) {
//         printf("test_popLadderFromFront passed.\n");
//     } else {
//         printf("test_popLadderFromFront failed.\n");
//     }
//     if (test_findShortestWordLadder()) {
//         printf("test_findShortestWordLadder passed.\n");
//     } else {
//         printf("test_findShortestWordLadder failed.\n");
//     }
//     return 0;
// }