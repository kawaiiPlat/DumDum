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
    float fScore;
    User(string name, float score){
        sName = name;
        fScore = score;
    }
    bool operator<(User LHS){
        if(LHS.fScore > fScore)
            return true;

        return false;
    }
    bool operator>(User LHS){
        if(LHS.fScore > fScore)
            return false;

        return true;
    }
};

void parseUserDataInTo(vector<User>& users){
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
                float fScore = 0;
                getline(data,sName); // username

                getline(data, line); // numberoftweets
                int iNumTweets = stoi(line);

                for(int j = 0; j < iNumTweets; j++){
                    
                }

                users.emplace_back(User(sName,fScore));
            }

        }
    } else {
        cerr << "data file could not be opened" << endl;
    }
    
}

void updateTwitter(string name, int mode, vector<User>& users){
    Timer t3;
    cout << "reading from twitter, please be patient..." << endl;
    std::string test = "python python_scripts/twitterAccessor.py";
    test = test + " " + name + " " + to_string(mode);
    //std::string test = "python python_scripts/twitterAccessor.py LinusTech 0 > /dev/null 2>&1";
    system(test.c_str());
    cout << "search time: " << t3.elapsed() << endl << endl;

    parseUserDataInTo(users);
}

float findAverageOf(string userName, int mode){
    // todo calculate with the file
    return 0.0f;
}

void loadWordlist(unordered_map<string,int>& um, map<string,int>& om){
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

long getWordScore(unordered_map<string,int>& um, map<string,int>& om, float& umTime, float& omTime){
    return 0;
}

string parseUsername(){
   return ""; 
}

int main(){
    unordered_map<string,int>   um;
    map<string, int>            om;
    loadWordlist(um,om);
    long umTime = 0, omTime = 0;
    vector<User> userList;
    cout << setprecision(3) << fixed;
    const string sBadInput = "bad input, enter the number of an option you want";
    const string sUsernamePrompt = "Enter the twitter username you want to know about, without the '@' ";
    
    //updateTwitter("LinusTech", (int)opType::followers);

    int choice = -1;
    while (choice != 0){
        cout << "---------------------------------------" << endl;
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
            cout << "single user" << endl;
            cout << sUsernamePrompt << endl;
            cin >> input;
            updateTwitter(input, (int)opType::single, userList);

        } else if (choice == 2) {
            // followers scores, give second menu
            cout << "followers" << endl;
            cout << sUsernamePrompt << endl;
            cin >> input;
            updateTwitter(input, (int)opType::followers, userList);
           
            //----------------------------------------------------------
            // multi user menu
            choice = -1;
            while (choice != 0){
                cout << "What operation do you want?" << endl;
                cout << "[0] Go back\n[1] View top scoring following\n[2] View bottom scoring following\n[3] view average score of following" << endl;
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
                } else if (choice == 1){
                    
                } else if (choice == 2){

                } else if (choice == 3){

                }
            }
            choice = 2; // so the menu will loop around

        } else {
            cout << "pick a valid option" << endl;
        }


        cout << endl ;


    }

/*
    Timer t1;
    cout << um.find("test")->second << endl;
    cout << "Search time in µs for umap: " << t1.elapsed()*1000000 << endl << endl;

    Timer t2;
    cout << om.find("test")->second << endl;
    cout << "Search time in µs for omap: " << t2.elapsed()*1000000 << endl;
*/
}