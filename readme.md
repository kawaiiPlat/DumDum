---
title: Read Me
author: @kawaiiPlat
---

# This is a final project for COP3530 Fall 2020 at the University of Florida.
## The point of this program was to compare the use of multiple data structures in a "real life" setting. 

This program is meant as a joke, and doesn't actually speak to anyone's intelligence. It pretends to calculate comparitive intelligence by comparing the frequency of words used in users tweets. The idea being that you can find out who the most drull of your friends are and call them **_Dum Dums_**.

***
## Dependencies:
* Linux / macOS shell to run in
* Python
  * Python 3.8 tested 
  * Tweepy 3.9.0 tested
* g++ 10.2.0 tested

## Optional:
* API Key encryption scripts:
  * openssl 1.1.1h
  * tar 1.3.2

*** 
## To use
1. Place twitter api keys in a .json file according to the spec listed in python_files.
2. Compile and run main.cpp ( possibly with `./run.sh` if using g++ )
3. Follow the prompts from the program

***
## API Keys

{ "CONSUMER_KEY": "blah blah blah", "CONSUMER_KEY_SECRET": "blah blah blah"}

Save the above line (with the blahs replaced with your keys) with the file location, name, and extension: "python_scripts/apiKeys.json"

We only use two values from the API keys, as we are using the twitter API V2

## Twitter data

The data is saved in the following format in text_files/tweet_data.txt

Mode = 1 (follower data = 1, single user = 0)
NumberOfUsers = 2
username1
NumberOfTweets = 2
tweet
tweet
username2
NumberOfTweets = 1
tweet

