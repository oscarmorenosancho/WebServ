
# i=0
# while [ $i -lt 2147483647 ]; do
# 	((i++))
# done

echo "<h1>Made in Bash though CGI</h1>"
echo "<h3>This is the environment</h3>"
# export
echo "<div style="background-color:AA9988">"
echo "<p>SERVER_SOFTWARE:" $SERVER_SOFTWARE "</p>"
echo "<p>SERVER_NAME:" $SERVER_NAME "</p>"
echo "<p>GATEWAY_INTERFACE:" $GATEWAY_INTERFACE "</p>"
echo "<p>SERVER_PROTOCOL:" $SERVER_PROTOCOL "</p>"
echo "<p>SERVER_PORT:" $SERVER_PORT "</p>"
echo "<p>REQUEST_METHOD:" $REQUEST_METHOD "</p>"
echo "<p>PATH_INFO:" $PATH_INFO "</p>"
echo "<p>PATH_TRANSLATED:" $PATH_TRANSLATED "</p>"
echo "<p>SCRIPT_NAME:" $SCRIPT_NAME "</p>"
echo "<p>QUERY_STRING:" $QUERY_STRING "</p>"
echo "<p>REMOTE_HOST:" $REMOTE_HOST "</p>"
echo "<p>REMOTE_ADDRESS:" $REMOTE_ADDRESS "</p>"
echo "<p>AUTH_TYPE:" $AUTH_TYPE "</p>"
echo "<p>REMOTE_USER:" $REMOTE_USER "</p>"
echo "<p>REMOTE_IDENT:" $REMOTE_IDENT "</p>"
echo "<p>CONTENT_LENGTH:" $CONTENT_LENGTH "</p>"
echo "<p>HTTP_ACCEPT:" $HTTP_ACCEPT "</p>"
echo "<p>USER_AGENT:" $USER_AGENT "</p>"
echo "<p>__CF_USER_TEXT_ENCODING:" $__CF_USER_TEXT_ENCODING "</p>"
echo "<p>LC_CTYPE:" $LC_CTYPE "</p>"
echo "</div>"
