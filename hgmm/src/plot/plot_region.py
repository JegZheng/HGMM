import os,shutil,sys
import numpy as np 
from pylab import *

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

		plot(xrange(len(ip[0])),e,'b*-')
		#title('the region distribution in terms of the interest')
		title('Cluster' + str(count))
		axis([0,len(ip[0])-1,0,maxv])
		ylabel('Probability')
		xlabel('Region')
		savefig('plot_region/' + str(count) + '.jpg')
		#savefig('plot_region/' + str(count) + '.pdf')
		clf()
		count += 1


if os.path.exists('plot_region'):
	shutil.rmtree('plot_region')
os.mkdir('plot_region')

ip = readp('theta.txt')
ip = array(ip)
ip = ip.T
plot_ip(ip)
