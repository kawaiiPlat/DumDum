import json
import os
import tweepy as tw
import re

# Enter your keys/secrets as strings in the following fields
credentials = {}
credentials['BEARER_TOKEN'] = "AAAAAAAAAAAAAAAAAAAAAEEYKQEAAAAA7OkbXl6oEnLibQY4vBfOwXCcoig%3DkdDQL8dQa84eDrtFzUINRVps6aTOKGwx4MkuFsEg86g6ud2PdR"
credentials['CONSUMER_KEY'] = "KLJGBmD1Lmd36M8fSP31j4srM"  # same as API key
credentials['CONSUMER_KEY_SECRET'] = "ojqisfDzyTqrv0Q87nycxBmIibT2w6SbpCqmq6euZG2q7RSJJI"  # same as API key secret
credentials['ACCESS_TOKEN'] = "948800958439489536-FOTiPmy8MJjlPnFcYGoTZGl9hFrtNHY"
credentials['ACCESS_TOKEN_SECRET'] = "MdDcAsQjNgh0YqDGX7qL7U8eGYi5xvE3aOOR31zDNRuuE"

def main():
    screen_name = "LinusTech"
    all_posts = []
    data_file_name = "../text_files/tweet_data.txt"
    
    #auth = tw.OAuthHandler(credentials['CONSUMER_KEY'], credentials['CONSUMER_KEY_SECRET'])  # this is Auth version 1, not as good
    auth = tw.AppAuthHandler(credentials['CONSUMER_KEY'], credentials['CONSUMER_KEY_SECRET'])  # Auth version 2 better, idk why exactly, rate limits?
    #auth.set_access_token(credentials['ACCESS_TOKEN'] , credentials['ACCESS_TOKEN_SECRET'])  # only for Auth version 1
    api = tw.API(auth, wait_on_rate_limit=False)

    find_rate_limiter(api)
    all_followers = tw.Cursor(api.followers, screen_name).items(100)  # get 100 most recent followers
    num_followers = 0
    all_text_to_append = ""
    for follower in all_followers:
        try:
            user_info_to_append = ""
            all_posts_by_user = tw.Cursor(api.user_timeline, id=follower.id, include_rts=False).items(100)
            print()
            print("************** " + follower.screen_name + " **************")
            num_posts = 0
            for post in all_posts_by_user:
                tweet = remove_url(post.text.replace('\n', ' '))
                user_info_to_append += tweet
                user_info_to_append += "\n"
                # print()
                # print(tweet)
                num_posts += 1
            all_text_to_append += follower.screen_name + "\n" + str(num_posts) + "\n" + user_info_to_append
            num_followers += 1
        except:
            print("This follower is probably private. Skipping...")
        
    all_text_to_append = str(num_followers) + "\n" + all_text_to_append
        
    cur_path = os.path.dirname(__file__)
    new_path = os.path.join(cur_path, data_file_name)
    data_file = open(new_path, "a")
    data_file.truncate(0)  # deletes all content of file
    data_file.write(all_text_to_append)
    data_file.close()
    # example of searching for "wildfires" hashtags and printing the tweets
    
    # search_words = "#wildfires"
    # date_since = "2018-11-16"

    # tweets = tw.Cursor(api.search,
    #             q=search_words,
    #             lang="en",
    #             since=date_since).items(5)

    # for tweet in tweets:
    #     print()
    #     print(remove_url(tweet.text.replace('\n', ' ')))

def remove_url(txt):
    # regular expression from https://www.earthdatascience.org/courses/use-data-open-source-python/intro-to-apis/calculate-tweet-word-frequencies-in-python/
    return " ".join(re.sub("([^0-9A-Za-z \t])|(\w+:\/\/\S+)", "", txt).split())

def find_rate_limiter(api):
    rate_info = api.rate_limit_status()
    rate_str = str(rate_info)
    index = rate_str.find("'remaining': 0")
    if index != -1:
        print(rate_str[index - 50:index+50])
    else:
        print("No rate limiter found")
    

if __name__ == '__main__':
    main()