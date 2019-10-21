cd ~/Chinese/
python rec_features.py
cd ~/hgmm/scr/
I=10 C=200 ./main_rec.sh
cd ../hgmm_result/
python single_rec.py
