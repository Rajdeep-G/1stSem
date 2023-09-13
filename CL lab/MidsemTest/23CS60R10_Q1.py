import nltk
import string
import numpy as np
import pandas as pd
import math
from nltk.corpus import stopwords
from nltk.stem import WordNetLemmatizer
from nltk.corpus import inaugural


def remove_punctuation(data):
    for i in range(len(data)):
        data[i] = data[i].translate(str.maketrans('', '', string.punctuation))
    return data


def remove_stopwords(sentence, stop_words):
    return [word for word in sentence if word not in stop_words]


def lemmatize_sentence(sentence, lemmatizer):
    return [lemmatizer.lemmatize(word) for word in sentence]

def preprocess(document):
    # sentences = nltk.sent_tokenize(document)
    sentences=document
    remove_punctuation(sentences)  # remove punctuations

    sentences = [sentence.lower() for sentence in sentences]

    tokenized_sentences = [nltk.word_tokenize(sentence) for sentence in sentences]  # tokenize into words

    stop_words = set(stopwords.words('english'))
    filtered_sentences = [remove_stopwords(sentence, stop_words) for sentence in tokenized_sentences]  # Remove stopwords

    lemmatizer = WordNetLemmatizer()
    lemmatized_sentences = [lemmatize_sentence(sentence, lemmatizer) for sentence in filtered_sentences]  # Lemmatize using WordNetLemmatizer
    # print(lemmatized_sentences)
    return lemmatized_sentences, sentences

def tf_idf(lemmatized_sentences):

    tf_idf = []

    processed_sentences = [' '.join(sentence)for sentence in lemmatized_sentences]

    for sentence in lemmatized_sentences:
        tf_idf.append(dict.fromkeys(sentence, 0)) # Create a dictionary for each sentence
    
    for i in range(len(lemmatized_sentences)):
        for word in lemmatized_sentences[i]: # Calculate the term frequency
            tf_idf[i][word] += 1  # Increment the count of each word
    
    for i in range(len(lemmatized_sentences)): 
        for word in tf_idf[i]:
            idf=math.log(len(lemmatized_sentences)/sum([1 for sentence in lemmatized_sentences if word in sentence])) # Calculate the inverse document frequency
            tf_idf[i][word] *= idf 
   
    
    
    tfidf_matrix = pd.DataFrame(tf_idf)
    tfidf_matrix = tfidf_matrix.fillna(0)
    tfidf_matrix = tfidf_matrix.to_numpy()

    tf_idf_user=(tfidf_matrix[-1])

    tfidf_matrix = tfidf_matrix.T
    tf_idf_pd = pd.DataFrame(tfidf_matrix)

    return tfidf_matrix, processed_sentences,tf_idf_user


def annote_sen(tfidf_matrix, sentences):
    cosine_sim_matrix = np.zeros((tfidf_matrix.shape[0], tfidf_matrix.shape[0]))
    for i in range(tfidf_matrix.shape[0]):
        for j in range(tfidf_matrix.shape[0]):
            if i != j:
                cosine_sim_matrix[i][j] = np.dot(tfidf_matrix[i], tfidf_matrix[j])/(
                    np.linalg.norm(tfidf_matrix[i])*np.linalg.norm(tfidf_matrix[j]))

    # print("s")
    dr=[]
    dnr=[]

    #storin the index of sentences that are relvenat and that are not relevant
    for i in range(len(sentences)-1):
            if((cosine_sim_matrix[i, -1])>0.02):
                dr.append(i)
            else:
                dnr.append(i)

    return dr , dnr

def sim(v1,v2): #function to store similarity
    dot=np.dot(v1,v2)
    nv1=np.linalg.norm(v1)
    nv2=np.linalg.norm(v2)
    sim=dot/(nv1*nv2)
    return sim

def upd_Rocchio_and_find_top5(dr,dnr,tfidf_matrix,tf_idf_user, sentences):
    alpha=1
    beta=0.75
    gamma=0.15
    # print(len(tfidf_matrix.T))
    dr_tfidf=[]
    for i in dr:
        dr_tfidf.append(tfidf_matrix.T[i])
    dnr_tfidf=[]
    for i in dnr:
        dnr_tfidf.append(tfidf_matrix.T[i])

    dr_sum=np.sum(dr_tfidf,axis=0) #calculating the sum of the dr and dnr
    dnr_sum=np.sum(dnr_tfidf,axis=0)
    q_i_upd=(alpha*tf_idf_user) + beta*((dr_sum/len(dr)))-gamma*(dnr_sum/len(dnr))
    
    res={}

    for i in range(len(tfidf_matrix.T)):
        wt=sim(tfidf_matrix.T[i],q_i_upd)
        res[i]=wt

    keys=list(res.keys())
    values=list(res.values())

    sorted_index=np.argsort(values)
    sorted_res={keys[i]: values[i] for i in sorted_index}
    rev=dict(reversed(list(sorted_res.items())))

    i=0
    for k,v in rev.items():
        if i==5:
            break
        i+=1
        print(' '.join(sentences[k]))
        
    return q_i_upd


trump_speech_id=[file_id for file_id in inaugural.fileids() if "2017" in file_id and "Trump" in
file_id][0]

trump_speech_words=inaugural.words(trump_speech_id)
# print(trump_speech_words)


temp=[]
file=[]
for i in trump_speech_words:
    temp.append(i)
    if temp[-1]=='.':
        file.append(' '.join(temp))
        temp=[]


print("USER QUERY:")
user_q=input()
# user_q="America's future prosperity depends on unity and strong leadership"
# user_q="America is the world's greatest nation"
print("Top 5 query")
file.append(user_q)
lemmatized_sentences, sentences=preprocess(file)
tfidf_matrix, processed_sentences,tf_idf_user= tf_idf(lemmatized_sentences)
dr,dnr=annote_sen(tfidf_matrix, sentences)
upd_user_q=upd_Rocchio_and_find_top5(dr,dnr,tfidf_matrix,tf_idf_user, lemmatized_sentences)