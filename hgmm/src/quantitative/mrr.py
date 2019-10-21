#!/bin/python
#-*- coding: utf-8 -*-

import os,shutil,sys
import numpy as np 
import pylab as pl
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

x=[1,10,20,30,40,50,60,70,80,90,100]
interest = [10,15,20]
cluster = [200,500]
y=[]
config = []
for k in interest:
	for c in cluster:
		F = []
		for i in x:
			root = 'rec_origine/rec_' + str(i) + '/loc/'
			pro = os.listdir(root)
			score = 0
			precision = 0
			recall = 0
			#print pro
			for j in pro:
				#print j[len(j)-4:len(j)]
				if j == 'loc' or j[len(j)-4:len(j)] == '.txt':
					continue				
				path = root + j + '/' + str(k) + '_' + str(c) + '/simcity.txt'
				count = 1
				with open(path) as f:
					for line in f:
						if line.strip() == j:
							score += 1/float(count)
							break 
						count += 1
			F.append(score/34)
			F.sort()
		y.append(F)
		config.append([k,c])


GA=[]
with open('GMM_avg.txt') as ga:
	for line in ga:
		temp = np.array([float(e) for e in line.strip().split(', ')])
		GA.append(temp)		
marker = ['s','p','*','h','D','H']
color = ['b','g','r','c','m','y']
line = ['-','--']

for p in xrange(len(y)):
	pl.plot(x,y[p], marker =marker[p],color = color[p],label=str(config[p]))
pl.plot(x,GA[0],'--',label='GMM_200')
pl.plot(x,GA[1],'--',label='GMM_500')
pl.plot(x,GA[2],'-.',label='avg')
pl.legend(loc = 'upper left',ncol = 3)
pl.title('Retrieval accuracy')
pl.xlabel('Input number')
pl.ylabel('Mean reciprocal ranking')
pl.xlim(0,100)
pl.ylim(0,0.6)
pl.grid(True)
pl.savefig('mrr.jpg')
pl.close()
'''
plt.bar(x,F,alpha = .5, color = 'r')
plt.title('Distribution of F-score over numbers of input images')
plt.xlabel('Input number')
plt.ylabel('F-score')
plt.xlim(0,100)
plt.ylim(0,F[len(y)-1])
plt.savefig('Fscore.jpg')
plt.close()
'''
'''
for p in xrange(len(pro)):
	print pro[p]	
	if os.path.exists('../loc/'+pro[p]):
		shutil.rmtree('../loc/'+pro[p])
	os.mkdir('../loc/'+pro[p])
	os.mkdir('../loc/'+pro[p]+'/dis/')

	for i in xrange(len(ref)):
		temp = 0
		for k in xrange(len(ref[i])):
			temp += (new[p][k] - ref[i][k]) * (new[p][k] - ref[i][k])
		dis.append([temp,refpro[i],i])

	dis.sort()

	x = []
	for i in xrange(20):
		x.append(i)
	with open('../loc/'+pro[p]+'/simcity.txt','w') as w:
		for i in xrange(len(pro)):
			#print dis[i][1]
			plot1 = pl.plot(x,ref[dis[i][2]],'r*-',label='reference')
			plot2 = pl.plot(x,new[p],'b*-',label='input')
			pl.title('Probility in terms of interest')
			pl.xlabel('interests')
			pl.ylabel('probility')
			pl.xlim(0,10)
			pl.ylim(0,1)
			pl.legend()
			pl.savefig('../loc/'+pro[p]+'/dis/' + str(dis[i][1]) + '.jpg')
			pl.close()
			w.write(str(dis[i][1]) + '\n')
	dis = []
	x = []

'''
