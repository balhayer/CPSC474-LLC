// CPSC 474 LAMPORT LOGICAL CLOCK CALCULATION ALGORITHM

// Balwinder Hayer
// Saytu Singh

// Execute commands:
//
// g++ -o AlgoCalc AlgoCalc.cpp
// ./AlgoCalc
//
// Linux Platform

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include<fstream>

using namespace std;

struct events {

	// It holds what type of the event it is for example: Internal or External and Receive
	string whatTypeEvent;  
	
	// It holds the calculated clock value
	int calcClockValue=-1; 
};

// Created function known as calcLampClock
// It calculates When we have completed calculating Lamport clock for every Send and Internal events.

void calcLampClock(events tempClockValue[10][10], int n, int m, int LogicalLamportClock[10][10]);

// It matches the send event and return the Lamport Logical clock.

int calcSendEventClock(events tempClockValue[10][10], int n, int m, string a);


bool finalTestAll(events tempClockValue[10][10], int n, int m, int LogicalLamportClock[10][10]);
int main()
{
	// n=0 is the number of processes
	int n=0;

	// m=0 is the maximum number of events
    	int m=0;
	int logicalValue=1;
	events tempClockValue[10][10];
	int LogicalLamportClock[10][10]; //Clock values

    	// it asks for input for all the processes
	// enter how many number of processes
	cout << "Enter # of Processes: ";

	// holds the number of processes
	cin >> n;

	// used for loop to run it 'n' times to capture number of events per process
	for (int a = 0; a < n; a++)
	{
		cout << "Enter # of Events per process p" << a+1 << ":";
		cin >> m;

		for (int y = 0; y < m; y++)
		{
			cout << "p" << a+1 << ":";
			cin >> tempClockValue[a][y].whatTypeEvent;
		}
	}

    // it holds the computational calculations performed by the algorithm
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			// It checks if the event is internal or send event
			if (tempClockValue[i][j].whatTypeEvent.length()==1 || tempClockValue[i][j].whatTypeEvent[0]=='s') 
			{
				// j == 0 which verifies if the first occured event arrived at processor
				if (j == 0)
				{
					// It assigns the logical lamport clock equals to one or = 1 for every first occured event
					tempClockValue[i][j].calcClockValue = 1; 
					LogicalLamportClock[i][j] = tempClockValue[i][j].calcClockValue;
				}
				else if(tempClockValue[i][j-1].calcClockValue==-1)
                {
                    tempClockValue[i][j].calcClockValue=-1;
                    // lamport clock assignes to minus one = -1
                }
				else
				{
					logicalValue = tempClockValue[i][j - 1].calcClockValue + 1;
					
					// it holds and copies the value for an event in tempClockValue
					tempClockValue[i][j].calcClockValue = logicalValue; 
					LogicalLamportClock[i][j] = logicalValue;
					logicalValue++;
				}
			}

			// it checks whether it belongs to receive state 
			else if (tempClockValue[i][j].whatTypeEvent.length() > 1 && tempClockValue[i][j].whatTypeEvent[0]=='r') 
			{
				// j == 0 verifies whether the first occured event at processor
				if (j == 0) 
				{
					// 'k' returns the clock value of send event
					int k = calcSendEventClock(tempClockValue, n, m, tempClockValue[i][j].whatTypeEvent); 
					if (k == -1)
					{
						// it tells us that tempClockValue waits for the send event
						tempClockValue[i][j].calcClockValue = k; 
					}
					else
					{
						tempClockValue[i][j].calcClockValue = k + 1;
						LogicalLamportClock[i][j] = k + 1;
						logicalValue++;
					}
				}
				else
				{
					// it calculates the event send of logical lamport value
					int eventSend = calcSendEventClock(tempClockValue, n, m, tempClockValue[i][j].whatTypeEvent);
					if (eventSend >0)
					{
						// the temporary event which occured just before the receive event in the processor
						int k = tempClockValue[i][j - 1].calcClockValue; 
						tempClockValue[i][j].calcClockValue = max(k, eventSend) + 1;
						LogicalLamportClock[i][j] = tempClockValue[i][j].calcClockValue;
						logicalValue = tempClockValue[i][j].calcClockValue;
						logicalValue++;
					}
					else
					{
						// we set value to be (-1) but can be changed when send_events are processed
						tempClockValue[i][j].calcClockValue = -1;
						LogicalLamportClock[i][j] =-1;
					}
				}
			}
			// NULL means no value
			else if (tempClockValue[i][j].whatTypeEvent == "0" || tempClockValue[i][j].whatTypeEvent == "NULL" || tempClockValue[i][j].whatTypeEvent == "null" )
			{
				tempClockValue[i][j].calcClockValue = 0;
				LogicalLamportClock[i][j] = 0;
			}
			else
            {
                LogicalLamportClock[i][j]=-1;
                tempClockValue[i][j].calcClockValue=-99;
            }
		}
	}
    while(!finalTestAll(tempClockValue, n, m, LogicalLamportClock))
    {
	calcLampClock(tempClockValue, n, m, LogicalLamportClock);
    }
	// it prints out the Lamport Logical Clock Values
	cout<<endl<<"Lamport Logical Clock Values(LLC): "<<endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i+1 << ": ";
		for (int j = 0; j < m; j++)
		{
		    if(LogicalLamportClock[i][j]!=-1){
			cout<< LogicalLamportClock[i][j]<<"\t";
		    }
		}
		cout<< endl;
	}

	// generate output text file as a1output.txt which can be saved 
	// writes to output file
	ofstream outfile("a1output.txt");
	for (int i = 0; i < n; i++)
	{
		outfile << "P" << i+1 << ":";
		for (int j = 0; j < m; j++)
		{
			if(LogicalLamportClock[i][j]!=-1){
			outfile<< LogicalLamportClock[i][j]<<" ";
		    }
		}
		outfile << endl;
	}

return 0;
}


// function implementation
void calcLampClock(events tempClockValue[10][10], int n, int m, int LogicalLamportClock[10][10])
{
	// Send Event
	int eventSend = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (tempClockValue[i][j].calcClockValue == -1)
			{
				// checks for internal event
				if (tempClockValue[i][j].whatTypeEvent.length() == 1 || tempClockValue[i][j].whatTypeEvent[0]=='s') 
				{
				    if(tempClockValue[i][j-1].calcClockValue!=-1)
                    {
					// it calculates the lamport logical clock value for an internal event
					tempClockValue[i][j].calcClockValue = tempClockValue[i][j - 1].calcClockValue + 1; 
					LogicalLamportClock[i][j] = tempClockValue[i][j].calcClockValue;
                    }
				}
				else // checks for receive event
				{
					eventSend = calcSendEventClock(tempClockValue, n, m, tempClockValue[i][j].whatTypeEvent);
					if(eventSend!=-1)
					{
					// it calculates logical clock for receive event
					tempClockValue[i][j].calcClockValue = max(tempClockValue[i][j - 1].calcClockValue, eventSend) + 1; 
					LogicalLamportClock[i][j] = tempClockValue[i][j].calcClockValue;
					}
				}
			}
		}
	}
}

// calculates Send event clock

int calcSendEventClock(events tempClockValue[10][10], int n, int m, string a)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			// it checks the indexing of send values (S1 values)
			if (a[1] == tempClockValue[i][j].whatTypeEvent[1]) 
			{
			    if(a[0]==tempClockValue[i][j].whatTypeEvent[0])
                {

                }
                else {
			    if(tempClockValue[i][j].calcClockValue==-1)
                {
			// return it in case send event is not occured 
                    return -1; 
                }
                else
                {
                  return tempClockValue[i][j].calcClockValue;
                }
			}
			}
		}
	}

}

// check all final values 

bool finalTestAll(events tempClockValue[10][10], int n, int m, int LogicalLamportClock[10][10])
{
    for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (tempClockValue[i][j].calcClockValue == -1)
			{
			    return false;
			}
		}
	}
	return true;
}



