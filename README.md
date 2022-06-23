# OS-Scheduling-Simulator
Uses a Linked List-based structure to simulate 3 different popular Operating System process scheduling schemes

The user may choose between these three scheduling methods:<pre>
	* "-fifo" First-In-First-Out
	* "-sjf"  Shortest Job First
	* "-rr"   Round Robin</pre>
The program when run needs to be used in this Command Line Argument format: ./[binName] <schedulerChoice> <inFile> <outFile>
If run correctly, the outfile will be written to with the simulated scheduling details based on the user's choice of scheduler.

* The included "in1.txt" file is there for OPTIONAL use, and for showing off the format in which you could test your own input.
  The format for those text input files are as follows:

<pre>  _________________________________________
 |TaskArrivalTime TaskServiceTime          |
 |TaskArrivalTime TaskServiceTime          |
 |...             ...                      |
 </pre>
	
  
This project was particularly challenging to complete due to it being limited by a Linked List's basic structure, but success was achieved nonetheless.
