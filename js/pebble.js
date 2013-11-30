
var localStorage = window.localStorage;

var resources = {timeline:0, mentions:1, profile:2, keys:10}

//PEBBLE JS APP

var t = new Twitter({});
var keys = {};

Pebble.addEventListener("ready", function(e) {
   	
    console.log('APP and running: @'+t.screen_name)

    if (!t.screen_name){

    	Pebble.showSimpleNotificationOnPebble('Hey there!', 'In order to make the app work, you need to open the Pebble app in your phone, look for Twebble in your installed apps and enter your Twitter credentials in Settings :)')
    }
});

Pebble.addEventListener("appmessage", function(e){

	var resource = e.payload.resource;
	switch (resource){
		case resources.timeline:

			console.log('Asking for TL')
			t = new Twitter(JSON.parse(window.localStorage.getItem("keys")))
			t.getTimeline(function (data){
				for (t in data){
					var tweet = data[t]
					if (t == 0) tweet.start = 1;
					if (t == data.length-1) tweet.end = 1;
					tweet.resource = resources.timeline;
					send(tweet)
				}
			})
			break;
		case resources.mentions:

			console.log('Asking for Mentions')
			t = new Twitter(keys)
			t.getMentions(function (data){
				for (t in data){
					var tweet = data[t]
					if (t == 0) tweet.start = 1;
					if (t == data.length-1) tweet.end = 1;
					tweet.resource = resources.mentions;
					send(tweet)
				}
			})
		case resources.keys:

			keys = {"key":e.payload.key, "secret":e.payload.secret}
			t = new Twitter(keys)

			localStorage.setItem("keys", JSON.stringify(keys))

			console.log('SET KEYS')
			break;
	}
})
Pebble.addEventListener("showConfiguration", function (e){

	t = new Twitter(keys)

	if (!t.token || !t.screen_name){

		t.auth()
	}
	else {

		Pebble.openURL('http://izqui.me/html/twlog.html#'+t.screen_name)
	}
})
Pebble.addEventListener("webviewclosed", function (e){

	var r = JSON.parse(e.response)
	if (r.oauth_verifier){

		t.getAccessToken(r.oauth_verifier, function (){

			//Just gained access
			Pebble.showSimpleNotificationOnPebble('Hello @'+t.screen_name, 'Welcome to Twebble, we hope you enjoy the app :)')
		})
	}
	else if (r.logout){

		t.logout()
	}
})
