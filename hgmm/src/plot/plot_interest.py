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
	pro = []
	pro_c = []
	count = 0	
	pro = open('provinces.txt').readlines()
	for e in ip:
		minv = min(e)
		maxv = max(e)

		plt.bar(xrange(len(ip[0])),e,alpha = .5, color = 'g')
		#title('the interest distribution in terms of the region')
		title(str(pro[count].strip()))
		axis([0,len(ip[0]),0,maxv])
		xlabel('Regional interests')
		ylabel('Probability')
		#savefig('plot_interest/' + str(pro[count]) + '.eps')
		#savefig('plot_interest/' + str(pro[count]) + '.pdf')
		savefig('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/plot_interest/' + str(pro[count].strip()) + '.jpg')
		clf()
		count += 1


if os.path.exists('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+'plot_interest'):
	shutil.rmtree('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+'plot_interest')
os.mkdir('/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+'plot_interest')
'''
if os.path.exists('../../web/static/regions/'):
	shutil.rmtree('../../web/static/regions/')
os.mkdir('../../web/static/regions/')
'''
ip = readp('theta.txt')
plot_ip(ip)
