import json
import os
import tweepy as tw
import re
import sys
import json


def main():
    data_file_name = "../text_files/tweet_data.txt"
    
    if (len(sys.argv) < 3):
        all_text_to_append = "ERROR: not enough arguments"
        print("ERROR: Not enough arguments. Should be 'python file_name mode_option'")
        output_string_to_file(all_text_to_append, data_file_name)
        return
    screen_name = sys.argv[1]
    mode_option = sys.argv[2]  # "0" = get score for screen_name, "1" = get score for all of screen_name's followers
    MAX_FOLLOWER_NUM = 10 # lowered for testing
    MAX_TWEET_NUM = 10 #lowered for testing
    
    #with open("python_scripts/apiKeys.json","w") as write_file:
    #    json.dump(credentials,write_file)

    try:
        print("trying to load file")
        breadcrumb = open("text_files/breadcrumb.txt")

        line = breadcrumb.readline()
        print(int(line))
        print(int(mode_option))
        if (int(line) == int(mode_option)):
            line = breadcrumb.readline()
            print(line)
            if (line == screen_name):
                print("Data already cached")
                exit()
    except IOError:
        with open("text_files/breadcrumb.txt", "w") as new_breadcrumb:
            print("making new breadcrumb")
            newbc = mode_option + "\n" + screen_name
            new_breadcrumb.write(newbc) 
    finally:
        breadcrumb.close()

    credentials = {}
    with open("python_scripts/apiKeys.json","r") as cred_file:
        credentials = json.load(cred_file)

    #auth = tw.OAuthHandler(credentials['CONSUMER_KEY'], credentials['CONSUMER_KEY_SECRET'])  # this is Auth version 1, not as good
    auth = tw.AppAuthHandler(credentials['CONSUMER_KEY'], credentials['CONSUMER_KEY_SECRET'])  # Auth version 2 better, idk why exactly, rate limits?
    #auth.set_access_token(credentials['ACCESS_TOKEN'] , credentials['ACCESS_TOKEN_SECRET'])  # only for Auth version 1
    api = tw.API(auth, wait_on_rate_limit=False)

    find_rate_limiter(api)
    
    all_text_to_append = ""
    if (mode_option == "1"):
        all_followers = tw.Cursor(api.followers, screen_name).items(MAX_FOLLOWER_NUM)  # get 100 most recent followers
        num_followers = 0
        try:
            for follower in all_followers:
                try:
                    user_info_to_append, num_posts = get_all_posts_by_user(api, follower, MAX_TWEET_NUM)
                    all_text_to_append += follower.screen_name + "\n" + str(num_posts) + "\n" + user_info_to_append
                    num_followers += 1
                except:
                    print("This follower is probably private. Skipping...")
                min_count = str(min(MAX_TWEET_NUM, api.get_user(screen_name).followers_count))
                print("(" + str(num_followers) + " / " + str(min_count) + ")")
            all_text_to_append = "1\n" + str(num_followers) + "\n" + all_text_to_append
        except:
            print("ERROR: This user is not accessible.")  # probably because the account is private or similar
            all_text_to_append = "ERROR: This user is not accessible."  # example private account: DaleTheTank
            output_string_to_file(all_text_to_append, data_file_name)
            return
    elif mode_option == "0":
        try:
            user = api.get_user(screen_name)
            user_info_to_append, num_posts = get_all_posts_by_user(api, user, MAX_TWEET_NUM)
            all_text_to_append += "0\n1\n" + screen_name + "\n" + str(num_posts) + "\n" + user_info_to_append  # the leading 0\n1\n specifies mode is 0 and there's 1 user listed
        except:
            print("ERROR: This user is not accessible.")  # probably because the account is private or similar
            all_text_to_append = "ERROR: This user is not accessible."  # example private account: DaleTheTank
            output_string_to_file(all_text_to_append, data_file_name)
            return
    else:
        all_text_to_append = "ERROR: invalid mode"
        print("ERROR: invalid mode")
        output_string_to_file(all_text_to_append, data_file_name)
        return
    
    print("Writing to the output file...")
    output_string_to_file(all_text_to_append, data_file_name)
    print("Done writing")
    
    
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
    
def output_string_to_file(to_output, filename):
    cur_path = os.path.dirname(__file__)
    new_path = os.path.join(cur_path, filename)
    data_file = open(new_path, "a")
    data_file.truncate(0)  # deletes all content of file
    data_file.write(to_output)
    data_file.close()
    
def get_all_posts_by_user(api, user, MAX_TWEET_NUM):
    all_posts_by_user = tw.Cursor(api.user_timeline, id=user.id, include_rts=False, exclude_replies=True).items(MAX_TWEET_NUM)
    print()
    print("Accessing " + user.screen_name + "'s tweets...")
    num_posts = 0
    user_info_to_append = ""
    for post in all_posts_by_user:
        tweet = remove_url(post.text.replace('\n', ' '))
        regex = re.compile('[^a-zA-Z ]')  # removes all non-alphabetical characters
        tweet = regex.sub('', tweet)
        user_info_to_append += tweet
        user_info_to_append += "\n"
        # print()
        # print(tweet)
        num_posts += 1
    print("Done")
    return user_info_to_append, num_posts

if __name__ == '__main__':
    main()