#include <cmath>
#include <complex>
#include <vector>

#define TM 0
#define TE 1

void CalcolocampiEH(
    std::complex<float> n,
    float teta1,
    int pol,
    float lambda0,
    std::vector<float> vettorez,
    std::complex<float> E1inc,
    float zp,
    float xp
) {
    int mezzoi = Trovamezzo(vettorez, zp);
}

void VAmplitudeET(
    float n,
    float sinteta,
    int pol,
    int mezzo,
    float lambda0,
    std::vector<float> vettorez,
    float zp,
    float xp
) {
    std::vector<std::vector<float>> MatrixEH = {{0, 0}, {0, 0}, {0, 0}};
    if (pol == TM) {
        // MatrixEH[0][0] = sum(MatrixEHprovv)
        MatrixEH[2][0] = ((MatrixEHprovv(2,1)-MatrixEHprovv(1,1))/sqrt(1-sinteta(mezzoi)^2))*sinteta(mezzoi);
    }
}

std::vector<std::vector<float>> MatrixEHprovv();

int Trovamezzo(std::vector<float>, float);

float calcolosinteta(std::vector<float> n, float teta1) {
    std::vector<float> sinteta;
    sinteta.reserve(n.size());
    sinteta[0] = teta1;
    for (int i = 0; i < sinteta.size(); i++) {

    }
}

float ntrasversale(float ni, float sintetai, int pol) {
    if (pol == TE) return ni * std::sqrt(1-std::pow(sintetai, 2));
    return ni / std::sqrt(1-std::pow(sintetai, 2));
}
