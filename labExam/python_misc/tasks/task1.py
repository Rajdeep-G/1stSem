import nltk
from nltk.corpus import inaugural

nltk.download('inaugural')


trump_speech_id=[file_id for file_id in inaugural.fileids() if "2017" in file_id and "Trump" in
file_id][0]

trump_speech_words=inaugural.words(trump_speech_id)
total_no_of_words=len(trump_speech_words)
distinct_words=set(trump_speech_words)
total_no_of_distinct_words=len(distinct_words)
word_type_length=[len(word) for word in distinct_words]
avg_word_length=sum(word_type_length)/total_no_of_distinct_words

print(total_no_of_words)
print(total_no_of_distinct_words)
print(avg_word_length)