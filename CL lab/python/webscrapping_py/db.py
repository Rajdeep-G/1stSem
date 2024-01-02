
from email import header
import requests
import urllib.request
import sqlite3
import json
from bs4 import BeautifulSoup
import random as rd

def getData(url):
    response = requests.get(url)
    return response.text

def convertJson(data):
    return json.loads(data)

def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con

dbName = 'sample.db'
cursor,conn=createDatabaseConnect(dbName)
query = "CREATE TABLE IF NOT EXISTS sample(r1,r2,name)"
cursor.execute(query)

query = "INSERT INTO sample VALUES('%s', '%s','%s')"%('1','2','hello')
cursor.execute(query)
query = "INSERT INTO sample VALUES('%s', '%s','%s')"%('1','4','hello2222')
cursor.execute(query)
query = "INSERT INTO sample VALUES('%s', '%s','%s')"%('3','5','hello333')
cursor.execute(query)

# query = "SELECT * from sample"
# result = cursor.execute(query)
# for row in result:
#     print(row)
l1=['1','2']
query=f"SELECT name from sample WHERE r1 IN {tuple(l1)}"
result = cursor.execute(query)
print(result.fetchall())