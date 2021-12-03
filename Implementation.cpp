/*
File: main.cc
Author: Jorge Gonzalez
Procedures:

-uniform - provides a random uniform number
-scan - simulates the scan search policy
-cscan - simulates the cscan search policy
-fifo - simulates the first in first out policy
-sstf - simulates the sstf policy
 */

//libraries required for the program
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstddef>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

//prototype functions
int uniform(int, int);
int scan(int, int[], int[], int);
int cscan(int, int[], int[], int);
int fifo(int, int[], int[], int);
int sstf(int, int[], int[], int);

int main()
{
    srand(time(NULL));       
    int initialHeadLocation = 5000 / 2; 
    int scanSeekTimeAverage = 0, cscanSeekTimeAverage = 0;
    int fifoSeekTimeAverage = 0, sstfSeekTimeAverage = 0;

    for (int i = 0; i < 10; i++)
    {
        int requests = uniform(500, 1000); 
        int requestedTracks[requests];
        int requestedSectors[requests];
        for (int y = 0; y < requests; y++)
        {
            requestedTracks[y] = uniform(0, 5000);   
            requestedSectors[y] = uniform(0, 12000); 
        }
        
        cout << "Number of Requests: " << requests << endl;

        scanSeekTimeAverage += scan(requests, requestedTracks, requestedSectors, initialHeadLocation);
        cout << "Scan Avg Seek " << i + 1 << " :" << scan(requests, requestedTracks, requestedSectors, initialHeadLocation) << endl;
        cscanSeekTimeAverage += cscan(requests, requestedTracks, requestedSectors, initialHeadLocation);
        cout << "cScan Avg Seek " << i + 1 << " :" << cscan(requests, requestedTracks, requestedSectors, initialHeadLocation) << endl;
        fifoSeekTimeAverage += fifo(requests, requestedTracks, requestedSectors, initialHeadLocation);
        cout << "Fifo Avg Seek " << i + 1 << " :" << scan(requests, requestedTracks, requestedSectors, initialHeadLocation) << endl;
        sstfSeekTimeAverage += sstf(requests, requestedTracks, requestedSectors, initialHeadLocation);
        cout << "SSTF Avg Seek " << i + 1 << " :" << sstf(requests, requestedTracks, requestedSectors, initialHeadLocation) << endl;

        cout << "------------------------" << endl;
    }

    
    scanSeekTimeAverage = scanSeekTimeAverage / 10;
    cscanSeekTimeAverage = cscanSeekTimeAverage / 10;
    fifoSeekTimeAverage = fifoSeekTimeAverage / 10;
    sstfSeekTimeAverage = sstfSeekTimeAverage / 10;
    cout << "Scan Overall Avg Seek Time: " << scanSeekTimeAverage << endl;
    cout << "cScan Overall Avg Seek Time: " << cscanSeekTimeAverage << endl;
    cout << "Fifo Overall Avg Seek Time: " << fifoSeekTimeAverage << endl;
    cout << "SSTF Overall Avg Seek Time: " << sstfSeekTimeAverage << endl;

    return 0;
}

int uniform(int low, int high)
{
top:
    int x;
    int y = high - low + 1;
    int z = rand() / y;

    if (z == 0)
    {
        goto top;
    }

    while (y <= (x = (rand() / z)))
        ;

    return x + low;
}


int scan(int requests, int requestedTracks[], int requestedSectors[], int initialHeadLocation)
{

    vector<int> tempTrack;
    vector<int> tempTrack2;

    for (int i = 0; i < requests; i++) 
    {
        if (requestedTracks[i] >= initialHeadLocation)
        { 
            tempTrack.push_back(requestedTracks[i]);
        }
        else
        {
            tempTrack2.push_back(requestedTracks[i]);
        }
    }
    int size1 = tempTrack.size();
    int size2 = tempTrack2.size();
    vector<int> mergedTracks(requests); 

    sort(tempTrack.begin(), tempTrack.end());                  
    sort(tempTrack2.begin(), tempTrack2.end(), greater<int>()); 

    for (int i = 0; i < size1; i++)                        
  
    {
        mergedTracks[i] = tempTrack[i];
    }
    int mergeSize = mergedTracks.size();

    int x = 0;
    for (int i = size1; i < mergeSize; i++) 
    {
        mergedTracks[i] = tempTrack2[x];
        x++;
    }
    

    vector<int> tracksTraversed(requests);
    vector<int> sectorsTraversed(requests);
    vector<int> seekTimes(requests);

    for (int i = 0; i < size1; i++) {

        if (i != size1)
        {
            tracksTraversed[i] = tempTrack[i + 1] - tempTrack[i];

            for (int y = 0; y < requestedSectors[i]; y++)
            {
                sectorsTraversed[i] = requestedSectors[i];
            }
        }
    }
    for (int i = 0; i < size2; i++) 
    {
        if (i != size2)
        {
            tracksTraversed[i + size1] = tempTrack2[i] - tempTrack2[i + 1];

            for (int y = 0; y < requestedSectors[i + size1]; y++)
            {
                sectorsTraversed[i + size1] = requestedSectors[i + size1];
            }
        }
    }

    for (int i = 0; i < requests; i++) //
    {

        seekTimes[i] = tracksTraversed[i] + sectorsTraversed[i];
    }
    long seekTimeAverage = 0;

    for (int i = 0; i < requests; i++)
    {
        seekTimeAverage += seekTimes[i];
    }

    if (requests != 0)
    {
        seekTimeAverage = seekTimeAverage / requests;
    }
    else
    {
        cout << "hello" << endl;
    }
    return seekTimeAverage;
}

int cscan(int requests, int requestedTracks[], int requestedSectors[], int initialHeadLocation)
{
    vector<int> tempTrack;
    vector<int> tempTrack2;

    for (int i = 0; i < requests; i++) 
    {
        if (requestedTracks[i] >= initialHeadLocation)
        {
            tempTrack.push_back(requestedTracks[i]);
        }
        else
        {
            tempTrack2.push_back(requestedTracks[i]);
        }
    }

    int size1 = tempTrack.size();
    int size2 = tempTrack2.size();
    vector<int> mergedTracks(requests);

    sort(tempTrack.begin(), tempTrack.end()); 
    sort(tempTrack2.begin(), tempTrack2.end()); 

    for (int i = 0; i < size1; i++) 
    {
        mergedTracks[i] = tempTrack[i];
    }

    int x = 0;
    int mergedSize = mergedTracks.size();

    for (int i = size1; i < mergedSize; i++) 
    {
        mergedTracks[i] = tempTrack2[x];
        x++;
    }

    vector<int> tracksTraversed(requests);
    vector<int> sectorsTraversed(requests);
    vector<int> seekTimes(requests);

    for (int i = 0; i < size1; i++) //loop through the first tempTrack
    {
        if (i != size1)
        {
            tracksTraversed[i] = tempTrack[i + 1] - tempTrack[i]; //will capture the tracks traversed by subtracting the n+1 element by the n element

            for (int y = 0; y < requestedSectors[i]; y++) //sectors are captured as well
            {
                sectorsTraversed[i] = requestedSectors[i];
            }
        }
    }

    for (int i = 0; i < size2; i++) // loop through the second temp track
    {
        if (i != size2)
        {
            tracksTraversed[i + size1] = tempTrack2[i + 1] - tempTrack2[i]; //will capture the tracks traversed by subtracting the n+1 element by the n element

            for (int y = 0; y < requestedSectors[i + size1]; y++) // the sectors are captured as well
            {
                sectorsTraversed[i + size1] = requestedSectors[i] + size1;
            }
        }
    }

    for (int i = 0; i < requests; i++) //each seek time is calculated by track traversed + the sectors traversed
    {
        seekTimes[i] = tracksTraversed[i] + sectorsTraversed[i];
    }

    //  int seekSize = seekTimes.size();

    long seekTimeAverage = 0;

    for (int i = 0; i < requests; i++) //each seek time is summed up
    {
        seekTimeAverage += seekTimes[i];
    }

    if (requests != 0)
    {
        seekTimeAverage = seekTimeAverage / requests; //summed seek time is divided by the number of requests to find the average seek time
    }
    else
    {
        cout << "hello" << endl;
    }
    return seekTimeAverage;
}

/*
int fifo(int requests, int requestedTracks[], int requestedSectors[], int initialHeadLocation)

Author: Jorge Gonzalez

Date: 11/21/2019

Description: simulates the fifo disk algorithm for first in first out, summmed seek time is averaged and returned

Parameters:

requests              I/P   int     number of requests

requestedTracks       I/P   int[]   the requested tracks

requestedSectors      I/P   int[]   the requested sectors

initialHeadLocation   I/P   int     the initial starting point
 */
int fifo(int requests, int requestedTracks[], int requestedSectors[], int initialHeadLocation)
{
    vector<int> tempTrack(requests);
    vector<int> tracksTraversed(requests);
    vector<int> sectorsTraversed(requests);
    vector<int> seekTime(requests);
    for (int i = 0; i < requests; i++) //the requested tracks and sectors are stored in temp arrays
    {
        tempTrack[i] = requestedTracks[i];
        sectorsTraversed[i] = requestedSectors[i];
    }

    for (int i = 0; i < requests; i++) //number of tracks traversed is calculated by if n+1 - n element is a positive number, store than number, if not then n - (n+1) is stored
    {
        if (i != requests)
        {
            if (tempTrack[i] - (tempTrack[i + 1]) >= 0) //checks to see if the difference is positive
            {
                tracksTraversed[i] = tempTrack[i] - tempTrack[i + 1]; //if it is then store the tracks traversed
            }
            else if (tempTrack[i + 1] - (tempTrack[i]) >= 0) //checks to see if the difference is positive
            {
                tracksTraversed[i] = tempTrack[i + 1] - tempTrack[i]; // if it is then store the tracks traversed
            }
        }
    }

    for (int i = 0; i < requests; i++) //seek time is calculated by tracks traversed + sectors traversed
    {
        seekTime[i] = tracksTraversed[i] + sectorsTraversed[i];
    }

    //  int seekSize = seekTime.size();

    long seekTimeAverage = 0;

    for (int i = 0; i < requests; i++) //each seek time is summed up
    {
        seekTimeAverage += seekTime[i];
    }

    if (requests != 0)
    {
        seekTimeAverage = seekTimeAverage / requests; //average is calculated by the summed seek time / number of requests
    }
    else
    {
        cout << "hello" << endl;
    }
    return seekTimeAverage;
}

/*
int sstf(int requests, int requestedTracks[], int requestedSectors[], int initialHeadLocation)

Author: Jorge Gonzalez

Date: 11/21/2019

Description: simulates the sstf disk algorithm and returns the seek time average

Parameters

requests               I/P   int   number of requests

requestedTracks        I/P   int[] the requested tracks

requestedSectors       I/P   int[] the requested sectors

initialHeadLocation    I/P   int   initial starting point
 */
int sstf(int requests, int requestedTracks[], int requestedSectors[], int initialHeadLocation)
{
    vector<int> tempTrack(requests);
    vector<int> tempSector(requests);
    int currentDifference = 2500;
    vector<int> tracksTraversed(requests);
    for (int i = 0; i < requests; i++) //requested sectors and tracks are stored in a temp array
    {
        tempTrack[i] = requestedTracks[i];
        tempSector[i] = requestedSectors[i];
    }

    for (int i = 0; i < requests; i++) //loop through the number of requests, sorting by shortest number of tracks traversed
    {

        currentDifference = 2500;              //this will be the initial difference at the start of every loop
        for (int y = i + 1; y < requests; y++) //compare each element with every other element in the array
        {
            if (y != requests) //checks to see if it reached the end
            {
                if ((tempTrack[i] - tempTrack[y]) >= 0) //checks to see if the difference between elements is positive
                {
                    if ((tempTrack[i] - tempTrack[y]) < currentDifference) //checks to see if the current difference is smaller than the stored difference
                    {
                        currentDifference = tempTrack[i] - tempTrack[y]; //if it is then store it as the new difference
                    }
                }
                else if ((tempTrack[y] - tempTrack[i]) >= 0) //checks to see if the difference between elements is positive
                {
                    if ((tempTrack[y] - tempTrack[i]) < currentDifference) //checks to see if the current difference is smaller than the stored one
                    {
                        currentDifference = tempTrack[y] - tempTrack[i]; //if it is, then it is the new stored difference
                    }
                }
            }
        }
        tracksTraversed[i] = currentDifference; //once each element has been checked, the current difference is stored into the array and restart the process
    }

    vector<int> seekTime(requests);

    for (int i = 0; i < requests; i++)
    {
        seekTime[i] = tracksTraversed[i] + tempSector[i]; //calculate the average seek time by adding track traverse + temp sector
    }

    long seekTimeAverage = 0;

    for (int i = 0; i < requests; i++)
    {
        seekTimeAverage += seekTime[i]; //sum each seek time
    }

    if (requests != 0)
    {
        seekTimeAverage = seekTimeAverage / requests; //calculate the avg by dividing the sum by the number of requests
    }
    else
    {
        cout << "hello" << endl;
    }
    return seekTimeAverage;
}