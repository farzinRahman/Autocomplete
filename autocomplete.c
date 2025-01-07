#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"

int compare_str(const void *p_a, const void *p_b) {
    // p_a and p_b are pointers to two structures
    term* term1 = (term*)p_a;
    term* term2 = (term*)p_b;
    return strcmp(term1->term, term2->term);
}

int compare_ints(const void* p_a, const void* p_b) {
    // p_a and p_b are pointers to two structures
    term* term1 = (term*)p_a;
    term* term2 = (term*)p_b;
    double res = term2->weight - term1->weight;

    if (res > 0.0) {
        return 1;
    } else if (res < 0.0) {
        return -1;
    } else {
        return 0;
    }

}


void read_in_terms(struct term** terms, int* pnterms, char* filename) {
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        return;
    }

    char temp[200];
    fgets(temp, 200, fp);
    temp[strlen(temp) - 1] = '\0';

    // Store the number of terms in *pnterms
    *pnterms = atoi(temp);

    // Allocate memory for all the terms in the file
    // Store a pointer to this block in *terms
    *terms = (term*)malloc((*pnterms) * sizeof(term));

    // Read all the terms from filename
    // Place the terms in the block *terms is pointing to
    for (int i = 0; i < *pnterms; i++) {

        fgets(((*terms) + i)->term, 200, fp);
        (((*terms) + i)->term)[strlen(((*terms) + i)->term) - 1] = '\0';

        char temp[200];

        int j = 1;
        while ( (((*terms) + i)->term)[j] != '\t' ) {
            temp[j-1] = (((*terms) + i)->term)[j];
            j++;
        }

        temp[j-1] = '\0';
        ((*terms) + i)->weight = atof(temp);

        // j is where the tab is, right before the string starts
        int k = 0;
        j++;    // j is now the index where the string starts
        // str[k] = str[j+k];

        while ( (((*terms) + i)->term)[k] != '\0' ) {
            (((*terms) + i)->term)[k] = (((*terms) + i)->term)[j + k];
            k++;
        }
        (((*terms) + i)->term)[k] = '\0';

    }

    fclose(fp);

    // Sort the terms in ascending lexicographic order
    qsort(*terms, *pnterms, sizeof(term), compare_str);

}

int lowest_match(struct term* terms, int nterms, char* substr) {
    // Return the index of the first term in the terms block that matches the substr
    // Must run in O(log(nterms)) time
    // implement binary search - locate the leftmost term

    int low = 0;
    int high = nterms - 1;
    int mid;
    char temp[200];
    char temp2[200];

    while (low <= high) {
        mid = (high + low) / 2;
        strcpy(temp, (terms + mid)->term);
        temp[strlen(substr)] = '\0';

        if (strcmp(substr, temp) == 0) { // found an exact match

            strcpy(temp2, (terms + mid - 1)->term);
            temp2[strlen(substr)] = '\0';
            if (strcmp(substr, temp2) == 0) {
                high = mid - 1;
            } else {
                return mid;
            }

        } else if (strcmp(substr, temp) < 0) { // substr in first half of the arr
            high = mid - 1;
        } else if (strcmp(substr, temp) > 0) { // substr in second half of the arr
            low = mid + 1;
        }

    }

    return 0;
}


int highest_match(struct term* terms, int nterms, char* substr) {
    // Return the index of the last term in the terms block that matches the substr
    // Must run in O(log(nterms)) time
    // implement binary search - locate the rightmost term

    int low = 0;
    int high = nterms - 1;
    int mid;
    char temp[200];
    char temp2[200];

    while (low <= high) {   // less than 2
        mid = (high + low) / 2;
        strcpy(temp, (terms + mid)->term);
        temp[strlen(substr)] = '\0';

        if (strcmp(substr, temp) == 0) { // found an exact match

            strcpy(temp2, (terms + mid + 1)->term);
            temp2[strlen(substr)] = '\0';
            if (strcmp(substr, temp2) == 0) {
                low = mid + 1;
            } else {
                return mid;
            }

        } else if (strcmp(substr, temp) < 0) { // substr in first half of the arr
            high = mid - 1;
        } else if (strcmp(substr, temp) > 0) { // substr in second half of the arr
            low = mid + 1;
        }

    }


    return 0;
}

void autocomplete(struct term** answer, int* n_answer, struct term* terms, int nterms, char* substr) {

    int low = lowest_match(terms, nterms, substr);
    int high = highest_match(terms, nterms, substr);

    // Store the number of answers in *n_answer
    *n_answer = (high - low) + 1;   // number of answers
    *answer = (term*)malloc(*n_answer * sizeof(term));

    // Place the answers in the memory block *answer is pointing to
    for (int i = 0; i < *n_answer; i++) {
        ((*answer) + i)->weight = (terms + i + low)->weight;
        strcpy(((*answer) + i)->term, (terms + i + low)->term);
    }

    // Use qsort to sort the answers by weight in non-increasing order
    qsort(*answer, *n_answer, sizeof(term), compare_ints);


}













































