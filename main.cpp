/*
The chaining implementation ended up taking longer to insert all the values than 
the probing implementation. This was expected to me, because when I inserted to
the lists I inserted at the front, which probably took a bit of time, and with
a million insertions its bound to take longer than probing. 

The parallel implementation took a lot longer to insert all the values than the 
serial implementations. This was not expected to me, but it does make a bit of 
sense now that I think about it. I think this is because I put my check for rehash 
function in the critical section, which holds my entire rehash function in it as
well. Because there is so much in the critical section, and the hash table has to 
rehash so many times, the program slows down and has to wait for the rehash to 
complete before continuing insertions.

To improve performance I would find a way to reduce the amount of code in the 
critical section of my parallel block. At this time I'm not sure how I would
do that, because it shouldn't be possible to be rehashing in two threads at 
the same time, so all of that code seems necessary. I think that parallel 
programming might just be a bad fit in this situation because of all of the 
actions that need to be performed when inserting all the values into the hash
table.
*/

#include "ChainingHash.h"
#include "ProbingHash.h"
#include "ParallelProbingHash.h" 
#include <omp.h>
#include <ctime>
#include <fstream>

#define NUM_THREADS 2  // update this value with the number of cores in your system. 

using std::clock_t;

int main()
{
	/*Task I (a)- ChainingHash table*/

		//  create an object of type ChainingHash 
		ChainingHash<int,int> chainhash;

		// In order, insert values with keys 1 – 1,000,000. For simplicity, the key and value stored are the same. 
		clock_t start = clock();
		for (int i=1; i<1000001; i++){ 
			chainhash.insert({i,i});
		}
		clock_t end = clock();
		
		// Report the total amount of time, in seconds, required to insert the values to ChainingHash table. Write the results to a file called “HashAnalysis.txt”. 
		std::ofstream outfile;
		outfile.open("HashAnalysis.txt", std::ios::out);
		outfile << "***Chaining Analysis***\nChaining insertion time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Search for the value with key 177 in ChainingHash table. Report the time required to find the value in each table by writing it to the “HashAnalysis.txt” file. 
		start = clock();
		chainhash.at(177);
		end = clock();
		outfile << "Chaining search time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Search for the value with key 2,000,000 in ChainingHash table. Report the time required to find the value in each table by writing it to the file.  
		start = clock();
		chainhash.at(2000000);
		end = clock();
		outfile << "Chaining failed search time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Remove the value with key 177 from ChainingHash table. Report the time required to remove the value with in each table by writing it to the file.  
		start = clock();
		chainhash.erase(177);
		end = clock();
		outfile << "Chaining deletion time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Also, write to the file the final size, bucket count, and load factor of the hash for ChainingHash table. 
		outfile << "Table size: " << chainhash.size() << "\nBucket count: " << chainhash.bucket_count() << "\nLoad factor: " << chainhash.load_factor() << endl;

		/* Example output template:
			Chaining insertion time: 
			Chaining search time: 
			Chaining failed search time: 
			Chaining deletion time: 
			Table size: 
			Bucket count: 
			Load factor: 
		*/

	/*Task I (b) - ProbingHash table (using Linear Probing) */

		//  create an object of type ProbingHash 
		ProbingHash<int,EntryState> probehash;

		// In order, insert values with keys 1 – 1,000,000. For simplicity, the key and value stored are the same.
		start = clock();
		for (int i=1; i<1000001; i++){ 
			probehash.insert({i,VALID});
		}
		end = clock();

		// Report the total amount of time, in seconds, required to insert the values to ProbingHash table. Write the results to a file called “HashAnalysis.txt”. 
		outfile << "\n***Probing Analysis***\nLinear Probing insertion time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Search for the value with key 177 in ProbingHash table. Report the time required to find the value in each table by writing it to the “HashAnalysis.txt” file. 
		start = clock();
		probehash.at(177);
		end = clock();
		outfile << "Linear Probing search time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Search for the value with key 2,000,000 in ProbingHash table. Report the time required to find the value in each table by writing it to the file.  
		start = clock();
		probehash.at(2000000);
		end = clock();
		outfile << "Linear Probing failed search time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Remove the value with key 177 from ProbingHash table. Report the time required to remove the value with in each table by writing it to the file.  
		start = clock();
		probehash.erase(177);
		end = clock();
		outfile << "Linear Probing deletion time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Also, write to the file the final size, bucket count, and load factor of the hash for ProbingHash table. 
		outfile << "Table size: " << probehash.size() << "\nBucket count: " << probehash.bucket_count() << "\nLoad factor: " << probehash.load_factor() << endl;

		/* Example output template:
			Linear Probing insertion time: 
			Linear Probing search time: 
			Linear Probing failed search time: 
			Linear Probing deletion time: 
			Table size: 
			Bucket count: 
			Load factor: 
		*/
	
	/*Task II -  ParallelProbingHash table (using Linear Probing) */
      
	  // (a) Using a single thread:  
		//  create an object of type ParallelProbingHash 
		ParallelProbingHash<int,Entrystate> parallelhash;

		// Set the number of threads (omp_set_num_threads()) to 1 
		omp_set_num_threads(1);

		/* In an OpenMP parallel region (#pragma omp parallel), in order, insert values with keys 1 – 1,000,000. 
		Inside the parallel region make sure that the value for the iteration number of the loop is shared among all threads. 
		For simplicity, the key and value stored are the same.
        */
	    start = clock();
	   	#pragma omp parallel for
			for (int i=1; i<1000001; i++){ 
				parallelhash.insert({i,VALId});
				parallelhash.checkRehash();
		}
		end = clock();

		// Report the total amount of time, in seconds, required to insert the values to ParallelProbingHash table. Write the results to a file called “HashAnalysis.txt”. 
		outfile << "\n***Single Thread Parallel Analysis***\nParallel Probing insertion time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Search for the value with key 177 in ParallelProbingHash table. Report the time required to find the value in each table by writing it to the “HashAnalysis.txt” file. 
		start = clock();
		parallelhash.at(177);
		end = clock();
		outfile << "Parallel Probing search time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Search for the value with key 2,000,000 in ParallelProbingHash table. Report the time required to find the value in each table by writing it to the file.  
		start = clock();
		parallelhash.at(2000000);
		end = clock();
		outfile << "Parallel Probing failed search time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Remove the value with key 177 from ParallelProbingHash table. Report the time required to remove the value with in each table by writing it to the file.  
		start = clock();
		parallelhash.erase(177);
		end = clock();
		outfile << "Parallel Probing deletion time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Also, write to the file the final size, bucket count, and load factor of the hash for ParallelProbingHash table. 
		outfile << "Table size: " << parallelhash.size() << "\nBucket count: " << parallelhash.bucket_count() << "\nLoad factor: " << parallelhash.load_factor() << endl;

		/* Example output template:
			Parallel Probing insertion time: 
			Parallel Probing search time: 
			Parallel Probing failed search time: 
			Parallel Probing deletion time: 
			Table size: 
			Bucket count: 
			Load factor: 
		*/

	// (b) Using multiple threads:  
		//  create an object of type ParallelProbingHash 
		ParallelProbingHash<int,Entrystate> parallelhash2;

		// i.	Change the number of threads to match the number of cores on your system 
		omp_set_num_threads(NUM_THREADS);

		/* In an OpenMP parallel region (#pragma omp parallel), in order, insert values with keys 1 – 1,000,000. 
		Inside the parallel region make sure that the value for the iteration number of the loop is shared among all threads. 
		For simplicity, the key and value stored are the same.
        */
	   start = clock();
	   	#pragma omp parallel for
			for (int i=1; i<1000001; i++){ 
				parallelhash2.insert({i,VALId});
				#pragma omp critical
				{
					parallelhash2.checkRehash();
				}
		}
		end = clock();

		// Report the total amount of time, in seconds, required to insert the values to ParallelProbingHash table. Write the results to a file called “HashAnalysis.txt”. 
		outfile << "\n***Two Thread Parallel Analysis***\nParallel Probing insertion time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Search for the value with key 177 in ParallelProbingHash table. Report the time required to find the value in each table by writing it to the “HashAnalysis.txt” file. 
		start = clock();
		parallelhash2.at(177);
		end = clock();
		outfile << "Parallel Probing search time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Search for the value with key 2,000,000 in ParallelProbingHash table. Report the time required to find the value in each table by writing it to the file.  
		start = clock();
		parallelhash2.at(2000000);
		end = clock();
		outfile << "Parallel Probing failed search time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Remove the value with key 177 from ParallelProbingHash table. Report the time required to remove the value with in each table by writing it to the file.  
		start = clock();
		parallelhash2.erase(177);
		end = clock();
		outfile << "Parallel Probing deletion time: " << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;

		// Also, write to the file the final size, bucket count, and load factor of the hash for ParallelProbingHash table. 
		outfile << "Table size: " << parallelhash2.size() << "\nBucket count: " << parallelhash2.bucket_count() << "\nLoad factor: " << parallelhash2.load_factor() << endl;

		/* Example output template:
			Parallel Probing insertion time: 
			Parallel Probing search time: 
			Parallel Probing failed search time: 
			Parallel Probing deletion time: 
			Table size: 
			Bucket count: 
			Load factor: 
		*/
	outfile.close();
	return 0;
}