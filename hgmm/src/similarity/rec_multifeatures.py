import os,sys
import shutil
import random

path = 'location/'

num = sys.argv[1]

if os.path.exists('rec_' + num + '/'):
	shutil.rmtree('rec_' + num + '/')
os.mkdir('rec_' + num + '/')
os.mkdir('rec_'+ num + '/features')

files = os.listdir(path)
#fileid = int(random.uniform(0,33))


for fileid in xrange(34):
	features = []
	with open(path + files[fileid]) as f:
		for line in f:
			features.append(line.strip())
	province = files[fileid][:len(files[fileid])-4]
	#random.shuffle(features)

	if not os.path.exists('rec_'+num+'/'+province):
		os.mkdir('rec_'+num+'/'+province)
		os.mkdir('rec_'+num+'/'+province + '/img')
	with open('rec_'+num+'/features/' + files[fileid], 'w') as o:
		with open( 'rec_'+num+'/'+province + '/' +'imgs.txt', 'w') as o2:
			for i in xrange(int(num)):
				o.write(features[i] + '\n')
				a = features[i].strip().split('\t')
				#shutil.copyfile('../../../raid/hjzheng/ex/'+ province +'/'+ a[0],'rec_'+num+'/'+province + '/img/' +a[0])
				o2.write(a[0] + '\n')

