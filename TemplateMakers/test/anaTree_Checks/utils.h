#ifndef ANAUTILS_H_
#define ANAUTILS_H_

// Reformats a TH1D histogram to have equal bin width on log scale
void binLogX(TH1D* hist) {
    TAxis* axis = hist->GetXaxis();
    int bins = axis->GetNbins();

    Double_t fro = axis->GetXmin();
    Double_t to  = axis->GetXmax();
    Double_t width = (to - fro) / bins;
    Axis_t *new_bins = new Axis_t[bins+1];

    for (int i = 0; i <= bins; i++) {
        new_bins[i] = TMath::Power(10,fro + i*width);
    }
    axis->Set(bins,new_bins);
    delete[] new_bins;
}

template <typename Lep, typename Jet>
void print_event(std::string str, std::vector<Lep> leptons, std::vector<Jet> loose_jets, std::vector<Jet> medium_jets) {
    std::cout << "Printing Event: " << str << std::endl;
    for (const Lep& lep: leptons) {
        std::cout << "Lepton: " << lep.pdgID << " (Q=" << lep.charge << ")" << std::endl;
    }
    std::cout << "btagLoose:  " << loose_jets.size() << std::endl;
    std::cout << "btagMedium: " << medium_jets.size() << std::endl;
    std::cout << std::endl;
}

template <typename T>
std::vector<T> kinematicCut(std::vector<T> input, std::string cut_name, double cut_val, double cut_type) {
    // cut_type = 0 --> cut is minimum allowed
    // cut_type = 1 --> cut is maximum allowed
    std::vector<T> kept;
    double obj_val;
    for (const auto & it: input) {
        if (cut_name == "pt") {
            obj_val = it.obj.Pt();
        } else if (cut_name == "eta") {
            obj_val = abs(it.obj.Eta());
        } else {
            std::cout << "Unknown cut variable" << std::endl;
            break;
        }
        if (cut_type == 0 && obj_val > cut_val) {
            kept.push_back(it);
        } else if (cut_type == 1 && obj_val < cut_val) {
            kept.push_back(it);
        }
    }
    return kept;
}

template <typename T>
std::vector<T> splitCollection(std::vector<T> input, std::set<int> pdgIDs_to_keep) {
    std::vector<T> kept;
    for (const auto & it: input) {
        if (pdgIDs_to_keep.count(it.pdgID)) {
            kept.push_back(it);
        }
    }
}

template <typename T>
std::vector<T> mergeCollection(std::vector<T> v1, std::vector<T> v2) {
    std::vector<T> merged(v1);
    merged.insert(merged.end(),v2.begin(),v2.end());
    return merged;
}

template <typename T>
double getInvMass(const T& p1, const T& p2) {
    return (p1.obj + p2.obj).M();
}

template <typename T>
int sumCharge(std::vector<T> & input) {
    int q = 0;
    for (const auto& it: input) q += it.charge;
    return q;
}

template <typename T>
int countParticles(std::vector<T> & input, int pdg_id) {
    int count = 0;
    for (const auto& it: input) {
        if (pdg_id == abs(it.pdgID)) {
            count += 1;
        }
    }
    return count;
}

template <typename T>
double getMinInvMass(std::vector<T> input) {
    double min_mass = -1;
    for (uint i = 0; i < input.size(); i++) {
        T p1 = input.at(i);
        for (uint j = i; j < input.size(); j++) {
            if (i == j) continue;
            T p2 = input.at(j);
            // double inv_mass = (p1.obj + p2.obj).M();
            double inv_mass = getInvMass(p1,p2);
            if (min_mass == -1) {
                min_mass = inv_mass;
            } else {
                min_mass = std::min(inv_mass,min_mass);
            }
        }
    }
    return min_mass;
}

template <typename T>
bool hasSFZ(std::vector<T> leptons, double cut_window) {
    for (uint i = 0; i < leptons.size(); i++) {
        T p1 = leptons.at(i);
        for (uint j = i; j < leptons.size(); j++) {
            if (i == j) continue;
            T p2 = leptons.at(j);
            if (fabs(p1.pdgID) != fabs(p2.pdgID)) continue;
            double inv_mass = getInvMass(p1,p2);
            if (fabs(inv_mass - 91.2) < cut_window) {
                return true;
            }
        }
    }
    return false;
}

template <typename T> vector<T> sortParticles(vector<T> & particles) {
    vector<T> sorted_particles(particles.begin(),particles.end());
    std::sort(sorted_particles.begin(),sorted_particles.end(), [] (T a, T b) {return a.obj.Pt() > b.obj.Pt();});
    return sorted_particles;
}

void printProgress(int current_index, int total_entries, int interval=20) {
    if (current_index % max(int(total_entries*interval/100.),interval) == 0) {
        float fraction = 100.*current_index/total_entries;
        std::cout << int(fraction) << " % processed " << std::endl;
    }
}

//ANAUTILS_H
#endif