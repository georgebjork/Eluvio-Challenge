#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <set>
#include <functional>


//This vector will hold all of our binary files in numerical order. This allows for easy access
std::vector<std::vector<unsigned char>> vectorsC;


//This struct will hold all important information about the stadn we just found
struct strands
{
  strands(){};
  strands(int f1, int f2, int o1, int o2, int l){
    fileName1 = f1;
    fileName2 = f2;
    offset1 = o1;
    offset2 = o2;
    length = l;
  }

  //File names 
  int fileName1;
  int fileName2;

  //Offsets
  int offset1;
  int offset2;

  //Lengths
  int length;
};

//This function will read all the files into a unsigned char vector
std::vector<unsigned char> readFile(std::string fileName)
{
  std::fstream f;
  f.open("Binary Files/" + fileName, std::ios::in | std::ios::binary);
  if (f.is_open())
  {
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(f), {});
    std::cout << fileName << " has been opened and read into a byte vector with a size of " << buffer.size() << " bytes" << std::endl;
    return buffer;
  }
  else
  {
    std::cout << fileName + " has failed to open" << std::endl;
    std::vector<unsigned char> list;
    return list;
  }
}

strands findLongestStrand(std::vector<unsigned char> X, std::vector<unsigned char> Y, int file1, int file2)
{
  //We are going to create a 2d vector to act as a table.
  //Essentially this table will hold a int value that will represent the length of the longest strand.
  //If charcters at an x index match the charecter at a y index, we will insert length+1 into our table

	std::vector<std::vector<int>> table (X.size()+1, std::vector<int>(Y.size()+1, 0));  //2D table will the size based on the x and y vectors passed in and all values initalized at 0
  int endIndexI = X.size(); //Hold the ending index of the x vector so we can find the offeset 
  int endIndexJ = Y.size(); //Hold the ending index of the x vector so we can find the offeset
	int length = 0; // To store length 

  for (int i = 1; i <= X.size(); i++)
    {
        for (int j = 1; j <= Y.size(); j++)
        {

            // if the current character of X and Y matches
            if (X[i-1] == Y[j-1])
            {
                table[i][j] = table[i-1][j-1] + 1;
 
                // update length and index 
                if (table[i][j] > length)
                {
                    length = table[i][j];
                    endIndexI = i;
                    endIndexJ = j;
                }
            }
        }
    }
  //At the end we will push the strands into a strand struct to hold all important information
  strands s = strands(file1+1, file2+1, endIndexI-length, endIndexJ-length, length); 
	return s;
}


int main()
{
  //Put all binary files into an array
  std::string fileName = "";
  for (int i = 1; i <= 10; i++)
  {
    fileName = "sample." + std::to_string(i);
    vectorsC.push_back(readFile(fileName));
  }

  std::cout << "==========================================================================================" << std::endl;
  std::cout << "==========================================================================================" << std::endl;
  std::cout<< std::endl;

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  //This will run all comparisons
  std::vector<strands> s;
  for(int i = 0; i < vectorsC.size(); i++){
    for(int j = 0; j < vectorsC.size(); j++){
      if(j != i && j > i){
        std::cout << "Comparing sample." << i+1 << " and sample." << j+1 << std::endl;
        s.push_back(findLongestStrand(vectorsC[i], vectorsC[j], i, j));        
      }
    }
  }

  //This will find the longest strand in the 
  strands finalStrand;
  int length;
  for(int i = 0; i < s.size(); i++){
    if(s[i].length > length){
      length = s[i].length;
      finalStrand = s[i];
    }
  }

  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << "==========================================================================================" << std::endl;
  std::cout << "The strands found were in files sample." << finalStrand.fileName1 << " and sample." << finalStrand.fileName2 << " with offsets " << finalStrand.offset1 << " and " << finalStrand.offset2 << " with a length of " << finalStrand.length << std::endl;
  std::cout << "This program took " << elapsed_seconds.count()/60 << " minutes" << std::endl;
  return 0;
  }

 
    