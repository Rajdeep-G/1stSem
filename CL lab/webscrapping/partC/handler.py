from email import header
import requests
import urllib.request
import sqlite3
import json
from bs4 import BeautifulSoup
import random as rd
import os
import subprocess
import time

def getData(url):
    response = requests.get(url)
    return response.text

def convertJson(data):
    return json.loads(data)

def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con

########################################################################################################################
def initial_data(soup):
    table=soup.find_all('table',class_='sortable wikitable')[0]
    # print(table)
    alldata_p1=[]
    fixed_url='https://en.wikipedia.org'
    fixed_part=' Summer Olympics'
    for row in table.tbody.find_all('tr')[2:]:
        td=row.find_all('td')
        # print(td)
        try:
            href=f'{fixed_url}{td[1].a["href"]}'
            td[0].sup.decompose()
            year=int(td[0].text)
            name=td[0].text.strip()+fixed_part
            host=td[2].text.strip()
            no_of_athletes=int(td[5].text)
            if(year<=2020 and year>=1968):
                data_per_yr=[name,href,year,host,no_of_athletes]
                alldata_p1.append(data_per_yr)
            
        except:
            pass
    return alldata_p1


########################################################################################################################

def add_initial_data_to_db(all_data,cursor,conn):
    col1='WikiURL'
    col2='done_not_done'
    for i in range(len(all_data)):
        try:
            # wikiUrl=all_data[i][1]
            # done_not_done='0'
            # # query=f"INSERT INTO partC ({col1}, {col2}) VALUES ('%s','%s')"%(wikiUrl,done_not_done)
            # query=f"INSERT INTO partC ({col1}, {col2}) VALUES ('{wikiUrl}','{done_not_done}')"
            query="INSERT INTO partC(WikiURL,done_not_done) VALUES(?,?)"
            cursor.execute(query,(all_data[i][1],'0'))
        except:
            pass
            # print("error 22")
    
    # conn.commit()

########################################################################################################################

def handler_func():
    user_agent = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64)'
    url = 'https://en.wikipedia.org/wiki/Summer_Olympic_Games'
    req=urllib.request.Request(
        url,
        data=None,
        headers = {'User-Agent': user_agent}

    )
    text=urllib.request.urlopen(req).read().decode('utf-8')
    soup = BeautifulSoup(text, 'html.parser')

    alldata_p1=initial_data(soup)

    dbName = 'OlympicsData_partC.db'
    cursor,conn=createDatabaseConnect(dbName)
    query = "CREATE TABLE IF NOT EXISTS partC(Name,WikiURL PRIMARY KEY,year, host, no_atheletes,participatingNations, sports_list,r1,r2,r3,done_not_done)"
    cursor.execute(query)
    conn.commit()
    
    add_initial_data_to_db(alldata_p1,cursor,conn)   

    # query = "SELECT * from partC"
    # result = cursor.execute(query)
    # for row in result:
    #     print(row)
    conn.commit()
    conn.close()

handler_func()
i=0
while i<6:
    for _ in range(3):
        # if os.system("python scraper.py &")!=-0:
        #     break
        print(os.system("python scraper.py &"))
    i+=1
input("Press enter to exit: ")
# time.sleep(1)
exit()


