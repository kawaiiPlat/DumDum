// this is for the system commands
#include <stdlib.h>
#include <string>


#include <unordered_map>    // data structure
#include <map>              // data structure
#include <algorithm>        // for std::sort
#include <fstream>          // for files
#include <iostream>         // 
#include <iomanip>          // for setprecision
#include <sstream>          // for csv string parsing
#include <chrono>           // for timing the different data structures
#include <vector>           // for storing user objects


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

enum class opType{ single = 0, followers = 1};

struct User{
    string sName;
    long lScore;
    vector<string> words;
    User(string name){
        sName = name;
        lScore = 0;
    }
    bool operator<(User LHS){
        if(LHS.lScore > lScore)
            return true;

        return false;
    }
    bool operator>(User LHS){
        if(LHS.lScore > lScore)
            return false;

        return true;
    }
};

void parseUserDataInTo(vector<User>& users){
    cout << endl << "parsing user data..." << endl;
    ifstream data;
    string line;
    data.open("text_files/tweet_data.txt");
    if (data.is_open()){
        getline(data, line); // get the mode line
        if (line.at(0) == 'E'){
            cerr << "User inaccessable" << endl;
        } else {
            getline(data,line); // number of users
            int iNumUsers = stoi(line);

            // parse each user's data into a new user object
            for(int i = 0; i < iNumUsers; i++){
                string sName = "";
                getline(data,sName); // username

                getline(data, line); // numberoftweets
                User temp = User(sName);


                int iNumTweets = stoi(line);
                stringstream ss;
                string word;
                for(int j = 0; j < iNumTweets; j++){
                    getline(data,line);
                    ss << line;
                    while(getline(ss,word, ' ')){
                        temp.words.push_back(word);
                    }
                }

                users.emplace_back(temp);
            }
        }
    } else {
        cerr << "data file could not be opened" << endl;
    }
    
}

long getFreq(string word,unordered_map<string, long> um, map<string, long> om, long& fUMapTime, long& fOMapTime){
    long res = 0;
    if(um.count(word) > 0){
        Timer tUMap;
        res = um.find(word)->second; // unecessary, but I put it in to be fair between the two functions
        fUMapTime += tUMap.elapsed();

        Timer tOMap;
        res = om.find(word)->second;
        fOMapTime += tOMap.elapsed();
    }
    return res;
}

void calculateScores(vector<User>& users, unordered_map<string, long>& um, map<string, long>& om, long& fUMapTime, long& fOMapTime){
    cout << "calculating scores..." << endl;
    fOMapTime = 0.0f;
    fUMapTime = 0.0f;
    for(User& user : users){
        for(string word : user.words){
            user.lScore += getFreq(word, um, om, fUMapTime, fOMapTime);
        }
        user.lScore = user.lScore / (user.words.size() > 0? user.words.size() : 1); // get the average word score
    }
    cout << endl;
    cout << "Intelligence scores calculated." << endl;
    cout << "Unordered map searches took : " << fOMapTime*1000000 << " µs in total" << endl;
    cout << "Ordered map searches took   : " << fUMapTime*1000000 << " µs in total" << endl << endl;
}

void updateTwitter(string name, int mode, vector<User>& users, unordered_map<string, long> um, map<string, long> om, long fUMapTime, long fOMapTime ){
    Timer t3;
    cout << "reading from twitter, please be patient..." << endl;
    std::string command = "python python_scripts/twitterAccessor.py";
    command = command + " " + name + " " + to_string(mode);
    //std::string test = "python python_scripts/twitterAccessor.py LinusTech 0 > /dev/null 2>&1";
    system(command.c_str());
    cout << "search time: " << t3.elapsed() << endl << endl;

    parseUserDataInTo(users);
    calculateScores(users,um,om,fUMapTime,fOMapTime);
}

void loadWordlist(unordered_map<string,long>& um, map<string,long>& om){
    ifstream data;

    int i = 0;
    data.open("unigram_freq.csv");
    string temp, currWord;
    long numOccurences;


    getline(data, temp); // skip the header


    stringstream ss;

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
    return;
}

void printUserScores(vector<User> userList, string sSeperator, string title){

    cout << title << endl;
    cout << sSeperator;
    cout << "Twitter @                  Intelligence" << endl;
    cout << sSeperator;
    for(User user : userList){
        cout << setw(16) << left << user.sName << setw(13) << left << " | " << user.lScore << endl;
    }
    cout << sSeperator;
    cout << endl << endl;
}

/*

    "-g",
    "&&",
    "clear",
    "&&",
    "./out/${fileBasenameNoExtension}.out"
*/


int main(){
    unordered_map<string,long>   um;
    map<string, long>            om;
    long fOMapTime = 0, fUMapTime = 0;
    loadWordlist(um,om);
    vector<User> userList;
    cout << setprecision(3) << fixed;
    const string sBadInput = "bad input, enter the number of an option you want";
    const string sUsernamePrompt = "Enter the twitter username you want to know about, without the '@' ";
    const string sSeperator = "---------------------------------------\n";
    
    int choice = -1;
    while (choice != 0){
        cout << sSeperator;
        cout << "What operation do you want?" << endl;
        cout << "[0] Exit\n[1] Get a single account's score\n[2] Get a account's follower's scores" << endl;
        string input = "";
        cin >> input;
        
        cout << endl;

        try{
            choice = stoi(input);
        } catch (exception e){
            cout << sBadInput << endl;
        }

        // if the input is valid, continue, else print err message
        if(choice == 0){
            //exiting
            cout << "exiting..." << endl;
        } else if (choice == 1){
            // single user, just print their score and loop
            cout << sUsernamePrompt << endl;
            cin >> input;
            system("clear");
            updateTwitter(input, (int)opType::single, userList,um,om,fUMapTime,fOMapTime);

            for(User user : userList){
                cout << "-> @" << user.sName << " has a score of " << user.lScore << endl;
            }

        } else if (choice == 2) {
            // followers scores, give second menu
            cout << sUsernamePrompt << endl;
            cin >> input;
            updateTwitter(input, (int)opType::followers, userList, um, om, fUMapTime,fOMapTime); 
            string userName = input;
           
            //-----------------------------------------------------------------------------------------------------------------------------------------
            // multi user menu
            choice = -1;
            while (choice != 0){
                cout << sSeperator;
                cout << "What operation do you want?" << endl;
                cout << "[0] Go back\n[1] View top scoring following\n[2] View bottom scoring following\n[3] view average score of following" << endl;
                string graphTitle ="@" + userName + "'s followers, organized\nfrom the bottom ";
                string input = "";
                cin >> input;
                cout << endl;

                try{
                    choice = stoi(input);
                } catch (exception e){
                    cout << sBadInput << endl;
                }

                if(choice == 0){
                    cout << "Going Back... " << endl;
                    system("clear");
                } else if (choice == 1){
                    // highest scores
                    sort(userList.begin(), userList.end(), [](const User& lhs, const User& rhs){return lhs.lScore < rhs.lScore;});
                    graphTitle += "by highest intelligence";
                    system("clear");
                    printUserScores(userList,sSeperator,graphTitle);
                } else if (choice == 2){
                    // lowest scores
                    sort(userList.begin(), userList.end(), [](const User& lhs, const User& rhs){return lhs.lScore > rhs.lScore;});
                    graphTitle += "by lowest intelligence";
                    system("clear");
                    printUserScores(userList,sSeperator, graphTitle);

                } else if (choice == 3){
                    long long llAvgScore = 0;
                    for( User user : userList){                            
                        llAvgScore += user.lScore;
                    }
                    llAvgScore /= userList.size();

                    system("clear");
                    cout << endl << "the average intelligence of " + userName + "'s followers is " << llAvgScore << endl; 

                }
            }
            choice = 2; // so the menu will loop around

        } else {
            cout << "pick a valid option" << endl;
        }

        userList.clear();
        cout << endl ;


    }
}