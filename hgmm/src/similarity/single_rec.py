import os,sys
import shutil
import numpy as np 
import pylab as pl

temp = []

with open('rec/provinces.txt') as f1:
	for line in f1:
		pro = line.strip()
fpath = '../../Chinese/rec/'
with open(fpath + os.listdir(fpath)[0]) as f2:
	for line in f2:
		img = line.strip().split('\t')[0]
ipath = '../../Copy/conf/Chinese/' + pro + '/' + img
shutil.copyfile(ipath,'rec/'+img)

topic = []
with open('rec/z.txt') as z:
	for line in z:
		a = line.strip().split(' ')
	for i in a:
		topic.append(float(i))
	maxc =  topic.index(max(topic))

theta = []
with open('theta.txt') as f3:
	for line in f3:
		theta.append([float(e) for e in line.strip().split(' ')])	
	theta = np.array(theta)

phi = []
with open('phi.txt') as f4:
	for line in f4:
		phi.append([float(e) for e in line.strip().split(' ')])
	phi = np.array(phi)

id = []
with open('provinces.txt') as f5:
	for line in f5:	
		id.append(line.strip())

mapping = np.dot(theta,phi)
result = []
plot = []
for i in xrange(len(mapping)):
	result.append([mapping[i][maxc],i])
	plot.append(mapping[i][maxc])
	result = sorted(result,reverse=True)
x = []
for i in xrange(34):
	x.append(i)
pl.plot(x,plot,'b*-')
pl.title('Probility in terms of provinces')
pl.xlabel('id of provinces')
pl.ylabel('probility')
pl.xlim(0,34)
pl.ylim(0,max(plot))
pl.savefig('rec/' + 'simcities' + '.jpg')
pl.close()

with open('rec/simcities.txt','w') as o:
	for i in xrange(3):
		print id[result[i][1]]
		o.write(id[result[i][1]] + '\n')

