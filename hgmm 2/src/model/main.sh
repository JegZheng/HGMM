#!/bin/sh

cd ~/kmeans/;
echo $C>params.txt;
python load.py hgmm_ex;
matlab -nodesktop -nosplash -r "run ./vlkmeans.m;quit;";
cp pre_mean.txt ~/hgmm/;

cd ~/hgmm/;
g++ -o hgmm hgmm.cpp -lpthread;
./hgmm ~/../../../DATA2/data/hjzheng/hgmm_ex/features/ $I $C 30 20;
python interest_cluster.py;
python c_cluster.py $C;
python top.py 10;
python plot_topic.py;
python plot_interest.py;
python province_ic.py $C;
