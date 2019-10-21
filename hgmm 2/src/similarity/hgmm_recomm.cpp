#include "hgmm.h"

using namespace std;

hgmm::hgmm(string p,int k,int c,int it,int pron){
	user_path = p;

	K = k;
	C = c;
	Itern = it;
	Pron = pron;

	// initial usern,dimension,inum
	usern = 0;
	dimension = 0;
	uin = 0;
	int flag = 0;
	
	ofstream w("provinces.txt");

	DIR*dirp = opendir(user_path.c_str());
	struct dirent*dp;
	while((dp=readdir(dirp)) != NULL){
		string fname = dp->d_name;

		if(fname == "." || fname == "..")
			continue;
		else{
			string username = fname.substr(0,fname.size()-4);
			w<<username<<endl;

			usern += 1;

			ifstream f((user_path + fname).c_str());
			string temp;
			int count = 0;
			while(getline(f,temp)){
				count += 1;
				uin += 1;
				istringstream instr(temp);
				string imgname;
				instr>>imgname;

				if(flag == 0){
					double ele;
					while(instr>>ele){
						dimension ++;
					}
					flag = 1;
				}
			}
			f.close();

			inum.push_back(count);
		}		
	}

	w.close();
	
	cout<<"CLASS INPUT: \n(user_path,"<<user_path<<") (interests,"<<K<<") (clusters,"<<C<<") (iterations,"<<Itern<<")  (processors,"<<Pron<<")"<<endl;
	cout<<"(usern,"<<usern<<") (dimension,"<<dimension<<")"<<endl;
	cout<<"LOAD DATA......."<<endl;
	load_data();
	cout<<"LOAD COMPLETELY\nINITIAL PARAMS......."<<endl;
	initial_params();
	cout<<"INITIAL OK"<<endl;
}

hgmm::~hgmm(){
	cout<<"RELEASE MEMORY......."<<endl;
	release_params();
	cout<<"GAME OVER"<<endl;
}

void hgmm::load_data(){
	V = new double**[usern];
	for(int i=0;i<usern;i++){
		V[i] = new double*[inum.at(i)];
		for(int j=0;j<inum.at(i);j++)
			V[i][j] = new double[dimension];
	}

	DIR*dirp = opendir(user_path.c_str());
	struct dirent*dp;
	int ucount = 0;
	while((dp=readdir(dirp)) != NULL){
		string fname = dp->d_name;

		if(fname == "." || fname == "..")
			continue;
		else{

			ifstream f((user_path + fname).c_str());

			string temp;
			int icount = 0;
			while(getline(f,temp)){
				istringstream instr(temp);
				string imgname;
				instr>>imgname;

				double ele;
				int d = 0;
				while(instr>>ele){
					V[ucount][icount][d] = ele;
					d += 1;
				}
				
				icount += 1;
			}

			f.close();

			ucount += 1;
		}		
	}
}

void hgmm::initial_params(){
	// initialize alpha
	alpha = new double[K];
	ifstream finalpha("../alpha.txt");
	for(int k=0;k<K;k++){
		string temp;
		getline(finalpha,temp);
		istringstream is(temp);
		double tempin;
		is >> tempin;
		alpha[k] = tempin;
	}
	finalpha.close();

	// initialize phi and normalization smoothing
	phi = new double*[K];
	ifstream in("../phi.txt");
	for(int k=0;k<K;k++){
		string temp;
		phi[k] = new double[C];
		getline(in,temp);
		istringstream is(temp);
		for(int c=0;c<C;c++){
			double phivalue;
			is >> phivalue;
			phi[k][c] = phivalue;
			
		}
	}
	in.close();
	
	// initialize gaussian
	u = new double*[C];
	for(int c=0;c<C;c++)
		u[c] = new double[dimension];

	sigma = new double[C];
	ifstream f1("../gaussian.txt");
	int kid=0;
	string temp;
	while(getline(f1,temp)){
		
		istringstream instr(temp);

		double ele;
		instr >> ele;
		sigma[kid] = ele;
		int d=0;
		for(int d = 0; d < dimension; d++){
			instr >> ele;
			u[kid][d] = ele;
		
		}
		kid ++;
	}
	f1.close();
	

	// initialize r and normalization smoothing
	r = new double*[usern];
	for(int i=0;i<usern;i++){
		r[i] = new double[K];

		double sum = 0;
		for(int k=0;k<K;k++){
			r[i][k] = double(random() + 1.0)/(RAND_MAX + 1.0);
			sum += r[i][k];
		}
		for(int k=0;k<K;k++)
			r[i][k] /= sum;

		double rate = 1.0/K*1e-3;
		sum = 1.0 + rate*K;
		for(int k=0;k<K;k++)
			r[i][k] = (r[i][k] + rate)/sum;
	}

	// initialize psi and normalization smoothing
	psi = new double**[usern];
	for(int i=0;i<usern;i++){
		psi[i] = new double*[inum.at(i)];

		for(int j=0;j<inum.at(i);j++){
			psi[i][j] = new double[K];

			double sum=0;
			for(int k=0;k<K;k++){
				psi[i][j][k] = (random() + 1.0)/(RAND_MAX + 1.0);
				sum += psi[i][j][k];
			}

			for(int k=0;k<K;k++)
				psi[i][j][k] /= sum;

			double rate=1.0/K*1e-4;
			sum = 1.0 + rate*K;
			for(int k=0;k<K;k++)
				psi[i][j][k] = (psi[i][j][k] + rate)/sum;
		}
	}

	// initialize Phi without smoothing
	Phi = new double**[usern];
	for(int i=0;i<usern;i++){
		Phi[i] = new double*[inum.at(i)];

		for(int j=0;j<inum.at(i);j++)
			Phi[i][j] = new double[C];
	}
}

void *e_thread(void*arg){
	// convert parameters
	struct e_parameters*params = (e_parameters*)arg;
	int thread_id = params->thread_id;
	int pron = params->pron;
	int K = params->K;
	int C = params->C;
	int usern = params->usern;
	int dimension = params->dimension;
	vector<int>inum(params->inum);
	double ***V = params->V;
	double *alpha = params->alpha;
	double **phi = params->phi;
	double **u = params->u;
	double *sigma = params->sigma;
	double **r = params->r;
	double ***psi = params->psi;
	double ***Phi = params->Phi;

	for(int i=thread_id;i<usern;i+=pron){

		for(int j=0;j<inum.at(i);j++){
			// For Phi with smoothing
			double max;
			for(int c=0;c<C;c++){
				double sum1 = 0;
				for(int k=0;k<K;k++)
					sum1 += psi[i][j][k]*log(phi[k][c]);

				double sum2 = 0;
				for(int d=0;d<dimension;d++)
					sum2 -= (u[c][d] - V[i][j][d])*(u[c][d] - V[i][j][d]);
				sum2 = sum2/(2*sigma[c]) - dimension/2.0*log(2*pi*sigma[c]);

				Phi[i][j][c] = sum1 + sum2;

				if((c == 0)||(max < Phi[i][j][c]))
					max = Phi[i][j][c];
			}
			
			double sum=0;
			for(int c=0;c<C;c++){
				Phi[i][j][c] = exp(Phi[i][j][c] - max);
				sum += Phi[i][j][c];
			}

			for(int c=0;c<C;c++)
				Phi[i][j][c] /= sum;

			double rate = 1.0/C*1e-4;
			sum = 1.0 + rate*C;
			for(int c=0;c<C;c++)
				Phi[i][j][c] = (Phi[i][j][c] + rate)/sum;

			// For psi with smoothing
			for(int k=0;k<K;k++){
				double sum1 = 0;
				for(int c=0;c<C;c++)
					sum1 += Phi[i][j][c]*log(phi[k][c]);
				
				double sum2 = digamma(r[i][k]);
				
				psi[i][j][k] = sum1 + sum2;

				if((k==0) || (max < psi[i][j][k]))
					max = psi[i][j][k];
			}
			
			sum=0;
			for(int k=0;k<K;k++){
				psi[i][j][k] = exp(psi[i][j][k] - max);
				sum += psi[i][j][k];
			}

			for(int k=0;k<K;k++)
				psi[i][j][k] /= sum;

			rate = 1.0/K*1e-4;
			sum = 1.0 + rate*K;
			for(int k=0;k<K;k++)
				psi[i][j][k] = (psi[i][j][k] + rate)/sum;
		}

		// For r
		for(int k=0;k<K;k++){
			r[i][k] = alpha[k];
			for(int j=0;j<inum.at(i);j++)
				r[i][k] += psi[i][j][k];
		}
	}

	pthread_exit(NULL);	
}

void hgmm::e_step(){
	pthread_t *tids = new pthread_t[Pron];
	struct e_parameters *arg = new e_parameters[Pron];
	int *pronid = new int[Pron];	

	//create threads
	for(int i=0;i<Pron;i++){
		
		pronid[i] = i;
		arg[i] = e_parameters(pronid[i],Pron,K,C,usern,dimension,inum,V,alpha,phi,u,sigma,r,psi,Phi);
		int ret = pthread_create(&tids[i],NULL,e_thread,(void*)(arg+i));

		if(ret !=0)
			cout<<"Create threads error"<<endl;
	}

	//join
	void *status;
	for(int i=0;i<Pron;i++){
		pthread_join(tids[i],&status);
		free(status);
	}

	delete []arg;
	delete []pronid;
	delete []tids;
}

void *m_thread(void*arg){
	// convert parameters
	struct m_parameters*params = (m_parameters*)arg;
	int thread_id = params->thread_id;
	int pron = params->pron;
	int K = params->K;
	int C = params->C;
	int usern = params->usern;
	int dimension = params->dimension;
	vector<int>inum(params->inum);
	double ***V = params->V;
	double *alpha = params->alpha;
	double **phi = params->phi;
	double **u = params->u;
	double *sigma = params->sigma;
	double **r = params->r;
	double ***psi = params->psi;
	double ***Phi = params->Phi;
	double *g = params->G;
	double *h = params->H;
	double c_ = params->C_;

	for(int k=thread_id;k<K;k+=pron){
		
		// For phi
		double sum = 0;
		for(int c=0;c<C;c++){
			phi[k][c] = 0;
			for(int i=0;i<usern;i++){
				for(int j=0;j<inum.at(i);j++)
					phi[k][c] += psi[i][j][k]*Phi[i][j][c];
			}
			sum += phi[k][c];
		}

		for(int c=0;c<C;c++)
			phi[k][c] /= sum;

		// For alpha
		double new_alpha = alpha[k] - (g[k] - c_)/h[k];
		
		if((new_alpha > 0)&&(new_alpha < 1))
			alpha[k] = new_alpha;
		else{
			//cout<<"Newton Raphson Method Error: alpha < 0"<<endl;
			alpha[k] = double(random() + 1.0)/(RAND_MAX + 1.0);
		}
	}

	for(int c=thread_id;c<C;c+=pron){
		// For u
		for(int d=0;d<dimension;d++){
			double temp1=0,temp2=0;

			for(int i=0;i<usern;i++){
				for(int j=0;j<inum.at(i);j++){
					temp1 += Phi[i][j][c]*V[i][j][d];
					temp2 += Phi[i][j][c];
				}
			}

			u[c][d] = temp1/temp2;
		}

		// For sigma
		double temp1=0,temp2=0;

		for(int i=0;i<usern;i++){
			for(int j=0;j<inum.at(i);j++){
				double vec_prod = 0;
				for(int d=0;d<dimension;d++)
					vec_prod += (u[c][d] - V[i][j][d])*(u[c][d] - V[i][j][d]);	
			
				temp1 += Phi[i][j][c]*vec_prod;
				temp2 += Phi[i][j][c];	
			}
		}

		sigma[c] = temp1/(dimension*temp2);

	}

	pthread_exit(NULL);
}

void hgmm::m_step(){
	// for computation of alpha
	double *g = new double[K];
	double *h = new double[K];
	double z;
	double c_;
	
	double sumak=0;
	for(int k=0;k<K;k++)
		sumak += alpha[k];

	double temp_g_h = 0;
	double temp_1_h = 0;
	for(int k=0;k<K;k++){
	
		// compute g[k]
		double part1 = usern*(digamma(sumak) - digamma(alpha[k]));

		double part2 = 0;
		
		for(int i=0;i<usern;i++){
			
			double sumrik = 0;
			for(int k1=0;k1<K;k1++)
				sumrik += r[i][k1];

			part2 += (digamma(r[i][k]) - digamma(sumrik));

		}		

		g[k] = part1 + part2;

		// compute h[k]
		h[k] = usern*(de_digamma(sumak) - de_digamma(alpha[k]));

		temp_g_h += g[k]/h[k];
		temp_1_h += 1.0/h[k];

	}
	
	// compute z
	z = usern*de_digamma(sumak);

	// compute c_
	c_ = temp_g_h/(1.0/z + temp_1_h);

////////////////////////////////////////////////////////////////
	// start m_step
	pthread_t *tids = new pthread_t[Pron];
	int *pronid = new int[Pron];
	struct m_parameters *arg = new m_parameters[Pron];
	
	//create threads
	for(int i=0;i<Pron;i++){

		pronid[i] = i;
		//arg[i] = {pronid[i],Pron,imgn,K,dimension,data,theta,z,u,sigma};
		arg[i] = m_parameters(pronid[i],Pron,K,C,usern,dimension,inum,V,alpha,phi,u,sigma,r,psi,Phi,g,h,c_);
		int ret = pthread_create(&tids[i],NULL,m_thread,(void*)(arg + i));

		//cout<<"Create thread: "<<i<<endl;		

		if(ret !=0)
			cout<<"Create threads error"<<endl;
	}

	//join
	void *status;
	for(int i=0;i<Pron;i++){
		pthread_join(tids[i],&status);
		free(status);
	}

	delete []arg;
	delete []pronid;
	delete []tids;

	delete []g;
	delete []h;
}

void *c_thread(void *arg){
	// convert parameters
	struct c_parameters*params = (c_parameters*)arg;
	int thread_id = params->thread_id;
	int pron = params->pron;
	int C = params->C;
	int usern = params->usern;
	int dimension = params->dimension;
	vector<int>inum(params->inum);
	double ***V = params->V;
	double **u = params->u;
	double *sigma = params->sigma;
	double ***psi = params->psi;
	double *likelihoods = params->likelihoods;

	likelihoods[thread_id] = 0;
	double *logpv = new double[C];
	for(int i=thread_id;i<usern;i+=pron){
		for(int j=0;j<inum.at(i);j++){
			double logmax;
			for(int c=0;c<C;c++){
				logpv[c] = log(psi[i][j][c]) - 0.5*dimension*log(2*pi*sigma[c]);
				double sum = 0;
				for(int d=0;d<dimension;d++)
					sum += (u[c][d] - V[i][j][d])*(u[c][d] - V[i][j][d]);
				logpv[c] -= sum/(2*sigma[c]);

				if((c == 0)||(logmax < logpv[c]))
					logmax = logpv[c];
			}

			//cout<<logmax<<endl;
			
			double sum = 0;
			for(int c=0;c<C;c++)
				sum += exp(logpv[c] - logmax);
			if(isnan(sum))
				sum = 1;
			likelihoods[thread_id] = likelihoods[thread_id] + log(sum) + logmax;
		}
	}
	delete []logpv;

	pthread_exit(NULL);
}

void hgmm::convergence(){
	double *likelihoods = new double[Pron];

	pthread_t *tids = new pthread_t[Pron];
	int *pronid = new int[Pron];
	struct c_parameters *arg = new c_parameters[Pron];
	
	//create threads
	for(int i=0;i<Pron;i++){

		pronid[i] = i;
		arg[i] = c_parameters(pronid[i],Pron,C,usern,dimension,inum,V,u,sigma,psi,likelihoods);
		int ret = pthread_create(&tids[i],NULL,c_thread,(void*)(arg + i));	

		if(ret !=0)
			cout<<"Create threads error"<<endl;
	}

	//join
	void *status;
	for(int i=0;i<Pron;i++){
		pthread_join(tids[i],&status);
		free(status);
	}

	delete []arg;
	delete []pronid;
	delete []tids;

	//compute likelihood
	likelihood = 0;
	for(int i=0;i<Pron;i++){
		likelihood += likelihoods[i];
	}
	likelihood /= double(uin);

	delete []likelihoods;
}

void hgmm::run_em(){
	cout<<"RUNNING EM......."<<endl;

	likelihood = 0;
	ofstream f("likelihood.txt");

	for(int iter=0;iter<Itern;iter++){

		cout<<"E_STEP"<<endl;
		e_step();
		
		//cout<<"M_STEP"<<endl;
		//m_step();

		//cout<<"CONVERGENCE"<<endl;
		//convergence();

		cout<<"ITER "<<iter<<":\t"<<likelihood<<endl;

		f<<likelihood<<endl;
	}

	f.close();
}

void hgmm::output(){	
	

	

	ofstream w4("theta.txt");
	for(int i=0;i<usern;i++){
		double sum = 0;
		for(int k=0;k<K;k++)
			sum += r[i][k];
		for(int k=0;k<K;k++)
			w4<<r[i][k]/sum<<" ";
		w4<<endl;
	}
	w4.close();

	ofstream w5("I.txt");
	for(int i=0;i<usern;i++){
		for(int j=0;j<inum.at(i);j++){
			for(int k=0;k<K;k++)
				w5<<psi[i][j][k]<<" ";
			w5<<endl;
		}
	}
	w5.close();

	ofstream w6("z.txt");
	for(int i=0;i<usern;i++){
		for(int j=0;j<inum.at(i);j++){
			for(int c=0;c<C;c++)
				w6<<Phi[i][j][c]<<" ";
			w6<<endl;
		}
	}
	w6.close();
}

void hgmm::release_params(){
	delete []alpha;

	for(int k=0;k<K;k++)
		delete phi[k];
	delete []phi;

	for(int c=0;c<C;c++)
		delete [](u[c]);
	delete []u;
	delete []sigma;

	for(int i=0;i<usern;i++){
		for(int j=0;j<inum.at(i);j++){
			delete [](Phi[i][j]);
			delete [](psi[i][j]);
		}
		delete [](Phi[i]);
		delete [](psi[i]);
		delete [](r[i]);
	}
	delete []Phi;
	delete []psi;
	delete []r;
}

int main(int argc,char*argv[]){

	if(argc != 6){
		cout<<"INPUT ERROR: ./udhmm user_file_path interest_number cluster_number iter_number processor_number"<<endl;
		return -1;	
	}

	string p = argv[1];
	int K = atoi(argv[2]);
	int C = atoi(argv[3]);
	int Itern = atoi(argv[4]);
	int Pron = atoi(argv[5]);

	srandom(time(NULL));

	struct timespec start,finish;	
	clock_gettime(CLOCK_MONOTONIC,&start);

	hgmm instance(p,K,C,Itern,Pron);
	instance.run_em();
	instance.output();

	clock_gettime(CLOCK_MONOTONIC,&finish);

	double elapsed = finish.tv_sec - start.tv_sec;
	elapsed += (finish.tv_nsec - start.tv_nsec)/1e9;

	cout<<"RUNNING TIME: "<<elapsed<<" SECONDS"<<endl;

	return 0;

}
