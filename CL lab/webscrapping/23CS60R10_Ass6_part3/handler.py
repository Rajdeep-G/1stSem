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
    all_data=rd.sample(all_data,10)
    col1='WikiURL'
    col2='done_not_done'
    for i in range(len(all_data)):
        try:
            query="INSERT INTO SummerOlympics(WikiURL,done_not_done) VALUES(?,?)"
            cursor.execute(query,(all_data[i][1],'0'))
        except:
            pass

    
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

    dbName = 'OlympicsData.db'
    cursor,conn=createDatabaseConnect(dbName)
    query = "CREATE TABLE IF NOT EXISTS SummerOlympics(Name,WikiURL PRIMARY KEY,year, host, no_atheletes,participatingNations, sports_list,r1,r2,r3,done_not_done)"
    cursor.execute(query)
    conn.commit()
    
    add_initial_data_to_db(alldata_p1,cursor,conn)   

    # query = "SELECT * from partC"
    # result = cursor.execute(query)
    # for row in result:
    #     print(row)
    conn.commit()
    conn.close()


########################################################################################################################

os.system("rm OlympicsData.db")


########################################################################################################################

start_time=time.time()
handler_func()
i=0

for i in range(1):
# for _ in range(3):
    os.system(f"python scraper.py {start_time} & ")
    # i+=1
time.sleep(30)

time_all=[]
with open('execution_time.txt','r') as f:
    for line in f:
        time_all.append(float(line.strip()))

# delete the above file
os.system("rm execution_time.txt")

print("Execution time ->  ",max(time_all)," seconds")
exit()

########################################################################################################################

