from email import header
import requests
import urllib.request
import sqlite3
import json
from bs4 import BeautifulSoup
import random as rd
import os
import sys
import time
import subprocess

def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con


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

def inserting_data(olym_url):
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
    # print(alldata_p1[0])
    olympic_yr=0
    for i in range(len(alldata_p1)):
        if alldata_p1[i][1]==olym_url:
            olympic_yr=alldata_p1[i][2]
            break
    # print(olympic_yr)
    index=(olympic_yr-1968)//4
    reqd_all_info=rem_info(olym_url,alldata_p1[index],olympic_yr)
    return reqd_all_info


########################################################################################################################

def insert_into_db(all_data,cur,conn):
    try:
        Name=all_data[0]
        year=str(all_data[2])
        host=all_data[3]
        no_atheletes=str(all_data[4])
        participatingNations=", ".join(all_data[5])
        sports_list=", ".join(all_data[6])
        r1=all_data[7]
        r2=all_data[8]
        r3=all_data[9]
        query=f"UPDATE SummerOlympics SET Name='{Name}', year='{year}', host='{host}', no_atheletes='{no_atheletes}', participatingNations='{participatingNations}', sports_list='{sports_list}', r1='{r1}', r2='{r2}', r3='{r3}' WHERE WikiURL='{all_data[1]}'"
        cur.execute(query)

    except:
        print("error")
    
    conn.commit()

########################################################################################################################



dbName='OlympicsData.db'
cur,conn=createDatabaseConnect(dbName)

def multi_p():

    query = "SELECT wikiURL FROM SummerOlympics WHERE done_not_done = '0' LIMIT 1"
    cur.execute(query)
    row=cur.fetchone()
    # print(row)
    if row is None:
        conn.close()
        print("No more rows to be filled")
        conn.close()
        start_time=float(sys.argv[1])
        ex_time=time.time()-start_time
        print(ex_time)
        with open("execution_time.txt", "a") as f:
            f.write(str(ex_time)+"\n")
        exit()

    selected_row_id=row[0]
    cur.execute("SELECT * FROM SummerOlympics WHERE wikiURL = ?", (selected_row_id,))
    row=cur.fetchone()


    cur.execute("UPDATE SummerOlympics SET done_not_done = '1' WHERE wikiURL = ?", (selected_row_id,))
    conn.commit()


    url=row[1]
    print(url)
    ans=inserting_data(url)
    # print(ans)
    insert_into_db(ans,cur,conn)

    # query="SELECT * FROM partC"
    # cur.execute(query)
    # row=cur.fetchall()
    # print(row)

while True:
    multi_p()
    

########################################################################################################################