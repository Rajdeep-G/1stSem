import nltk
import networkx as nx
import string
import numpy as np
import pandas as pd
import math
from nltk.corpus import stopwords
from nltk.stem import WordNetLemmatizer
from sklearn.feature_extraction.text import TfidfVectorizer

# nltk.download('punkt')
# nltk.download('wordnet')
# nltk.download('stopwords')


def remove_punctuation(data):
    for i in range(len(data)):
        data[i] = data[i].translate(str.maketrans('', '', string.punctuation))
    return data


def remove_stopwords(sentence, stop_words):
    return [word for word in sentence if word not in stop_words]


def lemmatize_sentence(sentence, lemmatizer):
    return [lemmatizer.lemmatize(word) for word in sentence]


def preprocess_t1(document):
    sentences = nltk.sent_tokenize(document)
    remove_punctuation(sentences)  # remove punctuations

    sentences = [sentence.lower() for sentence in sentences]

    tokenized_sentences = [nltk.word_tokenize(sentence) for sentence in sentences]  # tokenize into words

    stop_words = set(stopwords.words('english'))
    filtered_sentences = [remove_stopwords(sentence, stop_words) for sentence in tokenized_sentences]  # Remove stopwords

    lemmatizer = WordNetLemmatizer()
    lemmatized_sentences = [lemmatize_sentence(sentence, lemmatizer) for sentence in filtered_sentences]  # Lemmatize using WordNetLemmatizer

    return lemmatized_sentences, sentences
    # Print the preprocessed sentences
    # for idx, sentence in enumerate(lemmatized_sentences, 1):
    # print(f"Sentence {idx}: {' '.join(sentence)}")


def tf_idf_t2(lemmatized_sentences):

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

    tfidf_matrix = tfidf_matrix.T
    tf_idf_pd = pd.DataFrame(tfidf_matrix)
    
    with open('output_t2.txt', 'w') as summary_file:
        summary_file.write(tf_idf_pd.to_string(index=True, header=True))

    print("'output_t2.txt' created successfully")
    return tfidf_matrix, processed_sentences


def page_rank_t3(tfidf_matrix, processed_sentences, sentences, n=2):
    # array for cosine similarity matrix
    cosine_sim_matrix = np.zeros((tfidf_matrix.shape[0], tfidf_matrix.shape[0]))
    for i in range(tfidf_matrix.shape[0]):
        for j in range(tfidf_matrix.shape[0]):
            if i != j:
                cosine_sim_matrix[i][j] = np.dot(tfidf_matrix[i], tfidf_matrix[j])/(
                    np.linalg.norm(tfidf_matrix[i])*np.linalg.norm(tfidf_matrix[j]))

    graph = nx.Graph()  # Create a graph

    for idx, sentence in enumerate(processed_sentences):
        graph.add_node(idx, sentences=sentence)  # Add nodes to the graph

    for i in range(len(sentences)):
        for j in range(i+1, len(sentences)):
            graph.add_edge(i, j, weight=cosine_sim_matrix[i, j]) # Add edges to the graph

    # print(graph.size()) 
    pagerank_scores = nx.pagerank(graph)  # Calculate PageRank scores
    sorted_sentences = sorted(pagerank_scores, key=pagerank_scores.get, reverse=True) # Sort the sentences based on their PageRank scores
    summary_indices = sorted_sentences[:n] # Get the top n sentence index no

    summary = [sentences[idx] for idx in sorted(summary_indices)] # Get the top n sentences

    with open('output_t3.txt', 'w') as summary_file:
        summary_file.write('\n'.join(summary))

    print("'output_t3.txt' created successfully")

    return pagerank_scores,cosine_sim_matrix


def MMR_t4(pagerank_scores,cosine_sim_matrix,processed_sentences,sentences,alpha,n=2):

    R=[]
    R_idx=[]
    sorted_sentences = sorted(pagerank_scores, key=pagerank_scores.get, reverse=True)
    R.append(processed_sentences[sorted_sentences[0]])
    R_idx.append(sorted_sentences[0])
    sorted_sentences.remove(sorted_sentences[0])
    x=0
    while len(R)<n:
        scores=[]
        index_track=[]
        for i in range(len(sorted_sentences)):
            scores.append(alpha*pagerank_scores[sorted_sentences[i]]-(1-alpha)*max(cosine_sim_matrix[sorted_sentences[i]][R_idx[j]] for j in range(len(R_idx))))
            index_track.append(sorted_sentences[i])
    
        index_track=scores.index(max(scores))
        R.append(processed_sentences[index_track])
        R_idx.append(index_track)
        sorted_sentences.remove(index_track)
    

        
    summary=[sentences[idx] for idx in R_idx]
    with open('output_t4.txt', 'w') as summary_file:
        summary_file.write('\n'.join(summary))
    print("'output_t4.txt' created successfully")










with open('input.txt', 'r') as file:    # Read the document from the input file
    document = file.read()
# Get the number of sentences to be included in the summary
print("Enter the number of sentences to be included in the summary(part1): ")
n = (int)(input())
# n=2
print("Enter the number of sentences to be included in the MMR summary: ")  
n2=(int)(input())
# n2=2
print("Enter the value of alpha: ")
alpha=(float)(input())
# alpha=0.5

lemmatized_sentences, sentences = preprocess_t1(document)
tfidf_matrix, processed_sentences = tf_idf_t2(lemmatized_sentences)
pagerank_scores,cosine_sim_matrix=page_rank_t3(tfidf_matrix, processed_sentences, sentences, n)
MMR_t4(pagerank_scores,cosine_sim_matrix,processed_sentences,sentences,alpha,n2)