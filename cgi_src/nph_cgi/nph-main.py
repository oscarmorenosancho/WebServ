import sys
import os
# sys.stderr.write("Executing python start->\n")

def getBody():
	env = ""
	for var in os.environ:
		env += "<p>" + var + ": " + os.environ[var] + "</p>\r\n"
	evalBody = "<body>\r\n"
	evalBody += "<h1 style=\"color: #F00F0F;\">This is the body given by python</h1>\r\n"
	evalBody += "<h3 style=\"color: #000F0F;\">This is the environment</h3>\r\n"
	evalBody += "<div>\r\n"
	evalBody += "<details close>\r\n"
	evalBody += "<summary>Environment Variables</summary>\r\n"
	evalBody += env
	evalBody += "</div>\r\n"
	evalBody += "</details>\n"
	userCookie = os.environ["HTTP_COOKIE"]
	userAr = userCookie.split("[")
	userCookie = userAr[-1].split("]")[0]
	userAr = userCookie.split("&")
	userCookie = userAr[0]
	userAr = userCookie.split("=")
	userCookie = userAr[-1]
	if (userCookie != "" ):
		evalBody += "<h3>Current User: " + userCookie + "</h3>\r\n"
		evalBody += "<button onclick=\"document.cookie = 'sessionId=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;'; location.reload();\">LOGOUT</button>\r\n"
	else:
		evalBody += "<form method=\"POST\" action=\"/cgi_src/nph_cgi/loginAction.py\" style=\"display: flex; flex-direction: column; max-width:20em; margin: auto; padding: 1em; border: solid black 1px\">\r\n"
		evalBody += "<h3>Login Form</h3>\r\n"
		evalBody += "<div style=\"max-width:18em; margin: auto; padding: 1em;\">\r\n"
		evalBody += "<label for=\"user\">user</label>\r\n"
		evalBody += "<input type=\"user\" id=\"user\" name=\"user\"\>\r\n"
		evalBody += "</div>\r\n"
		evalBody += "<div style=\"max-width:18em; margin: auto; padding: 1em;\">\r\n"
		evalBody += "<label for=\"pass\">password</label>\r\n"
		evalBody += "<input type=\"password\" id=\"pass\" name=\"pass\"\>\r\n"
		evalBody += "</div>\r\n"
		evalBody += "<input type=\"submit\" style=\"width:16em; self-justify: center; margin: auto; padding: 1em;\" id=\"submit\"\>\r\n"
		evalBody += "</form>\r\n"
	evalBody += ("</body>\r\n")
	return evalBody

value = sys.stdin.readline()
acum = ""
body = getBody()
acum += os.environ["SERVER_PROTOCOL"] + " 200 OK\r\n"
acum += "Server: " + os.environ["SERVER_NAME"] + "\r\n"
acum += "Content-Type: text/html\r\n"
acum += "Content-Length: " + str(len(body)) + "\r\n"
acum += "Location: "+ os.environ["SERVER_NAME"] +":" + os.environ["SERVER_PORT"] + "/\r\n"
acum += "\r\n"
acum += body
sys.stdout.write(acum)
# sys.stderr.write("Written to webserv: ")
# sys.stderr.write(acum)
# sys.stderr.write("Executing python finished-<\n")
