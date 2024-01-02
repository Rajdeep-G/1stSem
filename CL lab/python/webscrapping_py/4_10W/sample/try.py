import requests
import sqlite3
import json
from bs4 import BeautifulSoup

def getData(url):
    response = requests.get(url)
    #convert to text string and return 
    return response.text

def convertJson(data):
    return json.loads(data)

def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con

###########################################################################

# We will call https://cse.iitkgp.ac.in/~mainack/computing-lab/web-scraping/json.php
# Lets see what it returns
url = 'https://cse.iitkgp.ac.in/~mainack/computing-lab/web-scraping/json.php'
returnedData = getData(url)
print(returnedData)
## it will print {"a":"Apple","b":"Ball","c":"Cat"}

## This is structured data which can be converted into json format. 
## lets do that 
jsonData = convertJson(returnedData)
## Now jsonData contains a python dict
print(jsonData["a"])
## will print "Apple"

###########################################################################
## Now lets get some unstructured data

url = 'https://example.com'
html_doc = getData(url)
print(html_doc)
## it will print 
## <html>
## ...
## which in unstructured data, cannot be converted into json :( 
## bs4 to the rescue
## see the documentation here: 
soup = BeautifulSoup(html_doc, 'html.parser')
## Now bs4 will do the job for you
## Say we want to find all text within h1 tags
texts = soup.find_all('h1')
for text in texts:
    print(text.get_text())

###########################################################################
## Lastly lets store the data in a database

## create the database or if it already exisit connect to it

dbName = "tutorial.db"
cursor ,conn= createDatabaseConnect(dbName)

## Now you can create Table and insert/select records from there
## Lets create a Table "example" with three columns a, b and c to insert the structured data 
## we fecthed earlier

query = "CREATE TABLE IF NOT EXISTS example(a, b, c)"
cursor.execute(query)

query = "INSERT INTO example VALUES ('%s', '%s', '%s')"%(jsonData["a"], jsonData["b"], jsonData["c"])
cursor.execute(query)

conn.commit

## Lets see what is in the table
query = "SELECT * from example"
result = cursor.execute(query)
for row in result:
	print(row)
cursor.close()










# pritn multiple values in the database
# if exists part need to corrected 
#database commit