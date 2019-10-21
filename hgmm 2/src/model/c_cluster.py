#!/bin/python

import os,sys
import shutil

K = int(sys.argv[1])

folder = str(K) + '_clusters'

if os.path.exists('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+folder):
	shutil.rmtree('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+folder)

os.mkdir('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+folder)
for i in xrange(K):
	path = '/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/' + folder +'/' + str(i)
	#print path
	os.mkdir(path)

with open('ic.txt') as f:
	i = 0
	cluster = [] 
	for k in xrange(K):
		cluster.append(0)
	for line in f:
		if i%10000 == 0:
			print i
		u,r,interest,cid = line.strip().split(' ')
		if cluster[int(cid)-1] <= 2000:
			shutil.copyfile('../../../../disk/hjzheng/Chinese/' + u + '/' +  r,'/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+folder + '/' + cid + '/' + r)
			cluster[int(cid)-1] += 1
		i += 1
