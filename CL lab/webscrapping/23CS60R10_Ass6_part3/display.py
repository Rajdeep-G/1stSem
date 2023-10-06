
import sqlite3

def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con


dbName = 'OlympicsData.db'
cursor,conn=createDatabaseConnect(dbName)

query = "SELECT * from SummerOlympics"
result = cursor.execute(query)
for row in result:
    print(row, end="\n\n")