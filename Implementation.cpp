/*
File: Implementation.cpp
Author: Mr Arthor
Procedures:

-uniform - provides a random uniform number
-scan - simulates the scan search policy
-cscan - simulates the cscan search policy
-fifo - simulates the first in first out policy
-sstf - simulates the sstf policy
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstddef>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

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

    for (int i = 0; i < requests; i++) 
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

    for (int i = 0; i < size1; i++)
    {
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
            tracksTraversed[i + size1] = tempTrack2[i + 1] - tempTrack2[i]; 

            for (int y = 0; y < requestedSectors[i + size1]; y++) {
                sectorsTraversed[i + size1] = requestedSectors[i] + size1;
            }
        }
    }

    for (int i = 0; i < requests; i++) 
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


int fifo(int requests, int requestedTracks[], int requestedSectors[], int initialHeadLocation)
{
    vector<int> tempTrack(requests);
    vector<int> tracksTraversed(requests);
    vector<int> sectorsTraversed(requests);
    vector<int> seekTime(requests);
    for (int i = 0; i < requests; i++) {
        tempTrack[i] = requestedTracks[i];
        sectorsTraversed[i] = requestedSectors[i];
    }

    for (int i = 0; i < requests; i++) 
    {
        if (i != requests)
        {
            if (tempTrack[i] - (tempTrack[i + 1]) >= 0) 
            {
                tracksTraversed[i] = tempTrack[i] - tempTrack[i + 1]; 
            }
            else if (tempTrack[i + 1] - (tempTrack[i]) >= 0) 
            {
                tracksTraversed[i] = tempTrack[i + 1] - tempTrack[i]; 
            }
        }
    }

    for (int i = 0; i < requests; i++) 
    {
        seekTime[i] = tracksTraversed[i] + sectorsTraversed[i];
    }


    long seekTimeAverage = 0;

    for (int i = 0; i < requests; i++) 
    {
        seekTimeAverage += seekTime[i];
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


int sstf(int requests, int requestedTracks[], int requestedSectors[], int initialHeadLocation)
{
    vector<int> tempTrack(requests);
    vector<int> tempSector(requests);
    int currentDifference = 2500;
    vector<int> tracksTraversed(requests);
    for (int i = 0; i < requests; i++) 
    {
        tempTrack[i] = requestedTracks[i];
        tempSector[i] = requestedSectors[i];
    }

    for (int i = 0; i < requests; i++) 
    {

        currentDifference = 2500;            
        for (int y = i + 1; y < requests; y++) {
            if (y != requests) {
                if ((tempTrack[i] - tempTrack[y]) >= 0)
                {
                    if ((tempTrack[i] - tempTrack[y]) < currentDifference) 
                    {
                        currentDifference = tempTrack[i] - tempTrack[y]; 
                    }
                }
                else if ((tempTrack[y] - tempTrack[i]) >= 0) 
                {
                    if ((tempTrack[y] - tempTrack[i]) < currentDifference)
                    {
                        currentDifference = tempTrack[y] - tempTrack[i]; 
                    }
                }
            }
        }
        tracksTraversed[i] = currentDifference; 
    }

    vector<int> seekTime(requests);

    for (int i = 0; i < requests; i++)
    {
        seekTime[i] = tracksTraversed[i] + tempSector[i]; 
    }

    long seekTimeAverage = 0;

    for (int i = 0; i < requests; i++)
    {
        seekTimeAverage += seekTime[i];}

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