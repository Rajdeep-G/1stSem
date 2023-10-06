
import sqlite3

def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con


dbName = 'OlympicsData_partC.db'
cursor,conn=createDatabaseConnect(dbName)

query = "SELECT * from partC"
result = cursor.execute(query)
for row in result:
    print(row, end="\n\n")