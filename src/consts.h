enum {
  KEY_RESOURCE = 0,
  KEY_ID = 1,
  KEY_TEXT = 2, 
  KEY_SCREEN_NAME = 3,
  KEY_NAME = 4, 
  KEY_FOLLOWERS = 5, 
  KEY_FOLLOWING = 6,
  KEY_START = 7,
  KEY_END = 8,
  KEY_KEY = 9,
  KEY_SECRET = 10

};

enum {

  RESOURCE_TL = 0,
  RESOURCE_MENTIONS = 1,
  RESOURCE_PROFILE = 2,
  RESOURCE_KEYS = 10
};

typedef struct Tweet{
  int id;
  char text[170]; //RTs
  char screen_name[20];
  char name[30];

} Tweet;

typedef struct Profile{
  int id;
  char bio[140];
  char screen_name[20];
  char name[30];
  int followers;
  int following;
  //TODO: Tweet latest_tweets[3];

} Profile;

#define MAX_TWEETS 80
#define ITEMS 3

#define SPACING 3
#define DEF_FONT 18
#define DEF_TEXT 44
#define MAX_HEIGHT 200

#define WIDTH 144
#define HEIGHT 168

static char titles[ITEMS][25] = {"Timeline", "Mentions", "Profile"};