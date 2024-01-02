import nltk
import networkx as nx
import string
import numpy as np
import pandas as pd
import math
from nltk.corpus import stopwords
from nltk.stem import WordNetLemmatizer
import random

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


# Function to initialize centroids randomly
def initialize_centroids(tfidf_matrix, num_clusters):
    num_words,num_sentences = tfidf_matrix.shape
    centroids = np.zeros((num_words, num_clusters))

    for j in range(num_clusters):
        centroid = random.randint(0, num_sentences-1)
        centroids[:, j] = tfidf_matrix[:, centroid]

    return centroids

def assign_clusters(cosine_sim_matrix):
    num_words, num_sentences = cosine_sim_matrix.shape  # Updated dimensions
    
    cluster_assignments = np.zeros(num_sentences)

    for i in range(num_sentences):
        cluster_assignments[i] = np.argmax(cosine_sim_matrix[i, :])

    return cluster_assignments

# Function to update centroids based on the mean of sentences in each cluster
def update_centroids(tfidf_matrix, cluster_assignments, num_clusters):
    num_words, num_sentences = tfidf_matrix.shape  # Updated dimensions
    centroids = np.zeros((num_words, num_clusters))
    for j in range(num_clusters):
        cluster = np.where(cluster_assignments == j)[0]
        centroids[:, j] = np.mean(tfidf_matrix[:, cluster], axis=1)

    
    return centroids

# Custom K-Means function
def custom_kmeans_t21(tfidf_matrix, num_clusters, cosine_sim_matrix, max_iterations=100):
    centroids = initialize_centroids(tfidf_matrix, num_clusters)
    
    for i in range(max_iterations):
        cluster_assignments = assign_clusters(cosine_sim_matrix)
        centroids = update_centroids(tfidf_matrix, cluster_assignments, num_clusters)

        if np.array_equal(centroids, update_centroids(tfidf_matrix, cluster_assignments, num_clusters)):
            break
    print("The cluster assignments are: ",cluster_assignments)
    return cluster_assignments



def find_common_bigrams(sentence1, sentence2):
    # Tokenize sentences into bigrams
    bigrams1 = set(sentence1.split())
    bigrams2 = set(sentence2.split())

    # Find common bigrams
    common_bigrams = bigrams1.intersection(bigrams2)

    return common_bigrams

def find_s1_and_s2(cluster_sentences, cosine_sim_matrix):
    num_sentences = len(cluster_sentences)
    similarities = np.zeros((num_sentences, num_sentences))

    for i in range(num_sentences):
        for j in range(num_sentences):
            similarities[i][j] = cosine_sim_matrix[i][j]
    
        # Find the sentence (S1) closest to the cluster centroid
    cluster_centroid = np.mean(similarities, axis=0)
    s1_index = np.argmax(cluster_centroid)
    s1 = cluster_sentences[s1_index]

    # Find a sentence (S2) with at least 3 common bigrams with S1
    s2 = None
    for i in range(num_sentences):
        if i != s1_index:
            common_bigrams = find_common_bigrams(s1, cluster_sentences[i])
            if len(common_bigrams) >= 3:
                s2 = cluster_sentences[i]
                break
    return s1, s2

# def construct_sentence_graph(sentence, start_node, end_node):
#     G = nx.DiGraph()
#     G.add_node(start_node)
#     l_bigram = sentence.split()
#     l_bigram.append(end_node)

#     G.add_edge(start_node, l_bigram[0])
#     for i in range(len(l_bigram)-2):
#         G.add_edge(l_bigram[i], l_bigram[i+1])

#     print("GRAPH")
#     print (G.edges())

#     return G

def construct_sentence_graph(sentence, start_node, end_node, G=None):
    if G is None:
        G = nx.DiGraph()
        G.add_node(start_node)
    
    l_bigram = sentence.split()
    l_bigram.append(end_node)

    current_node = start_node

    for bigram in l_bigram:
        if not G.has_node(bigram):
            G.add_node(bigram)
        G.add_edge(current_node, bigram)
        current_node = bigram

    print("GRAPH")
    print(G.edges())

    return G

def generate_sentence(G, start_node, end_node):
    sentence = []
    current_node = start_node

    while current_node != end_node:
        neighbors = list(G.neighbors(current_node))

        if not neighbors:
            # No valid neighbors, terminate the loop
            break

        next_node = random.choice(neighbors)
        sentence.append(next_node)
        current_node = next_node

    generated_sentence = ' '.join(sentence)
    return generated_sentence

def generate_all_sentences(G, start_node, end_node):
    def generate_sentences_recursive(current_node, path, sentences):
        if current_node == end_node:
            sentences.append(' '.join(path))
            return
        neighbors = list(G.neighbors(current_node))
        for neighbor in neighbors:
            new_path = path + [neighbor]
            generate_sentences_recursive(neighbor, new_path, sentences)
    
    sentences = []
    generate_sentences_recursive(start_node, [start_node], sentences)
    return sentences


def sentence_word_t22():
    sen=[]
    graph = None
    for cluster_id in range(k):
        cluster_sentences = [sentences[i] for i, cluster_assignment in enumerate(cluster_assignments) if cluster_assignment == cluster_id]

        # Find S1 and S2 for the current cluster
        s1, s2 = find_s1_and_s2(cluster_sentences, cosine_sim_matrix)

        if s2:
            print("S2 there")
            if graph is None:
                graph = construct_sentence_graph(s1, 'start', 'end')
            else:
                # Add nodes for S2 to the existing graph
                graph = construct_sentence_graph(s2, 'start', 'end', graph)

            # Generate all possible sentences from the graph for S1 and S2
                sentences_s1_and_s2 = generate_all_sentences(graph, 'start', 'end')
                print(sentences_s1_and_s2)
            # Now you can access sentences_s1 as sentences_s1_and_s2[0] and sentences_s2 as sentences_s1_and_s2[1]
            # sentences_s1 = sentences_s1_and_s2[0]
            # sentences_s2 = sentences_s1_and_s2[1]

            # print(sentences_s1)
            # print(sentences_s2)
            
            # temp=[]
            # temp.append(sentence_generated_s1)
            # temp.append(sentence_generated_s2)
            # sen.append(temp)
            
        else:
            print("S2 not there")
            print(f"Cluster {cluster_id} Generated Sentence 1: {s1}")
            temp=[]
            temp.append(s1)
            sen.append(temp)
            # 
    
    # return sentence_generated_s1,sentence_generated_s2, sen





def final_order(jumble_sentences,sentences):
    sentence_to_position = {sentence: position for position, sentence in enumerate(sentences)}

    # Convert both sets of sentences to lowercase for case-insensitive comparison
    jumble_sentences = [[sentence.lower() for sentence in group] for group in jumble_sentences]
    sentences = [sentence.lower() for sentence in sentences]

    # Calculate cosine similarities between jumble sentences and all sentences manually
    similarities = []
    for jumble_group in jumble_sentences:
        jumble_vector = ' '.join(jumble_group)
        similarity_scores = [np.dot(jumble_vector.split(), sentence.split()) / (np.linalg.norm(jumble_vector.split()) * np.linalg.norm(sentence.split())) for sentence in sentences]
        similarities.append(similarity_scores)

    # Find the sentence with the highest similarity for each jumble sentence
    closest_matches = [sentences[np.argmax(scores)] for scores in similarities]

    # Now, closest_matches will contain the closest matching sentences from the original list
    for sentence in closest_matches:
        print(sentence)





with open('input.txt', 'r') as file:    # Read the document from the input file
    document = file.read()
# Get the number of sentences to be included in the summary
print("Enter the number of sentences to be included in the summary(part1): ")
# n = int(input())
n=2
print("Enter the number of sentences to be included in the MMR summary: ")  
# n2=(int)(input())
n2=2
print("Enter the value of alpha: ")
# alpha=(float)(input())
alpha=0.5

lemmatized_sentences, sentences = preprocess_t1(document)
tfidf_matrix, processed_sentences = tf_idf_t2(lemmatized_sentences)
pagerank_scores,cosine_sim_matrix=page_rank_t3(tfidf_matrix, processed_sentences, sentences, n)
k=5
cluster_assignments = custom_kmeans_t21(tfidf_matrix,k,cosine_sim_matrix)
# sentence_generated_s1,sentence_generated_s2,sen=
sentence_word_t22()
# print(".............")
# print(sen)
# print(sentences)
# final_order(sen,sentences)


