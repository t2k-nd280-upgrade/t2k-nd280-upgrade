#include "TROOT.h"
#include "TRandom3.h"
#include "TH1F.h"

const Int_t 	N = 100000;
const Float_t M = 939.565379;
void n_model() {
	TRandom3* rand = new TRandom3();

	TH1F* energy_acc = new TH1F("e_acc", "energy accuracy in the ROI", 120, -1., 1.);

	Float_t beta, dist_cubes, first_hit_time, time_abs, ekin2;
	Float_t ekin = 100;
	dist_cubes = 60.;

	beta = sqrt(ekin * ekin + 2*M*ekin) / (ekin + M);	
	time_abs = dist_cubes / (beta * 30);

	for (Int_t i = 0; i < N; ++i) {
		first_hit_time = rand->Gaus(time_abs, 1.5/sqrt(3));
		//beta = neutron_dist_true / (neutron_time * 30.);
    beta = dist_cubes / (first_hit_time * 30.);
    ekin2 = sqrt(M*M / (1 - beta*beta)) - M;

    energy_acc->Fill((ekin2 - ekin) / ekin);
	}

	energy_acc->Draw();
}