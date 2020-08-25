#include <bls.hpp>
#include <random>
#include <atomic>
#include <cmath>
#include <set>
#include <test-utils.hpp>
#include <libnet.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

using namespace bls;

int main(int argc, char* argv[]) {
    vector<PrivateKey> vec;

    int total = 5000;
    for (size_t id = 0; id < total; id ++) {
        vector<uint8_t> seed = {0, static_cast<uint8_t>(id), 6, 244, 24, 199, 1, 25, 52, 88, 192,
                                19, 18, 12, 89, 6, static_cast<uint8_t>(id), 18, 102, 58, 209,
                                82, 12, 62, 89, 110, 182, 9, 44, 20, static_cast<uint8_t>(id), 22};

        vec.emplace_back(PrivateKey::FromByteVector(seed, true));
    }

    vector<uint8_t> msg = {100, 2, 254, 88, 90, 45, 23};
    uint8_t hash[bls::BLS::MESSAGE_HASH_LEN];
    bls::Util::Hash256(hash, msg.data(), msg.size());

    vector<G2Element> sigVec;
    vector<G1Element> pubVec;

    sigVec.reserve(vec.size());

    for (const PrivateKey& key : vec) {
        sigVec.emplace_back(PopSchemeMPL::Sign(key, msg));
        pubVec.emplace_back(key.GetG1Element());
    }

    struct timeval timeStart,
            timeEnd;
    gettimeofday(&timeStart, NULL);

    bls::G2Element sig = PopSchemeMPL::Aggregate(sigVec);

    gettimeofday(&timeEnd, NULL);

    std::cout << "This aggregating slow piece of code took "
              << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
              << " us to execute."
              << std::endl;


    vector<G1Element> pubVec1;
    vector<G1Element> pubVec2;

    for (int i = 0; i < total/2; i++) {
        pubVec1.emplace_back(vec[i].GetG1Element());
        pubVec2.emplace_back(vec[total/2+i].GetG1Element());
    }

    gettimeofday(&timeStart, NULL);

    bls::G1Element pub1 = pubVec1[0];
    // New way of aggregating
    for (int i = 1; i < pubVec1.size(); i++) {
        pub1 = pub1 + pubVec1[i];
    }

    gettimeofday(&timeEnd, NULL);

    std::cout << "This aggregation1 slow piece of code took "
              << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
              << " us to execute."
              << std::endl;

    gettimeofday(&timeStart, NULL);

    bls::G1Element pub2 = pubVec2[0];

    // New way of aggregating
    for (int i = 1; i < pubVec2.size(); i++) {
        pub2 = pub2 + pubVec2[i];
    }

    gettimeofday(&timeEnd, NULL);

    std::cout << "This aggregation2 slow piece of code took "
              << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
              << " us to execute."
              << std::endl;


    gettimeofday(&timeStart, NULL);

    bls::G1Element pub = pub1 + pub2;

    gettimeofday(&timeEnd, NULL);

    std::cout << "This aggregation3 slow piece of code took "
              << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
              << " us to execute."
              << std::endl;

    gettimeofday(&timeStart, NULL);

    cout << PopSchemeMPL::Verify(pub, msg, sig) << endl;


    gettimeofday(&timeEnd, NULL);

    std::cout << "This th computing slow piece of code took "
              << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
              << " us to execute."
              << std::endl;
}