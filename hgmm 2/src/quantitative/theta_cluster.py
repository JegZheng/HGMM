'''
python theta_cluster.py <sourcedir> <num_cluster> <targetfile(L)>
'''
import glob
import os
import numpy as np 
import sys
import scipy.io


def count(cid,m,n):
	dic = {}
	cnt = {}
	with open(cid + 'province_name.txt') as place:
		places = place.readlines()
	for i in xrange(m):
		dic[places[i].strip()] = i
		cnt[places[i].strip()] = 0
	count = np.zeros((m,n))
	with open(cid + 'user_img_cid.txt') as r:
		for line in r:
			city, img, cluster = line.strip().split(' ')
			count[dic[city]][int(cluster)-1] += 1
			cnt[city] += 1
	for j in xrange(len(count)):
		count[j] /= cnt[places[j].strip()]
	return count

path = sys.argv[1]
theta = count(path,34,int(sys.argv[2]))
target = sys.argv[3] + '_' + sys.argv[2] + 'k'
#scipy.io.savemat(target,mdict = {'arr': theta})
np.savetxt(target+'.txt',theta)
os.system("python laplacian.py"+" "+target+".txt "+ target)
