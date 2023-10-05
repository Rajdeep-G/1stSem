from email import header
import requests
import urllib.request
import sqlite3
import json
from bs4 import BeautifulSoup
import random as rd
import os
import subprocess

def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con



dbName='OlympicsData_partC.db'
cur,conn=createDatabaseConnect(dbName)

print("SUC")

query="SELECT * FROM partC"
cur.execute(query)
row=cur.fetchall()
print(row)


query="SELECT * FROM partC WHERE done_not_done ='0'"
cur.execute(query)
row=cur.fetchone()
if row is None:
    conn.close()
    exit()

selected_row_id=row[1]
query=f"UPDATE partC SET done_not_done = '1' WHERE id = {selected_row_id}"
cur.execute(query)
conn.commit()


# display the entire row that got updated
query=f"SELECT * FROM partC WHERE id = {selected_row_id}"
cur.execute(query)
row=cur.fetchone()
print(row)
conn.close()

# exit()