
struct Particle {
    int pid;
    double P, Theta, Phi;
};

void calculateStats(const std::vector<double>& values, double& mean, double& stddev) {
    double sum = 0.0, sum_sq = 0.0;
    for (const auto& val : values) {
        sum += val;
        sum_sq += val * val;
    }
    mean = sum / values.size();
    stddev = std::sqrt(sum_sq / values.size() - mean * mean);
}

int read_rec_particle() {
    HipoChain chain;
    chain.Add("./out/recon.out.hipo");
    auto& c12 = chain.C12ref();

    std::vector<double> P_e, Theta_e, Phi_e;
    std::vector<double> P_pi, Theta_pi, Phi_pi;
    std::vector<double> P_n, Theta_n, Phi_n;

    int eventNumber = 0;

    while (chain.Next() == true) {
        auto event = c12->event();
        auto parts = c12->getDetParticles(); 
        
        std::vector<Particle> particles;
        for (unsigned int idx = 0; idx < parts.size(); idx++) {
            auto particle = parts.at(idx);
            int pid = particle->getPid();
            double P = particle->getP();
            double Theta = particle->getTheta() * 180.0 / M_PI; // Convert to degrees
            double Phi = particle->getPhi() * 180.0 / M_PI; // Convert to degrees
            if (pid == 11 || pid == 211 || pid == 2112) {
                particles.push_back({pid, P, Theta, Phi});
            }
        }

        if (!particles.empty()) {
            std::cout << "=========== Event " << ++eventNumber << " ==============\n";
            for (const auto& p : particles) {
                if (p.pid == 11) {
                    std::cout << "Electron: \t P: " << p.P << " \t Theta: " << p.Theta << " \t Phi: " << p.Phi << "\n";
                    P_e.push_back(p.P);
                    Theta_e.push_back(p.Theta);
                    Phi_e.push_back(p.Phi);
                } else if (p.pid == 211) {
                    std::cout << "PiPlus: \t P: " << p.P << " \t Theta: " << p.Theta << " \t Phi: " << p.Phi << "\n";
                    P_pi.push_back(p.P);
                    Theta_pi.push_back(p.Theta);
                    Phi_pi.push_back(p.Phi);
                } else if (p.pid == 2112) {
                    std::cout << "Neutron: \t P: " << p.P << " \t Theta: " << p.Theta << " \t Phi: " << p.Phi << "\n";
                    P_n.push_back(p.P);
                    Theta_n.push_back(p.Theta);
                    Phi_n.push_back(p.Phi);
                }
            }
        }
    }

    double mean_P, stddev_P;
    double mean_Theta, stddev_Theta;
    double mean_Phi, stddev_Phi;

    calculateStats(P_e, mean_P, stddev_P);
    calculateStats(Theta_e, mean_Theta, stddev_Theta);
    calculateStats(Phi_e, mean_Phi, stddev_Phi);
    std::cout << "\nElectron P: mean = " << mean_P << ", stddev = " << stddev_P << "\n";
    std::cout << "Electron Theta: mean = " << mean_Theta << ", stddev = " << stddev_Theta << "\n";
    std::cout << "Electron Phi: mean = " << mean_Phi << ", stddev = " << stddev_Phi << "\n";

    calculateStats(P_pi, mean_P, stddev_P);
    calculateStats(Theta_pi, mean_Theta, stddev_Theta);
    calculateStats(Phi_pi, mean_Phi, stddev_Phi);
    std::cout << "\nPiPlus P: mean = " << mean_P << ", stddev = " << stddev_P << "\n";
    std::cout << "PiPlus Theta: mean = " << mean_Theta << ", stddev = " << stddev_Theta << "\n";
    std::cout << "PiPlus Phi: mean = " << mean_Phi << ", stddev = " << stddev_Phi << "\n";

    calculateStats(P_n, mean_P, stddev_P);
    calculateStats(Theta_n, mean_Theta, stddev_Theta);
    calculateStats(Phi_n, mean_Phi, stddev_Phi);
    std::cout << "\nNeutron P: mean = " << mean_P << ", stddev = " << stddev_P << "\n";
    std::cout << "Neutron Theta: mean = " << mean_Theta << ", stddev = " << stddev_Theta << "\n";
    std::cout << "Neutron Phi: mean = " << mean_Phi << ", stddev = " << stddev_Phi << "\n";

    return 0;
}