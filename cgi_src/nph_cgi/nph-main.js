let body = "<h1>Rendered with CGI through NODEJS</h1>\n";
body += "<div style=\"background-color:9900FF\">\n"
body += Object.keys(process.env).map(it => ("<p>" + it + ": " + process.env[it] + "</p>")).join("\n");
body += "</div>";
console.log(process.env["SERVER_PROTOCOL"] + " 200 OK\r");
console.log("Server: " + process.env["SERVER_NAME"] + "\r");
console.log("Content-Type: text/html\r");
console.log("Content-Length: " + String(body.length) + "\r");
console.log("\r");
console.log(body);
// console.log("<h1>Rendered with CGI through NODEJS</h1>");
// console.log("<div style=\"background-color:9900FF\">");
// for (i=0; i<10000000000; i++)
// {
// 	parseInt(String(i));
// }
// Object.keys(process.env).map(it => console.log("<p>" + it + ": " + process.env[it] + "</p>"));
// console.log("</div>");
