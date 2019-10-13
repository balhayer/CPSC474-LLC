# CPSC 474

# VERIFICATION PART2 COMPLETED solely by SAYTU SINGH

# MEMBERS NAME:
# Balwinder Singh Hayer
# Saytu Singh

# EXECUTE COMMANDS:
# python verify.py

# To check example 2 and 3, open this project.py file and edit input text file name at top to input_1.txt, input_2.txt, input_3.txt

# For Example 1 -> edit file name to input_1.txt inside project.py file at top section
# For Example 2 -> edit file name to input_2.txt inside project.py file at top section
# For Example 3 -> edit file name to input_3.txt inside project.py file at top section


delimiter = ' '
inputFileName = "input_3.txt"     # EDIT Input text file to test Example 1, 2, 3

# It prints out two-dimensional array with tab separation
def printArray(myArray):
    for rows in myArray:
        for cols in rows:
            print cols,'\t',
        print '\n'


# It reads input values from text file which is stated at top
with open(inputFileName) as f:
    inputTrue = [x.split(delimiter) for x in [line.rstrip('\n') for line in f]]

# It converts all strings values to integer list values
inputTrue = [list(map(int, c)) for c in inputTrue]


# It initialize final values of N & M
N = len(inputTrue)
M = len(inputTrue[0])

# It initialize final result using finalResult array
finalResult = [['-1' for x in range(M)] for y in range(N)]

# It initialize all dictionaries
send,receiveEvent,int_events = {},{},{}

# It maps all alphabets ranges from a to z 
labels = list(map(chr, range(97, 123)))    

''' ------------- Initializations Complete ------------- '''

''' ------------- Verifying Lamport Logical Clocks ------------- '''

# Draws all alphabets and populate dictionaries alphabets
for i in range(N):
    oldValue = 0
    for j in range(M):
        
	# oldValue > 1 used to find gap between two logical lamport clock
        if inputTrue[i][j] - oldValue > 1:                
            receiveEvent[inputTrue[i][j]] = (i,j)

            found = False
            for l in [x for x in range(N) if x != i]: # Verifies all processes rows exluding current ith row
                
                for m in range(M):
		    # Verify every column for by LogicalClock(s) -> LogicalClock(r) - 1                    
                    if inputTrue[l][m] == inputTrue[i][j] - 1:
                        found = True
                        send[inputTrue[l][m]] = (l,m)

                if found:
                    break

            # Returns INVALID case if it founds value equals to Zero (0)
            if not found:     
		# Prints out the INVALID message when send event msg   
                print "INVALID" 
                exit(0)
        
        if inputTrue[i][j] == 0:            
            finalResult[i][j] = 'NULL'

        oldValue = inputTrue[i][j]

# Populate finalResult array with send and receiveEvent events

actualMessageCounter = 1
for LamportClockSend, CurrentValue in sorted(send.iteritems()):
    
    # It is for sending value
    i,j = CurrentValue
    finalResult[i][j] = 's{0}'.format(actualMessageCounter)

    # It is for receive event
    # It is Lamport Logical Clock of send Event 
    i,j = receiveEvent[LamportClockSend + 1]     
    finalResult[i][j] = 'r{0}'.format(actualMessageCounter)

    actualMessageCounter += 1

    
# Populate pending elements in finalResult array with internal events
from itertools import cycle

# It initiate iteration for all remaining labels
iter = cycle(labels)

for i in range(N):
    for j in range(M):
        if finalResult[i][j] == '-1':
	    # It looks for next available label then assigns it 
            finalResult[i][j] = next(iter)       


# It prints out the output
printArray(finalResult)

#end

