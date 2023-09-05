import nltk
import networkx as nx
import string
import numpy as np
import pandas as pd
import math
from nltk.corpus import stopwords
from nltk.stem import WordNetLemmatizer
import random
from sklearn.metrics.pairwise import cosine_similarity

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
    
    with open('output_t2_matrix.txt', 'w') as summary_file:
        summary_file.write(tf_idf_pd.to_string(index=True, header=True))

    print("'output_t2_matrix.txt' created successfully")
    return tfidf_matrix, processed_sentences


def page_rank_t3(tfidf_matrix, processed_sentences, sentences, n=2):
    # array for cosine similarity matrix
    cosine_sim_matrix = np.zeros((tfidf_matrix.shape[0], tfidf_matrix.shape[0]))
    for i in range(tfidf_matrix.shape[0]):
        for j in range(tfidf_matrix.shape[0]):
            if i != j:
                cosine_sim_matrix[i][j] = np.dot(tfidf_matrix[i], tfidf_matrix[j])/(np.linalg.norm(tfidf_matrix[i])*np.linalg.norm(tfidf_matrix[j]))

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

    with open('output_t3_pagerank.txt', 'w') as summary_file:
        summary_file.write('\n'.join(summary))

    print("'output_t3_pagerank.txt' created successfully")

    return pagerank_scores,cosine_sim_matrix


#<!...........................END OF PART 1...............................................!>

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
    with open('output_MMR.txt', 'w') as summary_file:
        summary_file.write('\n'.join(summary))
    print("'output_MMR.txt' created successfully")


#<!...........................END OF PART 2 SUMMARY MMR...............................................!>

with open('input3.txt', 'r') as file:    # Read the document from the input file
    document = file.read()
# Get the number of sentences to be included in the summary
print("Enter the number of sentences to be included in the summary(part1): ")
# n = (int)(input())
n=2
print("Enter the number of sentences to be included in the MMR summary: ")  
# n2=(int)(input())
n2=2
print("Enter the value of alpha: ")
# alpha=(float)(input())
alpha=0.5

print("Enter the number of clusters: ")
# k=(int)(input())
k=3

lemmatized_sentences, sentences = preprocess_t1(document)
tfidf_matrix, processed_sentences = tf_idf_t2(lemmatized_sentences)
pagerank_scores,cosine_sim_matrix=page_rank_t3(tfidf_matrix, processed_sentences, sentences, n)
MMR_t4(pagerank_scores,cosine_sim_matrix,processed_sentences,sentences,alpha,n2)
tfidf_matrix2=tfidf_matrix.T

#<!...........................END OF FUNCTION CALLS of prev part...............................................!>

def cluster_t21(tfidf_matrix, sentences, K, max_iterations=100):
    # Initialize K centroids randomly
    random.seed(42)
    initial_centroids_indices = random.sample(range(tfidf_matrix.shape[0]), K)
    centroids = tfidf_matrix[initial_centroids_indices]

    
    for _ in range(max_iterations): # Perform K-means clustering
        
        cosine_similarities = cosine_similarity(tfidf_matrix, centroids) # Assign each sentence to the nearest centroid based on cosine similarity
        cluster_assignments = np.argmax(cosine_similarities, axis=1)

        
        new_centroids = np.zeros_like(centroids) # Update centroids by calculating the mean of all points in each cluster
        for cluster_idx in range(K):
            cluster_points = tfidf_matrix[cluster_assignments == cluster_idx]
            if cluster_points.shape[0] > 0:
                new_centroids[cluster_idx] = np.mean(cluster_points, axis=0)

        
        if np.all(centroids == new_centroids): # Check for convergence
            break

        centroids = new_centroids #upd

    
    clusters = {} # Store cluster assignments
    for cluster_idx in range(K):
        clusters[cluster_idx] = []

    for i in range(len(sentences)):
        cluster_idx = cluster_assignments[i]
        clusters[cluster_idx].append(sentences[i])
    
    return clusters

def print_clusters(clusters):
    # for idx in range(len(clusters)):
    #     print(f"Cluster {idx + 1} :\n")
    #     for sen in clusters[idx]:
    #         print(f"{sen}")

    with open('output_t21_clusters.txt', 'w') as file:
        file.write("Content for each cluster\n")
        for idx in range(len(clusters)):
            file.write(f"Cluster {idx + 1} :\n")
            k=1
            for sen in clusters[idx]:
                file.write(f"{k}. {sen}\n")
                k+=1
            file.write("\n")

    print("'output_t21_clusters.txt' created successfully")



clusters = cluster_t21(tfidf_matrix2, sentences, k, max_iterations=100)
print_clusters(clusters)

#<!...........................END OF PART 2 CLUSTERing K means...............................................!>


def find_closest_sentence_to_centroid(cluster, tfidf_matrix):
    centroid = np.mean(tfidf_matrix[cluster['indices']], axis=0)
    cosine_similarities = cosine_similarity([centroid], tfidf_matrix[cluster['indices']])[0]
    return cluster['indices'][np.argmax(cosine_similarities)]

def find_sentence_with_common_bigrams(cluster, sentences, s1_idx):
    s1_bigrams = set(cluster['bigrams'][s1_idx])

    for idx, _ in enumerate(sentences):
        if idx==s1_idx:
            continue

        sentence_bigrams = set(cluster['bigrams'][idx])
        common_bigrams = set()

        for bigram in s1_bigrams:
            if bigram in sentence_bigrams:
                common_bigrams.add(bigram)

        if len(common_bigrams) >= 3:
            return idx
    
    return None

def add_node(graph, node):
    if node not in graph:
        graph[node] = []

def add_edge(graph, node1, node2):
    if node1 not in graph:
        graph[node1] = []
    graph[node1].append(node2)

def add_bigram(graph, start_node, end_node, s):
    for i,bigram in enumerate(s):
        if i==0:
            add_edge(graph,start_node,bigram)
        if i==len(s)-1:
            add_edge(graph,bigram,end_node)
        else:
            add_edge(graph,s[i-1],bigram)
    



def construct_sentence_graph(s1, s2):
    start_node = 'start'
    end_node = 'end'
    graph = {'start': []}

    s1_bigrams = s1['bigrams'].split()
    s2_bigrams = s2['bigrams'].split()

    add_bigram(graph, start_node, end_node, s1_bigrams)
    add_bigram(graph, start_node, end_node, s2_bigrams)

    return generate_sentence(graph)

def generate_sentence(graph):
    generated_sentence = []
    current_node = 'start'

    while True:
        next_nodes = graph.get(current_node, [])

        if not next_nodes:
            break

        next_node = random.choice(next_nodes)

        if next_node == 'end':
            break

        generated_sentence.append(next_node)
        current_node = next_node

    final_summary= ' '.join(generated_sentence)
    return final_summary

def summary_t22(clusters, sentences,tfidf_matrix):
    summary=[]
    s1_index_track = []
    for cluster_idx, cluster_sentences in clusters.items():
        indices_dict = {'indices': range(len(cluster_sentences))}
        s1_idx = find_closest_sentence_to_centroid(indices_dict, tfidf_matrix) #find the sentence that is closest to the cluster centroid (S1)
        s1 = cluster_sentences[s1_idx] #the sentence actual
        s1_index_track.append((cluster_idx,sentences.index(s1))) #keeping its track for final ordering
        s2_idx = find_sentence_with_common_bigrams({'bigrams': cluster_sentences}, sentences, s1_idx) #finding sentences with atleast 3 bigrams

        if s2_idx is not None: #if sentence with atleast 3 bigram is found
            s2 = cluster_sentences[s2_idx] # we retrieve that sentence
            sentence_graph = construct_sentence_graph({'bigrams': s1}, {'bigrams': s2}) # forming the graph & then the summary sentence
            summary.append(sentence_graph)
        else:
            print("sentence not found with atleast 3 bigrams")
    
    return summary,s1_index_track

def print_summary(summary):
    # print("Summary: ")
    # for i in range(len(summary)):
    #     print(f'Cl- {i+1}: {summary[i]}')

    
    with open('output_t22_summary.txt', 'w') as file:
        file.write("Summary sentences for each cluster: \n\n")
        for i in range(len(summary)):
            file.write(f'Cl- {i+1}: {summary[i]}\n')
    print("'output_t22_summary.txt' created successfully")



summary,s1_index_track=summary_t22(clusters, sentences,tfidf_matrix.T)
print_summary(summary)

#<!...........................END OF PART 2 summary sentences...............................................!>


def final_ordering(summary,s1_index_track):
    # print(s1_index_track)
    s1_index_track.sort(key=lambda x: x[1])
    final_ordering=[]
    for i in range(len(s1_index_track)):
        final_ordering.append(summary[s1_index_track[i][0]])
    return final_ordering

def print_final_ordered_summary(final_ordering_clusters):
    # print("Final Ordered Summary: ")
    # for i in range(len(final_ordering_clusters)):
    #     print(f'Cl- {i+1}: {final_ordering_clusters[i]}')

    
    with open('output_t23_final_ordered_summary.txt', 'w') as summary_file:
        summary_file.write("Final Summary in the actual order of input based on s1: \n\n")
        for i in range(len(final_ordering_clusters)):
            summary_file.write(f'Cl- {i+1}: {final_ordering_clusters[i]}\n')
            
    print("'output_t23_final_ordered_summary.txt' created successfully")

final_ordering_clusters=final_ordering(summary,s1_index_track)
print_final_ordered_summary(final_ordering_clusters)

#<!...........................END OF PART 2 ordering summary sentences...............................................!>
