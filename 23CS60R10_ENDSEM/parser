from email import header
import requests
import urllib.request
import sqlite3
import json
from bs4 import BeautifulSoup
import random as rd
import sys

######################################################################################################################

def extract_store(filename,output_filename):
    filename='/home/user/Desktop/r10/ENDexam/files/'+filename
# with open('files/amazon-com-inc.html','r') as file:
    # with open('files/nvidia-corp.html','r') as file:
    with open(filename,'r') as file:
        html_content=file.read() #reading from file

    soup=BeautifulSoup(html_content,'html.parser')
    # print(soup.prettify())

    name_tag=soup.find('h1')
    name=name_tag.text.strip() #name of the stock
    print(name)

    # print(name_tag.parent.parent.parent)

    reqd_div=soup.find('div',attrs={"data-test":['instrument-header-details']})
    stock_prize=reqd_div.find_all('div')[0].find_all('div')[0]

    stock_prize=float(stock_prize.text.strip().replace(",","")) #stock prixe
    

    print(stock_prize)

    with open(output_filename,'a') as f:
        print(name,file=f)
        print(stock_prize,file=f)

if len(sys.argv)==3:
    html_fname=sys.argv[1]
    output_file=sys.argv[2] 
else:
    print("USAGE python3 filename.py htmlfilename outputfilename")
    exit()
extract_store(html_fname,output_file)