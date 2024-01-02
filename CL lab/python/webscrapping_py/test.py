import urllib.request
import re

# URL of the webpage to fetch
url = "https://moodlecse.iitkgp.ac.in/moodle/login/index.php"


headers = {


    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) '
                  'AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36'


}


request = urllib.request.Request(url, headers=headers)

# Fetch the HTML content from the webpage
response = urllib.request.urlopen(request)
html_content = response.read().decode('utf-8')
# print(html_content)
# Use regular expressions to extract the title from the HTML
title_pattern = re.compile(r'<title>(.*?)</title>')
match = title_pattern.search(html_content)

# Print the title
if match:
    # Print the title
    title = match.group(1)
    print("Title:", title)
else:
    print("Title not found on the webpage.")


    # year=[]
# count=0
# for row in soup.findAll('table',class_='sortable wikitable')[0].tbody.findAll('tr'):

#     x=(row.find('td'))
#     print(x)
#     patt=re.compile('<td>(.*?)<sup (.*?)')
#     # print(patt.findall(str(x)))
#     for i in patt.findall(str(x)):
#         if i[0] != '':
#             year.append(i[0])

# final_yr_list=[]
# for i in range(len(year)-3):
#         final_yr_list.append(year[i])

# print(final_yr_list)