'''
python laplacian.py <sourcefile(theta)> <targetfile(L)>
'''

import os,shutil,sys
import scipy.io
import numpy as np 
import pylab as pl
from matplotlib.font_manager import FontProperties

def load(file):
	ref = []
	with open(file) as f1:
		for line in f1:
			temp = np.array([float(e) for e in line.strip().split(' ')])
			ref.append(temp)
	return np.array(ref)

def laplacian(A,n):
	#A is an array, n is the size of the target matrix
	ADJ = np.zeros((n,n))
	D = np.zeros((n,n))
	L = np.zeros((n,n))
	for i in xrange(n):
		for j in xrange(n):
			if i == j:
				ADJ[i][j] = 0
			else:
				ADJ[i][j] = np.linalg.norm(A[i] - A[j])
	'''
	for k in xrange(n):
		D[k][k] = np.sum(ADJ[k])
	L = D - ADJ
	'''
	return ADJ
	


n=34
target = sys.argv[1]
W = load(target)
A = laplacian(W,n)
path = sys.argv[2]
scipy.io.savemat(path,mdict = {'arr': A})
#np.savetxt(path+'.txt',A)
