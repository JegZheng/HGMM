cd ~/hgmm/scr;
g++ -o hgmm_rec hgmm_recomm.cpp -lpthread;
./hgmm_rec ~/../../../DATA2/data/hjzheng/rec_$N/features/ $I $C 30 8;
rm -r ../loc
mkdir ../loc
mv *txt ../loc/

