console.error("Finish NodeJS script <--")
console.log("Content-Type: text/html\r");
console.log("Status: 200 OK\r");
console.log("Location: localhost:8000/\r");
console.log("Set-Cookie: sessionId=nodejs38afes7a8; Max-Age=2592000\r");
console.log("\r");
console.log("<h1>Rendered with CGI through NODEJS</h1>");
console.log("<div style=\"background-color:9900FF\">");
Object.keys(process.env).map(it => console.log("<p>" + it + ": " + process.env[it] + "</p>"));
console.log("</div>");
console.error("Start NodeJS script -->")
    