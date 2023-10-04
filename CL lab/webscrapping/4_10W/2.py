from email import header
import requests
import urllib.request
import sqlite3
import json
from bs4 import BeautifulSoup
import re

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
    

## Now lets get some unstructured data
user_agent = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64)'
url = 'https://en.wikipedia.org/wiki/Summer_Olympic_Games'

req=urllib.request.Request(
    url,
    data=None,
    headers = {'User-Agent': user_agent}

)
text=urllib.request.urlopen(req).read().decode('utf-8')
# print(text)
# html_doc = getData(url)
# print(html_doc)
## it will print 
## <html>
## ...
## which in unstructured data, cannot be converted into json :( 
## bs4 to the rescue
# ## see the documentation here: 
soup = BeautifulSoup(text, 'html.parser')
for table in soup.find_all('table'):
    print(table.get('class'))

texts=soup.find_all('table',class_='sortable wikitable')
# print(texts[0])


print("............................")
for row in soup.findAll('table',class_='sortable wikitable')[0].tbody.findAll('tr'):
    # first_column = row.findAll('th')[0].contents
    # third_column = row.findAll('td').contents
    print(row.findAll('td'))