import cgi, cgitb, time
import os

form = cgi.FieldStorage() 

# fname = form.getvalue('empty')
# lname = form.getvalue('name')
# print ("HTTP/1.1 400 OK\n")
print ("Content-type:text/html\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello fibonachi  </h2>")
print(os.environ)

print ("</body>")
print ("</html>")