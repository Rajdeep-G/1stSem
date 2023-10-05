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

def all_countries(soup,year):
    if(year==1976):
        table=soup.find_all('table',class_='collapsible')[1]
    elif (year==2012):
        table=soup.find_all('table',class_='wikitable')[1]
    elif (year==2020):
        table=soup.find_all('table',class_='wikitable')[2]
    else:
        table=soup.find_all('table',class_='collapsible')[0]
    # print(len(table))
    all_part_countries=[]
    # print the first table content
    for row in table.tbody.find_all('tr')[1].td.div.ul.find_all('li'):
        all_part_countries.append((row.text.split('(')[0]))
        
    for i in range(len(all_part_countries)):
        all_part_countries[i]=all_part_countries[i].replace('\xa0','')

    return all_part_countries

def all_sports(soup):
    all_sports=[]
    all_sports2=[]
    span_tag = soup.find('span', text='Calendar')
    h3_tag = span_tag.parent
    second_table = h3_tag.find_next('table').find_next('table')

    for row in second_table.tbody.find_all('tr')[2:]:
        td=row.find('td')
        if td is not None:
            all_sports.append(td.text)

    for i in range(len(all_sports)):
        all_sports2.append(all_sports[i][0:len(all_sports[i])-2].strip())

    return all_sports2

def rank_top3(soup):
    table=soup.find_all('table',attrs={'class':['plainrowheaders']})[0]
    rank=[]
    for row in table.tbody.find_all('tr')[1:4]:
        try:
            th=row.find_all('th')
            nation_name=th[0].text.strip()
        except:
            td=row.find_all('td')
            nation_name=td[0].text.strip()
        rank.append(nation_name.split('\xa0')[0])

    return rank

########################################################################################################################

def rem_info(url,olympic_info_year,olympic_yr):

    url_sp=url
    user_agent = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64)'
    req=urllib.request.Request(
    url_sp,
    data=None,
    headers = {'User-Agent': user_agent}
    )
    text=urllib.request.urlopen(req).read().decode('utf-8')
    soup = BeautifulSoup(text, 'html.parser')

    olympic_info_year.append(all_countries(soup,olympic_yr))
    olympic_info_year.append(all_sports(soup))

    rank=rank_top3(soup)
    for i in range(3):
        olympic_info_year.append(rank[i])


    return olympic_info_year

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

for _ in range(3):
    os.system("python scraper.py &")
# input("Press enter to exit: ")
time.sleep(5)
exit()

# subprocesses = []
# for _ in range(3):
#     process = subprocess.Popen(['python', 'scraper.py'])
#     subprocesses.append(process)

# for process in subprocesses:
#     process.wait()

