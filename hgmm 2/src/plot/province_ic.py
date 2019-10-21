import sys,os
import shutil

province = []
topc = []
num = int(sys.argv[1])
folder = '/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+str(num) + '_clusters/'

with open('provinces.txt','r') as pro:
	for i in pro:
		province.append(i.strip())
with open('top10.txt','r') as top:
	for line in top:
		topc.append(line.strip())
with open ('theta.txt') as theta:
	with open('province_interest_cluster.txt','w') as pic:
		line_index = 0
		path = '/DATA2/data/hjzheng/hgmm_ex/hgmm_experiment/hgmm_ex_15_200/'+'img'
		if not(os.path.exists(path)):
			os.mkdir(path)
		for line in theta:
			temp = []
			count = 0
			index = []
			
			for i in line.strip().split(' '):
				temp.append([float(i),count])
				count += 1	
			temp.sort(reverse = True)
			for j in temp:
				if j[0] > 0.2:
					index.append(j[1])
			# write in txt
			pic.write(str(province[line_index]) + '\t' + 'interest: ' )
			for k in index:
				pic.write(str(k + 1) + ' ')
			pic.write('\t' + 'cluster: ') 
			for k in index:
				pic.write(str(topc[k]) + '  ')	
			pic.write('\n')

			# move to folders
			print 'dealing with: '+str(province[line_index])+'\n'
			
			path_dir = path + '/' + str(province[line_index]) + '/'
			if not(os.path.exists(path_dir)):
				os.mkdir(path_dir)			

			for k in index:
				tempcnt = 0
				cluster = []
				if not(os.path.exists(path + str(k+1))):
					os.mkdir(path + str(k+1))
				print 'interest: ' + str(k+1)
				for item in topc[k].strip().split(' '):
					item = int(item)
					if tempcnt == 5:
						break
					print 'cluster: ' + str(item)
					files = os.listdir(folder + str(item))
					if not(os.path.exists(path+ str(k+1) + '/' + str(item))):
						os.mkdir(path + str(k+1) + '/' + str(item))
					print path
					for cnt in xrange(3000):
						shutil.copyfile(folder + str(item) + '/' + files[cnt],path + str(k+1) + '/' + str(item) + '/' + files[cnt])
					print '1000 photos\n'
					tempcnt += 1
			line_index += 1				
