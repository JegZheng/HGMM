#!/usr/bin/python

import os,shutil,sys
import numpy as np 
from pylab import *
import matplotlib.pyplot as plt

def readp(path):
	users = open(path).readlines()
	
	ip = []
	for i in xrange(len(users)):
		p = np.array([float(e) for e in users[i].strip().split(' ')])
		ip.append(p)
	
	return ip

def plot_ip(ip):
	count = 0;
	for e in ip:
		minv = min(e)
		maxv = max(e)

		plt.bar(xrange(len(ip[0])),e,alpha = .5, color = 'g')
		#title('the cluster distribution in terms of the interest')
		title('Interest' + str(count+1))
		axis([1,len(ip[0]),0,maxv])
		xlabel('Cluster')
		ylabel('Probability')
		#savefig('plot/' + str(count) + '.eps')
		savefig('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+'plot/' + str(count) + '.jpg')
		clf()
		count += 1


if os.path.exists('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+'plot'):
	shutil.rmtree('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+'plot')
os.mkdir('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+'plot')

ip = readp('phi.txt')
plot_ip(ip)
	
	










	

















