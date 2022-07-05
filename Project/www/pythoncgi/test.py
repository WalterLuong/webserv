print ("Content-type: text/html;\r\n\r\n")
print ("<html><head><title>Répertoire local</title></head><body>")
import cgitb
cgitb.enable()
import os
print ("Le fichier CGI se trouve dans :")
print (os.path.dirname(__file__))
print ("</body></html>")