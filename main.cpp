// this is for the system commands
#include <stdlib.h>
#include <string>


#include <unordered_map>    // data structure
#include <map>              // data structure
#include <fstream>          // for files
#include <iostream>         //
#include <iomanip>          // for setprecision
#include <sstream>          // for csv string parsing
#include <chrono>           // for timing the different data structures


// from https://www.learncpp.com/cpp-tutorial/8-16-timing-your-code/
class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
	
	std::chrono::time_point<clock_t> m_beg;
 
public:
	Timer() : m_beg(clock_t::now())
	{
	}
	
	void reset()
	{
		m_beg = clock_t::now();
	}
	
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};


using namespace std;

    /*
int main(){
    //std::string test = "python python_scripts/test.py";
    //system(test.c_str());
    
    unordered_map<string,int>   um;
    map<string, int>            om;
    ifstream data;

    data.open("unigram_freq.csv");
    string temp, currWord;
    long numOccurences;


    getline(data, temp); // skip the header


    stringstream ss;

    int i = 0;
    for(string line; getline(data,line); ){
        ss << line;

        getline(ss, currWord, ',');
        //cout << currWord << " | ";

        getline(ss, temp, ',');
        numOccurences = stol(temp);
        //cout << numOccurences << endl;
        
        ss.clear();

        um.emplace(currWord,numOccurences);
        om.emplace(currWord,numOccurences);
    }


    cout << setprecision(3) << fixed;
    Timer t1;
    cout << um.count("mapquwst") << endl;
    cout << "Search time in µs for umap: " << t1.elapsed()*1000000 << endl << endl;

    Timer t2;
    cout << om.count("mapquwst") << endl;
    cout << "Search time in µs for omap: " << t2.elapsed()*1000000 << endl;


    unsigned int nbuckets = um.bucket_count();
    int currMax = 0, currBucket = 0;
    for (int i = 0; i < nbuckets; i++){
        currBucket = um.bucket_size(i);
        currMax = currBucket == 0 ? 1 : 0;
    }

    cout << currMax / (float)nbuckets << endl;


}



*/


//--------------------------------------------------------------------------------------------------------------------


#include "trie.hpp"  
// Driver 
int main() 
{ 
    Trie temp;
    
    temp.insertWord("test");
    /*
    // Input keys (use only 'a' through 'z' 
    // and lower case) 
    string keys[] = {"the", "a", "there", 
                    "answer", "any", "by", 
                     "bye", "their" }; 
    int n = sizeof(keys)/sizeof(keys[0]); 
  
    TrieNode *root = getNode(); 
  
    // Construct trie 
    for (int i = 0; i < n; i++) 
        insert(root, keys[i]); 
  
    // Search for different keys 
    search(root, "the")? cout << "Yes\n" : 
                         cout << "No\n"; 
    search(root, "these")? cout << "Yes\n" : 
                           cout << "No\n"; 
    return 0; 
    */
} 
