var http = require('http');

var server = http.createServer(function (request, response) {
  response.writeHead(200, {"Content-Type": "text/plain"});
  response.end("Groene stroom, direct frituren!\n");
});
server.listen(80);
console.log("Server running at http://192.168.1.104:80/");




