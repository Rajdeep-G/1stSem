import nltk
from nltk.corpus import inaugural
from collections import Counter
nltk.download('inaugural')


trump_speech_id=[file_id for file_id in inaugural.fileids() if "2017" in file_id and "Trump" in file_id][0]
trump_speech_words=inaugural.words(trump_speech_id)

trump_speech_words_lower=[word.lower() for word in trump_speech_words]
word_freq_dis=Counter(trump_speech_words_lower)


print("\nWords with their frequency in DESCENDING ORDER")
for i,j in word_freq_dis.most_common():
    print('Word->',i,' Frequency-> ',j)

print("\n")
def most_frequent(word_freq_dis, k):
    stopwords = nltk.corpus.stopwords.words('english')
    for word in stopwords:
        if word in word_freq_dis:
            del word_freq_dis[word]
    print(f'{k} most frequent words:')
    return Counter(word_freq_dis).most_common(k)    

print(most_frequent(word_freq_dis, 50))

def freq_of_word(word_freq_dis, word):
    return word_freq_dis[word]


print("\n")
freq_am=freq_of_word(word_freq_dis, 'america')
print(f'Freq of the word america {freq_am}')
freq_ind=freq_of_word(word_freq_dis, 'india')
print(f'Freq of the word india {freq_ind}')
