import json
import os
import tweepy as tw
import re
import sys

def main():
    data_file_name = "../text_files/tweet_data.txt"

    if (len(sys.argv) < 3):
        all_text_to_append = "ERROR: not enough arguments"
        print("ERROR: Not enough arguments. Should be 'python file_name mode_option'")
        output_string_to_file(all_text_to_append, data_file_name)
        return

    screen_name = sys.argv[1]
    mode_option = sys.argv[2]  # "0" = get score for screen_name, "1" = get score for all of screen_name's followers
    MAX_FOLLOWER_NUM = 100
    MAX_TWEET_NUM = 100

    credentials = {}
    with open("python_scripts/apiKeys.json","r") as cred_file:
        credentials = json.load(cred_file)  # Load the credentials file (not included directly for security)

    auth = tw.AppAuthHandler(credentials['CONSUMER_KEY'], credentials['CONSUMER_KEY_SECRET'])  # Auth version 2 better, idk why exactly, rate limits?
    api = tw.API(auth, wait_on_rate_limit=False)

    find_rate_limiter(api)  # in case Twitter limits our access, this will identify it and tell us exactly what's being limited
    
    all_text_to_append = ""
    if (mode_option == "1"):  # get intelligence scores of all user's followers
        all_followers = tw.Cursor(api.followers, screen_name).items(MAX_FOLLOWER_NUM)  # get MAX_FOLLOWER_NUM most recent followers
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
            all_text_to_append = "ERROR: This user is not accessible."
            output_string_to_file(all_text_to_append, data_file_name)
            return
    elif mode_option == "0":  # get intelligence score of user
        try:
            user = api.get_user(screen_name)
            user_info_to_append, num_posts = get_all_posts_by_user(api, user, MAX_TWEET_NUM)
            all_text_to_append += "0\n1\n" + screen_name + "\n" + str(num_posts) + "\n" + user_info_to_append  # the leading 0\n1\n specifies mode_option is 0 and there is 1 user listed
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

def remove_url(txt):
    # regular expression from https://www.earthdatascience.org/courses/use-data-open-source-python/intro-to-apis/calculate-tweet-word-frequencies-in-python/
    return " ".join(re.sub("([^0-9A-Za-z \t])|(\w+:\/\/\S+)", "", txt).split())

def find_rate_limiter(api):
    rate_info = api.rate_limit_status()
    rate_str = str(rate_info)
    index = rate_str.find("'remaining': 0")
    if index != -1:
        print("Rate limiter found:")
        print(rate_str[index - 50:index+50])
    else:
        print("No rate limiter found")

def output_string_to_file(to_output, filename):
    cur_path = os.path.dirname(__file__)
    new_path = os.path.join(cur_path, filename)
    data_file = open(new_path, "a")  # opens files and sets mode to "append"
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
        tweet = remove_url(post.text.replace('\n', ' '))  # if the tweet contains any urls, remove them
        regex = re.compile('[^a-zA-Z ]')  # removes all non-alphabetical characters
        tweet = regex.sub('', tweet)
        tweet = tweet.lower()  # converts to lowercase
        user_info_to_append += tweet
        user_info_to_append += "\n"
        num_posts += 1
    print("Done")
    return user_info_to_append, num_posts

if __name__ == '__main__':
    main()
