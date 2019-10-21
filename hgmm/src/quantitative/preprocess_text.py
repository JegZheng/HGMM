'''
python preprocess_text.py <sourcefile(text)for lda>
'''
import sys
import glob
import re
import nltk
from nltk.corpus import wordnet as wn
from nltk.corpus import stopwords
import pickle
import random
from nltk.tokenize import word_tokenize

rule = r'[%|_|+]'
sw = stopwords.words('english')


raw = open(sys.argv[1]).readlines()

sents = [[sent.lower() for sent in line.strip().split(' ')]for line in raw]

sent_token = []
for i in sents:
	temp = []
	for word in i:
		w_t = word_tokenize(word)
		for j in w_t:
			temp.append(j)
	sent_token.append(temp)

sent_nopucsw = []
punctuation = [',','.',';',':','?','(',')','[',']','!','&','*','@','#','$','%']
for i in sent_token:
	temp = []
	for j in i:
		if not (j in punctuation or j in sw):
			temp.append(j.decode('utf-8'))
	sent_nopucsw.append(temp)

sent_final = []
for i in sent_nopucsw:
	temp = []
	for j in i:
		if wn.lemmas(j):
			temp.append(j)
	sent_final.append(temp)

with open('chinesetext.txt','w') as w:
	for line in sent_final:
		for word in line:
			w.write(word + ' ')
		w.write('\n')
