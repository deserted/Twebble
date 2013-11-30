
//  Twitter
var Twitter = function(keys){

	this.consumer_key = keys.key
	this.consumer_secret = keys.secret

	this.baseurl = 'https://api.twitter.com'
	this.version = '/1.1'
	this.callback = 'http://izqui.me/html/tw.html'

	this.token = localStorage.getItem('oauth_token') ||''
	this.token_secret = localStorage.getItem('oauth_token_secret') ||''
	this.screen_name = localStorage.getItem('screen_name') ||''
	this.user_id = localStorage.getItem('user_id') ||''

	this.localProps = ['oauth_token', 'oauth_token_secret', 'screen_name', 'user_id']
}

Twitter.prototype.doRequest = function (method, path, v, parameters, body, cb){

	if (!this.consumer_key || !this.consumer_secret){

		Pebble.showSimpleNotificationOnPebble("Twitter error", "Close the app an open it again. Known bug, working on it. ")
	}
	else {
		var req = new XMLHttpRequest()
		req.onload = cb

		var u = ''
		if (v){
			u = this.baseurl + this.version + path
		}
		else u = this.baseurl + path

		var auth = {

			oauth_nonce:'twebbleakakakakakak'+new Date().getTime(),
			oauth_signature_method:"HMAC-SHA1",
			oauth_timestamp:Math.round(new Date().getTime()/1000),
			oauth_consumer_key:this.consumer_key,
			oauth_version:'1.0'
		}

		if (this.token != ''){

			auth.oauth_token = this.token
		}
		else {

			auth.oauth_callback = this.callback
		}

		var d = {}

		for (i in auth){

			d[i] = auth[i]
		}

		
		for (i in parameters){

			d[i] = parameters[i]
		}
		


		for (i in body){

			d[i] = body[i]
		}

		var k = Object.keys(d)
		k.sort()
		
		var ps = ''

		for (a in k){

			ps += encodeURIComponent(k[a])+'='+encodeURIComponent(d[k[a]])+'&'
		}

		ps = ps.substring(0, ps.length-1)

		var s = method+'&'+encodeURIComponent(u)+'&'+encodeURIComponent(ps)

		var signing_key = this.consumer_secret+'&'
		if (this.token_secret != '') signing_key += this.token_secret
		

		auth.oauth_signature = CryptoJS.HmacSHA1(s, signing_key).toString(CryptoJS.enc.Base64)


		var aHeader = 'OAuth '
		var sort = Object.keys(auth).sort()
		for (i in sort){

			aHeader += encodeURIComponent(sort[i])+'="'+encodeURIComponent(auth[sort[i]])+'", '
		}
		
		aHeader = aHeader.substring(0, aHeader.length-2)


		u += '?'+serialize(parameters)
		var b = null
		if (method == 'POST'){

			b = serialize(body)
		}
		req.open(method, u, true)
		console.log('Requesting '+u)
		req.setRequestHeader('Authorization', aHeader)
		
		req.send(b)
	}	

	
}

Twitter.prototype.auth = function (){

	var that = this
	t.doRequest('POST', '/oauth/request_token', false, {}, {}, function (e){


	if (this.readyState == 4) {
	if(this.status == 200) {


	   var args = deserialize(this.responseText)

	   that.token = args.oauth_token
	   that.token_secret = args.oauth_token_secret
	   that.openAuth()
	        
		 }
		 else { console.log("Error"); }
		}
	})

}

Twitter.prototype.openAuth = function (){

	var url = this.baseurl+'/oauth/authenticate?oauth_token='+this.token
	
	Pebble.openURL(url)
}

Twitter.prototype.getAccessToken = function (auth_ver, cb){

	var that = this
	t.doRequest('POST', '/oauth/access_token', false, {}, {'oauth_verifier':auth_ver}, function (e){

		if (this.readyState == 4) {
        if(this.status == 200) {

        	var args = deserialize(this.responseText)
        	that.saveConfig(args)
        	cb()
       
     	 	}
     	}
	})
}
Twitter.prototype.saveConfig = function (config){

	for (i in config){

		localStorage.setItem(i, config[i])
	}

	this.reload()
}

Twitter.prototype.logout = function (){

	for (i in this.localProps){

		localStorage.setItem(this.localProps[i], '')
	}

	this.reload()
}

Twitter.prototype.reload = function (){

	this.token = localStorage.getItem('oauth_token') ||''
	this.token_secret = localStorage.getItem('oauth_token_secret') ||''
	this.screen_name = localStorage.getItem('screen_name') ||''
	this.user_id = localStorage.getItem('user_id') ||''
}

//APP RESOURCES

Twitter.prototype.getTimeline = function (cb){

	this.doRequest('GET', '/statuses/home_timeline.json', true, {count:25}, {}, function (e){

		if (this.readyState == 4 && this.status == 200){
			
			var data = JSON.parse(this.responseText);
			var tweets = []

			for (i in data){

				var t = data[i]
				var tweet = {text:tweetsRegex(t['text']), screen_name:'@'+t['user']['screen_name'], name:t['user']['name']};
			
				tweets.push(tweet)
			}

			cb(tweets);
			
		}
		else {

			console.log('SHIT '+this.status);
		}
	})
}

Twitter.prototype.getMentions = function (cb){

	this.doRequest('GET', '/statuses/mentions_timeline.json', true, {count:25}, {}, function (e){

		if (this.readyState == 4 && this.status == 200){
			
			var data = JSON.parse(this.responseText);
			var tweets = []

			for (i in data){

				var t = data[i]
				var tweet = {text:tweetsRegex(t['text']), screen_name:'@'+t['user']['screen_name'], name:t['user']['name']};
			
				tweets.push(tweet)
			}

			cb(tweets);
			
		}
	})
}

