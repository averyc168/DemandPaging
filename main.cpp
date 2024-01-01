#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <queue>

using namespace std;

class scheduler {
    int pgNum;  // number of available physical pages; # of rows for the scheduler; inputted by user
    int pgRef;  // number of unique page references; inputted by user
    int pgRefNum; // amount of unique page references; # of cols for the scheduler; inputted by user

    // struct that has more info on each page that is put into the scheduler
    struct pageInfo {
        char letter;    // the letter or "name" of the page;
        int count;      // the count of that letter or "name";
    };

    // data structure that keep track of what is happening to schedule
    pageInfo *pageArray = nullptr;   // array of counts for each unique page reference, going to be size of pgNum

    // data structures keeping track of what is happening to REFERENCE STREAM
    queue<char> pgRefQueue; // queue that has the order of all the unique page references
    string pgRefArray;  // string of unique page references, going to be size of pgRefNum

    int hits; // num of hits

public:
    scheduler(int, int, int);   // DEFAULT CTOR for the scheduler class
    void randomToStr();         // creates a random reference stream string
    int toSchedule(int);        // schedules the reference string to get the number of cache hits
};

/**
 * DEFAULT CTOR for the scheduler class, acts like a second main to calculate cache hits and get
 * hit ratio
 * @param physicalPages - the user inputted number for the number of physical pages they want (3-5)
 * @param pageRef - the user inputted number for the number of unique page references (5-7)
 * @param totPageRef - the user inputted number for the length of the reference stream (> 100)
 *
 * @return: the number of hits should be recorded, as well as the items are "scheduled", and with the
 * number of hits, we can calculate the hit ratio
 */
scheduler :: scheduler(int physicalPages, int pageRef, int totPageRef) {
    pgNum = physicalPages;
    pgRef = pageRef;
    pgRefNum = totPageRef;

    pgRefArray = "";
    pageArray = new pageInfo[pgNum];    // creating an array to hold the counts
    hits = 0;
    randomToStr();

    cout << endl << "Reference Stream is: " << pgRefArray << endl << endl;

    // fills each of the pages with blanks
    for (int i = 0; i < pgNum; i++) {
        pageArray[i].letter = '-';
    }

    // this starts off the scheduler by filling up the necessary starting pages
    int filled = 0; // iterator to keep track of how much of the inital pageArray is filled before we start scheduling
    int streamIter = 0; // iterator to see how far in the reference stream we are
    while (filled < pgNum && streamIter < pgRefNum) {
        bool replace = true; // bool value that should see if the value in the array should be replaced
        for (int i = 0; i < pgNum; i++) {
            if (pageArray[i].letter == pgRefArray[streamIter]) {
                hits++;
                pageArray[i].count++;
                replace = false;
            }
        }
        if (replace) {
            int iter = 0;   // iterator for the physical pages in pageArray
            while (replace && iter < pgNum) {
                if (pageArray[iter].letter == '-') {
                    pageArray[iter].letter = pgRefArray[streamIter];
                    pageArray[iter].count = 1;
                    pgRefQueue.push(pageArray[iter].letter);
                    //cout << "front is: " << pgRefQueue.back() << endl;
                    replace = false;
                    filled++;
                }
                iter++;
            }
        }
        streamIter++;
    }

    // calculates the hit ratio
    hits += toSchedule(streamIter);
    cout << "Number of hits: " << hits << endl;
    float hitRatio = float(hits) / float(pgRefNum);
    printf("The hit ratio is: %.2f\n", hitRatio);

    delete pageArray;
}

/**
 * to schedule function that schedules all of the pages in the reference stream and gets number of
 * cache hits
 * @param streamIter - the starting index of the reference string left off after initializing the pageArray
 * @return an int that has the number of cache hits
 */
int scheduler ::toSchedule(int streamIter) {
    int numOfHits = 0;  // variable to keep track of the number of cache hits
    int i = streamIter; // new variable to index

    // while loop that schedules all the page references in an application
    while (i < pgRefNum) {
        bool replace = true;    // see if an element in pageArray needs to be replaced
        for (int j = 0; j < pgNum; j++) {
            if (pgRefArray[i] == pageArray[j].letter) {
                numOfHits++;
                pageArray[j].count++;
                replace = false;
            }
        } // endFor

        // if we are replacing something in the pageArray
        if (replace) {
            bool noCount = true;    // bool value to see if none of the counts are past 3
            int k = 0;      // iterator to pageArray; number of physical pages
            while (noCount && k < pgNum) {
                if (pageArray[k].count >= 3) {
                    noCount = false; // will go to false once it sees a count of 3 or more
                    // if the unique page reference is at the front of the queue
                    if (pageArray[k].letter == pgRefQueue.front()) {
                        pgRefQueue.pop();
                    }
                    // if the unique page reference is not at the front of the queue
                    else {
                        char firstLetter = pgRefQueue.front();  // the first letter in the queue
                        pgRefQueue.pop();
                        pgRefQueue.push(firstLetter);

                        // circulates the queue until it reaches the front, in case the unique page reference
                        // is in the middle of the queue
                        while (firstLetter != pgRefQueue.front()) {
                            if (pgRefQueue.front() == pageArray[k].letter) {
                                pgRefQueue.pop();
                            }
                            else {
                                char letter = pgRefQueue.front();
                                pgRefQueue.pop();
                                pgRefQueue.push(letter);
                            }
                        } // end while
                    }
                    pgRefQueue.push(pgRefArray[i]);
                    pageArray[k].letter = pgRefQueue.back();
                    pageArray[k].count = 1;
                } // IF there are more than three occurrences
                k++;
            } // endFor

            // if none of them exceed 3, then it is just FIFO here
            if (noCount) {
                char firstLetter = pgRefQueue.front();  // the first letter in the queue
                pgRefQueue.pop();
                for (int m = 0; m < pgNum; m++) {
                    if (firstLetter == pageArray[m].letter) {
                        pageArray[m].letter = pgRefArray[i];
                        pageArray[m].count = 1;
                        pgRefQueue.push(pgRefArray[i]);
                    }
                } // endFor

            } // if noCount

        } // if replace

        i++;
    } // end while

    return numOfHits;
}

/**
 * randomToStr function creates a random number from 1 to 7 and appends to the string the letter that
 * corresponds to the random int generated
 *  (A == 1)
 *  (B == 2)
 *  ...
 *  (G == 7)
 * @param - n/a but the pgRefArray should be initialized to a blank string
 * @return - no real return values, but the string should be a user inputted elements long for how long
 * they want their reference stream to be, with different letters also inputted by the user
 */
void scheduler ::randomToStr() {
    srand(time(0));
    for (int i = 0; i < pgRefNum; i++) {
        int randChar = (rand() % pgRef) + 1;
        if (randChar == 1) {
            pgRefArray += 'A';
        }
        else if (randChar == 2) {
            pgRefArray += 'B';
        }
        else if (randChar == 3) {
            pgRefArray += 'C';
        }
        else if (randChar == 4) {
            pgRefArray += 'D';
        }
        else if (randChar == 5) {
            pgRefArray += 'E';
        }
        else if (randChar == 6) {
            pgRefArray += 'F';
        }
        else if (randChar == 7) {
            pgRefArray += 'G';
        }
    }
}

int main(int argc, char ** argv) {
    int physicalPages = stoi(argv[1]); // number of physical pages in scheduler
    int pageRef = stoi(argv[2]);    // number of unique page references for application
    int totPageRef = stoi(argv[3]); // length of the reference stream

    scheduler schedule(physicalPages, pageRef, totPageRef);

    return 0;
}
