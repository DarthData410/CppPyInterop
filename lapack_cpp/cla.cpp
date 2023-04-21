#include "dgeev.hpp"

int main() {

    EigValsParms vp = cla::newEVParms(2,2);
    vp.matrix_data[0] = 1.23;
    vp.matrix_data[2] = 0.081;
    vp.matrix_data[1] = 0.44;
    vp.matrix_data[3] = 0.13287;

    cout << " --- EIGVALS ---" << endl;
    vector<complex<double>> eigvals = cla::EigVals(vp);
    for(complex<double> cd : eigvals) {
        cout << " [" << to_string(cd.real());
        cout << ":" << to_string(cd.imag()) << "] \n";
    }

    return 0;
}