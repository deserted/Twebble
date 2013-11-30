var serialize = function(obj) {
  var str = [];
  for(var p in obj)
     str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
  return str.join("&");
}
var deserialize = function (str){

	return JSON.parse('{"' + decodeURI(str).replace(/"/g, '\\"').replace(/&/g, '","').replace(/=/g,'":"') + '"}')
}

var jobs = 0;
var send = function(data) {
	
	setTimeout(function() {
		
		Pebble.sendAppMessage(data);
		jobs--;
	}, jobs++*100);
}

var tweetsRegex = function (t){

	var urlReg = /(\b(https?|ftp|file):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig;
	var nlReg = /\n/g;

	return t.replace(urlReg, "[link]").replace(nlReg, "  ")
}