#!/usr/bin/python

users = []
with open('provinces.txt') as f:
	for line in f:
		users.append(line.strip())

uis = []
for e in users:
	with open('/DATA2/data/hjzheng/hgmm_ex/features/' + e + '.txt') as f:
		for line in f:
			i,vs = line.strip().split('\t')
			uis.append(e + ' ' + i)

f1 = open('I.txt').readlines()
f2 = open('z.txt').readlines()
with open('ic.txt','w') as w:
	for e in xrange(len(uis)):
		alist1 = [float(v) for v in f1[e].strip().split(' ')]
		alist2 = [float(v) for v in f2[e].strip().split(' ')]
		index1 = alist1.index(max(alist1))
		index2 = alist2.index(max(alist2))
		w.write(' '.join([uis[e],str(index1),str(index2)]) + '\n')
		
