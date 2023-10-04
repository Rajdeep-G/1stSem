import requests
import sqlite3
import json
from bs4 import BeautifulSoup
import random
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


def alldata(jsonData):
    city=jsonData['name']
    temperature=jsonData['main']['temp']
    description=jsonData['weather'][0]['description']
    humidity=jsonData['main']['humidity']
    windSpeed=jsonData['wind']['speed']

    return city,temperature,description,humidity,windSpeed



def lat_long(k):
    input=[]
    for i in range(k):
        latitude = round(random.uniform(-90, 90), 2)
        longitude = round(random.uniform(-180, 180), 2)
        input.append((latitude,longitude))
    return input

def fetch_store(url,cursor,conn):
    url=url
    returnedData = getData(url)
    jsonData = convertJson(returnedData)

    city, temperature, description,humidity,windSpeed=alldata(jsonData)

 
    
    query = "CREATE TABLE IF NOT EXISTS city_weather(city PRIMARY KEY, temp, desc,hum,wspd)"
    cursor.execute(query)
    try:
        query = "INSERT INTO city_weather VALUES ('%s', '%s', '%s', '%s', '%s')"%(city, temperature, description,humidity,windSpeed)
        cursor.execute(query)
    except:
        print("this city already exists or some error in the fetching of API has been caused")

    conn.commit()



k=3
dbName = 'weather.db'
cursor,conn = createDatabaseConnect(dbName)
data=lat_long(k)
for i in range(k):
    a,b=data[i]
    url = f'https://api.openweathermap.org/data/2.5/weather?lat={a}&lon={b}&appid=a56f7b3d62592d9404c4312793049d2c'
    fetch_store(url,cursor,conn)
    print(url)

    ## Lets see what is in the table
query = "SELECT * from city_weather"
result = cursor.execute(query)
for row in result:
    print(row)
cursor.close()