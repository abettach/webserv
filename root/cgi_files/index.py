name = "web"
lastName = "server"
print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<center><h2 style='color:dodgerblue; font-size:50px; margin-top:400px'>First Name:  " + name + "</h2>")
print ("<h2 style='color:dodgerblue; font-size:50px'>Last  Name:  " + lastName + "</h2></center>")
print ("</body>")
print ("</html>")