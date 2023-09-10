import nltk
import networkx as nx
import string
import numpy as np
import pandas as pd
import math
from nltk.corpus import stopwords
from nltk.stem import WordNetLemmatizer


with open('input.txt','r') as file :
    doc=file.read() # read the entire file
    
sentences=nltk.sent_tokenize(doc) # split the document into sentences
# print(len(sentences))

x='The Indian economy is a complex and dynamic entity that hasn\'t undergone significant transformations over the years.'
x=x.translate(str.maketrans('', '', string.punctuation))
print(x)

x=x.lower()
print(x)

tokenized=nltk.word_tokenize(x)
print(tokenized)

stopwords=set(stopwords.words('english'))
sen_without_stopwords=[i for i in tokenized if i not in stopwords]

print(sen_without_stopwords)

lem=WordNetLemmatizer()
lemmatized=[lem.lemmatize(i) for i in sen_without_stopwords]
print(lemmatized)