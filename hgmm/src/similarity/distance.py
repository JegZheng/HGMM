#!/bin/python
#-*- coding: utf-8 -*-

import os,shutil,sys
import numpy as np 
import pylab as pl
from matplotlib.font_manager import FontProperties


ref = []
new = []
pro = []
refpro = []
with open('../theta.txt') as f1:
	for line in f1:
		temp = np.array([float(e) for e in line.strip().split(' ')])
		ref.append(temp)

with open('../loc/theta.txt') as f2:
	for line in f2:
		temp = np.array([float(e) for e in line.strip().split(' ')])
		new.append(temp)

with open('../loc/provinces.txt') as f3:
	for line in f3:
		pro.append(line.strip())

with open('../provinces.txt') as f3:
	for line in f3:
		refpro.append(line.strip())
 
dis = []

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
