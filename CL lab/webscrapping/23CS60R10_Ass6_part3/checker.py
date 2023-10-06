import sqlite3
from collections import Counter
def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con


dbName = 'OlympicsData.db'
cursor,conn=createDatabaseConnect(dbName)


# check if all the values in done_not_done column are 1
query = "SELECT done_not_done from SummerOlympics"
result = cursor.execute(query)
c=0
for row in result:
    if row[0] == '0':
        print("Not all rows are filled")
        c=1
        break
if c==0:
    print("All rows are filled")


########################################################################################################################


# QUERY1 :What are the years you chose?
query = "SELECT year from SummerOlympics"
result = cursor.execute(query)
column_data = cursor.fetchall()
print("\nThe years you chose are:")
for row in column_data:
    print(row[0], end=" ")
print()

########################################################################################################################
# QUERY2 :Which country was with in top3 for the maximum time in your database?
query = "SELECT r1,r2,r3 from SummerOlympics"
result = cursor.execute(query)
rows = cursor.fetchall()
allCountries = []
for row in rows:
    for i in range(3):
        allCountries.append(row[i])

counter = Counter(allCountries)
top3 = counter.most_common(1)
print("\nThe top most countries are: ",top3[0][0])


########################################################################################################################
# QUERY3 :What is the average number of athletes
query = "SELECT no_atheletes from SummerOlympics"
result = cursor.execute(query)
rows = cursor.fetchall()
count=0
sum=0
for row in rows: 
    sum+=int(row[0])
    count+=1
print("\nThe average number of athletes is:",round((sum/count)))

conn.close()