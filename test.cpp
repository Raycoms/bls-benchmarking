#include <bls.hpp>
#include <random>
#include <atomic>
#include <cmath>
#include <set>
#include <libnet.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

using namespace bls;

int main(int argc, char* argv[]) {

    vector<int> tests;
    tests.reserve(6);

    tests.push_back(2);
    tests.push_back(10);
    tests.push_back(100);
    tests.push_back(500);
    tests.push_back(1000);
    tests.push_back(5000);

    for (int total : tests) {
        vector<PrivateKey> vec;

        for (size_t id = 0; id < total; id++) {
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

        sigVec.reserve(total);
        pubVec.reserve(total);

        for (const PrivateKey &key : vec) {
            sigVec.emplace_back(PopSchemeMPL::Sign(key, msg));
            pubVec.emplace_back(key.GetG1Element());
        }

        struct timeval timeStart,
                timeEnd;

        /*vector<G2Element> popVec;
        popVec.reserve(total);
        gettimeofday(&timeStart, NULL);

        for (const PrivateKey &key : vec) {
            popVec.push_back(PopSchemeMPL::PopProve(key));
        }

        gettimeofday(&timeEnd, NULL);

        std::cout << "Pop Prove: "
                  << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
                  << " us to execute."
                  << std::endl;

        gettimeofday(&timeStart, NULL);

        for (int i = 0; i < popVec.size(); i++) {
            PopSchemeMPL::PopVerify(pubVec[i], popVec[i]);

        }
        gettimeofday(&timeEnd, NULL);

        std::cout << "Pop verify: "
                  << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
                  << " us to execute."
                  << std::endl;
        */

        gettimeofday(&timeStart, NULL);

        bls::G2Element sig = PopSchemeMPL::Aggregate(sigVec);

        gettimeofday(&timeEnd, NULL);

        std::cout << "Signature Aggregate: "
                  << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
                  << " us to execute."
                  << std::endl;

        gettimeofday(&timeStart, NULL);

        bls::G1Element pub = PopSchemeMPL::Aggregate(pubVec);

        gettimeofday(&timeEnd, NULL);

        std::cout << "Pub Aggregation: "
                  << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
                  << " us to execute."
                  << std::endl;

        gettimeofday(&timeStart, NULL);

        cout << PopSchemeMPL::Verify(pub, msg, sig) << endl;

        gettimeofday(&timeEnd, NULL);

        std::cout << "Agg Verify "
                  << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
                  << " us to execute."
                  << std::endl;

        gettimeofday(&timeStart, NULL);

        PopSchemeMPL::FastAggregateVerify(pubVec, msg, sig);

        gettimeofday(&timeEnd, NULL);

        std::cout << "Fast Agg Verify: "
                  << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
                  << " us to execute."
                  << std::endl;

        std::cout << "----------------------------" << std::endl;
    }
}