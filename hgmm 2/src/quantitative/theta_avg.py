'''
python theta_cluster.py <sourcefile(image features dir)> <targetfile(L)>
'''
import glob
import numpy as np 
import sys
import scipy.io

def load(path):
	ref = []
	for k in glob.glob(path+'/*'): 
		print k
		with open(k) as f:
			province = []
			for line in f:	
				temp = np.array([float(e) for e in line.strip().split('\t')[1].split(' ')])
				province.append(temp)
			province = np.array(province)
			ref.append(avgtheta(province))
	return np.array(ref)

def avgtheta(mat):
	res = np.zeros(len(mat[0]))
	for i in xrange(len(mat)):
		res += mat[i]
	res = res/len(mat)
	return res


path = sys.argv[1]
theta = load(path)
target = sys.argv[2]
#scipy.io.savemat(target,mdict = {'arr': theta})
np.savetxt(target+'.txt',theta)
os.system("python laplacian.py"+" "+target+".txt "+ target)
