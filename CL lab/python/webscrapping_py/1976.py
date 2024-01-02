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

def test(yr):

    user_agent = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64)'

    url = f'https://en.wikipedia.org/wiki/{yr}_Summer_Olympics'
    req=urllib.request.Request(
        url,
        data=None,
        headers = {'User-Agent': user_agent}

    )

    text=urllib.request.urlopen(req).read().decode('utf-8')
    soup = BeautifulSoup(text, 'html.parser')

    table=soup.find_all('table',class_='collapsible')[1]
        # print(len(table))
    all_part_countries=[]
        # print the first table content
    # x=table.tbody.find_all('tr')[1].td
    # print(x.text)
    for row in table.tbody.find_all('tr')[1].td.div.ul.find_all('li'):
        all_part_countries.append((row.text.split('(')[0]))
            
    for i in range(len(all_part_countries)):
        all_part_countries[i]=all_part_countries[i].replace('\xa0','')

    print(all_part_countries)

# for i in range(2016,2024,4):
# test(2012) 
# test(2020)
test(1976)

# 1976 - 2nd table [1]
# 2012 - wikitable [1]
# 2020-  wikitable [2]