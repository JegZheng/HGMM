#!/usr/bin/python
import sys

top = int(sys.argv[1])

with open('top'+ str(top) + '.txt','w') as w:
	with open('phi.txt') as f:
		for line in f:
			vs = []
			count = 0
			for e in line.strip().split(' '):
				vs.append([float(e),count])
				count += 1
			vs.sort(reverse=True)
			for e in xrange(top):
				#print vs[e][0],
				w.write(str(vs[e][1]) + ' ')
			w.write('\n')
			
