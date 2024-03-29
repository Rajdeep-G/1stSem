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

########################################################################################################################


user_agent = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64)'
url = 'https://en.wikipedia.org/wiki/Summer_Olympic_Games'
req=urllib.request.Request(
    url,
    data=None,
    headers = {'User-Agent': user_agent}

)
text=urllib.request.urlopen(req).read().decode('utf-8')

soup = BeautifulSoup(text, 'html.parser')
# for table in soup.find_all('table'):
#     print(table.get('class'))

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
# print(alldata_p1)
k=2
olympic_info=rd.sample(alldata_p1,k)


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


for i in range(len(olympic_info)):
    url=olympic_info[i][1]
    year=olympic_info[i][2]
    per_yr_info=rem_info(url,olympic_info[i],year)
    olympic_info[i]=per_yr_info

print(olympic_info)
########################################################################################################################

# for i in range(10):
#     print(type(olympic_info[0][i]))


dbName = 'OlympicsData.db'
cursor,conn=createDatabaseConnect(dbName)
query = "CREATE TABLE IF NOT EXISTS SummerOlympics(Name,WikiURL,year PRIMARY KEY, host, no_atheletes,participatingNations, sports_list,r1,r2,r3)"
cursor.execute(query)

for i in range(len(olympic_info)):
    try:
        Name=olympic_info[i][0]
        WikiURL=olympic_info[i][1]
        year=str(olympic_info[i][2])
        host=olympic_info[i][3]
        no_atheletes=str(olympic_info[i][4])
        participatingNations=", ".join(olympic_info[i][5])
        sports_list=", ".join(olympic_info[i][6])
        r1=olympic_info[i][7]
        r2=olympic_info[i][8]
        r3=olympic_info[i][9]
        query = "INSERT INTO SummerOlympics VALUES ('%s', '%s', '%s', '%s', '%s','%s','%s','%s','%s','%s')"%(Name,WikiURL,year,host,no_atheletes,participatingNations,sports_list,r1,r2,r3)     
        cursor.execute(query)
    except:
        print("this year already exists or some error in the fetching of API has been caused")

    # conn.commit()



query = "SELECT * from SummerOlympics"
result = cursor.execute(query)
for row in result:
    print(row)


########################################################################################################################

# Query 1 What are the years you chose?
query="SELECT year FROM SummerOlympics"
result = cursor.execute(query)
column_data = cursor.fetchall()
yr=[]
print("SELECTED YEARS: ")
for row in column_data:
    yr.append(row[0])
    print(row[0])


# Query 2 What is the average number of countries participating in the two olympics?
query="SELECT participatingNations FROM SummerOlympics"
result = cursor.execute(query)
rows = cursor.fetchall()
count=0
sum=0
for row in rows: 
    sum+=len(row[0].split(', '))
    count+=1

print("average- ",(sum/count))

#Query 3 Print the overlap (i.e., common nations) within <Rank_1_nation, Rank_2_nation and
# Rank_3_nation> for your chosen two years.
columns = ['r1', 'r2', 'r3'] 
query="SELECT r1,r2,r3 FROM SummerOlympics"
result = cursor.execute(query)
rows = cursor.fetchall()
cursor.close()

if len(rows) >= 2:
    row1 = list(rows[0])
    row2 = list(rows[1])
    intersection = []

    for col1, col2 in zip(row1, row2):
        if col1 == col2:
            intersection.append(col1)

    print("Intersection values:", intersection)
else:
    print("There are not enough rows to find an intersection.")




